#pragma once
#include <qobject.h>
class View_CarIdentifyInfoDisplay;

class Model_CarIdentifyInfoDisplay :public QObject
{
	Q_OBJECT
public:
	explicit Model_CarIdentifyInfoDisplay(QObject* handle = nullptr);
	~Model_CarIdentifyInfoDisplay();

	void setView(QObject* handle = nullptr);
	void setCarPlate(QString);
	void setSoundPw(double);
	void setCarLaneInfo(QString);
protected:
	void initMembers();
	void initConnects();

private:
	void init();

private:
	View_CarIdentifyInfoDisplay* _pView_CarIdentifyInfoDisplay = nullptr;


};

