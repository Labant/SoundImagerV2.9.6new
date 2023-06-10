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
	//主视图
	MainViewControl* _pMainViewControl = nullptr;
	//数据采集
	//opencv 视频流  使用IMX291摄像头
	Model_VideoDevice* m_model_videoDevice = nullptr;

	//过滤数据，显示特定频段的数据
	Model_filteringSetting* _pModel_filteringSetting = nullptr;
	//数据采集
	Model_AudioCollector* _pModel_AudioCollector = nullptr;
	//数据裁剪
	Model_HMCutOut* _pModel_HMCutOut = nullptr;
	//热力图大小
	Model_HmSizeFactor* _Model_HmSizeFactor = nullptr;

	//热力图数据处理和图像显示
	Model_hotMapAndVideoDisplay* _pModel_hotMapAndVideoDisplay = nullptr;

protected:
	void replotVideo();
	void replotHm();
private:

	
};

