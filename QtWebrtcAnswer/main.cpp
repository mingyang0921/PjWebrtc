#include "QtWebrtcAnswer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWebrtcOffer w;
    w.show();
    return a.exec();
}
