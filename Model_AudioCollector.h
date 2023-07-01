#pragma once
#include <qobject.h>
#include <QQueue>
#include "MyStruct.h"
#include "DataPack.h"
#include <QMutex>
#include <QTimer>
#include <QThread>
#include "TcpDiscoverServer.h"
#include <QTcpServer>
#include <QTcpSocket>

//class DataPack;
class View_TabWidget_Setting;

using namespace MYSTRUCT;




class Model_AudioCollector :public QObject
{
	Q_OBJECT
public:
	explicit Model_AudioCollector(View_TabWidget_Setting *handle,QObject* parent = nullptr);
	Model_AudioCollector();
	~Model_AudioCollector();
	void setView(View_TabWidget_Setting* handle);
	void start();
	void stop();

protected:
	void init();
private:
	void initConnect();
	void initMember();
	void read();

	//��ʼ��tcpServer
	void initTcpServer();
	//���ÿͻ��˲���
	void initClientParameters();
	void startTcpServer();
	void stopTcpServer();

signals:
	void on_signal_stopTimer();
	void on_signal_startTimer();
public:
	//�洢ԭʼ����
	QQueue<RawDataPack> mRawData;
	//���ݰ�
	RawDataPack mRawDataPack;
	//������
	QMutex mMutex;
	//�ɼ�������
	int mCaptrueType = CAPTURETYPE::VKCaptureCard;
	//�������߳�
	QThread mThread;


	//VK�ɼ���-���ݷ�����
	TcpDiscoverServer* _pTcpDiscoverServer = nullptr;

	//test
	QTcpServer* _pTcpServer = nullptr;
	QTcpSocket* _pTcpSocket = nullptr;

protected:
	//ͨ������
	double p[4800], x[4800], y[4800], z[4800];
	//���ƶ�ȡ���
	QTimer mTimer;

private:
	//��ȡNI�������������
	View_TabWidget_Setting* mView_TabWidget_Setting = nullptr;

};

