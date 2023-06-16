#pragma once
#include <qobject.h>
#include "Model_filteringSetting.h"
#include "Model_AudioCollector.h"
#include "Model_HMCutOut.h"
#include "Model_HmSizeFactor.h"
#include "Model_VideoDevice.h"


class MainViewControl;
class Model_hotMapAndVideoDisplay;

class MainModel :public QObject
{
	Q_OBJECT
public:
	explicit MainModel(QObject* parent = nullptr);
	~MainModel();
	void setView(MainViewControl* hendle);
protected:
	void initMember();
	void initConnect();
	
private:
	void init();
	void quit();
public:
	//����ͼ
	MainViewControl* _pMainViewControl = nullptr;
	//���ݲɼ�
	//opencv ��Ƶ��  ʹ��IMX291����ͷ
	Model_VideoDevice* m_model_videoDevice = nullptr;

	//�������ݣ���ʾ�ض�Ƶ�ε�����
	Model_filteringSetting* _pModel_filteringSetting = nullptr;
	//���ݲɼ�
	Model_AudioCollector* _pModel_AudioCollector = nullptr;
	//���ݲü�
	Model_HMCutOut* _pModel_HMCutOut = nullptr;
	//����ͼ��С
	Model_HmSizeFactor* _Model_HmSizeFactor = nullptr;

	//����ͼ���ݴ����ͼ����ʾ
	Model_hotMapAndVideoDisplay* _pModel_hotMapAndVideoDisplay = nullptr;

protected:
	void replotVideo();
	void replotHm();
private:

	
};

