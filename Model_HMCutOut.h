#pragma once
#include <qobject.h>

class View_HMCutOut;

class Model_HMCutOut :public QObject
{
	Q_OBJECT
public:
	Model_HMCutOut(View_HMCutOut* hendle = nullptr, QObject* parent = nullptr);
	~Model_HMCutOut();
	void setView(View_HMCutOut* handle = nullptr);

protected:
	void initMember();
	void initConnect();
private:
	void init();
public:
protected:
//slots
	void On_setXAxisSidlerValue(QString);
	void On_setYAxisSidlerValue(QString);
	void On_setSidlerXAxisValue(int);
	void On_setSidlerYAxisValue(int);
	void on_setHmThresholdValue();
signals:
	void xAxisCutOutChanged(quint16);
	void yAxisCutOutChanged(quint16);
	void on_signal_thresholdChange(double);
private:
	View_HMCutOut* _pView_HMCutOut = nullptr;
};

