#include "QtWebrtc.h"

#include "QtWidgetsLocalVideo.h"

//#undef emit
//#include "conductor.h"
//#define emit Q_EMIT

QtWebrtc::QtWebrtc(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QtWidgetsLocalVideo* local_video = new QtWidgetsLocalVideo();
    local_video->show();
    //init_base();
}

QtWebrtc::~QtWebrtc()
{}

int QtWebrtc::init_base()
{
#if 0
    PeerConnectionClient client;
    rtc::scoped_refptr<Conductor> conductor(
        new rtc::RefCountedObject<Conductor>(&client));

    if (!conductor->InitializePeerConnection()) {
        RTC_LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
        //client->SignOut();
        return 0;
    }
    //conductor->peer_connection_->CreateOffer(
    //    this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
#endif
    return 0;
}