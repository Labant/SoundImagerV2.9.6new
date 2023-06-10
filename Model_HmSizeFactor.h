#pragma once
#include <qobject.h>

class View_HmSizeFactor;

class Model_HmSizeFactor :public QObject
{
	Q_OBJECT
public:
	Model_HmSizeFactor(View_HmSizeFactor* handle = nullptr,QObject *parent = nullptr);
	~Model_HmSizeFactor();
	void setView(View_HmSizeFactor* handle = nullptr);
protected:
	void initMember();
	void initConnect();
private:
	void init();

	void On_GetFactorValue(unsigned short value);

signals:
	void On_Signal_FactoeValue(unsigned short value);

public:
protected:

private:
	View_HmSizeFactor* _pView_HmSizeFactor = nullptr;
};

