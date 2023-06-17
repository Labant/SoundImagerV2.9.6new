#include "Model_hotMapAndVideoDisplay.h"
#include "View_hotMapAndVideoDisplay.h"
#include "Model_AudioCollector.h"
#include "Model_VideoDevice.h"
#include "CVMat2QImage.h"
#include "CustomHm.hpp"
#include "Model_HKDeviceGetRealTimeData.h"

#include "Model_CarPlateIdentify.h"

//#ifndef MYNAMESPACE_H
//#include "MyNameSpace.h"
//#endif



Model_hotMapAndVideoDisplay::Model_hotMapAndVideoDisplay(View_hotMapAndVideoDisplay* handle, QObject* _handle)
{

	//���ر� ��ʼ��
	this->mWidthPixRate = this->mPixWidth / this->mXAxisCutOut;//all 1280/90 = 14.2
	this->mHightPixRate = this->mPixHight / this->mYAxisCutOut;//all 720/60 = 12

	this->_pP1_DData = new double[64800];
	memset(this->_pP1_DData, 0, sizeof(double) * 64800);

	//���ƶ��߳�
	this->moveToThread(&this->_mThread);
	this->_mThread.start();
	//�ڽ��г�ʼ��
	this->_pView_hotMapAndVideoDisplay = handle;
	//��Ƶ�ӿڳ�ʼ��
	this->mModel_VideoDevice = qobject_cast<Model_VideoDevice*>(_handle); //opencv��ȡ
	this->_pModel_HKDeviceGetRealTimeData = qobject_cast<Model_HKDeviceGetRealTimeData*>(_handle); //����
	this->init();

	////test
	//test_ = imread("C:/Users/lvs/Pictures/46.jpg");
}

Model_hotMapAndVideoDisplay::~Model_hotMapAndVideoDisplay()
{
	if (this->_pView_hotMapAndVideoDisplay != nullptr)
	{
		this->_pView_hotMapAndVideoDisplay = nullptr;
	}
	delete this->_pP1_DData;
	this->_pP1_DData = nullptr;
	delete this->Fine_S;
	this->Fine_S = nullptr;
}

void Model_hotMapAndVideoDisplay::setView(View_hotMapAndVideoDisplay* handle)
{
	this->_pView_hotMapAndVideoDisplay = handle;
}

void Model_hotMapAndVideoDisplay::start()
{
	this->_pTimer->start();
}

void Model_hotMapAndVideoDisplay::stop()
{
	this->_pTimer->stop();
}

void Model_hotMapAndVideoDisplay::initMember()
{
	this->_pTimer = new QTimer;
	this->_pTimer->setInterval(9);
	this->_pTimer->moveToThread(&this->_mThread);

	//qDebug() << "this->_pTimer->thread:" << this->_pTimer->thread();
	this->_pCustomHm = new CustomHm(this);

	//����ʶ��
	this->_pModel_CarPlateIdentify = new Model_CarPlateIdentify(this);

}

void Model_hotMapAndVideoDisplay::initConnect()
{

	//��������ͼ��С����
	//connect()

	//��ʱ�������������dll�����ں����ж�ȡһ������
	connect(this->_pTimer, &QTimer::timeout, this, &Model_hotMapAndVideoDisplay::invokeDll,Qt::DirectConnection);

	//��ʱ������
	connect(this->_pView_hotMapAndVideoDisplay, &View_hotMapAndVideoDisplay::m_signal_start, this, &Model_hotMapAndVideoDisplay::start, Qt::QueuedConnection);
	//��ʱ���ر�
	connect(this->_pView_hotMapAndVideoDisplay, &View_hotMapAndVideoDisplay::m_signal_stop, this, &Model_hotMapAndVideoDisplay::stop, Qt::QueuedConnection);

	//�������̷߳������÷�Χ������
	connect(this->_pView_hotMapAndVideoDisplay, &View_hotMapAndVideoDisplay::on_signal_xAxisRange, this, &Model_hotMapAndVideoDisplay::on_setXAxisCutoutValue, Qt::QueuedConnection);
	connect(this->_pView_hotMapAndVideoDisplay, &View_hotMapAndVideoDisplay::on_signal_yAxisRange, this, &Model_hotMapAndVideoDisplay::on_setYAxisCutoutValue, Qt::QueuedConnection);
	//ʶ����
	//connect(this, &Model_hotMapAndVideoDisplay::on_signal_sendImageMat, this->_pModel_CarPlateIdentify, &Model_CarPlateIdentify::on_signal_receiveMat, Qt::QueuedConnection);

	//����ʶ����
	//����
	connect(this->_pModel_CarPlateIdentify, &Model_CarPlateIdentify::on_signal_sendIdentifyRes, this, &Model_hotMapAndVideoDisplay::on_setCarPlate,Qt::DirectConnection);
	//������
	connect(this->_pModel_CarPlateIdentify, &Model_CarPlateIdentify::on_signal_sendSoundPw, this, &Model_hotMapAndVideoDisplay::on_setSoundPw, Qt::DirectConnection);
	//����
	connect(this->_pModel_CarPlateIdentify, &Model_CarPlateIdentify::on_signal_sendCarLane, this, &Model_hotMapAndVideoDisplay::on_setLane, Qt::DirectConnection);

}

void Model_hotMapAndVideoDisplay::init()
{
	this->initMember();
	this->initConnect();
}
void Model_hotMapAndVideoDisplay::renderHM(const double* data)
{
	//qDebug() << "data[0]:" << data[0];
	//qDebug() << "data[2332799]:" << data[2332799];

	if (this->_pView_hotMapAndVideoDisplay == nullptr)
		return;

	//���ϵ���ı��С
	static QCPColorGradient cpColorGradient;
	cpColorGradient.clearColorStops();
	//cpColorGradient.setPeriodic()//������
	//cpColorGradient.setLevelCount(360);//Ĭ��ֵ350
	cpColorGradient.setColorStopAt(0, QColor(Qt::transparent));//͸��
	cpColorGradient.setColorStopAt(this->posColor[0] + this->mHmSizeFactor * 2.0f - 0.001, QColor(Qt::transparent));//͸��
	//�ռ�ȣ�0.02��0.03��0.01��0.01 ��6��

	cpColorGradient.setColorStopAt(posColor[0] + this->mHmSizeFactor * 2.0f, hotMapCoefficient > 0 ? QColor(115, 115, 255, 150) : QColor(Qt::transparent));//������ɫ ����ʾ
	cpColorGradient.setColorStopAt(this->posColor[1] + this->mHmSizeFactor * 3.0f, this->hotMapCoefficient > 0 ? QColor(77, 255, 255, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(this->posColor[2] + this->mHmSizeFactor * 1.0f, this->hotMapCoefficient > 0 ? QColor(255, 255, 0, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(this->posColor[3] + this->mHmSizeFactor * 1.0f, this->hotMapCoefficient > 0 ? QColor(255, 71, 26, 150) : QColor(Qt::transparent));//����ɫ
	cpColorGradient.setColorStopAt(this->posColor[4], this->hotMapCoefficient > 0 ? QColor(255, 71, 26, 255) : QColor(Qt::transparent));//����ɫ 255
	//cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(102, 20, 0, 150) : QColor(Qt::transparent));//���ɫ ���ÿ�

	//cpColorGradient.setColorStops()
	cpColorGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciHSV);//ciHSV
	this->_pView_hotMapAndVideoDisplay->colorMap->setGradient(cpColorGradient);

	//test
	//this->_pView_hotMapAndVideoDisplay->colorMap->setGradient(QCPColorGradient::gpJet);

	// ������������ά�ȣ���ɫ������ʹ�������ݵ㶼λ����ɫ������ʾ�ķ�Χ��:
	this->_pView_hotMapAndVideoDisplay->colorMap->rescaleDataRange(true);

	// �������ż���x����ֵ��y���ᣬ�Ա���Կ���������ɫͼ��
	this->_pView_hotMapAndVideoDisplay->rescaleAxes();

	//δ����6��
	//IMX291
	//manual
	this->_pView_hotMapAndVideoDisplay->colorMap->data()->setSize(this->mXAxisCutOut * 2, this->mYAxisCutOut * 2); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	this->_pView_hotMapAndVideoDisplay->colorMap->data()->setRange(QCPRange(0, 360), QCPRange(0, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ
	//Ŀ��ü���0-30 ��벿��
	for (int xIndex = 0; xIndex < this->mXAxisCutOut; ++xIndex)//�ü��ڼ���
	{
		for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
		{
			this->_pView_hotMapAndVideoDisplay->colorMap->data()->setCell(xIndex + this->mXAxisCutOut, yIndex - (90 - this->mYAxisCutOut), data[xIndex + yIndex * 360]);
		}
	}
	//Ŀ��ü���330-360 ǰ�벿��
	for (int xIndex = 360 - this->mXAxisCutOut; xIndex < 360; ++xIndex)//�ü��ڼ���
	{
		for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
		{
			this->_pView_hotMapAndVideoDisplay->colorMap->data()->setCell(xIndex - (360 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), data[xIndex + yIndex * 360]);
		}
	}

	//��������ͼ����
	memset((void*)data, 0, sizeof(double) * SAMPLE);

	//Ѱ�Ҳü������е����ֵ

	this->mCutoutMax = 0;
	this->mMaxPos.setX(-1);
	this->mMaxPoint.setY(-1);

	for (int i = 0; i < (this->mXAxisCutOut << 1); i++)//<< 1
	{
		for (int j = 0; j < (this->mYAxisCutOut << 1); j++)//<< 1
		{
			
			if (this->_pView_hotMapAndVideoDisplay->colorMap->data()->cell(i, j) > mCutoutMax)
			{
				//qDebug() << this->_pView_hotMapAndVideoDisplay->colorMap->data()->cell(i, j) << ">" << mCutoutMax;
				//qDebug() << "MAXPOS:" << i << "," << j;
				mCutoutMax = this->_pView_hotMapAndVideoDisplay->colorMap->data()->cell(i, j);
				mMaxPos.setX(i>>1); mMaxPos.setY(j);//(this->mYAxisCutOut << 1) - 
			}
		}
	}

	//qDebug() << "mMaxPos:" << mMaxPos << "it's value :"<< mCutoutMax;

	//��Ⱦ����ͼ -->replot
	//emit this->on_signal_replot_hotmap();

	//// ������������ά�ȣ���ɫ������ʹ�������ݵ㶼λ����ɫ������ʾ�ķ�Χ��:
	//this->_pView_hotMapAndVideoDisplay->colorMap->rescaleDataRange(true);

	//// �������ż���x����ֵ��y���ᣬ�Ա���Կ���������ɫͼ��
	//this->_pView_hotMapAndVideoDisplay->rescaleAxes();
}

void Model_hotMapAndVideoDisplay::renderFM()
{
	//qDebug() << "1!";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
	//IMX291���  ���ñ���
	{
		//qDebug() << "->1!";//1��-2����10-30ms !!!
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//QMutexLocker mLocker(&this->mModel_VideoDevice->m_mutex_frame);
		//qDebug() << "->2!";
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//if (this->mModel_VideoDevice->frame.empty())return;
		//qDebug() << "->3!"; //3��->4����2ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//this->_pView_hotMapAndVideoDisplay->setBackground(QPixmap::fromImage(CVMat2QImage::QCVMat2QImage(this->mModel_VideoDevice->frame)), true, Qt::KeepAspectRatioByExpanding);//KeepAspectRatio
		//qDebug() << "->4!";
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		
		/*�������*/
		//QImage mImage;
		//{
		//	if (this->mModel_VideoDevice->mFrames.isEmpty())
		//		return;
		//	//QQueue���� + Lock����
		//	qDebug() << "1->!";//1��-2����1ms
		//	qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	QMutexLocker m_MutexLocker(&this->mModel_VideoDevice->m_mutex_frames);
		//	qDebug() << "2->!";//1��-2����1ms
		//	qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	mImage = CVMat2QImage::QCVMat2QImage(this->mModel_VideoDevice->mFrames.takeFirst());//.copy()
		//	qDebug() << "3->!";//1��-2����1ms
		//	qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		//	////wait lock ����
		//	//qDebug() << "1->!";//1��-2����1ms
		//	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	//if (this->mModel_VideoDevice->frame.empty())return;
		//	//qDebug() << "2->!";//1��-2����1ms
		//	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	//QMutexLocker mLocker(&this->mModel_VideoDevice->m_mutex_frame);
		//	//qDebug() << "3->!";//1��-2����1ms
		//	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	//mImage = CVMat2QImage::QCVMat2QImage(this->mModel_VideoDevice->frame);
		//	//qDebug() << "4->!";//1��-2����1ms
		//	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		//	//��֯��ͼ����
		//	this->mPoint.posX = this->mMaxPos.rx() * this->mWidthPixRate;
		//	this->mPoint.posY = (this->mYAxisCutOut - this->mMaxPos.ry()) * this->mHightPixRate;//QColorMap��QPainter��Ⱦ�ĳ�ʼλ�ò�ͬ��ǰ�ߴ����£����ߴ����ϡ�
		//	this->mPoint.radius = this->mRadius;
		//	this->mPoint.count = 0;
		//	qDebug() << "5->!";//1��-2����1ms
		//	qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		//}
		//{
		//	//��ͼ���� �����ⲿ��
		//	this->_pCustomHm->DrawCloudMap(mImage, mPoint, this->mModel_VideoDevice->m_mutex_frames);
		//	qDebug() << "6->!";//1��-2����1ms
		//	qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//	//qDebug() << mImage.save("mImage.jpg","JPG",100);
		//	this->_pView_hotMapAndVideoDisplay->setBackground(QPixmap::fromImage(mImage), true, Qt::KeepAspectRatioByExpanding);//KeepAspectRatio KeepAspectRatioByExpanding

		//}
		//

	
		//�������
		QImage mImage;
		cv::Mat src;
		{
			if (mHKFrames.isEmpty())
				return;
			//QQueue���� + Lock����
			//qDebug() << "1->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
			QMutexLocker mLocker(&mFramesMutex);
			//qDebug() << "2->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
			src = mHKFrames.takeFirst();
			mImage = CVMat2QImage::QCVMat2QImage(src);//.copy()
			//qDebug() << "3->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

			//if (mM_image.isNull())
			//	return;

			//��֯��ͼ����
			this->mPoint.posX = this->mMaxPos.rx() * this->mWidthPixRate;
			this->mPoint.posY = (this->mYAxisCutOut - this->mMaxPos.ry()) * this->mHightPixRate;//QColorMap��QPainter��Ⱦ�ĳ�ʼλ�ò�ͬ��ǰ�ߴ����£����ߴ����ϡ�
			this->mPoint.radius = this->mRadius;
			this->mPoint.count = 0;
			//qDebug() << "5->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

			//mImage = mM_image;
		}
		{
			if (this->hotMapCoefficient)//Ϊ�棬����ͼ
			{

				//��ͼ���� �����ⲿ��
				//this->_pCustomHm->DrawCloudMap(mImage, mPoint,mFramesMutex);
				this->_pCustomHm->DrawCloudMap(mImage, mPoint, mFramesMutex);

				//ʶ���� //���߳�ʶ��
				emit this->on_signal_sendImageMat(src, this->P_1D_MAX_lvs);
			}


			//qDebug() << "6->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
			//qDebug() << mImage.save("mImage.jpg","JPG",100);
			this->_pView_hotMapAndVideoDisplay->setBackground(QPixmap::fromImage(mImage), true, Qt::KeepAspectRatioByExpanding);//KeepAspectRatio KeepAspectRatioByExpanding
			//this->_pView_hotMapAndVideoDisplay->setBackground(QPixmap::fromImage(mM_image), true, Qt::KeepAspectRatioByExpanding);//KeepAspectRatio KeepAspectRatioByExpanding

		}


	}
	//qDebug() << "6!";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
	//ˢ����Ƶ�� -->report()
	emit this->on_signal_replot_video();
	//this->_pView_hotMapAndVideoDisplay->replot();
}
void Model_hotMapAndVideoDisplay::on_setXAxisCutoutValue(unsigned short value)
{
	//qDebug() << "1:" << value;
	this->mXAxisCutOut = value;
	//���ر�
	this->mWidthPixRate = this->mPixWidth / this->mXAxisCutOut;
}
void Model_hotMapAndVideoDisplay::on_setYAxisCutoutValue(unsigned short value)
{
	//qDebug() << "2:" << value;
	this->mYAxisCutOut = value;
	//���ر�
	this->mHightPixRate = this->mPixHight / this->mYAxisCutOut;
}
void Model_hotMapAndVideoDisplay::on_setCarPlate(QString str)
{
	emit this->on_signal_setCarPaltedisplay(str);
}
void Model_hotMapAndVideoDisplay::on_setSoundPw(double pw)
{
	emit this->on_signal_setSoundPw(pw);
}
void Model_hotMapAndVideoDisplay::on_setLane(QString lane)
{
	emit this->on_signal_setCarLane(lane);
}
void Model_hotMapAndVideoDisplay::invokeDll()
{
	//test
	//emit this->on_signal_replot_hotmap();

	//
	//emit this->on_signal_replot_video();

	//qDebug() << "!1:" << QDateTime::currentMSecsSinceEpoch();


	//qDebug() << "!2:" << QDateTime::currentMSecsSinceEpoch();
	//�ж�pData xData yData zData ����Ϊ0��ִ��
	if (this->_pP1_DData[0] == 0)
		return;

	//������ͼλ��
	this->renderHM(this->_pP1_DData);
	//��Ƶ��Ⱦ
	this->renderFM();

	//qDebug() << "!3:" << QDateTime::currentMSecsSinceEpoch();

	//��Ⱦ����ͼ
	//this->renderHM(this->_pP1_DData);
	//this->renderHM(this->Fine_S);

	//qDebug() << "!4:" << QDateTime::currentMSecsSinceEpoch();

	//�ú���0
	{
		QMutexLocker mLocker(&this->mMutex);
		memset(this->_pP1_DData, 0, 64800 * sizeof(double));
	}
	//qDebug() << "!5:" << QDateTime::currentMSecsSinceEpoch();



}
void Model_hotMapAndVideoDisplay::On_setHmFactorValue(unsigned short value)
{
	//QColorMap �汾
	/**��Ҫ����0.94���ɷ�Ϊ��0.95(��С)��0.96��0.97��0.98��0.99�����
	 * valueֵ����Ϊ��50->90 =>/1000 = 0.05->0.09;�������Ϊ0.9
	 */
	//this->mHmSizeFactor = (1.0f - (value / 1000.0f + 0.9f)) / 6.0f;
	//this->mHmSizeFactor = 1.05;

	//QPainter+QImage�汾
	this->mRadius = value;
	//qDebug() << "this->mHmSizeFactor:1" << this->mHmSizeFactor;
}

void Model_hotMapAndVideoDisplay::on_setHmThreshold(double value)
{
	this->hotMapCoefficientvalue = value;

}

void Model_hotMapAndVideoDisplay::on_setP1_DValue(double value)
{
	this->P_1D_MAX_lvs = value;
	//qDebug() << "this->P_1D_MAX_lvs:" << this->P_1D_MAX_lvs;
	if (this->P_1D_MAX_lvs > this->hotMapCoefficientvalue)
	{
		this->hotMapCoefficient = 1;//��ʾ��Ϊ��
	}
	else
	{
		this->hotMapCoefficient = 0;//��ʾ��Ϊ��
	}
}

