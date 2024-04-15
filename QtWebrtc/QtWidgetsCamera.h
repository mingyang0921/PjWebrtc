#pragma once

#include <QWidget>
#include <QComboBox>

class QtWidgetsCamera : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsCamera(QWidget *parent = nullptr);
	~QtWidgetsCamera();

private slots:
	void on_pushButton_clicked();

private:
	void init();

private:
	QComboBox* comboBox;


	//QTimer m_timer;
	//rtc::scoped_refptr<webrtc::VideoCaptureModule> m_module;
	//CaptureObserver m_captureObserver;
	//webrtc::VideoCaptureCapability m_capability;
};
