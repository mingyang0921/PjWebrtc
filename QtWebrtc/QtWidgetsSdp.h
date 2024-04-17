#pragma once

#include <QWidget>

#undef emit
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "PeerConnectionCallback.h"
#include "conductor.h"
#define emit Q_EMIT


class QtWidgetsSdp : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsSdp(QWidget *parent = nullptr);
	~QtWidgetsSdp();

private:
	void init();
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
		peer_connection_factory_;
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
	rtc::scoped_refptr<ConductorCallback> conductor;
	rtc::scoped_refptr<Conductor> conductornew;
};
