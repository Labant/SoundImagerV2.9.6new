#include "ObServerationAngle.h"
#include <QOpenGLWidget>
#include <QMouseEvent>
ObServerationAngle::ObServerationAngle(QObject* p)
{
}

ObServerationAngle::~ObServerationAngle()
{
}

//��������¼�
bool ObServerationAngle::eventFilter(QObject* obj, QEvent* ev)
{
	////Qt openGl �汾
	//auto _widget = qobject_cast<QOpenGLWidget*>(obj);
	//auto event = static_cast<QMouseEvent*>(ev);

	//Pure OpenGL�汾
	auto _widget = qobject_cast<QWidget*>(obj);
	auto event = static_cast<QMouseEvent*>(ev);

	static int i = 1;
	//�����
	if (event->type() == QEvent::MouseButtonPress)
	{
		m_posStart = event->pos();

		//if (i == 1)
		//{
		//	m_posStart = event->pos();
		//	//qDebug() << "m_posStart:" << m_posStart;
		//	i += 1;
		//}
		//else
		//{
		//	//m_posStart = m_posMany;

		//}

	}

	//����ƶ�
	if (event->type() == QEvent::MouseMove)
	{
		isMove = 1;

		////��һ������
		//mYAxisKeepData.setX(m_pomoving.ry());

		//�ƶ��Ĺ���
		m_pomoving = event->pos();
		//qDebug() << "m_pomoving:" << m_pomoving;
		//qDebug() << "m_posStart:" << m_posStart;
		//qDebug() << "m_pomoving - m_posStart:" << m_pomoving - m_posStart;

		////��һ������
		//mYAxisKeepData.setY(m_pomoving.ry());

		//�ƶ��Ķ���
		m_posTemp = m_pomoving - m_posStart;

		////�ж��Ƿ�����
		//if (m_posTemp.ry() < 0)
		//{
		//	m_posStart = m_pomoving;
		//	m_posTemp = m_pomoving - m_posStart;
		//}

		//qDebug() << "m_posTemp:" << m_posTemp;

		//��ӽ����ƶ�
		//if (m_posMany.ry() > 360)
		//{
		//	m_posMany.setY(0);
		//}
		m_posMany += m_posTemp;
		//qDebug() << "m_posMany:" << m_posMany;

		//��ʼ��λ���Ƶ��������
		//m_posStart = m_pomoving;

		////��X���������ƶ�
		//if (m_posMany.x() > 0 && m_posMany.x() != 0)
		//{
		//	m_angle_xAxis += m_posMany.x();
		//	if (m_angle_xAxis >= 360)
		//		m_angle_xAxis = 0;
		//}
		//else
		//{////��X�Ḻ�����ƶ�
		//	m_angle_xAxis += m_posMany.x();
		//	if (m_angle_xAxis <= -360)
		//		m_angle_xAxis = 0;
		//}
		////��Y���������ƶ�
		//if (m_posMany.y() > 0 && m_posMany.y() != 0)
		//{
		//	m_angle_yAxis += m_posMany.y();
		//	//qDebug() << "m_angle_yAxis:" << m_angle_yAxis;
		//	if (m_angle_yAxis >= 360)
		//		m_angle_yAxis = 0;
		//}
		//else
		//{//��Y�Ḻ�����ƶ�
		//	m_angle_yAxis += m_posMany.y();
		//	if (m_angle_yAxis <= -360)
		//		m_angle_yAxis = 0;
		//}
		
		//Qt openGl�汾
		//m_model.setToIdentity();
		//m_model.rotate(-m_angle_xAxis, 0, 1, 0);//m_angle m_posMany.y()
		//m_model.rotate(-m_angle_yAxis, 1, 0, 0);// * m_moveSpeed

	}
	isMove = 0;
	//qDebug() << "m_posMany:" << m_posMany;
	return false;
}
