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

	void openMonitorPort();//�������˿ڡ��ȴ��ͻ����Զ�����
	void clossTheServer();//ֹͣTcpServer
	void start();//��ʼ��������
	void stop();//ֹͣ��������
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



	//����Hex����,�ڴ��п��Ե��� dealWithPack()
	void dealWithData();
	//����ÿ�����ݰ�,�ڴ��п��Ե��� calculateVolt()
	void dealWithPack(QString, SAMPLEBITS);

	//��������
	int TcpDiscoverServer::Convert16BitstoInt(unsigned char lsb, unsigned char msb);
	int TcpDiscoverServer::Convert24bitstoInt(unsigned char lsb1, unsigned char lsb, unsigned char msb);
	double TcpDiscoverServer::calculateVolt(QString str_val_hex, SAMPLEBITS);

signals:
	void on_signal_startAnalyze();
	void on_signal_stopAnalyze();

	//-->View_DeviceChoose ��֪����״̬
	void on_signal_TcpState(int);
public:
	QQueue<RawDataPack> mRawDataQueue;
	//�ⲿ������AudioCollector����
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
	QString mCutOffStr;//����������
	int mPackNums = 0;//����֡���pack��
	//���տͻ������ݺʹ���ͻ�������֮��Ĺ�����
	QMutex mMutexLockForData;
	//�����������
	SAMPLEBITS mBits = SAMPLEBITS::BITS_24;

	//test
	qint64 tt1 = 0;

};

