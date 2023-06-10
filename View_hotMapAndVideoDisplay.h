#pragma once
#include <qwidget.h>
#include <QObject>
#include "qcustomplot.h"
#include "MyStruct.h"


class View_hotMapAndVideoDisplay :public QCustomPlot
{
	Q_OBJECT
public:
	View_hotMapAndVideoDisplay(QWidget* parent = nullptr);
	~View_hotMapAndVideoDisplay();

signals:
	void m_signal_start();
	void m_signal_stop();

protected:
	void initMember();
	void initLayout();
	
private:
	void init();

signals:
	void on_signal_xAxisRange(unsigned short);//更改X轴裁剪范围
	void on_signal_yAxisRange(unsigned short);//更改Y轴裁剪范围

public:

	QCPColorMap* colorMap = nullptr;
protected:
	//颜色阈值
	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//热力图大小系数
	double mHmSizeFactor = (1.0f - (80.0f / 1000.0f + 0.9f)) / 6.0f;
	//double mHmSizeFactor = 1.0f;
	//热力图是否显示系数。基数为1
	float hotMapCoefficient = 1;
private:
	//用于声明QColorMap
	QCustomPlot mCustomPlot;
};

