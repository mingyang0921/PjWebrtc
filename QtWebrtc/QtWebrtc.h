#pragma once

#include <QtWidgets/QWidget>

class QtWebrtc : public QWidget
{
    Q_OBJECT

public:
    QtWebrtc(QWidget *parent = nullptr);
    ~QtWebrtc();

private slots:
    void on_pushButton_desktop_clicked();
    void on_pushButton_camera_clicked();
    void on_pushButton_sdp_clicked();
public:
    int init_base();
};
