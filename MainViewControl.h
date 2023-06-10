#pragma once

#include <QtWidgets/QWidget>
//#include "ui_test_customPlot_hreatMap.h"

#include "GLEWINQT2.h"
#include <qcustomplot.h>
#include <QFile>
#include "direction_spl.h"
#include "rtwtypes.h"
#include <QTimer>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "fftw3.h"
#include "FFT_change.h"
#include <QThread>
#include "Custom_Box.h"
#include "CVMat2QImage.h"
#include "View_DeviceChoose.h"
#include "View_FrequencyTimeDomain.h"
#include "Model_RecordAudio.h"
#include "MyStruct.h"
#include "View_TabWidget_Setting.h"
#include "Model_VideoDevice.h"
#include <QQueue>
#include "CalibrationDll.h"
#include "MainModel.h"
#include "filtDll.h"
#include "DataPack.h"
#include "View_hotMapAndVideoDisplay.h"
#include <QTimer>
#include "GlobalConfig.hpp"

//#include "include/ObServerationAngle.h"
//#include "include/Camera.h"


QT_BEGIN_NAMESPACE
namespace Ui { class test_customPlot_hreatMapClass; };
QT_END_NAMESPACE

using namespace MYSTRUCT;



class MainModel;

class MainViewControl : public QWidget
{
    Q_OBJECT

public:
	MainViewControl(QWidget *parent = nullptr);
    ~MainViewControl();
    
public:
	//receive resData
    //double P_beamforming[64800];
	int P1_D_Size = 64800 * 4;
	double P_1D[64800];//未双向双倍插值
	//double *P_1D = new double[64800 * 4];//双向双倍插值
	double P_1D_MAX_lvs;
	//receive rawData
	//double p[1000], x[1000], y[1000], z[1000];
	double p[4800], x[4800], y[4800], z[4800];
	//拷贝 p x y z数据，用于进行排序后对时域y轴设置，避免对频域进行影响
	double p_copy[4800], x_copy[4800], y_copy[4800], z_copy[4800];
	//拷贝 频率数据，用于进行排序后对于频域y轴设置，避免对频域渲染进行影响
	double *p_fre_copy = nullptr, *x_fre_copy = nullptr, *y_fre_copy = nullptr, *z_fre_copy = nullptr;
	QMutex m_dataCopy;

	//fft out
	float p_out[1000];
	//判断doFFTData函数是否完成
	bool isFftDown = 0;
	
	//Save File
	QFile* p_file = nullptr, * x_file = nullptr, * y_file = nullptr, * z_file = nullptr, * res_file = nullptr;
	//采集卡类型
	int mCaptrueType = CAPTURETYPE::SoundCard;
	////声卡对象
	//soundCard* pSoundCard = nullptr;
	////ni 采集卡对象
	//NiDevice* m_niDevice = nullptr;

	QCPColorMap* colorMap = nullptr;
	int nx = 180;
	int ny = 130;
	float hotMapCoefficient = 1; //热力图大小系数。基数为1

	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//double posColor[5] = { 0.93 + (0.05 / 6.0f) * 2,0.95 + (0.05 / 6.0f) * 3,0.98 + (0.05 / 6.0f) * 1,0.99 + (0.05 / 6.0f) * 1,1 };//小了点
	//空间比：0.02，0.03，0.01，0.01 共6份
	/**只要大于0.94都可以（1 - 0.99 = 0.01；0.01*6 = 0.06；1-0.06 = 0.94；即大于0.94保证不越界）
	*1-0.95 = 0.05:效果变小。 
	*1 - 0.97 = 0.03:效果变小
	 * 1-0.99 = 0.01：等同0.03效果，间隔增幅太小
	 */
	
	//test
	//double posColor[5] = { 0.975,0.98,0.985,0.99,1 };
	//double posColor[5] = { 0.99200,0.99300,0.99700,0.99800,1 };
	//double posColor[5] = { 0.993,0.995,0.998,0.999,1 }; //不规则圆形

	//添加系数改变大小
	//QCPColorGradient cpColorGradient;

	//test 写入csv
	QFile P_1DCSV;

	// 主渲染定时器
	QTimer timer;

	//每一帧最大值
	QPair<int, int> maxp;//at[0]:0~180 ; at[1]:0~360

	//每次转换的image
	QImage t_image;//(QString("E:/YoudaoData/qq7418C6DA0DA187D4CBD6E5B83787CEFB/02739732A5A4493F8ECA09C2D099D80A/0.jpeg"))

	//loyout
	//QCustomPlot *plot_heatmap = nullptr;//热力图
	View_hotMapAndVideoDisplay* _pView_hotMapAndVideoDisplay = nullptr;

	//Test_openGL* m_shereLocalation = nullptr;
	//球体
	GLEWINQT2* m_shereLocalation = nullptr;

	//通用
	View_FrequencyTimeDomain* m_freTime_general = nullptr;
	//时频域阈值
	float mThresholdVT, mThresholdVF;
	//QVBoxLayout* vbox;//主容器
	//QHBoxLayout *hBox[2];//左边 二级容器
	QHBoxLayout* hbox;//主容器
	QVBoxLayout* vBox[2];//左边 二级容器
	QHBoxLayout* xyBox;//二级容器
	QVBoxLayout* vbox_e_a;//三级容器 方位俯仰角在二级容器中的容器
	QVBoxLayout* vbox_time_pxyz;//三级容器 时域集合
	QGroupBox* mGroupBox_XYPos;
	QGroupBox* mGroupBox_shere;
	QGroupBox* mGroupBox_freTimeDomain;
	QGroupBox* mGroupBox_videoHotMap;
	//QGroupBox* mGroupBox_eleAngle;
	//QGroupBox* mGroupBox_azimAngle;
	QVBoxLayout* plot_shere_box;
	QVBoxLayout* plot_timefreqDomain_box;
	QVBoxLayout* plot_videoDomain_box;
	QVBoxLayout* plot_eleAngle_box;
	QVBoxLayout* plot_azimAngle_box;

	//X Y value 显示的label
	QLabel *xylable[4];

	//设置box 选择视频设备和音频设备
	View_TabWidget_Setting* _mView_TabWidget_Setting = nullptr;

	//MainModel
	MainModel* _pMainModel = nullptr;

	//fft obj
	FFT_change *fft_change = nullptr;
	//timer obj  fft框框的线程定时器
	QTimer* timer_thread_1 = nullptr;
	//QThread obj fft框框的线程
	QThread *fft_thread = nullptr;

	//timer obj  相机阻塞等待数据的线程定时器
	QTimer* timer_thread_2 = nullptr;
	//QThread obj 相机阻塞等待数据的线程
	QThread* camera_thread = nullptr;

	//相机参数
	//const int camera_frame_width = 848;//640  848
	//const int camera_frame_height = 480;
	//const int camera_fps = 60;
	//int camera_exposure = 200;
	//rs2::pipeline         pipe;
	//rs2::frameset frames;
	//是否初始化相机
	bool isInitCamera = 0;

	////flag 标志 阻塞数据帧是否结束等待
	//bool b_flag_Done = false;

	//时频域中 combox的index
	//int m_int_comboxIndex = 0;

	//录音mode
	Model_RecordAudio* m_Model_AudioRecord = nullptr;
	//录音 thread
	QThread* m_thread_AudioRecord = nullptr;

	//录音定时器
	QTimer* m_timer_record = nullptr;
	//录音 标识符
	bool m_bool_record = 0;

	////opencv 视频流  使用IMX291摄像头
	//Model_VideoDevice* m_model_videoDevice = nullptr;

	////test 插值
	//double* Fine_S = nullptr;


private:
	//时序结构体
	TimeSequence mStructTS;//写
	TimeSequence _mStructTS[3];//读

	//预计保存三组时域数据，用来展示
	QQueue<TimeSequence> mTimeSequence;
	//语谱图数据结构
	QList<QVector<double>> mSpectrumSequence_P, mSpectrumSequence_X, mSpectrumSequence_Y, mSpectrumSequence_Z;

	//频响矫正
	CalibrationDll* _pCalibrationDll = nullptr;
	//带通过滤
	filtDll* _pFiltDll = nullptr;
	bool isFilt = 0;
	float filt_low = 0.0f, filt_hight = 0.0f;

	RawDataPack* myRawDataPack = nullptr;

	//裁剪范围
	quint16 mXAxisCutOut = 30 * 6, mYAxisCutOut = 30 * 6;
	//QMutex
	QMutex mXAxis_CO_Mutex , mYAxis_CO_Mutex;

	//热力图大小系数
	double mHmSizeFactor = 0.0f;

signals:
	void doFffOnce();
	void m_signal_recordOver();
	void m_signal_initCamera();
	void m_signal_closeModelStream();
	void on_signal_P1_DMaxValue(double);
	void on_signal_fftStart();
	void on_signal_fftStop();
public slots:
	void realDrawplot();
	void doFFTData();
	void doCameraData();
	//void selectDisplay(int);
	void findVideoDevs();
public:
	//设置逻辑
	void setModel(MainModel * hendle);
    //initloyout
	void initLoyout();
	//initConnected
	void initConnectedAndUpData();
	void setCSS();
	void openFile(int&&);
	void readFileData();
	void invokeDLL();
	void startSound();
	void stopWorkSound();
	//void paintEvent(QPaintEvent*) override;
	void read();
	void creatPlot();
	void changePlot();
	//各个部件刷新函数
	void realDraw_plot_heatmap();
	void realDraw_plot_timeDomain();
	void realDraw_plot_freqDomain();
	void realDraw_plot_eleAngle();
	void realDraw_plot_azimAngle();

	//initCamera
	//void initCamera();
	void initCamera(int camIndex);
	//改变热力图大小
	void changeHotMapcoefficient(double& coefficient);
	void initSpectrumSequence();
private:
	void m_solt_chooseCaptureType(int);
	void m_solt_record();

	void On_setXAxisCutOutValue(uInt16);
	void On_setYAxisCutOutValue(uInt16);

	void On_setHmFactorValue(uInt16);
protected:

//until
protected:
	double MAX_VALUE(double *value, double len);

private:
    Ui::test_customPlot_hreatMapClass *ui;
};
