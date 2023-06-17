#include "View_CarIdentifyInfoDisplay.h"
#include "Model_CarIdentifyInfoDisplay.h"
View_CarIdentifyInfoDisplay::View_CarIdentifyInfoDisplay(QWidget* parent)
{
	this->init();
}

View_CarIdentifyInfoDisplay::~View_CarIdentifyInfoDisplay()
{
}

void View_CarIdentifyInfoDisplay::setModel(QObject& handle)
{
	this->_pModel_CarIdentifyInfoDisplay = qobject_cast<Model_CarIdentifyInfoDisplay*>(&handle);
}

void View_CarIdentifyInfoDisplay::setCSS(QPalette& palette)
{


	this->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
	this->setPalette(palette);
	this->setStyleSheet(QString("%1%2%3%4").arg("#hiht {background-color:rgb(204, 255, 255);}")
		.arg(" #info {background-color:rgb(153, 204, 255);}")
		.arg("#info{ background - color:rgb(153, 204, 255); }")
		.arg("#layout{ background - color:rgb(102, 102, 153); }"));

	this->_pLabelCarPlate_hint->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));
	this->_pLabelSoundPw_hint->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));
	this->_pLabelCarLaneInfo_hint->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));

	this->_pLabelCarPlate->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));
	this->_pLabelSoundPw->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));
	this->_pLabelCarLaneInfo->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, 1));

}

void View_CarIdentifyInfoDisplay::setCarPlate(QString plate)
{
	this->_pLabelCarPlate->setText(plate);
}

void View_CarIdentifyInfoDisplay::setSoundPw(double pw)
{
	this->_pLabelSoundPw->setText(QString::number(pw, 'f', 16));
}

void View_CarIdentifyInfoDisplay::setCarLaneInfo(QString carLaneinfo)
{
	this->_pLabelCarLaneInfo->setText(carLaneinfo);
}

void View_CarIdentifyInfoDisplay::initMembers()
{
	this->_pMainLayout_Grid = new QGridLayout;

	this->_pLabelCarPlate_hint = new QLabel(QString::fromLocal8Bit("车牌信息:"));
	this->_pLabelSoundPw_hint = new QLabel(QString::fromLocal8Bit("声功率信息:"));
	this->_pLabelCarLaneInfo_hint = new QLabel(QString::fromLocal8Bit("车道信息:"));

	this->_pMainLayout_Grid->setObjectName("layout");
	this->_pLabelCarPlate_hint->setObjectName("hiht");
	this->_pLabelSoundPw_hint->setObjectName("hiht");
	this->_pLabelCarLaneInfo_hint->setObjectName("hiht");


	this->_pLabelCarPlate = new QLabel(QString::fromLocal8Bit("1:"));
	this->_pLabelSoundPw = new QLabel(QString::fromLocal8Bit("2:"));
	this->_pLabelCarLaneInfo = new QLabel(QString::fromLocal8Bit("1:"));

	this->_pLabelCarPlate->setObjectName("info");
	this->_pLabelSoundPw->setObjectName("info");
	this->_pLabelCarLaneInfo->setObjectName("info");
}

void View_CarIdentifyInfoDisplay::initLayout()
{

	this->setLayout(this->_pMainLayout_Grid);


	this->_pMainLayout_Grid->addWidget(this->_pLabelCarPlate_hint, 0, 0,1,1);
	this->_pMainLayout_Grid->addWidget(this->_pLabelSoundPw_hint, 1, 0, 1, 1);
	this->_pMainLayout_Grid->addWidget(this->_pLabelCarLaneInfo_hint, 2, 0, 1, 1);

	this->_pMainLayout_Grid->addWidget(this->_pLabelCarPlate, 0, 2, 1, 1);
	this->_pMainLayout_Grid->addWidget(this->_pLabelSoundPw, 1, 2, 1, 1);
	this->_pMainLayout_Grid->addWidget(this->_pLabelCarLaneInfo, 2, 2, 1, 1);


}

void View_CarIdentifyInfoDisplay::init()
{
	this->initMembers();
	this->initLayout();
}
