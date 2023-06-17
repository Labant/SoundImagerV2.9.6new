#pragma once
#include <qwidget.h>
#include <QTabWidget>
#include "View_AudioRecord.h"
#include "View_DeviceChoose.h"
#include "View_filteringSetting.h"
#include "View_HMCutOut.h"
#include "View_CarIdentifyInfoDisplay.h"

class View_TabWidget_Setting : public QTabWidget
{
	Q_OBJECT
public:
	explicit View_TabWidget_Setting(QWidget* parent = nullptr);
	~View_TabWidget_Setting();


	void initObject();
	void initLayout();
	void initConnect();
	void setCSS();
signals:

	void m_signal_recordding();
public:
	//设置box 选择视频设备和音频设备
	View_DeviceChoose* mView_deviceChoose = nullptr;

	//设置
	QTabWidget* _pTabWidget = nullptr;
	//子设置 录音setting
	View_AudioRecord* m_View_AudioRecord = nullptr;
	//子设置 滤波窗口
	View_filteringSetting* _pView_filteringSetting = nullptr;
	//子设置 裁剪数据窗口
	View_HMCutOut* _pView_HMCutOut = nullptr;

	//车辆识别
	View_CarIdentifyInfoDisplay* _pView_CarIdentifyInfoDisplay = nullptr;
};

