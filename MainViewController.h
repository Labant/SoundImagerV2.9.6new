#pragma once
#include <qwidget.h>

class MainViewControl;
class MainModel;

class MainViewController :public QWidget
{
	Q_OBJECT
public:
	MainViewController(QWidget* parent = nullptr);
	~MainViewController();

private:
	MainViewControl* _pVC = nullptr;
	MainModel* _pMM = nullptr;
};

