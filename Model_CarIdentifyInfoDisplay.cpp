#include "Model_CarIdentifyInfoDisplay.h"
#include "View_CarIdentifyInfoDisplay.h"
Model_CarIdentifyInfoDisplay::Model_CarIdentifyInfoDisplay(QObject* handle)
{
	this->_pView_CarIdentifyInfoDisplay = qobject_cast<View_CarIdentifyInfoDisplay*>(handle);
	this->init();
}

Model_CarIdentifyInfoDisplay::~Model_CarIdentifyInfoDisplay()
{
}

void Model_CarIdentifyInfoDisplay::setView(QObject* handle)
{
	this->_pView_CarIdentifyInfoDisplay = qobject_cast<View_CarIdentifyInfoDisplay*>(handle);
}

void Model_CarIdentifyInfoDisplay::setCarPlate(QString palte)
{
	this->_pView_CarIdentifyInfoDisplay->setCarPlate(palte);
}

void Model_CarIdentifyInfoDisplay::setSoundPw(double dou)
{
	this->_pView_CarIdentifyInfoDisplay->setSoundPw(dou);
}

void Model_CarIdentifyInfoDisplay::setCarLaneInfo(QString lane)
{
	this->_pView_CarIdentifyInfoDisplay->setCarLaneInfo(lane);
}

void Model_CarIdentifyInfoDisplay::initMembers()
{

}

void Model_CarIdentifyInfoDisplay::initConnects()
{
}

void Model_CarIdentifyInfoDisplay::init()
{
	this->initMembers();
	this->initConnects();
}
