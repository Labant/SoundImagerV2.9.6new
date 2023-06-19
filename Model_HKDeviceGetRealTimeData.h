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

protected:
	void initMembers();
	void initConnects();

private:
	void init();
	void initHKDevice();//��ʼ��HK�豸
	void startCollectFrames();//��ʼ�ɼ���Ƶ֡
	void stopCollectFrames();//ֹͣ�ɼ���Ƶ֡

	//����2023.6.19KHʶ����
	void setMessageCallBack();//���ñ����ص�����
	void whitelist();//�������ȶ�
	void blacklist();//�������ȶ�
	void setupAlarm();//��������
	void closeAlarm();//��������


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

	//Model_CarPlateIdentify* _pModel_CarPlateIdentify;
};



