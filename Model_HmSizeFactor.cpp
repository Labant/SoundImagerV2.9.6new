#include "Model_HmSizeFactor.h"
#include "View_HmSizeFactor.h"
#include <QDebug>

Model_HmSizeFactor::Model_HmSizeFactor(View_HmSizeFactor* handle, QObject* parent)
{
	this->_pView_HmSizeFactor = handle;
	this->init();
}

Model_HmSizeFactor::~Model_HmSizeFactor()
{
}

void Model_HmSizeFactor::setView(View_HmSizeFactor* handle)
{
	this->_pView_HmSizeFactor = handle;
}

void Model_HmSizeFactor::initMember()
{
}

void Model_HmSizeFactor::initConnect()
{
	//热力图伸缩因子改变
	connect(this->_pView_HmSizeFactor->_pSlider, &QAbstractSlider::valueChanged, this, &Model_HmSizeFactor::On_GetFactorValue);
	
	//初始化 热力图大小factor
	emit this->On_Signal_FactoeValue(this->_pView_HmSizeFactor->_pSlider->value());
}

void Model_HmSizeFactor::init()
{
	this->initMember();
	this->initConnect();
}

void Model_HmSizeFactor::On_GetFactorValue(unsigned short value)
{
	emit this->On_Signal_FactoeValue(value);
}
