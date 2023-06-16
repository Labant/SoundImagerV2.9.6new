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
	//��������з������¼�
	bool eventFilter(QObject* obj, QEvent* ev) override;
public:
	QMatrix4x4 m_model;
	//�ƶ��˶���
	QPoint m_posMany;
	//��ʱ
	QPoint m_posTemp;
	//���Ŀǰλ��
	QPoint m_pomoving;
	//��꿪ʼλ��
	QPoint m_posStart;
	//�ƶ���
	bool isMove = 0;
	//����Y����һ�����ݣ����������껬������Ч��,��һ��Ϊ��һ�Σ��ڶ���Ϊ����
	QPoint mYAxisKeepData;

private:
	float m_angle_xAxis = 0.0;
	float m_angle_yAxis = 0.0;
	float m_moveSpeed = 0.1;
};

