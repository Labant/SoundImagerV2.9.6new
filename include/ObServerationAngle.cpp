#include "ObServerationAngle.h"
#include <QOpenGLWidget>
#include <QMouseEvent>
ObServerationAngle::ObServerationAngle(QObject* p)
{
}

ObServerationAngle::~ObServerationAngle()
{
}

//监视鼠标事件
bool ObServerationAngle::eventFilter(QObject* obj, QEvent* ev)
{
	////Qt openGl 版本
	//auto _widget = qobject_cast<QOpenGLWidget*>(obj);
	//auto event = static_cast<QMouseEvent*>(ev);

	//Pure OpenGL版本
	auto _widget = qobject_cast<QWidget*>(obj);
	auto event = static_cast<QMouseEvent*>(ev);

	static int i = 1;
	//鼠标点击
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

	//鼠标移动
	if (event->type() == QEvent::MouseMove)
	{
		isMove = 1;

		////上一次数据
		//mYAxisKeepData.setX(m_pomoving.ry());

		//移动的过程
		m_pomoving = event->pos();
		//qDebug() << "m_pomoving:" << m_pomoving;
		//qDebug() << "m_posStart:" << m_posStart;
		//qDebug() << "m_pomoving - m_posStart:" << m_pomoving - m_posStart;

		////这一次数据
		//mYAxisKeepData.setY(m_pomoving.ry());

		//移动的多少
		m_posTemp = m_pomoving - m_posStart;

		////判断是否逆行
		//if (m_posTemp.ry() < 0)
		//{
		//	m_posStart = m_pomoving;
		//	m_posTemp = m_pomoving - m_posStart;
		//}

		//qDebug() << "m_posTemp:" << m_posTemp;

		//添加进总移动
		//if (m_posMany.ry() > 360)
		//{
		//	m_posMany.setY(0);
		//}
		m_posMany += m_posTemp;
		//qDebug() << "m_posMany:" << m_posMany;

		//开始的位置移到最新起点
		//m_posStart = m_pomoving;

		////沿X轴正方向移动
		//if (m_posMany.x() > 0 && m_posMany.x() != 0)
		//{
		//	m_angle_xAxis += m_posMany.x();
		//	if (m_angle_xAxis >= 360)
		//		m_angle_xAxis = 0;
		//}
		//else
		//{////沿X轴负方向移动
		//	m_angle_xAxis += m_posMany.x();
		//	if (m_angle_xAxis <= -360)
		//		m_angle_xAxis = 0;
		//}
		////沿Y轴正方向移动
		//if (m_posMany.y() > 0 && m_posMany.y() != 0)
		//{
		//	m_angle_yAxis += m_posMany.y();
		//	//qDebug() << "m_angle_yAxis:" << m_angle_yAxis;
		//	if (m_angle_yAxis >= 360)
		//		m_angle_yAxis = 0;
		//}
		//else
		//{//沿Y轴负方向移动
		//	m_angle_yAxis += m_posMany.y();
		//	if (m_angle_yAxis <= -360)
		//		m_angle_yAxis = 0;
		//}
		
		//Qt openGl版本
		//m_model.setToIdentity();
		//m_model.rotate(-m_angle_xAxis, 0, 1, 0);//m_angle m_posMany.y()
		//m_model.rotate(-m_angle_yAxis, 1, 0, 0);// * m_moveSpeed

	}
	isMove = 0;
	//qDebug() << "m_posMany:" << m_posMany;
	return false;
}
