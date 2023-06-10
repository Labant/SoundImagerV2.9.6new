#pragma once
#include <QCameraViewfinder>
#include "qcustomplot.h"
#include <QWidget>
#include <QLabel>
// public QCameraViewfinder , 
class Custom_Box :public QCustomPlot
{
	Q_OBJECT
public:
	Custom_Box();
	Custom_Box(QWidget*);
	~Custom_Box();

};

