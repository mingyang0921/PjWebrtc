#include "QtWidgetsLocalVideo.h"

#undef emit
#include "localCapturer.h"
#include "rtc_base/ref_counted_object.h"
#include "api/create_peerconnection_factory.h"
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

#define emit Q_EMIT

QtWidgetsLocalVideo::QtWidgetsLocalVideo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

QtWidgetsLocalVideo::~QtWidgetsLocalVideo()
{}

int QtWidgetsLocalVideo::init()
{

	peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
		nullptr /* network_thread */, nullptr /* worker_thread */,
		nullptr /* signaling_thread */, nullptr /* default_adm */,
		webrtc::CreateBuiltinAudioEncoderFactory(),
		webrtc::CreateBuiltinAudioDecoderFactory(),
		webrtc::CreateBuiltinVideoEncoderFactory(),
		webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
		nullptr /* audio_processing */);

	webrtc::PeerConnectionInterface::RTCConfiguration config;
	config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
	config.enable_dtls_srtp = true;
	webrtc::PeerConnectionInterface::IceServer server;
	//server.uri = GetPeerConnectionString();
	config.servers.push_back(server);
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
	//peer_connection_ = peer_connection_factory_->CreatePeerConnection(
	//	config, nullptr, nullptr, conductor);

	video_device = new rtc::RefCountedObject<localCapturer>();

	if (video_device) {
		video_device->startCapturer();
		//main_wnd_->StartLocalRenderer(video_track_);
		//new VideoRenderer(handle(), 1, 1, local_video)
	}
	return 0;
}
