#pragma once

#include <QWidget>

class QtWidgetsCamera : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsCamera(QWidget *parent = nullptr);
	~QtWidgetsCamera();

private:
	void init();
};
