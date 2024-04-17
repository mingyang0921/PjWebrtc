#pragma once

#include <QWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#undef emit
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "PeerConnectionCallback.h"
#include "media/base/video_broadcaster.h"
#define emit Q_EMIT

class QtWidgetsTest;

class BaseVideoSubscriber : public rtc::VideoBroadcaster {
public:
	void OnFrame(const webrtc::VideoFrame& frame) override;
	void setUi(QtWidgetsTest* widget,bool is_local);
public:
	QtWidgetsTest* ui;
	bool ui_is_local;
};


class QtWidgetsTest : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsTest(QWidget *parent = nullptr);
	~QtWidgetsTest();

public:
	bool if_check_desktop();

private slots:
	void on_pushButton_makesure_clicked();
	void on_pushButton_start_localcamera_clicked();
	void on_pushButton_showsdp_localcamera_clicked();
private:
	void initUI();
	void checkCamera();
	void webrtc_init();

private:
	int comboBox_cocal_camera_check_num_max = 0;
	int comboBox_cocal_camera_check_num=0;

private:
	QComboBox* comboBox_cocal_camera_check;
	QTextEdit* textEdit_local_sdp_text;
	QTextEdit* textEdit_remote_sdp_text;
public:
	QLabel* label_local_camera_show;
	QLabel* label_remote_camera_show;
private:
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>peer_connection_factory_;
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
	rtc::scoped_refptr<ConductorCallback> conductor;

};
