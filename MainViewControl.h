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
	double P_1D[64800];//δ˫��˫����ֵ
	//double *P_1D = new double[64800 * 4];//˫��˫����ֵ
	double P_1D_MAX_lvs;
	//receive rawData
	//double p[1000], x[1000], y[1000], z[1000];
	double p[4800], x[4800], y[4800], z[4800];
	//���� p x y z���ݣ����ڽ���������ʱ��y�����ã������Ƶ�����Ӱ��
	double p_copy[4800], x_copy[4800], y_copy[4800], z_copy[4800];
	//���� Ƶ�����ݣ����ڽ�����������Ƶ��y�����ã������Ƶ����Ⱦ����Ӱ��
	double *p_fre_copy = nullptr, *x_fre_copy = nullptr, *y_fre_copy = nullptr, *z_fre_copy = nullptr;
	QMutex m_dataCopy;

	//fft out
	float p_out[1000];
	//�ж�doFFTData�����Ƿ����
	bool isFftDown = 0;
	
	//Save File
	QFile* p_file = nullptr, * x_file = nullptr, * y_file = nullptr, * z_file = nullptr, * res_file = nullptr;
	//�ɼ�������
	int mCaptrueType = CAPTURETYPE::SoundCard;
	////��������
	//soundCard* pSoundCard = nullptr;
	////ni �ɼ�������
	//NiDevice* m_niDevice = nullptr;

	QCPColorMap* colorMap = nullptr;
	int nx = 180;
	int ny = 130;
	float hotMapCoefficient = 1; //����ͼ��Сϵ��������Ϊ1

	double posColor[5] = { 0.93,0.95,0.98,0.99,1 };
	//double posColor[5] = { 0.93 + (0.05 / 6.0f) * 2,0.95 + (0.05 / 6.0f) * 3,0.98 + (0.05 / 6.0f) * 1,0.99 + (0.05 / 6.0f) * 1,1 };//С�˵�
	//�ռ�ȣ�0.02��0.03��0.01��0.01 ��6��
	/**ֻҪ����0.94�����ԣ�1 - 0.99 = 0.01��0.01*6 = 0.06��1-0.06 = 0.94��������0.94��֤��Խ�磩
	*1-0.95 = 0.05:Ч����С�� 
	*1 - 0.97 = 0.03:Ч����С
	 * 1-0.99 = 0.01����ͬ0.03Ч�����������̫С
	 */
	
	//test
	//double posColor[5] = { 0.975,0.98,0.985,0.99,1 };
	//double posColor[5] = { 0.99200,0.99300,0.99700,0.99800,1 };
	//double posColor[5] = { 0.993,0.995,0.998,0.999,1 }; //������Բ��

	//���ϵ���ı��С
	//QCPColorGradient cpColorGradient;

	//test д��csv
	QFile P_1DCSV;

	// ����Ⱦ��ʱ��
	QTimer timer;

	//ÿһ֡���ֵ
	QPair<int, int> maxp;//at[0]:0~180 ; at[1]:0~360

	//ÿ��ת����image
	QImage t_image;//(QString("E:/YoudaoData/qq7418C6DA0DA187D4CBD6E5B83787CEFB/02739732A5A4493F8ECA09C2D099D80A/0.jpeg"))

	//loyout
	//QCustomPlot *plot_heatmap = nullptr;//����ͼ
	View_hotMapAndVideoDisplay* _pView_hotMapAndVideoDisplay = nullptr;

	//Test_openGL* m_shereLocalation = nullptr;
	//����
	GLEWINQT2* m_shereLocalation = nullptr;

	//ͨ��
	View_FrequencyTimeDomain* m_freTime_general = nullptr;
	//ʱƵ����ֵ
	float mThresholdVT, mThresholdVF;
	//QVBoxLayout* vbox;//������
	//QHBoxLayout *hBox[2];//��� ��������
	QHBoxLayout* hbox;//������
	QVBoxLayout* vBox[2];//��� ��������
	QHBoxLayout* xyBox;//��������
	QVBoxLayout* vbox_e_a;//�������� ��λ�������ڶ��������е�����
	QVBoxLayout* vbox_time_pxyz;//�������� ʱ�򼯺�
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

	//X Y value ��ʾ��label
	QLabel *xylable[4];

	//����box ѡ����Ƶ�豸����Ƶ�豸
	View_TabWidget_Setting* _mView_TabWidget_Setting = nullptr;

	//MainModel
	MainModel* _pMainModel = nullptr;

	//fft obj
	FFT_change *fft_change = nullptr;
	//timer obj  fft�����̶߳�ʱ��
	QTimer* timer_thread_1 = nullptr;
	//QThread obj fft�����߳�
	QThread *fft_thread = nullptr;

	//timer obj  ��������ȴ����ݵ��̶߳�ʱ��
	QTimer* timer_thread_2 = nullptr;
	//QThread obj ��������ȴ����ݵ��߳�
	QThread* camera_thread = nullptr;

	//�������
	//const int camera_frame_width = 848;//640  848
	//const int camera_frame_height = 480;
	//const int camera_fps = 60;
	//int camera_exposure = 200;
	//rs2::pipeline         pipe;
	//rs2::frameset frames;
	//�Ƿ��ʼ�����
	bool isInitCamera = 0;

	////flag ��־ ��������֡�Ƿ�����ȴ�
	//bool b_flag_Done = false;

	//ʱƵ���� combox��index
	//int m_int_comboxIndex = 0;

	//¼��mode
	Model_RecordAudio* m_Model_AudioRecord = nullptr;
	//¼�� thread
	QThread* m_thread_AudioRecord = nullptr;

	//¼����ʱ��
	QTimer* m_timer_record = nullptr;
	//¼�� ��ʶ��
	bool m_bool_record = 0;

	////opencv ��Ƶ��  ʹ��IMX291����ͷ
	//Model_VideoDevice* m_model_videoDevice = nullptr;

	////test ��ֵ
	//double* Fine_S = nullptr;


private:
	//ʱ��ṹ��
	TimeSequence mStructTS;//д
	TimeSequence _mStructTS[3];//��

	//Ԥ�Ʊ�������ʱ�����ݣ�����չʾ
	QQueue<TimeSequence> mTimeSequence;
	//����ͼ���ݽṹ
	QList<QVector<double>> mSpectrumSequence_P, mSpectrumSequence_X, mSpectrumSequence_Y, mSpectrumSequence_Z;

	//Ƶ�����
	CalibrationDll* _pCalibrationDll = nullptr;
	//��ͨ����
	filtDll* _pFiltDll = nullptr;
	bool isFilt = 0;
	float filt_low = 0.0f, filt_hight = 0.0f;

	RawDataPack* myRawDataPack = nullptr;

	//�ü���Χ
	quint16 mXAxisCutOut = 30 * 6, mYAxisCutOut = 30 * 6;
	//QMutex
	QMutex mXAxis_CO_Mutex , mYAxis_CO_Mutex;

	//����ͼ��Сϵ��
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
	//�����߼�
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
	//��������ˢ�º���
	void realDraw_plot_heatmap();
	void realDraw_plot_timeDomain();
	void realDraw_plot_freqDomain();
	void realDraw_plot_eleAngle();
	void realDraw_plot_azimAngle();

	//initCamera
	//void initCamera();
	void initCamera(int camIndex);
	//�ı�����ͼ��С
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
