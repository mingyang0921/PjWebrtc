#include "QtWebrtc.h"

#undef emit
#include "conductor.h"
#define emit Q_EMIT

QtWebrtc::QtWebrtc(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    init_base();
}

QtWebrtc::~QtWebrtc()
{}

int QtWebrtc::init_base()
{
    PeerConnectionClient client;
    rtc::scoped_refptr<Conductor> conductor(
        new rtc::RefCountedObject<Conductor>(&client));

    if (!conductor->InitializePeerConnection()) {
        RTC_LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
        //client->SignOut();
        return 0;
    }
    return 0;
}