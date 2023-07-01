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

	//�޿�ģʽ
	//this->setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);

	//����ģ̬����
	//setWindowModality(Qt::ApplicationModal);//Qt::ApplicationModal ������Ӧ��ģ��
	//setAttribute(Qt::WA_ShowModal, true);

	////���ñ���
	//this->setWindowTitle("ValueSet");
	//this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/image/logo.png")));

	////ȥ����󻯡���С����ť�������رհ�ť
	//this->setWindowFlags(Qt::WindowCloseButtonHint);

	//�����ָ��
	//this->_pGridLayout = new QGridLayout;
	this->_pHBoxLayout = new QHBoxLayout;
	this->setLayout(this->_pHBoxLayout);

	//Auto ����
	_pPtnAuto = new QPushButton(QString::fromLocal8Bit("Manual:"));
	this->_pHBoxLayout->addWidget(_pPtnAuto);
	//_pLabelAuto->setAlignment(Qt::AlignLeft);

	//vt��ֵ����
	pLabelVT = new QLabel(QString::fromLocal8Bit("VTT:"));
	//vt��ֵ����
	pLineVT = new QLineEdit(QString::fromLocal8Bit("0.0015"));
	//�����Զ���ť���趨ֵ
	if (mAutoValue)pLineVT->setEnabled(!mAutoValue);

	//vf��ֵ����
	pLabelVF = new QLabel(QString::fromLocal8Bit("VFT:"));
	//vf��ֵ����
	pLineVF = new QLineEdit(QString::fromLocal8Bit("1.0"));
	//�����Զ���ť���趨ֵ
	if (mAutoValue)pLineVF->setEnabled(!mAutoValue);
	//���ʵ��
	this->_pHBoxLayout->addWidget(pLabelVT);
	this->_pHBoxLayout->addWidget(pLineVT);
	this->_pHBoxLayout->addWidget(pLabelVF);
	this->_pHBoxLayout->addWidget(pLineVF);

	//ʵ��
	m_comBox_chooseDisplay = new QComboBox;
	//���ʵ��
	this->_pHBoxLayout->addWidget(m_comBox_chooseDisplay);//, 0, Qt::AlignRight
	//this->_pHBoxLayout->setAlignment(Qt::AlignRight);
	//this->_pHBoxLayout->setContentsMargins(QMargins(5, 0, 5, 5));
	//����combox��Ϣ
	initCombox();

}

void View_ValueOfTFSetting::initCombox()
{
	if (m_comBox_chooseDisplay != nullptr)
	{
		QStringList items;
		items << "P" << "X" << "Y" << "Z";
		m_comBox_chooseDisplay->addItems(items);

		//���ÿ��
		//m_comBox_chooseDisplay->setMaximumWidth(50);
		////���þ���
	}

}
void View_ValueOfTFSetting::initSheetStyle()
{
	//���ÿؼ��߶�
	m_comBox_chooseDisplay->setMaximumHeight(HEIGHT);
	_pPtnAuto->setMaximumHeight(HEIGHT);
	pLabelVT->setMaximumHeight(HEIGHT);
	pLineVT->setMaximumHeight(HEIGHT);
	pLabelVF->setMaximumHeight(HEIGHT);
	pLineVF->setMaximumHeight(HEIGHT);


	QPalette pal = this->palette();
	pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
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
