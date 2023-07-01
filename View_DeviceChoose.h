#pragma once
#include <QObject>
#include <qwidget.h>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QRadioButton>
#include "GlobalConfig.hpp"
#include "MyStruct.h"
#include "nidevice.h"
#include "soundcard.h"
#include "TcpDiscoverServer.h"
#include "View_AudioRecord.h"
#include <QDebug>
#include <QTimer>
#include <QTcpSocket>

using namespace MYSTRUCT;

class GlobalConfig;

class View_DeviceChoose :public QWidget
{
	Q_OBJECT
public:
	View_DeviceChoose(QWidget* p = nullptr);
	~View_DeviceChoose();

	//友元
	friend class test_customPlot_hreatMap;
	void setCSS(QPalette& palette);
	void m_solt_device_stop();
	// Tcp 连接状态
	void on_slot_tcpStateMonitor(int);
private:
	void initLayout();
	void initConnect();

	//solt
	void m_audio_serach();
	void m_video_serach();
	void m_serach_commit();
	void m_slot_soundCardModel();
	void m_slot_NiCardModel();
	void m_slot_VKCardModel();
	//void m_serach_commit_back();
	void m_solt_chooseCaptureType(int );

signals:
	void m_signal_audioStart();
	void m_signal_stop();
	void m_signal_captureType(int);
	void m_signak_recording();
	void m_signal_place_reFoundAudioBtn();
	void m_signal_findVideoDevs();
	void m_signal_camera(int index);
	void m_signal_modelStop();//停止子线程采集frame
	void m_signal_modelStart();

	//初始化tcp server
	void on_signal_initTcpServerAndMonitor();
	void on_signal_initTcpClientParameter();

	//tcp 采集客户端传送数据
	void on_signal_collectData();
	//tcp 停止客户端传送数据
	void on_signal_collectData_stop();

	//告知MainModel 初始化TcpState connect
	void on_signal_initTCPState();


private:
	//UI 内的布局格式
	QVBoxLayout* main_layout;

	// 总框 "设置"
	QGroupBox* mGroupBox_videoAudioDevice;
	// 总框 "设置"布局格式
	QVBoxLayout* mGroupBox_videoAudioDevice_layout;

	//音频
	QGroupBox* m_group[2];
	//组内布局格式
	QHBoxLayout* m_hBox[2];
	//audio组内组件
	QLabel* m_label[2];

	//采集卡 框
	QGroupBox* m_captureCardType = nullptr;
	QHBoxLayout* m_hLayout_selectCapture = nullptr;
	QRadioButton* m_radioBtn_captureCard[3];

public:

	QComboBox* m_comBox[2];
	QPushButton* m_pushButton[2];

private:
	//summit box
	QGridLayout* m_hlayout_summit = nullptr;

	//summit btn
	QPushButton* m_btn_summit = nullptr;//开始按钮
	QPushButton* m_btn_summit_back = nullptr;//停止按钮

	////录音setting
	//View_AudioRecord* m_View_AudioRecord = nullptr;

public:
	//采集卡类型
	int m_captrueType = CAPTURETYPE::VKCaptureCard;
	//声卡对象
	soundCard* pSoundCard = nullptr;
	//ni 采集卡对象
	NiDevice* m_niDevice = nullptr;
	//VK采集卡-接受数据服务器
	//TcpDiscoverServer* _pTcpDiscoverServer = nullptr;

	bool m_bool_initCamera = 0;
private:
	bool m_initNiDev = 0;
	bool m_bool_audio_reFound = 0;//改变采集卡类型提示
	bool m_bool_serached = 0; //是否搜索过

	//test
	bool xx;

	//Tcp 多线程
	QThread mThread;
	//2023.6.30新增
	//Tcp 连接状态显示
	QLabel* _pLabelTcpState = nullptr;
	//连接状态保存
	QString mSaveTcpState;
};

