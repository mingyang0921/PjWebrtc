
#ifndef EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_
#define EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_

#include "api/peer_connection_interface.h"
#include "rtc_base/strings/json.h"

const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

namespace webrtc {
    class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
    class VideoRenderer;
}  // namespace cricket

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

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override {
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxx\n");

#if 0
        sdp_mid = candidate->sdp_mid();
        sdp_mline_index = candidate->sdp_mline_index();
        std::string sdp;
        if (!candidate->ToString(&sdp)) {
            RTC_LOG(LS_ERROR) << "Failed to serialize candidate";
            return;
        }
        sdp_candidate = sdp;
#endif
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
        my_candidate = writer.write(jmessage);

    }

    void OnIceConnectionReceivingChange(bool receiving) override {}

public:
    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
        peer_connection_->SetLocalDescription(
            DummySetSessionDescriptionObserver::Create(), desc);
#if 1     
        std::string sdp;
        desc->ToString(&sdp);
        //mysdp = sdp;
#endif
        times++;

        Json::StyledWriter writer;
        Json::Value jmessage;
        jmessage[kSessionDescriptionTypeName] =
            webrtc::SdpTypeToString(desc->GetType());
        jmessage[kSessionDescriptionSdpName] = sdp;
        mysdp = writer.write(jmessage);
    }
    void OnFailure(webrtc::RTCError error) override {
        printf("yyyyyyyyyyyyyyyyyyy\n");
    }

public:
    std::string sdpGet() {
        return mysdp;
    }

    std::string candidateGet() {
        return my_candidate;
    }

public:
    //void Release() {}
    //void AddRef() {}
    void SetPeerConnection(webrtc::PeerConnectionInterface* peer_connection) {
        peer_connection_ = peer_connection;
    }
private:
    std::string mysdp;
    std::string my_candidate;

    std::string sdp_mid;
    std::string sdp_mline_index;
    std::string sdp_candidate;
    int times=0;
private:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_=nullptr;
};

#endif  // EXAMPLES_PEERCONNECTION_CLIENT_CALLBACK_H_
#pragma once
