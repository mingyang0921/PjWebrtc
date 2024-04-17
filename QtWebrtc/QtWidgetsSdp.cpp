#include "QtWidgetsSdp.h"

#undef emit
#include "PeerConnectionCallback.h"
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
#include "localCapturer.h"
#include "conductor.h"


#include "absl/flags/parse.h"
#include "rtc_base/checks.h"
#include "rtc_base/constructor_magic.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/string_utils.h"  // For ToUtf8
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"

#define emit Q_EMIT

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QString>
#include <QThread>

const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamId[] = "stream_id";
const uint16_t kDefaultServerPort = 8888;


class CapturerTrackSource : public webrtc::VideoTrackSource 
{
public:
    static rtc::scoped_refptr<CapturerTrackSource> Create() {
        const size_t kWidth = 1920;
        const size_t kHeight = 1080;
        const size_t kFps = 30;
        std::unique_ptr<webrtc::test::VcmCapturer> capturer;
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
            webrtc::VideoCaptureFactory::CreateDeviceInfo());
        if (!info) {
            return nullptr;
        }
        int num_devices = info->NumberOfDevices();
        int i = 0;
        for (i = 0; i < num_devices; ++i)
        {
            const uint32_t kSize = 256;
            char name[kSize] = { 0 };
            char id[kSize] = { 0 };
            if (info->GetDeviceName(i, name, kSize, id, kSize) != -1)
            {
                printf("%s\n",name);
            }
        }

        for (i = 1; i < num_devices; ++i) {
            //capturer = webrtc_demo::VcmCapturerTest::Create(kWidth, kHeight, kFps, 2);
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

QtWidgetsSdp::QtWidgetsSdp(QWidget *parent)
	: QWidget(parent)
{
    conductor = new rtc::RefCountedObject<ConductorCallback>();
	init();
    initUi();
}

QtWidgetsSdp::~QtWidgetsSdp()
{

}

void QtWidgetsSdp::initUi()
{
    QHBoxLayout* horizontalLayout;
    QVBoxLayout* verticalLayout;
    QSpacerItem* verticalSpacer;
    QPushButton* pushButton;
    QSpacerItem* verticalSpacer_2;

    this->resize(816, 600);

    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    textEdit = new QTextEdit();
    textEdit->setObjectName(QString::fromUtf8("textEdit"));

    horizontalLayout->addWidget(textEdit);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    pushButton = new QPushButton();
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setText(u8"ÏÔÊ¾SDP");
    verticalLayout->addWidget(pushButton);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer_2);
    horizontalLayout->addLayout(verticalLayout); 

    connect(pushButton, &QPushButton::clicked, this, &QtWidgetsSdp::on_pushButton_clicked);
}


void QtWidgetsSdp::init()
{
    //Conductor* conductor = new Conductor();
    //conductornew = new rtc::RefCountedObject<Conductor>();
    //conductornew->ConnectToPeer(0);
    //return;

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
    std::string addr = "stun:stun.l.google.com:19302";
    server.uri = addr;
    config.servers.push_back(server);

    //conductor = new rtc::RefCountedObject<ConductorCallback>();
    //conductor = new ConductorCallback();

    peer_connection_ = peer_connection_factory_->CreatePeerConnection(
        config, nullptr, nullptr, conductor);


    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory_->CreateAudioTrack(
            kAudioLabel, peer_connection_factory_->CreateAudioSource(
                cricket::AudioOptions())));

    auto result_or_error = peer_connection_->AddTrack(audio_track, { kStreamId });
    if (!result_or_error.ok()) {
        RTC_LOG(LS_ERROR) << "Failed to add audio track to PeerConnection: "
            << result_or_error.error().message();
    }

    rtc::scoped_refptr<CapturerTrackSource> video_device = CapturerTrackSource::Create();
    //rtc::scoped_refptr<localCapturer> video_device = new rtc::RefCountedObject<localCapturer>();

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


    peer_connection_->CreateOffer(
        conductor, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());

    //QThread::sleep(3);
    //std::string uisdp = conductor->sdpGet();
}

void QtWidgetsSdp::on_pushButton_clicked()
{
    std::string uisdp = conductor->sdpGet();

    QString qstr;
    qstr = QString::fromStdString(uisdp);

    textEdit->setText(qstr);
}