#include "View_filteringSetting.h"

View_filteringSetting::View_filteringSetting(QWidget* parent)
{
	this->init();
}

View_filteringSetting::~View_filteringSetting()
{
}

void View_filteringSetting::initLayout()
{
	//member
	this->_pPushButtonSure = new QPushButton(QString::fromLocal8Bit("ȷ��"));
	this->_pLabelTxt1 = new QLabel(QString::fromLocal8Bit("�˲�"));
	this->_pLabelTxt2 = new QLabel(QString::fromLocal8Bit("kHz"));
	this->_pLELow = new QLineEdit(QString::fromLocal8Bit("0.7"));
	this->_pLEHigh = new QLineEdit(QString::fromLocal8Bit("1.5"));

	//layout
	this->_pHBoxLayout = new QHBoxLayout();

	//member -> layout
	_pHBoxLayout->addWidget(this->_pLabelTxt1);
	_pHBoxLayout->addWidget(this->_pLELow);
	_pHBoxLayout->addWidget(this->_pLEHigh);
	_pHBoxLayout->addWidget(this->_pLabelTxt2);
	_pHBoxLayout->addWidget(this->_pPushButtonSure);

	//main.setLayout
	this->setLayout(_pHBoxLayout);

	//orientation layout
	_pHBoxLayout->setAlignment(Qt::AlignTop);
}

void View_filteringSetting::initConnect()
{
}

void View_filteringSetting::setCSS(QPalette& palette)
{
	//QPalette pal = this->palette();
	//pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
	this->setPalette(palette);
}

void View_filteringSetting::init()
{
	initLayout();
	initConnect();
}
