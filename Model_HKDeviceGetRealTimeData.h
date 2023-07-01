#pragma once
#include <qobject.h>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <HCNetSDK.h>
#include <plaympeg4.h>
#include "PlayM4.h"
#include <opencv2\opencv.hpp>
#include <QMessageBox>
#include "GlobalConfig.hpp"
#include <QDateTime>

using namespace cv;

class View_DeviceChoose;
class Model_CarPlateIdentify;

class Model_HKDeviceGetRealTimeData :public QObject
{
	Q_OBJECT
public:
	explicit Model_HKDeviceGetRealTimeData(View_DeviceChoose* handle = nullptr, QObject* parent = nullptr);
	~Model_HKDeviceGetRealTimeData();
	static  void doLicensePlate(QString);
	void setupAlarm();//��������
	void closeAlarm();//��������

	//����2023.6.21����ץ��
	void contuinCapture();
protected:
	void initMembers();
	void initConnects();
	//void MSesGCallback(LONG lCommand, NET_DVR_ALARMER* pAlarmer, char* pAlarmInfo, DWORD dwBufLen, void* pUser);

private:
	void init();
	void initHKDevice();//��ʼ��HK�豸
	void startCollectFrames();//��ʼ�ɼ���Ƶ֡
	void stopCollectFrames();//ֹͣ�ɼ���Ƶ֡

	//����2023.6.19 KHʶ����
	void setMessageCallBack();//���ñ����ص�����
	void whitelist();//�������ȶ�
	void blacklist();//�������ȶ�
	void setArmParas();

	 
signals:


public:

private:
	//handle
	View_DeviceChoose* _pView_DeviceChoose = nullptr;
	QThread mThread;
	//���
	LONG lRealPlayHandle = NULL;
	//���ʱ�ж��Ƿ������
	bool isInitCamera = true;
	//����Ƿ���Ƶ����
	bool isUpDataCameraStream = false;
	//����Ԥ�������ûص�������
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };

	//������������
	NET_DVR_SETUPALARM_PARAM struSetupParam = { 0 };

	//�������Ĵ���
	tagNET_DVR_SNAPCFG mtagNET_DVR_SNAPCFG;
	//tagNET_DVR_SNAPCFG mNET_DVR_SNAPCFG = {0};
	//LPNET_DVR_SNAPCFG _pLPNET_DVR_SNAPCFG;
	NET_DVR_JPEGPARA mNET_DVR_JPEGPARA = {0};

	//Model_CarPlateIdentify* _pModel_CarPlateIdentify;
};



