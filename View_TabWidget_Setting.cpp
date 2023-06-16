#include "View_TabWidget_Setting.h"

View_TabWidget_Setting::View_TabWidget_Setting(QWidget* parent)
{

	//this->setParent(parent);

	this->initObject();
	this->initLayout();
	this->initConnect();

	//recordAudio
	//if (this->m_View_AudioRecord->m_checkBox->isChecked())
	//{
	//	emit this->m_View_AudioRecord->m_signal_recordding();
	//}
	this->setCSS();
}

View_TabWidget_Setting::~View_TabWidget_Setting()
{

	this->mView_deviceChoose->deleteLater();
}

void View_TabWidget_Setting::initObject()
{
	//����box ѡ����Ƶ�豸����Ƶ�豸
	mView_deviceChoose = new View_DeviceChoose;
	//����
	_pTabWidget = new QTabWidget(this);
	//¼��setting
	m_View_AudioRecord = new View_AudioRecord(this);
	//�˲�setting
	_pView_filteringSetting = new View_filteringSetting(this);
	//�ü�����setting
	_pView_HMCutOut = new View_HMCutOut(this);
}

void View_TabWidget_Setting::initLayout()
{
	this->addTab(this->mView_deviceChoose, QString::fromLocal8Bit("�豸ѡ��"));
	this->addTab(this->_pTabWidget, QString::fromLocal8Bit("����"));
	this->_pTabWidget->addTab(this->m_View_AudioRecord, QString::fromLocal8Bit("��Ƶ¼��"));
	this->_pTabWidget->addTab(this->_pView_filteringSetting, QString::fromLocal8Bit("�˲�����"));
	this->_pTabWidget->addTab(this->_pView_HMCutOut, QString::fromLocal8Bit("���ݼ���"));
}

void View_TabWidget_Setting::initConnect()
{
	//connect(this->mView_deviceChoose, &View_DeviceChoose::m_signak_recording, this, &View_TabWidget_Setting::m_signal_recordding);
	//connect(this->mView_deviceChoose, &View_AudioRecord::m_signal_recordding, this, &View_TabWidget_Setting::m_signal_recordding);
	connect(this->m_View_AudioRecord->m_checkBox, &QPushButton::clicked, this, &View_TabWidget_Setting::m_signal_recordding);

}

void View_TabWidget_Setting::setCSS()
{
	QPalette pal = this->palette();
	//pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	//this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
	//this->setPalette(pal);

	this->_pView_filteringSetting->setCSS(pal);
	this->m_View_AudioRecord->setCSS(pal);
	this->mView_deviceChoose->setCSS(pal);
	this->_pView_HMCutOut->setCSS(pal);
}
