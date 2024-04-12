#pragma once

#include <QWidget>
#include "ui_QtWidgetsLocalVideo.h"

#undef emit
#define emit Q_EMIT

class QtWidgetsLocalVideo : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsLocalVideo(QWidget *parent = nullptr);
	~QtWidgetsLocalVideo();

public:
	int start();
	int stop();

private:
	Ui::QtWidgetsLocalVideoClass ui;
};
