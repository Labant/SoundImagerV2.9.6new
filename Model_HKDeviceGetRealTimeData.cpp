#include "Model_HKDeviceGetRealTimeData.h"
#include "View_DeviceChoose.h"
#include "CVMat2QImage.h"
#include "Model_CarPlateIdentify.h"
//#ifndef MYNAMESPACE_H
//#include "MyNameSpace.h"
//#endif

#ifndef HHKK
#define HHKK 1
QQueue<cv::Mat> mHKFrames;
QMutex mFramesMutex;
QImage mM_image; //BGR转为QImage
#endif//@HHKK

Mat g_BGRImage;// YUV转为BGR
LONG g_nPort;//网络端口号
LONG lUserID;







//数据解码回调函数，
//功能：将YV_12格式的视频数据流转码为可供opencv处理的BGR类型的图片数据，并实时显示。
void CALLBACK DecCBFun(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo, long nUser, long nReserved2)//CALLBACK
{


	if (pFrameInfo->nType == T_YV12)
	{
		//std::cout << "the frame infomation is T_YV12" << std::endl;
		if (g_BGRImage.empty())
		{
			g_BGRImage.create(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
			//g_BGRImage.create(848, 640, CV_8UC3);
		}


		Mat YUVImage(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, (unsigned char*)pBuf);
		//Mat YUVImage(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, (unsigned char*)pBuf);


		//qDebug() << "1:" << QDateTime::currentMSecsSinceEpoch();

		cvtColor(YUVImage, g_BGRImage, COLOR_YUV2BGR_YV12);//耗时长 40ms左右 

		{
			QMutexLocker mLocker(&mFramesMutex);
			//转为QImage //不在此转 render时转
			mM_image = CVMat2QImage::QCVMat2QImage(g_BGRImage);
		}



		//存储进队列
		if (mHKFrames.size() < 2)
		{
			//qDebug() << "11->:" << QDateTime::currentMSecsSinceEpoch();
			QMutexLocker mLocker(&mFramesMutex);
			//qDebug() << "12->:" << QDateTime::currentMSecsSinceEpoch();
			mHKFrames.append(g_BGRImage);
			//qDebug() << "13->:" << QDateTime::currentMSecsSinceEpoch();

		}
		else
		{
			//qDebug() << "11->:" << QDateTime::currentMSecsSinceEpoch();
			QMutexLocker mLocker(&mFramesMutex);
			//qDebug() << "12->:" << QDateTime::currentMSecsSinceEpoch();
			mHKFrames.removeFirst();
			//qDebug() << "13->:" << QDateTime::currentMSecsSinceEpoch();
			mHKFrames.append(g_BGRImage);

		}

		//qDebug() << "2:" << QDateTime::currentMSecsSinceEpoch();

		//imshow("RGBImage1", g_BGRImage);
		//waitKey(15);

		YUVImage.~Mat();
	}
}

//实时视频码流数据获取 回调函数
void CALLBACK g_RealDataCallBack_V30(LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
	//qDebug() << "1:" << QDateTime::currentMSecsSinceEpoch();
	if (dwDataType == NET_DVR_STREAMDATA)//码流数据
	{
		if (dwBufSize > 0 && g_nPort != -1)
		{
			if (!PlayM4_InputData(g_nPort, pBuffer, dwBufSize))
			{
				std::cout << "fail input data" << std::endl;
			}
			else
			{
				std::cout << "success input data" << std::endl;
			}

		}
	}
	//qDebug() << "2:" << QDateTime::currentMSecsSinceEpoch();
}



Model_HKDeviceGetRealTimeData::Model_HKDeviceGetRealTimeData(View_DeviceChoose* handle, QObject* parent)
{
	this->moveToThread(&this->mThread);
	this->mThread.start(QThread::Priority::HighestPriority);
	this->_pView_DeviceChoose = handle;
	this->init();


}

Model_HKDeviceGetRealTimeData::~Model_HKDeviceGetRealTimeData()
{
	if (lUserID > 0 || lRealPlayHandle > 0)
	{
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		//关闭预览 实时推流
		NET_DVR_StopRealPlay(lRealPlayHandle);
		//注销用户
		NET_DVR_Logout(lUserID);
		//释放SDK资源
		NET_DVR_Cleanup();
	}
}

void Model_HKDeviceGetRealTimeData::initMembers()
{
}

void Model_HKDeviceGetRealTimeData::initConnects()
{
	//start/stop
	connect(this->_pView_DeviceChoose, &View_DeviceChoose::m_signal_modelStart, this, &Model_HKDeviceGetRealTimeData::startCollectFrames, Qt::QueuedConnection);
	connect(this->_pView_DeviceChoose, &View_DeviceChoose::m_signal_modelStop, this, &Model_HKDeviceGetRealTimeData::stopCollectFrames, Qt::QueuedConnection);
}

void Model_HKDeviceGetRealTimeData::init()
{
	this->initHKDevice();
	this->initMembers();
	this->initConnects();
}

void Model_HKDeviceGetRealTimeData::initHKDevice()
{
	// 初始化
	NET_DVR_Init();
	// 设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	// 登录
	NET_DVR_USER_LOGIN_INFO pLoginInfo = { 0 };
	NET_DVR_DEVICEINFO_V40 lpDeviceInfo = { 0 };

	pLoginInfo.bUseAsynLogin = 0;  // 同步登录方式
	//char* sDeviceAddress, * sUserName, * sPassword;
	QString sDeviceAddress, sUserName, sPassword;

	struPlayInfo = { 0 };
	sDeviceAddress = "192.168.1.64";
	sUserName = "admin";
	sPassword = "ls123456";
	strcpy_s(pLoginInfo.sDeviceAddress, sDeviceAddress.toStdString().c_str());
	strcpy_s(pLoginInfo.sUserName, sUserName.toStdString().c_str());
	strcpy_s(pLoginInfo.sPassword, sPassword.toStdString().c_str());
	pLoginInfo.wPort = 8000;

	//-->登录
	lUserID = NET_DVR_Login_V40(&pLoginInfo, &lpDeviceInfo);
	//QMessageBox::warning(this, "lUserID", QString::number(lUserID));

	if (lUserID < 0) {
		qDebug() << "Login failed, error code: " << NET_DVR_GetLastError();
		qDebug() << "NET_DVR_SetLogToFile, error code: " << NET_DVR_SetLogToFile();
		//QMessageBox::warning(this, "Login failed", QString("%1%2").arg("error code : ").arg(NET_DVR_GetLastError()));
		NET_DVR_Cleanup();

	}
	else {
		qDebug() << QString::fromLocal8Bit("Login succeed！");
		//Sleep(1000);  // 显示注册相关信息

		isInitCamera = true;

		//设置流处理
		if (PlayM4_GetPort(&g_nPort))            //获取播放库通道号
		{
			if (PlayM4_SetStreamOpenMode(g_nPort, STREAME_REALTIME))      //设置流模式
			{
				if (PlayM4_OpenStream(g_nPort, NULL, 0, 1024 * 1024))         //打开流
				{
					if (PlayM4_SetDecCallBackExMend(g_nPort, DecCBFun, NULL, 0, NULL)) //设置回调函数 DecCBFun 
					{
						if (PlayM4_Play(g_nPort, NULL))
						{
							std::cout << "success to set play mode" << std::endl;
						}
						else
						{
							std::cout << "fail to set play mode" << std::endl;
						}
					}
					else
					{
						std::cout << "fail to set dec callback " << std::endl;
					}
				}
				else
				{
					std::cout << "fail to open stream" << std::endl;
				}
			}
			else
			{
				std::cout << "fail to set stream open mode" << std::endl;
			}
		}
		else
		{
			std::cout << "fail to get port" << std::endl;
		}
	}

	////启动预览并设置回调数据流
	//NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = NULL; //窗口为空，设备SDK不解码只取流
	//struPlayInfo.hPlayWnd = (HWND)m_right_VideoLabel_videoOut->winId(); //窗口为空，设备SDK不解码只取流
	struPlayInfo.lChannel = 1; //Channel number 设备通道
	struPlayInfo.dwStreamType = 1;// 码流类型，0-主码流，1-子码流，2-码流3，3-码流4, 4-码流5,5-码流6,7-码流7,8-码流8,9-码流9,10-码流10
	struPlayInfo.dwLinkMode = 0;// 0：TCP方式,1：UDP方式,2：多播方式,3 - RTP方式，4-RTP/RTSP,5-RSTP/HTTP 
	struPlayInfo.bBlocked = 0; //0-非阻塞取流, 1-阻塞取流, 如果阻塞SDK内部connect失败将会有5s的超时才能够返回,不适合于轮询取流操作.

}

void Model_HKDeviceGetRealTimeData::startCollectFrames()
{
	//推流中...
	this->isUpDataCameraStream = true;


	//软件解码调用 g_RealDataCallBack_V30函数
	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, g_RealDataCallBack_V30, NULL);

	if (lRealPlayHandle < 0)
	{

		//printf("NET_DVR_RealPlay_V40 error\n");
		//QMessageBox::warning(this, "Video Stream open faild", "NET_DVR_RealPlay_V40 error");
		qDebug() << __FUNCDNAME__ << "error:" << "NET_DVR_RealPlay_V40 error!";
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
}

void Model_HKDeviceGetRealTimeData::stopCollectFrames()
{
	if (lRealPlayHandle < 0)
		return;

	//关闭预览 实时推流
	NET_DVR_StopRealPlay(lRealPlayHandle);
}
