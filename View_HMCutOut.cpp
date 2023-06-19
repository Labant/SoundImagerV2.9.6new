#include "View_HMCutOut.h"

View_HMCutOut::View_HMCutOut(QWidget* parent)
{
	this->init();
}

View_HMCutOut::~View_HMCutOut()
{
}

void View_HMCutOut::setCSS(QPalette& pal)
{
	this->setAutoFillBackground(true);
	this->setPalette(pal);
}

void View_HMCutOut::on_showP1_DMaxValue(double value)
{
	this->_pHmHm_P1D_DisplayValue->setText(QString::number(value, 'f', 16));
}

void View_HMCutOut::initLayout()
{
	this->_pGridLayout = new QGridLayout(this);
	this->setLayout(this->_pGridLayout);


	for (int i = 0; i < 2; i++)
	{
		this->_pSlider[i] = new QSlider(Qt::Horizontal, this);
		this->_pSlider[i]->setFixedSize(300, 50);
		//this->_pSlider[i]->move(10, 50);
		this->_pSlider[i]->setRange(1, i > 0 ? 60 : 90);//0- 180 ==>/2 ==>90，分别最大为90，60。 以0为中心环形对称裁剪
		this->_pSlider[i]->setValue(i > 0 ? 41 : 31);
		this->_pSlider[i]->setSingleStep(1);
		this->_pSlider[i]->setStyleSheet("QSlider{background-color: transparent;}"
			"QSlider::groove:horizontal{border: 0px solid red;background: rgb(31, 147, 251);height:20px;border-radius: 10px;}"
			"QSlider::handle:horizontal{width:50px;background-color:rgb(255,255,225);margin:-15px 0px -15px 0px;border-radius:25px;}"//margin:上 右 下 左
			"QSlider::sub-page:horizontal{background: rgb(31, 147, 251);border-top-left-radius:10px;border-bottom-left-radius:10px;}"
			"QSlider::add-page:horizontal{background: rgb(255, 0, 0);border: 0px solid white;border-top-right-radius:10px;border-bottom-right-radius:10px;}");

		this->_pGridLayout->addWidget(this->_pSlider[i], i, 2);

		this->_pLabel[i] = new QLabel(i > 0 ? QString::fromLocal8Bit("Y 轴裁剪数值:") : QString::fromLocal8Bit("X 轴裁剪数值:"), this);
		this->_pGridLayout->addWidget(this->_pLabel[i], i, i == 1 ? (i -1) : i);
	
		this->_pLineEdit[i] = new QLineEdit(QString::number(30), this);
		this->_pLineEdit[i]->setStyleSheet("QLineEdit {border-bottom-left-radius:10px;border-top-right-radius:10px;border-bottom-right-radius:10px;border-top-left-radius:10px;}");
		this->_pLineEdit[i]->setAlignment(Qt::AlignHCenter);
		this->_pGridLayout->addWidget(this->_pLineEdit[i], i, 1);
	}

	
	//热力图系数
	this->_pView_HmSizeFactor = new View_HmSizeFactor(this);
	this->_pGridLayout->addWidget(this->_pView_HmSizeFactor, 2, 0, 1, 3);//

	//2023.6.9新增
	this->_pHmthresholdDisplay = new QLabel(QString::fromLocal8Bit("HM阈值:"));
	this->_pHmthresholdValue = new QLineEdit(QString::fromLocal8Bit("0.0000007"));
	this->_pGridLayout->addWidget(this->_pHmthresholdDisplay, 3, 0, 1, 1);
	this->_pGridLayout->addWidget(this->_pHmthresholdValue, 3, 1, 1, 2);

	this->_pHm_P1D_Display = new QLabel(QString::fromLocal8Bit("目前HM最大值值:"));
	this->_pHmHm_P1D_DisplayValue = new QLineEdit();
	this->_pGridLayout->addWidget(this->_pHm_P1D_Display, 4, 0, 1, 1);
	this->_pGridLayout->addWidget(this->_pHmHm_P1D_DisplayValue, 4, 1, 1, 2);




}

void View_HMCutOut::initConnect()
{
}

void View_HMCutOut::init()
{
	this->initLayout();
	this->initConnect();
}
