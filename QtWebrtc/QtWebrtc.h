#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtWebrtc.h"

class QtWebrtc : public QWidget
{
    Q_OBJECT

public:
    QtWebrtc(QWidget *parent = nullptr);
    ~QtWebrtc();
public:
    int init();
private:
    Ui::QtWebrtcClass ui;
};
