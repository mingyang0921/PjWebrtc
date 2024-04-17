#pragma once

#include <QWidget>

#undef emit
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "PeerConnectionCallback.h"
#include "conductor.h"
#define emit Q_EMIT

#include <QTextEdit>

class QtWidgetsSdp : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsSdp(QWidget *parent = nullptr);
	~QtWidgetsSdp();

private slots:
	void on_pushButton_clicked();

private:
	void init();
	void initUi();
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
		peer_connection_factory_;
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
	rtc::scoped_refptr<ConductorCallback> conductor;
	//ConductorCallback* conductor=nullptr;
	rtc::scoped_refptr<Conductor> conductornew;

private:
	QTextEdit* textEdit;
};
