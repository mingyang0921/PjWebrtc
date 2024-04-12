#pragma once

#include <QWidget>

#undef emit
#include "demo_desktop_capture.h"
#define emit Q_EMIT

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
};
