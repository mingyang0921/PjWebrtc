#include "QtWebrtc.h"

#include "QtWidgetsLocalVideo.h"
#include "QtWidgetsCamera.h"

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
    button_desktop->setText(u8"ÆÁÄ»Â¼ÖÆ");
    connect(button_desktop, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_desktop_clicked);
    pVlay->addWidget(button_desktop);

    QPushButton* button_camera = new QPushButton();
    button_camera->setText(u8"ÉãÏñÍ·");
    connect(button_camera, &QPushButton::clicked, this, &QtWebrtc::on_pushButton_camera_clicked);
    pVlay->addWidget(button_camera);

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
