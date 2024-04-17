
#include "QtWidgetsCamera.h"

#undef emit
#include "modules/video_capture/video_capture_factory.h"
#define emit Q_EMIT

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "demo_camera_capture.h"

QtWidgetsCamera::QtWidgetsCamera(QWidget *parent)
	: QWidget(parent)
{
    init();
}

QtWidgetsCamera::~QtWidgetsCamera()
{}

void QtWidgetsCamera::init()
{
    this->resize(800, 600);
    QHBoxLayout* horizontalLayout;
    QLabel* label;
    QVBoxLayout* verticalLayout;
    QSpacerItem* verticalSpacer;

    QPushButton* pushButton;
    QSpacerItem* verticalSpacer_2;
    //horizontalLayoutWidget->setGeometry(QRect(10, 0, 771, 601));

    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMinimumSize(QSize(680, 0));

    horizontalLayout->addWidget(label);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    comboBox = new QComboBox(this);
    comboBox->setObjectName(QString::fromUtf8("comboBox"));
    comboBox->setMaximumSize(QSize(150, 16777215));

    verticalLayout->addWidget(comboBox);

    pushButton = new QPushButton(this);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setMaximumSize(QSize(100, 16777215));

    connect(pushButton, &QPushButton::clicked, this, &QtWidgetsCamera::on_pushButton_clicked);

    verticalLayout->addWidget(pushButton);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer_2);

    horizontalLayout->addLayout(verticalLayout);

    webrtc::VideoCaptureModule::DeviceInfo
        * device = webrtc::VideoCaptureFactory::CreateDeviceInfo();
    int deviceNum = device->NumberOfDevices();

    for (int i = 0; i < deviceNum; ++i)
    {
        const uint32_t kSize = 256;
        char name[kSize] = { 0 };
        char id[kSize] = { 0 };
        if (device->GetDeviceName(i, name, kSize, id, kSize) != -1)
        {
            //qDebug() << "name" << QString(name) << "id" << QString(id);
            comboBox->addItem(QString(name));

        }
    }

    if (deviceNum == 0)
    {
        pushButton->setEnabled(false);
    }

}

//处理点击事件，打开或关闭摄像头
void QtWidgetsCamera::on_pushButton_clicked()
{
    const size_t kWidth = 1080;
    const size_t kHeight = 720;
    const size_t kFps = 30;

    //std::unique_ptr<webrtc_demo::VcmCapturerTest> capturer =  

        webrtc_demo::VcmCapturerTest * capturer =
        webrtc_demo::VcmCapturerTest::Create(kWidth, kHeight, kFps, 2);

    //rtc::scoped_refptr<webrtc_demo::VcmCapturerTest> capturer = 
    //    new rtc::RefCountedObject<localCapturer>();

#if 0
    static bool flag = true;
    if (flag)
    {

        webrtc::VideoCaptureModule::DeviceInfo
            * device_info_ = webrtc::VideoCaptureFactory::CreateDeviceInfo();

        char device_name[256];
        char unique_name[256];

        if (device_info_->GetDeviceName(comboBox->currentIndex(), device_name, 256,
            unique_name, 256) != 0)
        {
            qDebug() << "info->GetDeviceName error";
            return;
        }

        m_module =
            webrtc::VideoCaptureFactory::Create(unique_name);
        if (m_module.get() == NULL)
        {
            qDebug() << "webrtc::VideoCaptureFactory::Create error";
            return;
        }

        m_module->RegisterCaptureDataCallback(&m_captureObserver);

        device_info_->GetCapability(m_module->CurrentDeviceName(), 0, m_capability);


        //开始捕捉
        if (m_module->StartCapture(m_capability) != 0)
        {
            qDebug() << "m_module->StartCapture failed";
            return;
        }


        if (m_module->CaptureStarted())
        {
            qDebug() << "Capture is running";
        }

        m_timer.start(40);
        ui->pushButton->setText(tr("关闭"));
    }
    else
    {
        ui->pushButton->setText(tr("打开"));
        m_timer.stop();
        //重复连接会报错，需要先断开，才能再次连接
        m_module->StopCapture();
        if (!m_module->CaptureStarted())
        {
            qDebug() << "Capture is stoped";
        }
        ui->label->clear();
    }
    flag = !flag;
#endif
}