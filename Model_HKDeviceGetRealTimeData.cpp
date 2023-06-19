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
QImage mM_image; //BGRתΪQImage
Mat g_BGRImage;// YUVתΪBGR
#endif//@HHKK

//Mat g_BGRImage;// YUVתΪBGR
LONG g_nPort;//����˿ں�
LONG lUserID;
int iNum = 0; 	//ͼƬ�������
LONG IHandle = -1;//��������/����;

string carNum;//���ƺ�							
string LineByLine;//���ж�ȡ�ļ� 



//���ݽ���ص�������
//���ܣ���YV_12��ʽ����Ƶ������ת��Ϊ�ɹ�opencv�����BGR���͵�ͼƬ���ݣ���ʵʱ��ʾ��
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

			cvtColor(YUVImage, g_BGRImage, COLOR_YUV2BGR_YV12);//��ʱ�� 40ms���� 

			////תΪQImage //���ڴ�ת renderʱת
			//mM_image = CVMat2QImage::QCVMat2QImage(g_BGRImage);
		}
		//qDebug() << "2:" << QDateTime::currentMSecsSinceEpoch();


		//�洢������
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

//ʵʱ��Ƶ�������ݻ�ȡ �ص�����
void CALLBACK g_RealDataCallBack_V30(LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
	//qDebug() << "1:" << QDateTime::currentMSecsSinceEpoch();
	if (dwDataType == NET_DVR_STREAMDATA)//��������
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

//�����ص�����
void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER* pAlarmer, char* pAlarmInfo, DWORD dwBufLen, void* pUser)
{

	//ofstream oFile;//�����ļ������
	//oFile.open("���ƺ�.csv", ofstream::app);    //��Ҫ������ļ� 	
	//��ȡϵͳʱ��
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;

	int i = 0;
	char filename[100];
	FILE* fSnapPic = NULL;
	FILE* fSnapPicPlate = NULL;
	//���´�������ο���ʵ��Ӧ���в������ڸûص�������ֱ�Ӵ������ݱ����ļ���
	//�������ʹ����Ϣ�ķ�ʽ(PostMessage)����Ϣ��Ӧ��������д���
	switch (lCommand) {
		//��ͨץ�Ľ��(�ϱ�����Ϣ)
	case COMM_UPLOAD_PLATE_RESULT: {
		NET_DVR_PLATE_RESULT struPlateResult = { 0 };
		memcpy(&struPlateResult, pAlarmInfo, sizeof(struPlateResult));
		qDebug() << QString::fromLocal8Bit("���ƣ�") << QString(QString::fromLocal8Bit(struPlateResult.struPlateInfo.sLicense));
		//printf("���ƺ�: %s\n", struPlateResult.struPlateInfo.sLicense);//���ƺ�	
		// oFile << struPlateResult.struPlateInfo.sLicense << endl;//���泵�ƺŵ�csv�ļ�		
		//oFile << struPlateResult.struPlateInfo.sLicense << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//���泵�ƺŵ�csv�ļ�	
		//����ͼ
		if (struPlateResult.dwPicLen != 0 && struPlateResult.byResultType == 1)
		{
			//sprintf(filename, "./pic/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			//fSnapPic = fopen(filename, "wb");
			//fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPic);
			//iNum++;
			//fclose(fSnapPic);
		}
		//����ͼ
		if (struPlateResult.dwPicPlateLen != 0 && struPlateResult.byResultType == 1)
		{
			//sprintf(filename, "./pic/1/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			//fSnapPicPlate = fopen(filename, "wb");
			//fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPicPlate);
			//iNum++;
			//fclose(fSnapPicPlate);
		}
		//������Ϣ����......
		break;
	}
								 //��ͨץ�Ľ��(�±�����Ϣ)
	case COMM_ITS_PLATE_RESULT: {
		NET_ITS_PLATE_RESULT struITSPlateResult = { 0 };
		memcpy(&struITSPlateResult, pAlarmInfo, sizeof(struITSPlateResult));
		for (i = 0; i < struITSPlateResult.dwPicNum; i++)
		{
			qDebug() << QString::fromLocal8Bit("���ƣ�") << QString(QString::fromLocal8Bit(struITSPlateResult.struPlateInfo.sLicense));
			//printf("���ƺ�: %s\n", struITSPlateResult.struPlateInfo.sLicense);//���ƺ�
			carNum = struITSPlateResult.struPlateInfo.sLicense;
			//oFile << carNum << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//���泵�ƺŵ�csv�ļ�	
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 1) || (struITSPlateResult.struPicInfo[i].byType == 2))
			{
				//sprintf(filename, "./pic/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				//fSnapPic = fopen(filename, "wb");
				//fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, fSnapPic);
				//iNum++;
				//fclose(fSnapPic);
			}
			//����СͼƬ
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 0))
			{
				//sprintf(filename, "./pic/1/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				//fSnapPicPlate = fopen(filename, "wb");
				//fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, \
				//	fSnapPicPlate);
				//iNum++;
				//fclose(fSnapPicPlate);
			}
			//������Ϣ����......
		}
		//Whitelist();//�������ȶ�
		//Blacklist();//�������ȶ�
		break;
	}
	default: {
		std::cout << lCommand << endl;
		break;
	}
	}

	//oFile.close();//�ر��ļ�
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
		//�ر�Ԥ�� ʵʱ����
		NET_DVR_StopRealPlay(lRealPlayHandle);
		//ע���û�
		NET_DVR_Logout(lUserID);
		//�ͷ�SDK��Դ
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
	// ��ʼ��
	qDebug()<<NET_DVR_Init();
	// ��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	// ��¼
	NET_DVR_USER_LOGIN_INFO pLoginInfo = { 0 };
	NET_DVR_DEVICEINFO_V40 lpDeviceInfo = { 0 };

	pLoginInfo.bUseAsynLogin = 0;  // ͬ����¼��ʽ
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

	//-->��¼
	lUserID = NET_DVR_Login_V40(&pLoginInfo, &lpDeviceInfo);
	//QMessageBox::warning(this, "lUserID", QString::number(lUserID));

	if (lUserID < 0) {
		qDebug() << "Login failed, error code: " << NET_DVR_GetLastError();
		qDebug() << "NET_DVR_SetLogToFile, error code: " << NET_DVR_SetLogToFile();
		//QMessageBox::warning(this, "Login failed", QString("%1%2").arg("error code : ").arg(NET_DVR_GetLastError()));
		NET_DVR_Cleanup();

	}
	else {
		qDebug() << QString::fromLocal8Bit("Login succeed��");
		//Sleep(1000);  // ��ʾע�������Ϣ

		isInitCamera = true;

		//����������
		if (PlayM4_GetPort(&g_nPort))            //��ȡ���ſ�ͨ����
		{
			if (PlayM4_SetStreamOpenMode(g_nPort, STREAME_REALTIME))      //������ģʽ
			{
				if (PlayM4_OpenStream(g_nPort, NULL, 0, 1024 * 1024))         //����
				{
					if (PlayM4_SetDecCallBackExMend(g_nPort, DecCBFun, NULL, 0, NULL)) //���ûص����� DecCBFun 
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

	////����Ԥ�������ûص�������
	//NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = NULL; //����Ϊ�գ��豸SDK������ֻȡ��
	//struPlayInfo.hPlayWnd = (HWND)m_right_VideoLabel_videoOut->winId(); //����Ϊ�գ��豸SDK������ֻȡ��
	struPlayInfo.lChannel = 1; //Channel number �豸ͨ��
	struPlayInfo.dwStreamType = 1;// �������ͣ�0-��������1-��������2-����3��3-����4, 4-����5,5-����6,7-����7,8-����8,9-����9,10-����10
	struPlayInfo.dwLinkMode = 0;// 0��TCP��ʽ,1��UDP��ʽ,2���ಥ��ʽ,3 - RTP��ʽ��4-RTP/RTSP,5-RSTP/HTTP 
	struPlayInfo.bBlocked = 0; //0-������ȡ��, 1-����ȡ��, �������SDK�ڲ�connectʧ�ܽ�����5s�ĳ�ʱ���ܹ�����,���ʺ�����ѯȡ������.

	//���ò����ص�����
	this->setMessageCallBack();
	//���ñ���
	this->setupAlarm();
}

void Model_HKDeviceGetRealTimeData::startCollectFrames()
{
	//������...
	this->isUpDataCameraStream = true;


	//���������� g_RealDataCallBack_V30����
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

	//�رձ���
	this->closeAlarm();

	//�ر�Ԥ�� ʵʱ����
	NET_DVR_StopRealPlay(lRealPlayHandle);
}

void Model_HKDeviceGetRealTimeData::setMessageCallBack()
{
	qDebug()<<__FUNCDNAME__ << NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
}

void Model_HKDeviceGetRealTimeData::whitelist()
{
	ifstream iFile;
	iFile.open("������.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "�Ҳ����ļ�";

	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) ��ȡ���ƺš�����NF8202��ΪNF8202
			if (found != std::string::npos) {
				std::cout << "������:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//�ر��ļ�
}

void Model_HKDeviceGetRealTimeData::blacklist()
{
	ifstream iFile;
	iFile.open("������.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "�Ҳ����ļ�";

	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) ��ȡ���ƺš�����NF8202��ΪNF8202
			if (found != std::string::npos) {
				std::cout << "������:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//�ر��ļ�
}

void Model_HKDeviceGetRealTimeData::setupAlarm()
{
	//��������
	NET_DVR_SETUPALARM_PARAM struSetupParam = { 0 };
	struSetupParam.dwSize = sizeof(NET_DVR_SETUPALARM_PARAM);


	struSetupParam.byAlarmInfoType = 1;//�ϴ�������Ϣ���ͣ�0-�ϱ�����Ϣ(NET_DVR_PLATE_RESULT), 1-�±�����Ϣ(NET_ITS_PLATE_RESULT)
	struSetupParam.byLevel = 1;//�������ȼ���0- һ�ȼ����ߣ���1- ���ȼ����У���2- ���ȼ����ͣ�
	//bySupport ��λ��ʾ��ֵ��0 - �ϴ���1 - ���ϴ�;  bit0 - ��ʾ���������Ƿ��ϴ�ͼƬ;


	IHandle = NET_DVR_SetupAlarmChan_V41(lUserID, &struSetupParam);//���������ϴ�ͨ������ȡ��������Ϣ��
	if (IHandle < 0)
	{
		std::cout << "NET_DVR_SetupAlarmChan_V41 Failed! Error number��" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
	std::cout << "\n" << endl;
}

void Model_HKDeviceGetRealTimeData::closeAlarm()
{
	//���������ϴ�ͨ��
	if (!NET_DVR_CloseAlarmChan_V30(IHandle))//�������IHandle
	{
		std::cout << "NET_DVR_CloseAlarmChan_V30 Failed! Error number��" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
	IHandle = -1;//�������;
}
