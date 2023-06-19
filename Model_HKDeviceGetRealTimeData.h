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
	void initHKDevice();//初始化HK设备
	void startCollectFrames();//开始采集视频帧
	void stopCollectFrames();//停止采集视频帧

	//新增2023.6.19KH识别车牌
	void setMessageCallBack();//设置报警回调函数
	void whitelist();//白名单比对
	void blacklist();//黑名单比对
	void setupAlarm();//报警布防
	void closeAlarm();//报警撤防


public:

private:
	//handle
	View_DeviceChoose* _pView_DeviceChoose = nullptr;
	QThread mThread;
	//句柄
	LONG lRealPlayHandle = NULL;
	//监测时判断是否开启相机
	bool isInitCamera = true;
	//监测是否视频推流
	bool isUpDataCameraStream = false;
	//启动预览并设置回调数据流
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };

	//Model_CarPlateIdentify* _pModel_CarPlateIdentify;
};



