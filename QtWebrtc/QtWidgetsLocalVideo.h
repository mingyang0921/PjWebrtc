#pragma once

#include <QWidget>
#include <QLabel>

#undef emit
#include "demo_desktop_capture.h"
#include "api/video/video_frame.h"
#include "api/video/video_source_interface.h"
#include "media/base/video_adapter.h"
#include "media/base/video_broadcaster.h"
#define emit Q_EMIT

class QtWidgetsLocalVideo;

class VideoSubscriber : public rtc::VideoBroadcaster {
public:
	void OnFrame(const webrtc::VideoFrame& frame) override;
	void setUi(QtWidgetsLocalVideo* widget);
public:
	QtWidgetsLocalVideo* ui;
};

class QtWidgetsLocalVideo : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsLocalVideo(QWidget *parent = nullptr);
	~QtWidgetsLocalVideo();

private slots:
	void on_pushButton_clicked_begin();
	void on_pushButton_clicked_end();
private:
	webrtc_demo::DesktopCapture * capturer=nullptr;
	VideoSubscriber* videoSub = nullptr;

public:
	QLabel* label = nullptr;
};
