#pragma once

#include <QWidget>

class QtWidgetsTest : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsTest(QWidget *parent = nullptr);
	~QtWidgetsTest();

private:
	void initUI();
};
