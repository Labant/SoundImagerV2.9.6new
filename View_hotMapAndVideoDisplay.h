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
	void on_signal_xAxisRange(unsigned short);//����X��ü���Χ
	void on_signal_yAxisRange(unsigned short);//����Y��ü���Χ

public:

	QCPColorMap* colorMap = nullptr;
protected:
	//��ɫ��ֵ
	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//����ͼ��Сϵ��
	double mHmSizeFactor = (1.0f - (80.0f / 1000.0f + 0.9f)) / 6.0f;
	//double mHmSizeFactor = 1.0f;
	//����ͼ�Ƿ���ʾϵ��������Ϊ1
	float hotMapCoefficient = 1;
private:
	//��������QColorMap
	QCustomPlot mCustomPlot;
};

