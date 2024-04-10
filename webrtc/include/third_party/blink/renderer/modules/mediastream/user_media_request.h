/*
 * Copyright (C) 2011 Ericsson AB. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Ericsson nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_USER_MEDIA_REQUEST_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_USER_MEDIA_REQUEST_H_

#include "third_party/blink/public/common/privacy_budget/identifiable_surface.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_navigator_user_media_error_callback.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_navigator_user_media_success_callback.h"
#include "third_party/blink/renderer/core/execution_context/execution_context_lifecycle_observer.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/mediastream/media_constraints.h"
#include "third_party/blink/renderer/platform/mediastream/media_stream_source.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

class LocalDOMWindow;
class MediaErrorState;
class MediaStreamConstraints;
class MediaStreamDescriptor;
class UserMediaController;

class MODULES_EXPORT UserMediaRequest final
    : public GarbageCollected<UserMediaRequest>,
      public ExecutionContextLifecycleObserver {
 public:
  enum class Error {
    kNotSupported,
    kSecurityError,
    kPermissionDenied,
    kPermissionDismissed,
    kInvalidState,
    kDevicesNotFound,
    kTabCapture,
    kScreenCapture,
    kCapture,
    kTrackStart,
    kFailedDueToShutdown,
    kKillSwitchOn,
    kSystemPermissionDenied
  };

  enum class MediaType {
    kUserMedia,
    kDisplayMedia,
    kGetCurrentBrowsingContextMedia,
  };

  class Callbacks : public GarbageCollected<Callbacks> {
   public:
    virtual ~Callbacks() = default;

    virtual void OnSuccess(ScriptWrappable* callback_this_value,
                           MediaStream*) = 0;
    virtual void OnError(ScriptWrappable* callback_this_value,
                         DOMExceptionOrOverconstrainedError) = 0;

    virtual void Trace(Visitor*) const {}

   protected:
    Callbacks() = default;
  };

  class V8Callbacks;

  static UserMediaRequest* Create(ExecutionContext*,
                                  UserMediaController*,
                                  MediaType media_type,
                                  const MediaStreamConstraints* options,
                                  Callbacks*,
                                  MediaErrorState&,
                                  IdentifiableSurface surface);
  static UserMediaRequest* Create(ExecutionContext*,
                                  UserMediaController*,
                                  const MediaStreamConstraints* options,
                                  V8NavigatorUserMediaSuccessCallback*,
                                  V8NavigatorUserMediaErrorCallback*,
                                  MediaErrorState&,
                                  IdentifiableSurface surface);
  static UserMediaRequest* CreateForTesting(const MediaConstraints& audio,
                                            const MediaConstraints& video);

  UserMediaRequest(ExecutionContext*,
                   UserMediaController*,
                   MediaType media_type,
                   MediaConstraints audio,
                   MediaConstraints video,
                   Callbacks*,
                   IdentifiableSurface surface);
  ~UserMediaRequest() override;

  LocalDOMWindow* GetWindow();

  void Start();

  void Succeed(MediaStreamDescriptor*);
  void OnMediaStreamInitialized(MediaStream* stream);
  void FailConstraint(const String& constraint_name, const String& message);
  void Fail(Error name, const String& message);

  MediaType MediaRequestType() const;
  bool Audio() const;
  bool Video() const;
  MediaConstraints AudioConstraints() const;
  MediaConstraints VideoConstraints() const;

  // Flag tied to whether or not the similarly named Origin Trial is
  // enabled. Will be removed at end of trial. See: http://crbug.com/789152.
  bool ShouldDisableHardwareNoiseSuppression() const;

  // errorMessage is only set if requestIsPrivilegedContext() returns |false|.
  // Caller is responsible for properly setting errors and canceling request.
  bool IsSecureContextUse(String& error_message);

  // ExecutionContextLifecycleObserver
  void ContextDestroyed() override;

  void set_request_id(int id) { request_id_ = id; }
  int request_id() { return request_id_; }

  void set_has_transient_user_activation(bool value) {
    has_transient_user_activation_ = value;
  }
  bool has_transient_user_activation() const {
    return has_transient_user_activation_;
  }

  void Trace(Visitor*) const override;

 private:
  MediaType media_type_;
  MediaConstraints audio_;
  MediaConstraints video_;
  bool should_disable_hardware_noise_suppression_;
  bool has_transient_user_activation_ = false;
  int request_id_ = -1;

  Member<UserMediaController> controller_;

  Member<Callbacks> callbacks_;
  IdentifiableSurface surface_;
  bool is_resolved_ = false;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_USER_MEDIA_REQUEST_H_
