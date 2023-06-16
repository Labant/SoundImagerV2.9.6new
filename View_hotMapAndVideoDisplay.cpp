#include "View_hotMapAndVideoDisplay.h"

View_hotMapAndVideoDisplay::View_hotMapAndVideoDisplay(QWidget* parent)
{
	this->init();
	//this->setBackground(QPixmap::fromImage(QImage("C:/Users/lvs/Pictures/png/9439BD31B85F7377FC79CF2A6BE1FFBB.jpg")), true, Qt::KeepAspectRatio);
}

View_hotMapAndVideoDisplay::~View_hotMapAndVideoDisplay()
{
}

void View_hotMapAndVideoDisplay::initMember()
{
	// set up the QCPColorMap:
	//this->colorMap = new QCPColorMap(this->xAxis, this->yAxis);
	this->colorMap = new QCPColorMap(this->mCustomPlot.xAxis, this->mCustomPlot.yAxis);//���ԣ����ڴ洢�ü���Χ���������ֵ
	this->colorMap->setAntialiased(true);
	//IMX291
	//manual
	this->colorMap->data()->setSize(30 * 2, 30 * 2); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	this->colorMap->data()->setRange(QCPRange(0, 360), QCPRange(0, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ

	////// ���ɫ��:
////QCPColorScale* colorScale = new QCPColorScale(this);
////this->plotLayout()->addElement(0, 1, colorScale); // ������ӵ�������ε��Ҳ�
////colorScale->setType(QCPAxis::atRight); // �̶�ӦΪ��ֱ�����̶���/�������ǩ�Ҳࣨʵ���ϣ��Ҳ��Ѿ���Ĭ��ֵ��
////colorMap->setColorScale(colorScale); // ����ɫͼ��ɫ�����
////colorScale->axis()->setLabel("Magnetic Field Strength");


	//���ϵ���ı��С
	static QCPColorGradient cpColorGradient;
	cpColorGradient.clearColorStops();
	//cpColorGradient.setPeriodic()//������
	//cpColorGradient.setLevelCount(360);//Ĭ��ֵ350
	cpColorGradient.setColorStopAt(0, QColor(Qt::transparent));//͸��
	cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 2.0f - 0.001, QColor(Qt::transparent));//͸��
	//�ռ�ȣ�0.02��0.03��0.01��0.01 ��6��

	//cpColorGradient.setColorStopAt(posColor[0] + this->mHmSizeFactor * 2.0f, hotMapCoefficient > 0 ? QColor(115, 115, 255, 150) : QColor(Qt::transparent));//������ɫ ����ʾ
	cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 3.0f, hotMapCoefficient > 0 ? QColor(77, 255, 255, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(posColor[2] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 255, 0, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(posColor[3] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 71, 26, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(255, 71, 26, 255) : QColor(Qt::transparent));//����ɫ 255
	//cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(102, 20, 0, 150) : QColor(Qt::transparent));//���ɫ ���ÿ�

	//cpColorGradient.setColorStops()
	cpColorGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciHSV);//ciHSV
	colorMap->setGradient(cpColorGradient);

	//test
	//colorMap->setGradient(QCPColorGradient::gpJet);

	// ������������ά�ȣ���ɫ������ʹ�������ݵ㶼λ����ɫ������ʾ�ķ�Χ��:
	this->colorMap->rescaleDataRange(true);

	// �������ż���x����ֵ��y���ᣬ�Ա���Կ���������ɫͼ��
	this->rescaleAxes();
}

void View_hotMapAndVideoDisplay::initLayout()
{
	//����ͼ���񲻿ɼ�
	this->xAxis->grid()->setVisible(false);
	this->yAxis->grid()->setVisible(false);

	//����ͼx y �᲻�ɼ�
	this->xAxis->setVisible(false);
	this->xAxis2->setVisible(false);
	this->yAxis->setVisible(false);
	this->yAxis2->setVisible(false);

	// configure axis rect:���������
	//this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // ��Ҳ������ͨ����ק/���ų߶ȸı���ɫ��Χ
	this->axisRect()->setupFullAxesBox(true);
	this->xAxis->setLabel("x");
	this->yAxis->setLabel("y");
	this->xAxis->setVisible(false);
	this->yAxis->setVisible(false);
	this->xAxis2->setVisible(false);
	this->yAxis2->setVisible(false);
}

void View_hotMapAndVideoDisplay::init()
{
	this->initLayout();
	this->initMember();
}
