#include "QtWebrtc.h"

#include "QtWidgetsLocalVideo.h"
#include "QtWidgetsCamera.h"
#include "QtWidgetsSdp.h"
#include "QtWidgetsTest.h"

#include <QPushButton>
#include <QVBoxLayout>

QtWebrtc::QtWebrtc(QWidget *parent)
    : QWidget(parent)
{
    resize(400,400);
    init_base();
}

QtWebrtc::~QtWebrtc()
{}

int QtWebrtc::init_base()
{
    QVBoxLayout* pVlay = new QVBoxLayout(this);

    QPushButton* button_desktop = new QPushButton();
    button_desktop->setText(u8"屏幕显示");
    connect(button_desktop, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_desktop_clicked);
    pVlay->addWidget(button_desktop);

    QPushButton* button_camera = new QPushButton();
    button_camera->setText(u8"摄像头显示");
    connect(button_camera, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_camera_clicked);
    pVlay->addWidget(button_camera);

    QPushButton* button_sdp = new QPushButton();
    button_sdp->setText(u8"获取sdp");
    connect(button_sdp, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_sdp_clicked);
    pVlay->addWidget(button_sdp);

    QPushButton* button_connect = new QPushButton();
    button_connect->setText(u8"连接测试");
    connect(button_connect, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_test_clicked);
    pVlay->addWidget(button_connect);

    return 0;
}

void QtWebrtc::on_pushButton_desktop_clicked()
{
    QtWidgetsLocalVideo* local_video = new QtWidgetsLocalVideo();
    local_video->show();
}

void QtWebrtc::on_pushButton_camera_clicked()
{
    QtWidgetsCamera* local_camera = new QtWidgetsCamera();
    local_camera->show();
}

void QtWebrtc::on_pushButton_sdp_clicked()
{
    QtWidgetsSdp* local_sdp = new QtWidgetsSdp();
    local_sdp->show();
}

void QtWebrtc::on_pushButton_test_clicked()
{
    QtWidgetsTest* connect_test = new QtWidgetsTest();
    connect_test->show();
}