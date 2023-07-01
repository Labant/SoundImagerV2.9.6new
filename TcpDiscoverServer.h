#pragma once
#include <qobject.h>
#include <QTcpServer>
#include <QTcpSocket>
#include "GlobalConfig.hpp"
#include "DataPack.h"
#include <QQueue>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>


class TcpDiscoverServer :public QObject
{
	Q_OBJECT
public:
	TcpDiscoverServer(QObject* parent = nullptr);
	~TcpDiscoverServer();

	void openMonitorPort();//开启监测端口。等待客户端自动连接
	void clossTheServer();//停止TcpServer
	void start();//开始解析数据
	void stop();//停止解析数据
	void setVKParameter(SAMPLEBITS);

	QString getVKDeviceName();
protected:
	void initMembers();
	void initConnect();
private:
	void init();

	void discoverConnect();
	void initSocketConnect(QTcpSocket*);
	void discoverState(int);
	void readData();



	//处理Hex数据,在此中可以调用 dealWithPack()
	void dealWithData();
	//处理每个数据包,在此中可以调用 calculateVolt()
	void dealWithPack(QString, SAMPLEBITS);

	//辅助函数
	int TcpDiscoverServer::Convert16BitstoInt(unsigned char lsb, unsigned char msb);
	int TcpDiscoverServer::Convert24bitstoInt(unsigned char lsb1, unsigned char lsb, unsigned char msb);
	double TcpDiscoverServer::calculateVolt(QString str_val_hex, SAMPLEBITS);

signals:
	void on_signal_startAnalyze();
	void on_signal_stopAnalyze();

	//-->View_DeviceChoose 告知连接状态
	void on_signal_TcpState(int);
public:
	QQueue<RawDataPack> mRawDataQueue;
	//外部锁，与AudioCollector共享
	QMutex mOutMutex;
	QTcpSocket* _pTcpSocket = nullptr;
	bool mInitSocketFlag = 0;
protected:
	QTcpServer* _pTcpServer = nullptr;
	QTimer* _pTimer = nullptr;
	QThread mTimerThread;
	QThread mThreadSocket;
private:
	QString mHexData;
	RawDataPack mRawDataPack;
	int chdat;
	int has4800 = 0;
	QString mCutOffStr;//不完整数据
	int mPackNums = 0;//数据帧里的pack数
	//接收客户端数据和处理客户端数据之间的共享锁
	QMutex mMutexLockForData;
	//采样深度类型
	SAMPLEBITS mBits = SAMPLEBITS::BITS_24;

	//test
	qint64 tt1 = 0;

};

