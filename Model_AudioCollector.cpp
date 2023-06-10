#include "Model_AudioCollector.h"
#include "View_TabWidget_Setting.h"



Model_AudioCollector::Model_AudioCollector(View_TabWidget_Setting* handle, QObject* parent)
{
	//this->setParent(parent);

	this->moveToThread(&this->mThread);
	this->mThread.start();

	//qDebug() << "1:" << this->thread();
	this->mView_TabWidget_Setting = handle;
	this->init();
	this->mTimer.moveToThread(&this->mThread);
	this->mTimer.setInterval(20);//1s钟读取10次
	//qDebug() << "2:" << this->mTimer.thread();
}

Model_AudioCollector::Model_AudioCollector()
{
}

Model_AudioCollector::~Model_AudioCollector()
{
	this->stop();
	this->mThread.quit();
	this->mThread.wait();
}

void Model_AudioCollector::setView(View_TabWidget_Setting* handle)
{
	this->mView_TabWidget_Setting = handle;
}

void Model_AudioCollector::start()
{
	this->mTimer.start();
}

void Model_AudioCollector::stop()
{
	this->mTimer.stop();
}

void Model_AudioCollector::init()
{
	this->initMember();
	this->initConnect();
}

void Model_AudioCollector::initConnect()
{
	connect(&this->mTimer, &QTimer::timeout, this, &Model_AudioCollector::read);
	connect(this, &Model_AudioCollector::on_signal_stopTimer, this, &Model_AudioCollector::stop);
	connect(this, &Model_AudioCollector::on_signal_startTimer, this, &Model_AudioCollector::start);
}

void Model_AudioCollector::initMember()
{
}

void Model_AudioCollector::read()
{
	static QList<double> rawData;
	memset(mRawDataPack.pData, 0, sizeof(double) * 4800);
	memset(mRawDataPack.xData, 0, sizeof(double) * 4800);
	memset(mRawDataPack.yData, 0, sizeof(double) * 4800);
	memset(mRawDataPack.zData, 0, sizeof(double) * 4800);

	rawData.clear();

	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		//阅读一帧数据
		this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->read();

		//方法二：一个循环
		for (int i = 0; i < SAMPLE; i++)//共4810*4个点 P通道补偿9.6==10个点
		{
			//this->p[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i];
			//this->x[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 1 + 40];
			//this->y[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 2 + 40];
			//this->z[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 3 + 40];
			this->p[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i];
			this->x[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 1];
			this->y[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 2];
			this->z[i] = this->mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 3];
		}

		memcpy(this->mRawDataPack.pData, this->p, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.xData, this->x, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.yData, this->y, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.zData, this->z, sizeof(double) * 4800);

		{
			QMutexLocker mLocker(&this->mMutex);
			mRawData.append(mRawDataPack);
		}

		break;
	case CAPTURETYPE::NICaptureCard:

		if (this->mView_TabWidget_Setting == nullptr || this->mView_TabWidget_Setting->mView_deviceChoose == nullptr
			||this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
			return; 

		//阅读一帧数据
		this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->read();

		{
			QMutexLocker m_lock(&this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->m_lock_pRawData);
			for (int i = 0; i < SAMPLE; i++)//共4810*4个点	P通道补偿9.6==10个点
			{
				//分组取值，不是交错取值
				//this->p[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 10];
				//this->x[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 1 * (SAMPLE + 10)];//(FS + 10)
				//this->y[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 2 * (SAMPLE + 10)];
				//this->z[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 3 * (SAMPLE + 10)];

				this->p[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i];
				this->x[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 1 * (SAMPLE)];
				this->y[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 2 * (SAMPLE)];
				this->z[i] = this->mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 3 * (SAMPLE)];

				////交错取值
				//p1[i] = this->mRawData[i*4];
				//x1[i] = this->mRawData[i*4 + 1];
				//y1[i] = this->mRawData[i*4 + 2];
				//z1[i] = this->mRawData[i*4 + 3];

			}

		}

		memcpy(this->mRawDataPack.pData, this->p, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.xData, this->x, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.yData, this->y, sizeof(double) * 4800);
		memcpy(this->mRawDataPack.zData, this->z, sizeof(double) * 4800);

		if (mRawData.size() > 9)
		{
			mRawData.removeFirst();
		}
		else
		{
			QMutexLocker mLocker(&this->mMutex);
			mRawData.append(mRawDataPack);
		}
		

		break;
	default:
		break;
	}


}
