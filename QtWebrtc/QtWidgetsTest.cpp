#include "QtWidgetsTest.h"

#include <QApplication>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

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
#include "absl/flags/parse.h"
#include "rtc_base/checks.h"
#include "rtc_base/constructor_magic.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/string_utils.h"  // For ToUtf8
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"
#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "test/video_renderer.h"
#include "rtc_base/logging.h"
#include <thread>
#include "third_party/libyuv/include/libyuv.h"
#include "api/video/i420_buffer.h"
#include "api/video/video_frame.h"
#define emit Q_EMIT

#include "demo_camera_capture.h"
#include "localCapturer.h"

const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamId[] = "stream_id";
const uint16_t kDefaultServerPort = 8888;


class CapturerTrackSource : public webrtc::VideoTrackSource,
    public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
public:
    static rtc::scoped_refptr<CapturerTrackSource> Create(int id) {
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

        if (id >= num_devices) return nullptr;

        capturer = absl::WrapUnique(
            webrtc::test::VcmCapturer::Create(kWidth, kHeight, kFps, id));
        if (capturer) {
            return new rtc::RefCountedObject<CapturerTrackSource>(
                std::move(capturer));
        }

        return nullptr;

        int i = 0;
        for (i = 0; i < num_devices; ++i)
        {
            const uint32_t kSize = 256;
            char name[kSize] = { 0 };
            char id[kSize] = { 0 };
            if (info->GetDeviceName(i, name, kSize, id, kSize) != -1)
            {
                printf("%s\n", name);
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
public:
    void OnFrame(const webrtc::VideoFrame& frame) override {
        RTC_LOG(LS_INFO) << "OnFrame";
    }
};


QtWidgetsTest::QtWidgetsTest(QWidget *parent)
	: QWidget(parent)
{
    conductor = new rtc::RefCountedObject<ConductorCallback>();
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

    peer_connection_ = peer_connection_factory_->CreatePeerConnection(
        config, nullptr, nullptr, conductor);

    webrtc_init();

    initUI();
    checkCamera();
}

QtWidgetsTest::~QtWidgetsTest()
{}


void QtWidgetsTest::initUI()
{
    QVBoxLayout* verticalLayout_father;
    QHBoxLayout* horizontalLayout_up;
    QVBoxLayout* verticalLayout_local_camera;
    QHBoxLayout* horizontalLayout_local_camera_switch;
    QSpacerItem* horizontalSpacer_local_camera_left;
    QLabel* label_local_camera_info;
    QPushButton* pushButton_local_camera_makesure;
    QSpacerItem* horizontalSpacer_local_camera_right;
    QVBoxLayout* verticalLayout_local_sdp;
    QHBoxLayout* horizontalLayout_local_sdp_title;
    QSpacerItem* horizontalSpacer_local_sdp_title_left;
    QLabel* label_local_sdp_title_info;
    QSpacerItem* horizontalSpacer_local_sdp_title_right;
    QVBoxLayout* verticalLayout_local_button;
    QSpacerItem* verticalSpacer_local_button_up;
    QPushButton* pushButton_local_button_showsdp;
    QPushButton* pushButton_local_button_showcandidate;
    QPushButton* pushButton_local_button_press;
    QSpacerItem* verticalSpacer_local_button_down;
    QFrame* line;
    QHBoxLayout* horizontalLayout_down;
    QVBoxLayout* verticalLayout_remote_camera;
    QHBoxLayout* horizontalLayout_remote_camera_title;
    QSpacerItem* horizontalSpacer_remote_camera_title_left;
    QLabel* label_remote_camera_title_info;
    QSpacerItem* horizontalSpacer_remote_camera_title_right;
    QVBoxLayout* verticalLayout_remote_sdp;
    QHBoxLayout* horizontalLayout_remote_sdp;
    QSpacerItem* horizontalSpacer_remote_sdp_left;
    QLabel* label_remote_sdp_info;
    QSpacerItem* horizontalSpacer_remote_sdp_right;
    QVBoxLayout* verticalLayout_remote_button;
    QSpacerItem* verticalSpacer_remote_button_up;
    QPushButton* pushButton_remote_button_press;
    QSpacerItem* verticalSpacer_remote_button_down;

    this->resize(937, 623);

    verticalLayout_father = new QVBoxLayout(this);
    verticalLayout_father->setObjectName(QString::fromUtf8("verticalLayout_father"));
    verticalLayout_father->setContentsMargins(0, 0, 0, 0);

    horizontalLayout_up = new QHBoxLayout();
    horizontalLayout_up->setObjectName(QString::fromUtf8("horizontalLayout_up"));

    verticalLayout_local_camera = new QVBoxLayout();
    verticalLayout_local_camera->setObjectName(QString::fromUtf8("verticalLayout_local_camera"));
    horizontalLayout_local_camera_switch = new QHBoxLayout();
    horizontalLayout_local_camera_switch->setObjectName(QString::fromUtf8("horizontalLayout_local_camera_switch"));
    horizontalSpacer_local_camera_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_camera_switch->addItem(horizontalSpacer_local_camera_left);

    label_local_camera_info = new QLabel();
    label_local_camera_info->setObjectName(QString::fromUtf8("label_local_camera_info"));

    horizontalLayout_local_camera_switch->addWidget(label_local_camera_info);

    comboBox_cocal_camera_check = new QComboBox();
    comboBox_cocal_camera_check->setObjectName(QString::fromUtf8("comboBox_cocal_camera_check"));

    horizontalLayout_local_camera_switch->addWidget(comboBox_cocal_camera_check);

    pushButton_local_camera_makesure = new QPushButton();
    pushButton_local_camera_makesure->setObjectName(QString::fromUtf8("pushButton_local_camera_makesure"));

    horizontalLayout_local_camera_switch->addWidget(pushButton_local_camera_makesure);

    horizontalSpacer_local_camera_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_camera_switch->addItem(horizontalSpacer_local_camera_right);


    verticalLayout_local_camera->addLayout(horizontalLayout_local_camera_switch);

    label_local_camera_show = new QLabel();
    label_local_camera_show->setObjectName(QString::fromUtf8("label_local_camera_show"));

    verticalLayout_local_camera->addWidget(label_local_camera_show);


    horizontalLayout_up->addLayout(verticalLayout_local_camera);

    verticalLayout_local_sdp = new QVBoxLayout();
    verticalLayout_local_sdp->setObjectName(QString::fromUtf8("verticalLayout_local_sdp"));
    horizontalLayout_local_sdp_title = new QHBoxLayout();
    horizontalLayout_local_sdp_title->setObjectName(QString::fromUtf8("horizontalLayout_local_sdp_title"));
    horizontalSpacer_local_sdp_title_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_sdp_title->addItem(horizontalSpacer_local_sdp_title_left);

    label_local_sdp_title_info = new QLabel();
    label_local_sdp_title_info->setObjectName(QString::fromUtf8("label_local_sdp_title_info"));

    horizontalLayout_local_sdp_title->addWidget(label_local_sdp_title_info);

    horizontalSpacer_local_sdp_title_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_sdp_title->addItem(horizontalSpacer_local_sdp_title_right);


    verticalLayout_local_sdp->addLayout(horizontalLayout_local_sdp_title);

    textEdit_local_sdp_text = new QTextEdit();
    textEdit_local_sdp_text->setObjectName(QString::fromUtf8("textEdit_local_sdp_text"));
    verticalLayout_local_sdp->addWidget(textEdit_local_sdp_text);

    textEdit_local_candidate_text = new QTextEdit();
    textEdit_local_candidate_text->setObjectName(QString::fromUtf8("textEdit_local_scandidate_text"));
    verticalLayout_local_sdp->addWidget(textEdit_local_candidate_text);


    horizontalLayout_up->addLayout(verticalLayout_local_sdp);

    verticalLayout_local_button = new QVBoxLayout();
    verticalLayout_local_button->setObjectName(QString::fromUtf8("verticalLayout_local_button"));
    verticalSpacer_local_button_up = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_local_button->addItem(verticalSpacer_local_button_up);

    pushButton_local_button_press = new QPushButton();
    pushButton_local_button_press->setObjectName(QString::fromUtf8("pushButton_local_button_press"));

    verticalLayout_local_button->addWidget(pushButton_local_button_press);

    pushButton_local_button_showsdp = new QPushButton();
    pushButton_local_button_showsdp->setObjectName(QString::fromUtf8("pushButton_local_button_showsdp"));
    verticalLayout_local_button->addWidget(pushButton_local_button_showsdp);

    pushButton_local_button_showcandidate = new QPushButton();
    pushButton_local_button_showcandidate->setObjectName(QString::fromUtf8("pushButton_local_button_showcandidate"));
    verticalLayout_local_button->addWidget(pushButton_local_button_showcandidate);

    verticalSpacer_local_button_down = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_local_button->addItem(verticalSpacer_local_button_down);


    horizontalLayout_up->addLayout(verticalLayout_local_button);


    verticalLayout_father->addLayout(horizontalLayout_up);

    line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 24, 55);"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_father->addWidget(line);

    horizontalLayout_down = new QHBoxLayout();
    horizontalLayout_down->setObjectName(QString::fromUtf8("horizontalLayout_down"));
    verticalLayout_remote_camera = new QVBoxLayout();
    verticalLayout_remote_camera->setObjectName(QString::fromUtf8("verticalLayout_remote_camera"));
    horizontalLayout_remote_camera_title = new QHBoxLayout();
    horizontalLayout_remote_camera_title->setObjectName(QString::fromUtf8("horizontalLayout_remote_camera_title"));
    horizontalSpacer_remote_camera_title_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_camera_title->addItem(horizontalSpacer_remote_camera_title_left);

    label_remote_camera_title_info = new QLabel();
    label_remote_camera_title_info->setObjectName(QString::fromUtf8("label_remote_camera_title_info"));

    horizontalLayout_remote_camera_title->addWidget(label_remote_camera_title_info);

    horizontalSpacer_remote_camera_title_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_camera_title->addItem(horizontalSpacer_remote_camera_title_right);


    verticalLayout_remote_camera->addLayout(horizontalLayout_remote_camera_title);

    label_remote_camera_show = new QLabel();
    label_remote_camera_show->setObjectName(QString::fromUtf8("label_remote_camera_show"));

    verticalLayout_remote_camera->addWidget(label_remote_camera_show);


    horizontalLayout_down->addLayout(verticalLayout_remote_camera);

    verticalLayout_remote_sdp = new QVBoxLayout();
    verticalLayout_remote_sdp->setObjectName(QString::fromUtf8("verticalLayout_remote_sdp"));
    horizontalLayout_remote_sdp = new QHBoxLayout();
    horizontalLayout_remote_sdp->setObjectName(QString::fromUtf8("horizontalLayout_remote_sdp"));
    horizontalSpacer_remote_sdp_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_sdp->addItem(horizontalSpacer_remote_sdp_left);

    label_remote_sdp_info = new QLabel();
    label_remote_sdp_info->setObjectName(QString::fromUtf8("label_remote_sdp_info"));

    horizontalLayout_remote_sdp->addWidget(label_remote_sdp_info);

    horizontalSpacer_remote_sdp_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    horizontalLayout_remote_sdp->addItem(horizontalSpacer_remote_sdp_right);


    verticalLayout_remote_sdp->addLayout(horizontalLayout_remote_sdp);

    textEdit_remote_sdp_text = new QTextEdit();
    textEdit_remote_sdp_text->setObjectName(QString::fromUtf8("textEdit_remote_sdp_text"));
    verticalLayout_remote_sdp->addWidget(textEdit_remote_sdp_text);

    textEdit_remote_candidate_text = new QTextEdit();
    textEdit_remote_candidate_text->setObjectName(QString::fromUtf8("textEdit_remote_candidate_text"));
    verticalLayout_remote_sdp->addWidget(textEdit_remote_candidate_text);

    horizontalLayout_down->addLayout(verticalLayout_remote_sdp);

    verticalLayout_remote_button = new QVBoxLayout();
    verticalLayout_remote_button->setObjectName(QString::fromUtf8("verticalLayout_remote_button"));
    verticalSpacer_remote_button_up = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_remote_button->addItem(verticalSpacer_remote_button_up);

    pushButton_remote_button_press = new QPushButton();
    pushButton_remote_button_press->setObjectName(QString::fromUtf8("pushButton_remote_button_press"));
    
    verticalLayout_remote_button->addWidget(pushButton_remote_button_press);

    verticalSpacer_remote_button_down = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_remote_button->addItem(verticalSpacer_remote_button_down);


    horizontalLayout_down->addLayout(verticalLayout_remote_button);


    verticalLayout_father->addLayout(horizontalLayout_down);

    label_local_camera_info->setText(QApplication::translate("Widget", "\345\252\222\344\275\223\346\272\220\351\200\211\346\213\251", nullptr));
    pushButton_local_camera_makesure->setText(QApplication::translate("Widget", "\347\241\256\350\256\244", nullptr));
    label_local_camera_show->setText(QApplication::translate("Widget", "TextLabel", nullptr));
    label_local_sdp_title_info->setText(QApplication::translate("Widget", "\346\234\254\345\234\260SDP", nullptr));
    pushButton_local_button_press->setText(QApplication::translate("Widget", "\345\220\257\345\212\250", nullptr));
    pushButton_local_button_showsdp->setText(QApplication::translate("Widget", u8"ÏÔÊ¾SDP", nullptr));
    pushButton_local_button_showcandidate->setText(QApplication::translate("Widget", u8"ÏÔÊ¾candidate", nullptr));
    label_remote_camera_title_info->setText(QApplication::translate("Widget", "\345\257\271\347\253\257\345\252\222\344\275\223", nullptr));
    label_remote_camera_show->setText(QApplication::translate("Widget", "TextLabel", nullptr));
    label_remote_sdp_info->setText(QApplication::translate("Widget", "\345\257\271\347\253\257SDP", nullptr));
    pushButton_remote_button_press->setText(QApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));


    connect(pushButton_local_camera_makesure, &QPushButton::clicked, this, &QtWidgetsTest::on_pushButton_makesure_clicked);
    connect(pushButton_local_button_press, &QPushButton::clicked, this, &QtWidgetsTest::on_pushButton_start_localcamera_clicked);
    connect(pushButton_local_button_showsdp, &QPushButton::clicked, this, &QtWidgetsTest::on_pushButton_showsdp_localcamera_clicked);
    connect(pushButton_local_button_showcandidate, &QPushButton::clicked, this, &QtWidgetsTest::on_pushButton_showcandidate_localcamera_clicked);

    connect(pushButton_remote_button_press, &QPushButton::clicked, this, &QtWidgetsTest::on_pushButton_local_remote_connect_clicked);
}

void QtWidgetsTest::checkCamera()
{
    webrtc::VideoCaptureModule::DeviceInfo
        * device = webrtc::VideoCaptureFactory::CreateDeviceInfo();
    int deviceNum = device->NumberOfDevices();
    comboBox_cocal_camera_check_num_max = deviceNum;
    for (int i = 0; i < deviceNum; ++i)
    {
        const uint32_t kSize = 256;
        char name[kSize] = { 0 };
        char id[kSize] = { 0 };
        if (device->GetDeviceName(i, name, kSize, id, kSize) != -1)
        {
            //qDebug() << "name" << QString(name) << "id" << QString(id);
            comboBox_cocal_camera_check->addItem(QString(name));

        }
    }
    comboBox_cocal_camera_check->addItem(QString("desktop"));
    //comboBox_cocal_camera_check->currentIndex()
}

void QtWidgetsTest::on_pushButton_makesure_clicked()
{
    comboBox_cocal_camera_check_num = comboBox_cocal_camera_check->currentIndex();    
    comboBox_cocal_camera_check->setEnabled(false);
}

void QtWidgetsTest::on_pushButton_start_localcamera_clicked()
{
    webrtc_init();
}

void QtWidgetsTest::on_pushButton_showsdp_localcamera_clicked()
{
    std::string uisdp = conductor->sdpGet();

    QString qstr;
    qstr = QString::fromStdString(uisdp);

    textEdit_local_sdp_text->setText(qstr);
}

void QtWidgetsTest::on_pushButton_showcandidate_localcamera_clicked()
{
    std::string uicandidate = conductor->candidateGet();

    QString qstr;
    qstr = QString::fromStdString(uicandidate);

    textEdit_local_candidate_text->setText(qstr);
}

void QtWidgetsTest::connect_sdp_load()
{
    QString remote_sdp = textEdit_remote_sdp_text->toPlainText();

    std::string str_remote_sdp = remote_sdp.toStdString();

    Json::Reader reader;
    Json::Value jmessage;

    if (!reader.parse(str_remote_sdp, jmessage)) {
        RTC_LOG(WARNING) << "Received unknown message. " << str_remote_sdp;
        return;
    }
    std::string type_str;
    std::string json_object;

    rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName,
        &type_str);
    if (!type_str.empty()) {
        absl::optional<webrtc::SdpType> type_maybe =
            webrtc::SdpTypeFromString(type_str);
        if (!type_maybe) {
            RTC_LOG(LS_ERROR) << "Unknown SDP type: " << type_str;
            return;
        }
        webrtc::SdpType type = *type_maybe;
        std::string sdp;
        if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
            &sdp)) {
            RTC_LOG(WARNING) << "Can't parse received session description message.";
            return;
        }
        webrtc::SdpParseError error;
        std::unique_ptr<webrtc::SessionDescriptionInterface> session_description =
            webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp, &error);
        if (!session_description) {
            RTC_LOG(WARNING) << "Can't parse received session description message. "
                "SdpParseError was: "
                << error.description;
            return;
        }
        //RTC_LOG(INFO) << " Received session description :" << message;
        peer_connection_->SetRemoteDescription(
            DummySetSessionDescriptionObserver::Create(),
            session_description.release());
        if (type == webrtc::SdpType::kOffer) {
            //peer_connection_->CreateAnswer(
           //     conductor, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
        }
    }
}

void QtWidgetsTest::connect_candidate_load()
{
    QString remote_candidate = textEdit_remote_candidate_text->toPlainText();

    std::string str_remote_candidate = remote_candidate.toStdString();

    Json::Reader reader;
    Json::Value jmessage;
    if (!reader.parse(str_remote_candidate, jmessage)) {
        //RTC_LOG(WARNING) << "Received unknown message. " << message;
        return;
    }

    std::string type_str;
    std::string json_object;

    rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName,
        &type_str);
    if (type_str.empty()) {

        std::string sdp_mid;
        int sdp_mlineindex = 0;
        std::string sdp;
        if (!rtc::GetStringFromJsonObject(jmessage, kCandidateSdpMidName,
            &sdp_mid) ||
            !rtc::GetIntFromJsonObject(jmessage, kCandidateSdpMlineIndexName,
                &sdp_mlineindex) ||
            !rtc::GetStringFromJsonObject(jmessage, kCandidateSdpName, &sdp)) {
            RTC_LOG(WARNING) << "Can't parse received message.";
            return;
        }
        webrtc::SdpParseError error;
        std::unique_ptr<webrtc::IceCandidateInterface> candidate(
            webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
        if (!candidate.get()) {
            RTC_LOG(WARNING) << "Can't parse received candidate message. "
                "SdpParseError was: "
                << error.description;
            return;
        }
        if (!peer_connection_->AddIceCandidate(candidate.get())) {
            RTC_LOG(WARNING) << "Failed to apply the received candidate";
            return;
        }
        //RTC_LOG(INFO) << " Received candidate :" << message;
    }

}

void QtWidgetsTest::on_pushButton_local_remote_connect_clicked()
{
    connect_sdp_load();
    connect_candidate_load();
}


bool QtWidgetsTest::if_check_desktop()
{
    if (comboBox_cocal_camera_check_num == comboBox_cocal_camera_check_num_max) {
        return true;
    }
    else {
        return false;
    }
}

void QtWidgetsTest::webrtc_init()
{
    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory_->CreateAudioTrack(
            kAudioLabel, peer_connection_factory_->CreateAudioSource(
                cricket::AudioOptions())));

    auto result_or_error = peer_connection_->AddTrack(audio_track, { kStreamId });
    if (!result_or_error.ok()) {
        RTC_LOG(LS_ERROR) << "Failed to add audio track to PeerConnection: "
            << result_or_error.error().message();
    }

    if (if_check_desktop()) {
        rtc::scoped_refptr<localCapturer> video_device = new rtc::RefCountedObject<localCapturer>();
        video_device->startCapturer();
        rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_(
            peer_connection_factory_->CreateVideoTrack(kVideoLabel, video_device));

        result_or_error = peer_connection_->AddTrack(video_track_, { kStreamId });
        if (!result_or_error.ok()) {
            RTC_LOG(LS_ERROR) << "Failed to add video track to PeerConnection: "
                << result_or_error.error().message();
        }
    }
    else {
        rtc::scoped_refptr<CapturerTrackSource> video_device = 
            CapturerTrackSource::Create(comboBox_cocal_camera_check_num);

        //const size_t kWidth = 1080;
        //const size_t kHeight = 720;
        //const size_t kFps = 30;
        //std::unique_ptr<webrtc_demo::VcmCapturerTest> video_device = webrtc_demo::VcmCapturerTest::Create(kWidth, kHeight, kFps, comboBox_cocal_camera_check_num);

        rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_(
            peer_connection_factory_->CreateVideoTrack(kVideoLabel, video_device));
        BaseVideoSubscriber* videoSub = nullptr;
        videoSub = new BaseVideoSubscriber();
        videoSub->setUi(this,true);

        video_track_->AddOrUpdateSink(videoSub, rtc::VideoSinkWants());

        result_or_error = peer_connection_->AddTrack(video_track_, { kStreamId });
        if (!result_or_error.ok()) {
            RTC_LOG(LS_ERROR) << "Failed to add video track to PeerConnection: "
                << result_or_error.error().message();
        }
    }
    conductor->SetPeerConnection(peer_connection_);
    peer_connection_->CreateOffer(
        conductor, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());

}

void BaseVideoSubscriber::OnFrame(const webrtc::VideoFrame& frame)
{
    RTC_LOG(LS_INFO) << "VideoSubscriber::OnFrame=" << frame.video_frame_buffer()->type();
#if 1
    int height = frame.height();
    int width = frame.width();

    RTC_LOG(LS_INFO) << "VideoSubscriber::heigth=" << height << "  width=" << width;

    rtc::scoped_refptr<webrtc::I420BufferInterface> i420_buffer = frame.video_frame_buffer()->ToI420();

    QImage::Format imageFormat = QImage::Format_Invalid;
    const int length = frame.width() * frame.height() * 3;
    RTC_LOG(LS_INFO) << "VideoSubscriber::length=" << length;

    uchar* imageBuffer = new uchar[length];

    if (frame.video_frame_buffer()->type() == webrtc::VideoFrameBuffer::Type::kI420) {

        libyuv::I420ToRGB24(
            i420_buffer->DataY(),
            i420_buffer->StrideY(),
            i420_buffer->DataU(),
            i420_buffer->StrideU(),
            i420_buffer->DataV(),
            i420_buffer->StrideV(),
            imageBuffer,
            frame.width() * 3,
            frame.width(),
            frame.height()
        );

        QImage image(imageBuffer, frame.width(), frame.height(), QImage::Format::Format_RGB888);

        QTransform transform;
        transform.scale(0.5, 0.5);
        image = image.transformed(transform);

        QPixmap pixmap = QPixmap::fromImage(image);

        if (ui_is_local) {
            ui->label_local_camera_show->setPixmap(pixmap);
            ui->label_local_camera_show->show();
        }else{
            ui->label_remote_camera_show->setPixmap(pixmap);
            ui->label_remote_camera_show->show();
        }
    }
#endif
}

void BaseVideoSubscriber::setUi(QtWidgetsTest* widget, bool is_local)
{
    ui = widget;
    ui_is_local = is_local;
}