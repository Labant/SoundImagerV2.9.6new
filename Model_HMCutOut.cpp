#include "Model_HMCutOut.h"
#include "View_HMCutOut.h"
#include <QDebug>

Model_HMCutOut::Model_HMCutOut(View_HMCutOut* hendle,QObject* parent)
{
	this->_pView_HMCutOut = hendle;
	this->init();
}

Model_HMCutOut::~Model_HMCutOut()
{
}

void Model_HMCutOut::setView(View_HMCutOut* handle)
{
	this->_pView_HMCutOut = handle;
}

void Model_HMCutOut::initMember()
{

}

void Model_HMCutOut::initConnect()
{
	//X�������뻬��ͬ��
	connect(this->_pView_HMCutOut->_pLineEdit[0], &QLineEdit::textChanged, this, &Model_HMCutOut::On_setXAxisSidlerValue);
	//Y�������뻬��ͬ��
	connect(this->_pView_HMCutOut->_pLineEdit[1], &QLineEdit::textChanged, this, &Model_HMCutOut::On_setYAxisSidlerValue);

	//������X������ͬ��
	connect(this->_pView_HMCutOut->_pSlider[0], &QAbstractSlider::valueChanged, this, &Model_HMCutOut::On_setSidlerXAxisValue);
	//������Y������ͬ��
	connect(this->_pView_HMCutOut->_pSlider[1], &QAbstractSlider::valueChanged, this, &Model_HMCutOut::On_setSidlerYAxisValue);

	//����ͼ��ֵ
	connect(this->_pView_HMCutOut->_pHmthresholdValue, &QLineEdit::returnPressed, this, &Model_HMCutOut::on_setHmThresholdValue);
	//��ʼ����ֵ��ʾ
	this->on_setHmThresholdValue();
}

void Model_HMCutOut::init()
{
	this->initMember();
	this->initConnect();
}

void Model_HMCutOut::On_setXAxisSidlerValue(QString value)
{
	this->_pView_HMCutOut->_pSlider[0]->setValue(value.toUInt());
}

void Model_HMCutOut::On_setYAxisSidlerValue(QString value)
{
	this->_pView_HMCutOut->_pSlider[1]->setValue(value.toUInt());
}

void Model_HMCutOut::On_setSidlerXAxisValue(int value)
{
	this->_pView_HMCutOut->_pLineEdit[0]->setText(QString::number(value));
	emit this->xAxisCutOutChanged((quint16)value);
}

void Model_HMCutOut::On_setSidlerYAxisValue(int value)
{
	this->_pView_HMCutOut->_pLineEdit[1]->setText(QString::number(value));
	emit this->yAxisCutOutChanged((quint16)value);
}

void Model_HMCutOut::on_setHmThresholdValue()
{
	qDebug() << "this->_pView_HMCutOut->_pHmthresholdValue->text().toDouble():" << this->_pView_HMCutOut->_pHmthresholdValue->text().toDouble();
	emit this->on_signal_thresholdChange(this->_pView_HMCutOut->_pHmthresholdValue->text().toDouble());
}
