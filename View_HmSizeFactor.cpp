#include "View_HmSizeFactor.h"

View_HmSizeFactor::View_HmSizeFactor(QWidget* parent)
{
	this->init();
}

View_HmSizeFactor::~View_HmSizeFactor()
{


}

void View_HmSizeFactor::initLayout()
{
	this->setLayout(this->_pGridLayout);

	this->_pSlider->setFixedSize(300, 50);
	//this->_pSlider[i]->move(10, 50);
	//this->_pSlider->setRange(50, 90);//
	this->_pSlider->setRange(5, 600);//
	this->_pSlider->setValue(200);
	this->_pSlider->setSingleStep(1);
	this->_pSlider->setObjectName("View_HmSizeFactor__pSlider");
	this->_pSlider->setStyleSheet("#View_HmSizeFactor__pSlider{background-color: transparent;}"
		"#View_HmSizeFactor__pSlider::groove:horizontal{border: 0px solid red;background: rgb(31, 147, 251);height:20px;border-radius: 10px;}"
		"#View_HmSizeFactor__pSlider::handle:horizontal{width:50px;background-color:rgb(255,255,225);margin:-15px 0px -15px 0px;border-radius:25px;}"//margin:上 右 下 左
		"#View_HmSizeFactor__pSlider::sub-page:horizontal{background: rgb(31, 147, 251);border-top-left-radius:10px;border-bottom-left-radius:10px;}"
		"#View_HmSizeFactor__pSlider::add-page:horizontal{background: rgb(255, 0, 0);border: 0px solid white;border-top-right-radius:10px;border-bottom-right-radius:10px;}"
	);


	this->_pGridLayout->addWidget(this->_pLabelTitle, 0, 0, 1, 1);

	for (int i = 0; i < 2; i++)
	{
		this->_pLabel[i] = new QLabel(i > 0 ? QString::fromLocal8Bit("大") : QString::fromLocal8Bit("小"), this);
		if (i == 0)
		{
			this->_pGridLayout->addWidget(this->_pLabel[i], 0, 1,1,1);
		}
		else
		{
			this->_pGridLayout->addWidget(this->_pSlider, 0, 2, 1, 1);
			this->_pGridLayout->addWidget(this->_pLabel[i], 0, 3, 1, 1);
		}
	}
}

void View_HmSizeFactor::initObject()
{
	this->_pGridLayout = new QGridLayout(this);


	this->_pSlider = new QSlider(Qt::Horizontal, this);

	this->_pLabelTitle = new QLabel(QString::fromLocal8Bit("热力图大小:"), this);


}

void View_HmSizeFactor::init()
{
	this->initObject();
	this->initLayout();
}
