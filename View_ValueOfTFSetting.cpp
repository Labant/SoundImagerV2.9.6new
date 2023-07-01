#include "View_ValueOfTFSetting.h"
#include <QDir>
#include <QIcon>
#include <QDebug>

View_ValueOfTFSetting::View_ValueOfTFSetting(QWidget* parent)
{

	//qDebug() << "this->layout():" << this->layout();
	this->init();

}

View_ValueOfTFSetting::~View_ValueOfTFSetting()
{
}

void View_ValueOfTFSetting::init()
{
	this->initLayout();
	this->initConnect();
	this->initSheetStyle();
}

void View_ValueOfTFSetting::initConnect()
{
}

void View_ValueOfTFSetting::initLayout()
{

	//无框模式
	//this->setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);

	//设置模态窗口
	//setWindowModality(Qt::ApplicationModal);//Qt::ApplicationModal 对整个应用模块
	//setAttribute(Qt::WA_ShowModal, true);

	////设置标题
	//this->setWindowTitle("ValueSet");
	//this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/image/logo.png")));

	////去掉最大化、最小化按钮，保留关闭按钮
	//this->setWindowFlags(Qt::WindowCloseButtonHint);

	//主布局格局
	//this->_pGridLayout = new QGridLayout;
	this->_pHBoxLayout = new QHBoxLayout;
	this->setLayout(this->_pHBoxLayout);

	//Auto 描述
	_pPtnAuto = new QPushButton(QString::fromLocal8Bit("Manual:"));
	this->_pHBoxLayout->addWidget(_pPtnAuto);
	//_pLabelAuto->setAlignment(Qt::AlignLeft);

	//vt阈值描述
	pLabelVT = new QLabel(QString::fromLocal8Bit("VTT:"));
	//vt阈值输入
	pLineVT = new QLineEdit(QString::fromLocal8Bit("0.0015"));
	//根据自动按钮来设定值
	if (mAutoValue)pLineVT->setEnabled(!mAutoValue);

	//vf阈值描述
	pLabelVF = new QLabel(QString::fromLocal8Bit("VFT:"));
	//vf阈值输入
	pLineVF = new QLineEdit(QString::fromLocal8Bit("1.0"));
	//根据自动按钮来设定值
	if (mAutoValue)pLineVF->setEnabled(!mAutoValue);
	//添加实体
	this->_pHBoxLayout->addWidget(pLabelVT);
	this->_pHBoxLayout->addWidget(pLineVT);
	this->_pHBoxLayout->addWidget(pLabelVF);
	this->_pHBoxLayout->addWidget(pLineVF);

	//实体
	m_comBox_chooseDisplay = new QComboBox;
	//添加实体
	this->_pHBoxLayout->addWidget(m_comBox_chooseDisplay);//, 0, Qt::AlignRight
	//this->_pHBoxLayout->setAlignment(Qt::AlignRight);
	//this->_pHBoxLayout->setContentsMargins(QMargins(5, 0, 5, 5));
	//设置combox信息
	initCombox();

}

void View_ValueOfTFSetting::initCombox()
{
	if (m_comBox_chooseDisplay != nullptr)
	{
		QStringList items;
		items << "P" << "X" << "Y" << "Z";
		m_comBox_chooseDisplay->addItems(items);

		//设置宽度
		//m_comBox_chooseDisplay->setMaximumWidth(50);
		////设置居中
	}

}
void View_ValueOfTFSetting::initSheetStyle()
{
	//设置控件高度
	m_comBox_chooseDisplay->setMaximumHeight(HEIGHT);
	_pPtnAuto->setMaximumHeight(HEIGHT);
	pLabelVT->setMaximumHeight(HEIGHT);
	pLineVT->setMaximumHeight(HEIGHT);
	pLabelVF->setMaximumHeight(HEIGHT);
	pLineVF->setMaximumHeight(HEIGHT);


	QPalette pal = this->palette();
	pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
	this->setPalette(pal);

	//this->m_comBox_chooseDisplay->setAutoFillBackground(true);
	//QPalette pal2 = this->m_comBox_chooseDisplay->palette();
	//pal2.setBrush(QPalette::Base, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	//this->m_comBox_chooseDisplay->setPalette(pal2);
	//this->m_comBox_chooseDisplay->update();


	//QPalette pal3 = this->_pBtnSetting->palette();
	//pal3.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	//this->_pBtnSetting->setAutoFillBackground(true);
	//this->_pBtnSetting->setPalette(pal3);
	//this->_pBtnSetting->update();
}
