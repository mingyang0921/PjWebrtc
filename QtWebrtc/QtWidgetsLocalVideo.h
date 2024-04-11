#pragma once

#include <QWidget>
#include "ui_QtWidgetsLocalVideo.h"

#undef emit
#include "localCapturer.h"
#include "rtc_base/ref_counted_object.h"
#include "api/create_peerconnection_factory.h"
#define emit Q_EMIT

class QtWidgetsLocalVideo : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsLocalVideo(QWidget *parent = nullptr);
	~QtWidgetsLocalVideo();

public:
	int init();

private:
	Ui::QtWidgetsLocalVideoClass ui;
	rtc::scoped_refptr<localCapturer> video_device;
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
		peer_connection_factory_;
};
