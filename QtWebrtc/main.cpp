#include "QtWebrtc.h"
#include <QtWidgets/QApplication>

#undef emit
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#define emit Q_EMIT

int main(int argc, char *argv[])
{

    rtc::WinsockInitializer winsock_init;
    rtc::Win32SocketServer w32_ss;
    rtc::Win32Thread w32_thread(&w32_ss);
    rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);

    QApplication a(argc, argv);
    QtWebrtc w;
    w.show();
    return a.exec();
}
