
/*
 *  Copyright 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef PC_PEER_CONNECTION_FACTORY_H_
#define PC_PEER_CONNECTION_FACTORY_H_

#include <stdint.h>
#include <stdio.h>

#include <memory>
#include <string>

#include "absl/strings/string_view.h"
#include "api/audio_options.h"
#include "api/fec_controller.h"
#include "api/media_stream_interface.h"
#include "api/media_types.h"
#include "api/neteq/neteq_factory.h"
#include "api/network_state_predictor.h"
#include "api/peer_connection_interface.h"
#include "api/rtc_error.h"
#include "api/rtc_event_log/rtc_event_log.h"
#include "api/rtc_event_log/rtc_event_log_factory_interface.h"
#include "api/rtp_parameters.h"
#include "api/scoped_refptr.h"
#include "api/sequence_checker.h"
#include "api/task_queue/task_queue_factory.h"
#include "api/transport/network_control.h"
#include "api/transport/sctp_transport_factory_interface.h"
#include "api/transport/webrtc_key_value_config.h"
#include "call/call.h"
#include "media/sctp/sctp_transport_internal.h"
#include "p2p/base/port_allocator.h"
#include "pc/channel_manager.h"
#include "pc/connection_context.h"
#include "rtc_base/checks.h"
#include "rtc_base/rtc_certificate_generator.h"
#include "rtc_base/thread.h"
#include "rtc_base/thread_annotations.h"

namespace rtc {
class BasicNetworkManager;
class BasicPacketSocketFactory;
}  // namespace rtc

namespace webrtc {

class RtcEventLog;

class PeerConnectionFactory : public PeerConnectionFactoryInterface {
 public:
  // Creates a PeerConnectionFactory. It returns nullptr on initialization
  // error.
  //
  // The Dependencies structure allows simple management of all new
  // dependencies being added to the PeerConnectionFactory.
  static rtc::scoped_refptr<PeerConnectionFactory> Create(
      PeerConnectionFactoryDependencies dependencies);

  void SetOptions(const Options& options) override;

  rtc::scoped_refptr<PeerConnectionInterface> CreatePeerConnection(
      const PeerConnectionInterface::RTCConfiguration& configuration,
      std::unique_ptr<cricket::PortAllocator> allocator,
      std::unique_ptr<rtc::RTCCertificateGeneratorInterface> cert_generator,
      PeerConnectionObserver* observer) override;

  rtc::scoped_refptr<PeerConnectionInterface> CreatePeerConnection(
      const PeerConnectionInterface::RTCConfiguration& configuration,
      PeerConnectionDependencies dependencies) override;

  RTCErrorOr<rtc::scoped_refptr<PeerConnectionInterface>>
  CreatePeerConnectionOrError(
      const PeerConnectionInterface::RTCConfiguration& configuration,
      PeerConnectionDependencies dependencies) override;

  RtpCapabilities GetRtpSenderCapabilities(
      cricket::MediaType kind) const override;

  RtpCapabilities GetRtpReceiverCapabilities(
      cricket::MediaType kind) const override;

  rtc::scoped_refptr<MediaStreamInterface> CreateLocalMediaStream(
      const std::string& stream_id) override;

  rtc::scoped_refptr<AudioSourceInterface> CreateAudioSource(
      const cricket::AudioOptions& options) override;

  rtc::scoped_refptr<VideoTrackInterface> CreateVideoTrack(
      const std::string& id,
      VideoTrackSourceInterface* video_source) override;

  rtc::scoped_refptr<AudioTrackInterface> CreateAudioTrack(
      const std::string& id,
      AudioSourceInterface* audio_source) override;

  bool StartAecDump(FILE* file, int64_t max_size_bytes) override;
  void StopAecDump() override;

  SctpTransportFactoryInterface* sctp_transport_factory() {
    return context_->sctp_transport_factory();
  }

  virtual cricket::ChannelManager* channel_manager();

  rtc::Thread* signaling_thread() const {
    // This method can be called on a different thread when the factory is
    // created in CreatePeerConnectionFactory().
    return context_->signaling_thread();
  }

  const Options& options() const {
    RTC_DCHECK_RUN_ON(signaling_thread());
    return options_;
  }

  const WebRtcKeyValueConfig& trials() const { return context_->trials(); }

 protected:
  // Constructor used by the static Create() method. Modifies the dependencies.
  PeerConnectionFactory(rtc::scoped_refptr<ConnectionContext> context,
                        PeerConnectionFactoryDependencies* dependencies);

  // Constructor for use in testing. Ignores the possibility of initialization
  // failure. The dependencies are passed in by std::move().
  explicit PeerConnectionFactory(
      PeerConnectionFactoryDependencies dependencies);

  virtual ~PeerConnectionFactory();

 private:
  rtc::Thread* worker_thread() const { return context_->worker_thread(); }
  rtc::Thread* network_thread() const { return context_->network_thread(); }

  bool IsTrialEnabled(absl::string_view key) const;
  const cricket::ChannelManager* channel_manager() const {
    return context_->channel_manager();
  }

  std::unique_ptr<RtcEventLog> CreateRtcEventLog_w();
  std::unique_ptr<Call> CreateCall_w(RtcEventLog* event_log);

  rtc::scoped_refptr<ConnectionContext> context_;
  PeerConnectionFactoryInterface::Options options_
      RTC_GUARDED_BY(signaling_thread());
  std::unique_ptr<TaskQueueFactory> task_queue_factory_;
  std::unique_ptr<RtcEventLogFactoryInterface> event_log_factory_;
  std::unique_ptr<FecControllerFactoryInterface> fec_controller_factory_;
  std::unique_ptr<NetworkStatePredictorFactoryInterface>
      network_state_predictor_factory_;
  std::unique_ptr<NetworkControllerFactoryInterface>
      injected_network_controller_factory_;
  std::unique_ptr<NetEqFactory> neteq_factory_;
};

}  // namespace webrtc

#endif  // PC_PEER_CONNECTION_FACTORY_H_
