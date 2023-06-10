#pragma once
#include <qwidget.h>
class BaseWidget :public QWidget
{
public:
	BaseWidget(QWidget* w = nullptr);
	~BaseWidget();

protected:
	virtual void initLayout() = 0;
	virtual void initConnect() = 0;

};

