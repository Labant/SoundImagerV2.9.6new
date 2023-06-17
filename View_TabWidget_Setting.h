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
	//����box ѡ����Ƶ�豸����Ƶ�豸
	View_DeviceChoose* mView_deviceChoose = nullptr;

	//����
	QTabWidget* _pTabWidget = nullptr;
	//������ ¼��setting
	View_AudioRecord* m_View_AudioRecord = nullptr;
	//������ �˲�����
	View_filteringSetting* _pView_filteringSetting = nullptr;
	//������ �ü����ݴ���
	View_HMCutOut* _pView_HMCutOut = nullptr;

	//����ʶ��
	View_CarIdentifyInfoDisplay* _pView_CarIdentifyInfoDisplay = nullptr;
};

