#include "QtWidgetsLocalVideo.h"

#undef emit
#include "demo_desktop_capture.h"
#include "test/video_renderer.h"
#include "rtc_base/logging.h"
#include <thread>

#define emit Q_EMIT

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

QtWidgetsLocalVideo::QtWidgetsLocalVideo(QWidget *parent)
	: QWidget(parent)
{
	capturer = nullptr;

	QVBoxLayout* pVlay = new QVBoxLayout(this);
	QHBoxLayout* hVlay = new QHBoxLayout();

	QLabel* label = new QLabel();
	pVlay->addWidget(label);

	QPushButton* buttonBegin = new QPushButton();
	buttonBegin->setText(u8"¿ªÊ¼");
	connect(buttonBegin, &QPushButton::clicked, this, &QtWidgetsLocalVideo::on_pushButton_clicked_begin);

	QPushButton* buttonEnd = new QPushButton();
	buttonEnd->setText(u8"½áÊø");
	connect(buttonEnd, &QPushButton::clicked, this, &QtWidgetsLocalVideo::on_pushButton_clicked_end);

	hVlay->addWidget(buttonBegin);
	hVlay->addWidget(buttonEnd);
	pVlay->addLayout(hVlay);
}

QtWidgetsLocalVideo::~QtWidgetsLocalVideo()
{

}

void QtWidgetsLocalVideo::on_pushButton_clicked_begin()
{
	if (capturer) {
		capturer->StopCapture();
		delete capturer;
		capturer = nullptr;
	}
	capturer = webrtc_demo::DesktopCapture::Create(15, 0);
	capturer->StartCapture();
}
void QtWidgetsLocalVideo::on_pushButton_clicked_end()
{
	if(capturer)
		capturer->StopCapture();
	capturer = nullptr;
}
