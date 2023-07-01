#pragma once
#include <qobject.h>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "GlobalConfig.hpp"
#include "opencv2/core.hpp"
#include "LvsDrawGradientRound.hpp"


extern QQueue<cv::Mat> mHKFrames;
extern QMutex mFramesMutex;
extern QImage mM_image;
extern cv::Mat g_BGRImage;
extern QString mLicensePlate;
extern QString mLicenseLane;

class View_hotMapAndVideoDisplay;
class Model_VideoDevice;//opencv�汾
class CustomHm;
class Model_HKDeviceGetRealTimeData;

/**
 * ���ܣ�
 * 1������P1_D��648000�����ݽ��в�ֵ
 * 2����ֵ�������ʾ��Ⱦ
 */


class Model_hotMapAndVideoDisplay :public QObject
{
	Q_OBJECT
public:
	Model_hotMapAndVideoDisplay(View_hotMapAndVideoDisplay* handle = nullptr, QObject* _handle = nullptr);
	~Model_hotMapAndVideoDisplay();
	void setView(View_hotMapAndVideoDisplay* handle);
	void start();//��ʼ��Ⱦ
	void stop();//ֹͣ��Ⱦ
	void On_setHmFactorValue(unsigned short value);
	void on_setHmThreshold(double value);
	void on_setP1_DValue(double value);
protected:
	void initMember();
	void initConnect();
private:
	void init();
	void invokeDll();
	void renderHM(const double*);//��Ⱦ����ͼ
	void renderFM();//��Ⱦ��Ƶ֡
	void on_setXAxisCutoutValue(unsigned short);
	void on_setYAxisCutoutValue(unsigned short);
	void on_setCarPlate(QString str);
	void on_setSoundPw(double pw);
	void on_setLane(QString lane);

	//2023.6.26����
	//�ü����ĵ�����
	void on_setCutoutCenter(quint16);
signals:
	void on_signal_replot_video();
	void on_signal_replot_hotmap();
	void on_signal_setMaxValue(QPoint p);
	void on_signal_setCarPaltedisplay(QString);
	void on_signal_setSoundPw(double);
	void on_signal_setCarLane(QString);
	//void on_signal_sendImageMat(cv::Mat src);
	void on_signal_sendImageMat(cv::Mat src,double targetValue);



public:
	//cv::Mat test_;
	QMutex mMutex;
	//δ��ֵ������ͼ����
	double* _pP1_DData = nullptr;
	//P_1D_MAX_lvsĿǰ���ֵ
	double P_1D_MAX_lvs;
protected:
	QThread _mThread;
	QTimer* _pTimer = nullptr;
	//�ü���Χ
	quint16 mXAxisCutOut = 30, mYAxisCutOut = 30;
	//��ɫ��ֵ
	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//����ͼ��Сϵ��
	double mHmSizeFactor = 0.0f;
	//����ͼ�Ƿ���ʾϵ��������Ϊ1
	float hotMapCoefficient = 0;
	//��ʾ��ֵ
	double hotMapCoefficientvalue = 1;//����ʾ
	//��ɫ��ֵ
	//double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
private:
	View_hotMapAndVideoDisplay* _pView_hotMapAndVideoDisplay = nullptr;

	//��ֵ����
	int interp_multiple = 2;
	// ��ֵ�������ͼ����
	//double* Fine_S = new double[2332800];//6����ֵ
	double* Fine_S = new double[259200];//2����ֵ
	//testdlldata
	double* FINE_S = new double[10800];

	//��Ƶ�ӿ� opcv��ȡ����ͷ
	Model_VideoDevice *mModel_VideoDevice = nullptr;
	//��������ͷ
	Model_HKDeviceGetRealTimeData* _pModel_HKDeviceGetRealTimeData = nullptr;

	//����ͼ��Ⱦ QPainter+QImage�汾
	CustomHm *_pCustomHm = nullptr;
	//����ͼ��Ⱦ QPainter+QImage�汾 �Ż���
	LvsDrawGradientRound mLvsDrawGradientRound;

	//���س���
	int mPixWidth = PIXWIDTH;
	int mPixHight = PIXHIGHT;
	//ӳ���
	int mWidthPixRate;
	int mHightPixRate;
	//���ֵλ��
	QPoint mMaxPoint;
	//Դͼ
	//QImage mImage;
	//��ͼ�������еĵ�
	MyPoint mPoint;
	//��ͼ�İ뾶
	int mRadius = 200;
	//�ü�ֵ�е����ֵ
	double mCutoutMax = 0.0f;
	//���ֵλ�ڲü������е�λ��
	QPoint mMaxPos;
	//�ü�ƫ��
	int mOffset = 45;
	//������һ������
	QString mOldLicensePlate;
	//������һ��������
	QString mOldLicenseLane;

	//2023.6.26����
	//�ü����ĵ�����
	//�ü�Y�����ĵ�
	//int mYCenterValue = 90;
	int mYCenterValue = 125;

};

