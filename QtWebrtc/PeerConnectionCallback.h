
#ifndef EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_
#define EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_

#include "api/peer_connection_interface.h"

namespace webrtc {
    class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
    class VideoRenderer;
}  // namespace cricket

class ConductorCallback : public webrtc::PeerConnectionObserver,
                          public webrtc::CreateSessionDescriptionObserver
{

public:
    ConductorCallback() {}
    ~ConductorCallback() {}

    void OnSignalingChange(
        webrtc::PeerConnectionInterface::SignalingState new_state) override {}

    void OnAddTrack(
        rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
        const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
        streams) override {}

    void OnRemoveTrack(
        rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override {}

    void OnDataChannel(
        rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}

    void OnRenegotiationNeeded() override {}

    void OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}

    void OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override {}

    void OnIceConnectionReceivingChange(bool receiving) override {}

public:
    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
        printf("xxxxxxxxxxxxxxxxxxxxxxx\n");
        std::string sdp;
        desc->ToString(&sdp);
    }
    void OnFailure(webrtc::RTCError error) override {
        printf("yyyyyyyyyyyyyyyyyyy\n");
    }

public:
    void Release() {}
    void AddRef() {}
};

#endif  // EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_
#pragma once
