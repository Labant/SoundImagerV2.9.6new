#pragma once
#include <qobject.h>
#include "easypr.h"
#include "easypr/util/switch.hpp"
#include <QThread>
#include "GlobalConfig.hpp"

using namespace easypr;

class Model_hotMapAndVideoDisplay;

class Model_CarPlateIdentify :public QObject
{
	Q_OBJECT
public:
	explicit Model_CarPlateIdentify(QObject* parent = nullptr);
	~Model_CarPlateIdentify();

	QString IdentifyCarPalte(cv::Mat);
	QPair<QString, QString> IdentifyCarPalte(cv::Mat,bool isIndefiy);
protected:
	void initMembers();
	void initConnects();
private:
	void init();
	void IdentifyAndResSend(cv::Mat src,double maxValue);
	QString IdentifyCarlane(double value);
signals:
	void on_signal_sendIdentifyRes(QString plate);
	void on_signal_sendSoundPw(double);
	void on_signal_sendCarLane(QString);

public:
private:
	QThread mThread;
	CPlateRecognize pr;
	vector<easypr::CPlate> plateVec;
	Model_hotMapAndVideoDisplay* _pModel_hotMapAndVideoDisplay = nullptr;

	//车道分界线
	double mCarlaneDividingLine = PIXWIDTH / 3;
};

