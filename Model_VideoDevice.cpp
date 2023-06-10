#include "Model_VideoDevice.h"
#include "GlobalConfig.hpp"
#include "View_DeviceChoose.h"
#include <QDateTime>
#include "CustomHm.hpp"

Model_VideoDevice::Model_VideoDevice(View_DeviceChoose * handle,QObject* parent)
{
	this->_pView_DeviceChoose = handle;
	this->moveToThread(&this->mTherad);
	this->mTherad.start(QThread::Priority::HighestPriority);//QThread::Priority::HighestPriority HighPriority
	this->_pCap = new cv::VideoCapture;

	//设置相机参数

	//this->setFrameWidth(PIXWIDTH);
	//this->setFrameHeigth(PIXHIGHT);
	//this->setFrameRate(FRAMERATE);
	//this->setFrameWidth(1920);
	//this->setFrameHeigth(1080);
	//this->setFrameRate(60);

	this->setFrameWidth(PIXWIDTH);
	this->setFrameHeigth(PIXHIGHT);
	this->setFrameRate(FRAMERATE);

	//this->setFrameWidth(960);
	//this->setFrameHeigth(540);
	//this->setFrameRate(60);

}

Model_VideoDevice::Model_VideoDevice(QObject* parent)
{
}

Model_VideoDevice::~Model_VideoDevice()
{
	this->mtimer->stop();
	this->mTherad.quit();
	this->mTherad.wait();
	//cap.release();
	this->_pCap->release();
	delete this->_pCap;
}

QQueue<QString> Model_VideoDevice::getDeviceList()
{
	this->devicesDesc.clear();
	for each (QCameraInfo cameraInfo in QCameraInfo::availableCameras())
	{
		qDebug() << "cameraInfo:" << cameraInfo.description();
		devicesDesc.append(cameraInfo.description());
	}
	//emit this->m2mc_signal_postVideoDevs(devicesDesc.toVector());
    return devicesDesc;
}

void Model_VideoDevice::setDevice(int devNum)
{
	//cap.open(devNum);//, cv::CAP_DSHOW
	//cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));//视频流格式
	qDebug()<<this->_pCap->open(devNum);
	this->_pCap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));//视频流格式
}

void Model_VideoDevice::setDevices(QQueue<int> devNums)
{
}

void Model_VideoDevice::setFrameWidth(int fw)
{
	framewidth = fw;
}

void Model_VideoDevice::setFrameHeigth(int fh)
{
	frameheight = fh;
}

void Model_VideoDevice::setFrameRate(int fr)
{
	framerate = fr;
}

void Model_VideoDevice::setFrameWidth()
{
	//cap.set(cv::CAP_PROP_FRAME_WIDTH, framewidth);//
	this->_pCap->set(cv::CAP_PROP_FRAME_WIDTH, framewidth);//设置宽度
}

void Model_VideoDevice::setFrameHeigth()
{
	//cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameheight); //设置高度
	this->_pCap->set(cv::CAP_PROP_FRAME_HEIGHT, frameheight); //设置高度
}

void Model_VideoDevice::setFrameRate()
{
	//cap.set(cv::CAP_PROP_FPS, framerate);//帧率 帧/秒
	this->_pCap->set(cv::CAP_PROP_FPS, framerate);//帧率 帧/秒
}

void Model_VideoDevice::initDevice()
{
	//if (!cap.isOpened())
	//{
	//	qDebug() << "打开失败";

	//}
	if (!this->_pCap->isOpened())
	{
		qDebug() << "打开失败";

	}
}

void Model_VideoDevice::init()
{
	initMembers();
	initConnect();

	while(!this->_pView_DeviceChoose->m_bool_initCamera)
	{
		QThread::msleep(1);
	}

	setFrameWidth();
	setFrameHeigth();
	setFrameRate();


	qDebug() << __FUNCDNAME__ << "init done!";
}

void Model_VideoDevice::checkDeviceState()
{
}

void Model_VideoDevice::initMembers()
{
	this->mtimer = new QTimer(this);
	this->mtimer->setInterval(20);//访问锁，大概20ms左右，帧率理论上大于30帧
	this->mtimer->moveToThread(&this->mTherad);

	this->_pCustomHm = new CustomHm(this);
}

void Model_VideoDevice::initConnect()
{
	connect(this->mtimer, &QTimer::timeout, this, &Model_VideoDevice::readFrame);//&Model_VideoDevice::

	//start/stop
	connect(this->_pView_DeviceChoose, &View_DeviceChoose::m_signal_modelStart, this, &Model_VideoDevice::start, Qt::QueuedConnection);
	connect(this->_pView_DeviceChoose, &View_DeviceChoose::m_signal_modelStop, this, &Model_VideoDevice::stop,Qt::QueuedConnection);


}

void Model_VideoDevice::readFrame()
{
	//if (this->mtimer->isActive())
	//{
	{
		//qDebug() << "1->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//QMutexLocker mLocker(&this->m_mutex_frame);
		//qDebug() << "2->!";//1！-2！：
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//cap >> frame;//bool judge=cap.read(frame);

		{
			QMutexLocker mLocker(&this->m_mutex_frame);
			*this->_pCap >> frame;//bool judge=cap.read(frame);
		}

		if (this->mFrames.size() < 3)
		{
			this->mFrames.append(this->frame);
		}
		else
		{
			{
				QMutexLocker mMutexLocker_CustomHm(&this->_pCustomHm->mMutex);
				QMutexLocker mMutexLocker(&this->m_mutex_frames);
				this->mFrames.removeFirst();
			}
			
			this->mFrames.append(this->frame);
		}
		
		//qDebug() << "3->!";//1！-2！：20 - 30ms !!!
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
	}

	//	qDebug() << "xaxaxa";
	//}
	

}

void Model_VideoDevice::start()
{
	this->mtimer->start();
}

void Model_VideoDevice::stop()
{
	this->mtimer->stop();
}
