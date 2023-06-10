#include "Model_filteringSetting.h"
#include <QDebug>

Model_filteringSetting::Model_filteringSetting(View_filteringSetting* _handel, QObject* parent)
{
	_pView_filteringSetting = _handel;
	this->init();
}

Model_filteringSetting::~Model_filteringSetting()
{
}

void Model_filteringSetting::setView(View_filteringSetting* _handel)
{
	this->_pView_filteringSetting = _handel;
}

float Model_filteringSetting::getFiltLow()
{
	return this->_pView_filteringSetting->_pLELow->text().toDouble() * 1000.0f;
}

float Model_filteringSetting::getFiltHigh()
{
	return this->_pView_filteringSetting->_pLEHigh->text().toDouble() * 1000.0f;
}

void Model_filteringSetting::setBtnSwitchText(int _flag)
{

	if (this->_pView_filteringSetting != nullptr)
	{
		this->_pView_filteringSetting->_pPushButtonSure->setText(_flag > 0 ? QString::fromLocal8Bit("¹Ø±ÕÂË²¨") : QString::fromLocal8Bit("ÉèÖÃÂË²¨"));
	}
}

void Model_filteringSetting::initMember()
{

}

void Model_filteringSetting::initConnect()
{
	connect(this->_pView_filteringSetting->_pPushButtonSure, &QPushButton::clicked, this, &Model_filteringSetting::on_slot_filtBtn_clicked);
	////filt btn on_Click
	//connect(this->_pModel_filteringSetting, &Model_filteringSetting::on_signal_filtBtn_clicked, this, [=]() {
	//	isFilt = !isFilt;
	//if (isFilt)
	//{
	//	filt_low = this->_pModel_filteringSetting->getFiltLow();
	//	filt_hight = this->_pModel_filteringSetting->getFiltHigh();
	//	qDebug() << "filt_low:" << filt_low;
	//	qDebug() << "filt_hight:" << filt_hight;
	//	this->_pModel_filteringSetting->setBtnSwitchText(1);
	//}
	//else
	//{
	//	this->_pModel_filteringSetting->setBtnSwitchText(0);
	//}
	//	}, Qt::DirectConnection);
}

void Model_filteringSetting::init()
{
	this->initMember();
	this->initConnect();
}

void Model_filteringSetting::on_slot_filtBtn_clicked()
{
	emit this->on_signal_filtBtn_clicked();
}
