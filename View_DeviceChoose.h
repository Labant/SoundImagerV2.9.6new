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
#include "MyStruct.h"
#include "nidevice.h"
#include "soundcard.h"
#include "View_AudioRecord.h"
#include <QDebug>
#include <QTimer>

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
private:
	void initLayout();
	void initConnect();

	//solt
	void m_audio_serach();
	void m_video_serach();
	void m_serach_commit();
	void m_slot_soundCardModel();
	void m_slot_NiCardModel();
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
	QRadioButton* m_radioBtn_captureCard[2];

public:

	QComboBox* m_comBox[2];
	QPushButton* m_pushButton[2];

private:
	//summit box
	QHBoxLayout* m_hlayout_summit = nullptr;

	//summit btn
	QPushButton* m_btn_summit = nullptr;
	QPushButton* m_btn_summit_back = nullptr;

	////录音setting
	//View_AudioRecord* m_View_AudioRecord = nullptr;

public:
	//采集卡类型
	int m_captrueType = CAPTURETYPE::SoundCard;
	//声卡对象
	soundCard* pSoundCard = nullptr;
	//ni 采集卡对象
	NiDevice* m_niDevice = nullptr;

	bool m_bool_initCamera = 0;
private:
	bool m_initNiDev = 0;
	bool m_bool_audio_reFound = 0;//改变采集卡类型提示
	bool m_bool_serached = 0; //是否搜索过
	
};

