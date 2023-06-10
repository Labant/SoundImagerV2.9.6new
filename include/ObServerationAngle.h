#pragma once
#include <qobject.h>
#include <QMatrix4x4> 

class ObServerationAngle :public QObject
{
	Q_OBJECT
public:
	ObServerationAngle(QObject* p = nullptr);
	~ObServerationAngle();
protected:
	//监控主类中发生的事件
	bool eventFilter(QObject* obj, QEvent* ev) override;
public:
	QMatrix4x4 m_model;
	//移动了多少
	QPoint m_posMany;
	//临时
	QPoint m_posTemp;
	//鼠标目前位置
	QPoint m_pomoving;
	//鼠标开始位置
	QPoint m_posStart;
	//移动了
	bool isMove = 0;
	//保存Y轴上一次数据，用来解决鼠标滑动逆行效果,第一个为上一次，第二个为本次
	QPoint mYAxisKeepData;

private:
	float m_angle_xAxis = 0.0;
	float m_angle_yAxis = 0.0;
	float m_moveSpeed = 0.1;
};

