#include "MainModel.h"
#include "MainViewControl.h"
#include "Model_hotMapAndVideoDisplay.h"
#include "Model_HKDeviceGetRealTimeData.h"

MainModel::MainModel(QObject* parent)
{

}

MainModel::~MainModel()
{
	if (this->_pModel_filteringSetting != nullptr)
	{
		delete this->_pModel_filteringSetting;
		this->_pModel_filteringSetting = nullptr;
	}
}

void MainModel::setView(MainViewControl* hendle)
{
	this->_pMainViewControl = hendle;
	this->init();
}

void MainModel::initMember()
{
	this->_pModel_filteringSetting = new Model_filteringSetting(this->_pMainViewControl->_mView_TabWidget_Setting->_pView_filteringSetting);
	this->_pModel_AudioCollector = new Model_AudioCollector(this->_pMainViewControl->_mView_TabWidget_Setting);
	this->_pModel_HMCutOut = new Model_HMCutOut(this->_pMainViewControl->_mView_TabWidget_Setting->_pView_HMCutOut, this);
	this->_Model_HmSizeFactor = new Model_HmSizeFactor(this->_pMainViewControl->_mView_TabWidget_Setting->_pView_HMCutOut->_pView_HmSizeFactor, this);
	//opencv ��Ƶ��  ʹ��IMX291����ͷs
	this->m_model_videoDevice = new Model_VideoDevice(this->_pMainViewControl->_mView_TabWidget_Setting->mView_deviceChoose,this);
	//�������
	this->_pModel_HKDeviceGetRealTimeData = new Model_HKDeviceGetRealTimeData(this->_pMainViewControl->_mView_TabWidget_Setting->mView_deviceChoose, this);

	//����ͼ���ݴ����ͼ����ʾ
	this->_pModel_hotMapAndVideoDisplay = new Model_hotMapAndVideoDisplay(this->_pMainViewControl->_pView_hotMapAndVideoDisplay, this->m_model_videoDevice);
	
	//����ʶ����
	this->_pModel_CarIdentifyInfoDisplay = new Model_CarIdentifyInfoDisplay(this->_pMainViewControl->_mView_TabWidget_Setting->_pView_CarIdentifyInfoDisplay);
}

void MainModel::initConnect()
{
	connect(this->_pMainViewControl, &MainViewControl::m_signal_closeModelStream, this, &MainModel::quit,Qt::DirectConnection);
	//��Ƶ��������
	//connect(this->_pMainViewControl, &MainViewControl::m_signal_initCamera, this->m_model_videoDevice, &Model_VideoDevice::init, Qt::QueuedConnection);
	//��Ƶ��ʼ��
	connect(this->_pMainViewControl->_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_camera, this->_pMainViewControl, &MainViewControl::initCamera);
	
	//ˢ����Ƶ
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_replot_video, this, &MainModel::replotVideo,Qt::BlockingQueuedConnection);
	//ˢ������ͼ
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_replot_hotmap, this, &MainModel::replotHm, Qt::BlockingQueuedConnection);

	//Hm��ֵ���
	connect(this->_pModel_HMCutOut, &Model_HMCutOut::on_signal_thresholdChange, this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_setHmThreshold, Qt::QueuedConnection);

	//������ʾ
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_setCarPaltedisplay, this->_pModel_CarIdentifyInfoDisplay, &Model_CarIdentifyInfoDisplay::setCarPlate);
	//��������ʾ
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_setSoundPw, this->_pModel_CarIdentifyInfoDisplay, &Model_CarIdentifyInfoDisplay::setSoundPw);
	//����
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_setCarLane, this->_pModel_CarIdentifyInfoDisplay, &Model_CarIdentifyInfoDisplay::setCarLaneInfo);
}

void MainModel::init()
{
	initMember();
	initConnect();
}

void MainModel::quit()
{
	if (this->_pModel_AudioCollector->mThread.isRunning())
		this->_pModel_AudioCollector->mThread.quit();
}

void MainModel::replotVideo()
{
	if (this->_pMainViewControl->_pView_hotMapAndVideoDisplay != nullptr)
	{
		this->_pMainViewControl->_pView_hotMapAndVideoDisplay->replot();
	}
}

void MainModel::replotHm()
{
	if (this->_pMainViewControl->_pView_hotMapAndVideoDisplay != nullptr)
	{
		this->_pMainViewControl->_pView_hotMapAndVideoDisplay->replot();
	}
}
