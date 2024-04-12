#include "QtWidgetsLocalVideo.h"

#undef emit
#include "demo_desktop_capture.h"
#include "test/video_renderer.h"
#include "rtc_base/logging.h"
#include <thread>

#define emit Q_EMIT

QtWidgetsLocalVideo::QtWidgetsLocalVideo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	start();
}

QtWidgetsLocalVideo::~QtWidgetsLocalVideo()
{

}

int QtWidgetsLocalVideo::start()
{
	std::unique_ptr<webrtc_demo::DesktopCapture> capturer(webrtc_demo::DesktopCapture::Createx(15, 0));
	capturer->StartCapture();

	//std::unique_ptr<webrtc::test::VideoRenderer> renderer(webrtc::test::VideoRenderer::Create(capturer->GetWindowTitle().c_str(), 720, 480));
	//capturer->AddOrUpdateSink(renderer.get(), rtc::VideoSinkWants());

	//std::this_thread::sleep_for(std::chrono::seconds(30));
	//capturer->RemoveSink(renderer.get());
	return 0;
}

int QtWidgetsLocalVideo::stop()
{
	//capturer->RemoveSink(renderer.get());
	return 0;
}
