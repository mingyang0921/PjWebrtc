/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "conductor.h"

#include <stddef.h>
#include <stdint.h>

#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "absl/types/optional.h"
#include "api/audio/audio_mixer.h"
#include "api/audio_codecs/audio_decoder_factory.h"
#include "api/audio_codecs/audio_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/audio_options.h"
#include "api/create_peerconnection_factory.h"
#include "api/rtp_sender_interface.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_factory.h"
#include "p2p/base/port_allocator.h"
#include "pc/video_track_source.h"
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/rtc_certificate_generator.h"
#include "rtc_base/strings/json.h"
#include "test/vcm_capturer.h"
const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamId[] = "stream_id";
const uint16_t kDefaultServerPort = 8888;
namespace {
    // Names used for a IceCandidate JSON object.
    const char kCandidateSdpMidName[] = "sdpMid";
    const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
    const char kCandidateSdpName[] = "candidate";

    // Names used for a SessionDescription JSON object.
    const char kSessionDescriptionTypeName[] = "type";
    const char kSessionDescriptionSdpName[] = "sdp";

    class DummySetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
    public:
        static DummySetSessionDescriptionObserver* Create() {
            return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
        }
        virtual void OnSuccess() { RTC_LOG(INFO) << __FUNCTION__; }
        virtual void OnFailure(webrtc::RTCError error) {
            RTC_LOG(INFO) << __FUNCTION__ << " " << ToString(error.type()) << ": "
                << error.message();
        }
    };

    class CapturerTrackSource : public webrtc::VideoTrackSource {
    public:
        static rtc::scoped_refptr<CapturerTrackSource> Create() {
            const size_t kWidth = 640;
            const size_t kHeight = 480;
            const size_t kFps = 30;
            std::unique_ptr<webrtc::test::VcmCapturer> capturer;
            std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
                webrtc::VideoCaptureFactory::CreateDeviceInfo());
            if (!info) {
                return nullptr;
            }
            int num_devices = info->NumberOfDevices();
            for (int i = 1; i < num_devices; ++i) {
                capturer = absl::WrapUnique(
                    webrtc::test::VcmCapturer::Create(kWidth, kHeight, kFps, i));
                if (capturer) {
                    return new rtc::RefCountedObject<CapturerTrackSource>(
                        std::move(capturer));
                }
            }

            return nullptr;
        }

    protected:
        explicit CapturerTrackSource(
            std::unique_ptr<webrtc::test::VcmCapturer> capturer)
            : VideoTrackSource(/*remote=*/false), capturer_(std::move(capturer)) {}

    private:
        rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
            return capturer_.get();
        }
        std::unique_ptr<webrtc::test::VcmCapturer> capturer_;
    };

}  // namespace

Conductor::Conductor()
    : peer_id_(-1), loopback_(false) {
}

Conductor::~Conductor() {
    RTC_DCHECK(!peer_connection_);
}

bool Conductor::connection_active() const {
    return peer_connection_ != nullptr;
}

bool Conductor::InitializePeerConnection() {
    RTC_DCHECK(!peer_connection_factory_);
    RTC_DCHECK(!peer_connection_);

    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
        nullptr /* network_thread */, nullptr /* worker_thread */,
        nullptr /* signaling_thread */, nullptr /* default_adm */,
        webrtc::CreateBuiltinAudioEncoderFactory(),
        webrtc::CreateBuiltinAudioDecoderFactory(),
        webrtc::CreateBuiltinVideoEncoderFactory(),
        webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
        nullptr /* audio_processing */);

    if (!peer_connection_factory_) {
        DeletePeerConnection();
        return false;
    }

    if (!CreatePeerConnection(/*dtls=*/true)) {
        DeletePeerConnection();
    }

    AddTracks();

    return peer_connection_ != nullptr;
}

bool Conductor::ReinitializePeerConnectionForLoopback() {
    loopback_ = true;
    std::vector<rtc::scoped_refptr<webrtc::RtpSenderInterface>> senders =
        peer_connection_->GetSenders();
    peer_connection_ = nullptr;
    if (CreatePeerConnection(/*dtls=*/false)) {
        for (const auto& sender : senders) {
            peer_connection_->AddTrack(sender->track(), sender->stream_ids());
        }
        peer_connection_->CreateOffer(
            this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }
    return peer_connection_ != nullptr;
}

bool Conductor::CreatePeerConnection(bool dtls) {
    RTC_DCHECK(peer_connection_factory_);
    RTC_DCHECK(!peer_connection_);

    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    config.enable_dtls_srtp = dtls;
    webrtc::PeerConnectionInterface::IceServer server;
    std::string addr = "stun:stun.l.google.com:19302";
    server.uri = addr;
    config.servers.push_back(server);

    peer_connection_ = peer_connection_factory_->CreatePeerConnection(
        config, nullptr, nullptr, this);
    return peer_connection_ != nullptr;
}

void Conductor::DeletePeerConnection() {
    peer_connection_ = nullptr;
    peer_connection_factory_ = nullptr;
    peer_id_ = -1;
    loopback_ = false;
}

void Conductor::EnsureStreamingUI() {
    RTC_DCHECK(peer_connection_);
}

//
// PeerConnectionObserver implementation.
//

void Conductor::OnAddTrack(
    rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
    const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
    streams) {
    RTC_LOG(INFO) << __FUNCTION__ << " " << receiver->id();
}

void Conductor::OnRemoveTrack(
    rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) {
    RTC_LOG(INFO) << __FUNCTION__ << " " << receiver->id();
}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    RTC_LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
    // For loopback test. To save some connecting delay.
    if (loopback_) {
        if (!peer_connection_->AddIceCandidate(candidate)) {
            RTC_LOG(WARNING) << "Failed to apply the received candidate";
        }
        return;
    }

    Json::StyledWriter writer;
    Json::Value jmessage;

    jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
    jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        RTC_LOG(LS_ERROR) << "Failed to serialize candidate";
        return;
    }
    jmessage[kCandidateSdpName] = sdp;
    SendMessage(writer.write(jmessage));
}

//
// PeerConnectionClientObserver implementation.
//

//
// MainWndCallback implementation.
//


void Conductor::ConnectToPeer(int peer_id) {

    if (InitializePeerConnection()) {
        peer_id_ = peer_id;
        peer_connection_->CreateOffer(
            this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }
    else {
    }
}

void Conductor::AddTracks() {
    if (!peer_connection_->GetSenders().empty()) {
        return;  // Already added tracks.
    }

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory_->CreateAudioTrack(
            kAudioLabel, peer_connection_factory_->CreateAudioSource(
                cricket::AudioOptions())));
    auto result_or_error = peer_connection_->AddTrack(audio_track, { kStreamId });
    if (!result_or_error.ok()) {
        RTC_LOG(LS_ERROR) << "Failed to add audio track to PeerConnection: "
            << result_or_error.error().message();
    }

    rtc::scoped_refptr<CapturerTrackSource> video_device =
        CapturerTrackSource::Create();

    //rtc::scoped_refptr<MyCapturer> video_device = new rtc::RefCountedObject<MyCapturer>();

    if (video_device) {
        //video_device->startCapturer();
        rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_(
            peer_connection_factory_->CreateVideoTrack(kVideoLabel, video_device));

        result_or_error = peer_connection_->AddTrack(video_track_, { kStreamId });
        if (!result_or_error.ok()) {
            RTC_LOG(LS_ERROR) << "Failed to add video track to PeerConnection: "
                << result_or_error.error().message();
        }
    }
    else {
        RTC_LOG(LS_ERROR) << "OpenVideoCaptureDevice failed";
    }

}



void Conductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    peer_connection_->SetLocalDescription(
        DummySetSessionDescriptionObserver::Create(), desc);

    std::string sdp;
    desc->ToString(&sdp);

    // For loopback test. To save some connecting delay.
    if (loopback_) {
        // Replace message type from "offer" to "answer"
        std::unique_ptr<webrtc::SessionDescriptionInterface> session_description =
            webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp);
        peer_connection_->SetRemoteDescription(
            DummySetSessionDescriptionObserver::Create(),
            session_description.release());
        return;
    }

    Json::StyledWriter writer;
    Json::Value jmessage;
    jmessage[kSessionDescriptionTypeName] =
        webrtc::SdpTypeToString(desc->GetType());
    jmessage[kSessionDescriptionSdpName] = sdp;
    SendMessage(writer.write(jmessage));
}

void Conductor::OnFailure(webrtc::RTCError error) {
    RTC_LOG(LERROR) << ToString(error.type()) << ": " << error.message();
}

void Conductor::SendMessage(const std::string& json_object) {
    std::string* msg = new std::string(json_object);
}
