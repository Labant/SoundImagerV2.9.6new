#pragma once
#include <qobject.h>
#include <QQueue>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "GlobalConfig.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QCameraInfo>
#include <QTimer>
#include <QThread>
#include <QQueue>

class View_DeviceChoose;
class CustomHm;

class Model_VideoDevice :public QObject
{
	Q_OBJECT
public:
	explicit Model_VideoDevice(View_DeviceChoose* handle,QObject* parent = nullptr);
	Model_VideoDevice(QObject* parent = nullptr);
	~Model_VideoDevice();

public:
	QQueue<QString> getDeviceList();
	void checkDeviceState();
	void setDevice(int);
	void setDevices(QQueue<int>);
	void setFrameWidth(int);
	void setFrameHeigth(int);
	void setFrameRate(int);
	void setFrameWidth();
	void setFrameHeigth();
	void setFrameRate();
	void initDevice();
	void init();
signals:
	//signals
	void m2mc_signal_postVideoDevs(QVector<QString>);

private:
	void initMembers();
	void initConnect();
	void readFrame();
	void start();
	void stop();
public:
	QQueue<cv::Mat> mFrames;
	QMutex m_mutex_frames;

	cv::Mat frame;
	QMutex m_mutex_frame;
protected:
	QQueue<QString> devicesDesc;
private:
	//cv::VideoCapture cap;
	cv::VideoCapture *_pCap = nullptr;
	QTimer* mtimer = nullptr;
	int framewidth = 0;
	int frameheight = 0;
	int framerate = 0;

	View_DeviceChoose* _pView_DeviceChoose = nullptr;
	QThread mTherad;

	//访问绘制云图时的锁
	CustomHm* _pCustomHm;
};

