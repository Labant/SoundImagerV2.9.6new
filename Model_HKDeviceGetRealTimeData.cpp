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
Mat g_BGRImage;// YUV转为BGR
#endif//@HHKK

//Mat g_BGRImage;// YUV转为BGR
LONG g_nPort;//网络端口号
LONG lUserID;
int iNum = 0; 	//图片名称序号
LONG IHandle = -1;//报警布防/撤防;

string carNum;//车牌号							
string LineByLine;//逐行读取文件 



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


		{
			//QMutexLocker mLocker(&mFramesMutex);

			cvtColor(YUVImage, g_BGRImage, COLOR_YUV2BGR_YV12);//耗时长 40ms左右 

			////转为QImage //不在此转 render时转
			//mM_image = CVMat2QImage::QCVMat2QImage(g_BGRImage);
		}
		//qDebug() << "2:" << QDateTime::currentMSecsSinceEpoch();


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

//报警回调函数
void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER* pAlarmer, char* pAlarmInfo, DWORD dwBufLen, void* pUser)
{

	//ofstream oFile;//定义文件输出流
	//oFile.open("车牌号.csv", ofstream::app);    //打开要输出的文件 	
	//获取系统时间
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;

	int i = 0;
	char filename[100];
	FILE* fSnapPic = NULL;
	FILE* fSnapPicPlate = NULL;
	//以下代码仅供参考，实际应用中不建议在该回调函数中直接处理数据保存文件，
	//例如可以使用消息的方式(PostMessage)在消息响应函数里进行处理。
	switch (lCommand) {
		//交通抓拍结果(老报警消息)
	case COMM_UPLOAD_PLATE_RESULT: {
		NET_DVR_PLATE_RESULT struPlateResult = { 0 };
		memcpy(&struPlateResult, pAlarmInfo, sizeof(struPlateResult));
		qDebug() << QString::fromLocal8Bit("车牌：") << QString(QString::fromLocal8Bit(struPlateResult.struPlateInfo.sLicense));
		//printf("车牌号: %s\n", struPlateResult.struPlateInfo.sLicense);//车牌号	
		// oFile << struPlateResult.struPlateInfo.sLicense << endl;//保存车牌号到csv文件		
		//oFile << struPlateResult.struPlateInfo.sLicense << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//保存车牌号到csv文件	
		//场景图
		if (struPlateResult.dwPicLen != 0 && struPlateResult.byResultType == 1)
		{
			//sprintf(filename, "./pic/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			//fSnapPic = fopen(filename, "wb");
			//fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPic);
			//iNum++;
			//fclose(fSnapPic);
		}
		//车牌图
		if (struPlateResult.dwPicPlateLen != 0 && struPlateResult.byResultType == 1)
		{
			//sprintf(filename, "./pic/1/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			//fSnapPicPlate = fopen(filename, "wb");
			//fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPicPlate);
			//iNum++;
			//fclose(fSnapPicPlate);
		}
		//其他信息处理......
		break;
	}
								 //交通抓拍结果(新报警消息)
	case COMM_ITS_PLATE_RESULT: {
		NET_ITS_PLATE_RESULT struITSPlateResult = { 0 };
		memcpy(&struITSPlateResult, pAlarmInfo, sizeof(struITSPlateResult));
		for (i = 0; i < struITSPlateResult.dwPicNum; i++)
		{
			qDebug() << QString::fromLocal8Bit("车牌：") << QString(QString::fromLocal8Bit(struITSPlateResult.struPlateInfo.sLicense));
			//printf("车牌号: %s\n", struITSPlateResult.struPlateInfo.sLicense);//车牌号
			carNum = struITSPlateResult.struPlateInfo.sLicense;
			//oFile << carNum << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//保存车牌号到csv文件	
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 1) || (struITSPlateResult.struPicInfo[i].byType == 2))
			{
				//sprintf(filename, "./pic/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				//fSnapPic = fopen(filename, "wb");
				//fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, fSnapPic);
				//iNum++;
				//fclose(fSnapPic);
			}
			//车牌小图片
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 0))
			{
				//sprintf(filename, "./pic/1/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				//fSnapPicPlate = fopen(filename, "wb");
				//fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, \
				//	fSnapPicPlate);
				//iNum++;
				//fclose(fSnapPicPlate);
			}
			//其他信息处理......
		}
		//Whitelist();//白名单比对
		//Blacklist();//黑名单比对
		break;
	}
	default: {
		std::cout << lCommand << endl;
		break;
	}
	}

	//oFile.close();//关闭文件
	return;
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
	qDebug()<<NET_DVR_Init();
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

	//设置布防回调函数
	this->setMessageCallBack();
	//设置报警
	this->setupAlarm();
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

	//关闭报警
	this->closeAlarm();

	//关闭预览 实时推流
	NET_DVR_StopRealPlay(lRealPlayHandle);
}

void Model_HKDeviceGetRealTimeData::setMessageCallBack()
{
	qDebug()<<__FUNCDNAME__ << NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
}

void Model_HKDeviceGetRealTimeData::whitelist()
{
	ifstream iFile;
	iFile.open("白名单.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "找不到文件";

	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) 截取车牌号“蓝新NF8202”为NF8202
			if (found != std::string::npos) {
				std::cout << "白名单:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//关闭文件
}

void Model_HKDeviceGetRealTimeData::blacklist()
{
	ifstream iFile;
	iFile.open("黑名单.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "找不到文件";

	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) 截取车牌号“蓝新NF8202”为NF8202
			if (found != std::string::npos) {
				std::cout << "黑名单:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//关闭文件
}

void Model_HKDeviceGetRealTimeData::setupAlarm()
{
	//启动布防
	NET_DVR_SETUPALARM_PARAM struSetupParam = { 0 };
	struSetupParam.dwSize = sizeof(NET_DVR_SETUPALARM_PARAM);


	struSetupParam.byAlarmInfoType = 1;//上传报警信息类型：0-老报警信息(NET_DVR_PLATE_RESULT), 1-新报警信息(NET_ITS_PLATE_RESULT)
	struSetupParam.byLevel = 1;//布防优先级：0- 一等级（高），1- 二等级（中），2- 三等级（低）
	//bySupport 按位表示，值：0 - 上传，1 - 不上传;  bit0 - 表示二级布防是否上传图片;


	IHandle = NET_DVR_SetupAlarmChan_V41(lUserID, &struSetupParam);//建立报警上传通道，获取报警等信息。
	if (IHandle < 0)
	{
		std::cout << "NET_DVR_SetupAlarmChan_V41 Failed! Error number：" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
	std::cout << "\n" << endl;
}

void Model_HKDeviceGetRealTimeData::closeAlarm()
{
	//撤销布防上传通道
	if (!NET_DVR_CloseAlarmChan_V30(IHandle))//布防句柄IHandle
	{
		std::cout << "NET_DVR_CloseAlarmChan_V30 Failed! Error number：" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
	IHandle = -1;//布防句柄;
}
