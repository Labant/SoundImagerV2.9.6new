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
class Model_VideoDevice;//opencv版本
class CustomHm;
class Model_HKDeviceGetRealTimeData;

/**
 * 功能：
 * 1、处理P1_D，648000的数据进行插值
 * 2、插值后进行显示渲染
 */


class Model_hotMapAndVideoDisplay :public QObject
{
	Q_OBJECT
public:
	Model_hotMapAndVideoDisplay(View_hotMapAndVideoDisplay* handle = nullptr, QObject* _handle = nullptr);
	~Model_hotMapAndVideoDisplay();
	void setView(View_hotMapAndVideoDisplay* handle);
	void start();//开始渲染
	void stop();//停止渲染
	void On_setHmFactorValue(unsigned short value);
	void on_setHmThreshold(double value);
	void on_setP1_DValue(double value);
protected:
	void initMember();
	void initConnect();
private:
	void init();
	void invokeDll();
	void renderHM(const double*);//渲染热力图
	void renderFM();//渲染视频帧
	void on_setXAxisCutoutValue(unsigned short);
	void on_setYAxisCutoutValue(unsigned short);
	void on_setCarPlate(QString str);
	void on_setSoundPw(double pw);
	void on_setLane(QString lane);

	//2023.6.26新增
	//裁剪中心点设置
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
	//未插值的热力图数据
	double* _pP1_DData = nullptr;
	//P_1D_MAX_lvs目前最大值
	double P_1D_MAX_lvs;
protected:
	QThread _mThread;
	QTimer* _pTimer = nullptr;
	//裁剪范围
	quint16 mXAxisCutOut = 30, mYAxisCutOut = 30;
	//颜色阈值
	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//热力图大小系数
	double mHmSizeFactor = 0.0f;
	//热力图是否显示系数。基数为1
	float hotMapCoefficient = 0;
	//显示阈值
	double hotMapCoefficientvalue = 1;//不显示
	//颜色阈值
	//double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
private:
	View_hotMapAndVideoDisplay* _pView_hotMapAndVideoDisplay = nullptr;

	//插值倍数
	int interp_multiple = 2;
	// 插值后的热力图数据
	//double* Fine_S = new double[2332800];//6倍插值
	double* Fine_S = new double[259200];//2倍插值
	//testdlldata
	double* FINE_S = new double[10800];

	//视频接口 opcv读取摄像头
	Model_VideoDevice *mModel_VideoDevice = nullptr;
	//海康摄像头
	Model_HKDeviceGetRealTimeData* _pModel_HKDeviceGetRealTimeData = nullptr;

	//热力图渲染 QPainter+QImage版本
	CustomHm *_pCustomHm = nullptr;
	//热力图渲染 QPainter+QImage版本 优化版
	LvsDrawGradientRound mLvsDrawGradientRound;

	//像素长宽
	int mPixWidth = PIXWIDTH;
	int mPixHight = PIXHIGHT;
	//映射比
	int mWidthPixRate;
	int mHightPixRate;
	//最大值位置
	QPoint mMaxPoint;
	//源图
	//QImage mImage;
	//云图在像素中的点
	MyPoint mPoint;
	//云图的半径
	int mRadius = 200;
	//裁剪值中的最大值
	double mCutoutMax = 0.0f;
	//最大值位于裁剪矩阵中的位置
	QPoint mMaxPos;
	//裁剪偏移
	int mOffset = 45;
	//保存上一个车牌
	QString mOldLicensePlate;
	//保存上一个车道号
	QString mOldLicenseLane;

	//2023.6.26新增
	//裁剪中心点设置
	//裁剪Y轴中心点
	//int mYCenterValue = 90;
	int mYCenterValue = 125;

};

