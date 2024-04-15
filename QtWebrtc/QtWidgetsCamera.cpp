
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
    QComboBox* comboBox;
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