#pragma once
#include <qobject.h>
#include "View_filteringSetting.h"
#include "filtDll.h"

class Model_filteringSetting :public QObject
{
	Q_OBJECT
public:
	explicit Model_filteringSetting(View_filteringSetting* _handel = nullptr, QObject* parent = nullptr);
	~Model_filteringSetting();

	void setView(View_filteringSetting* _handel);
	float getFiltLow();
	float getFiltHigh();
	void  setBtnSwitchText(int);
protected:
	void initMember();
	void initConnect();

private:
	void init();
	void on_slot_filtBtn_clicked();
signals:
	void on_signal_filtBtn_clicked();
public:

protected:
	////´øÍ¨¹ýÂË
	//filtDll* _pFiltDll = nullptr;
	//bool isFilt = 0;
	//float filt_low = 0.0f, filt_hight = 0.0f;

private:
	View_filteringSetting* _pView_filteringSetting = nullptr;
};

