#pragma once
#include <qobject.h>
#include <QQueue>
#include "MyStruct.h"
#include "DataPack.h"
#include <QMutex>
#include <QTimer>
#include <QThread>

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
	int mCaptrueType = CAPTURETYPE::SoundCard;
	//�������߳�
	QThread mThread;
protected:
	//ͨ������
	double p[4800], x[4800], y[4800], z[4800];
	//���ƶ�ȡ���
	QTimer mTimer;

private:
	//��ȡNI�������������
	View_TabWidget_Setting* mView_TabWidget_Setting = nullptr;
};

