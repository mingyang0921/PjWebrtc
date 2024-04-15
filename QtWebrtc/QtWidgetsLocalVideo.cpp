#include "QtWidgetsLocalVideo.h"

#undef emit
#include "demo_desktop_capture.h"
#include "test/video_renderer.h"
#include "rtc_base/logging.h"
#include <thread>
#include "third_party/libyuv/include/libyuv.h"
#include "api/video/i420_buffer.h"
#include "api/video/video_frame.h"
#define emit Q_EMIT

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QImage>

QtWidgetsLocalVideo::QtWidgetsLocalVideo(QWidget *parent)
	: QWidget(parent)
{
	capturer = nullptr;

	QVBoxLayout* pVlay = new QVBoxLayout(this);
	QHBoxLayout* hVlay = new QHBoxLayout();

	label = new QLabel();
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

	videoSub = new VideoSubscriber();
	videoSub->setUi(this);

	capturer->AddOrUpdateSink(videoSub, rtc::VideoSinkWants());
}
void QtWidgetsLocalVideo::on_pushButton_clicked_end()
{
	if (capturer) {
		capturer->RemoveSink(videoSub);
		delete videoSub;
		videoSub = nullptr;
		capturer->StopCapture();
	}
	capturer = nullptr;
}

void VideoSubscriber::OnFrame(const webrtc::VideoFrame& frame) 
{
	RTC_LOG(LS_INFO) << "VideoSubscriber::OnFrame=" << frame.video_frame_buffer()->type();

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

		ui->label->setPixmap(pixmap);
		ui->label->show();

	}

}

void VideoSubscriber::setUi(QtWidgetsLocalVideo* widget)
{
	ui = widget;
}