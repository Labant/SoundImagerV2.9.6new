#pragma once
#include <qwidget.h>
#include <QLabel>
#include <QGridLayout>

class Model_CarIdentifyInfoDisplay;

class View_CarIdentifyInfoDisplay :public QWidget
{
	Q_OBJECT
public:
	explicit View_CarIdentifyInfoDisplay(QWidget* parent = nullptr);
	~View_CarIdentifyInfoDisplay();
	void setModel(QObject&);
	void setCSS(QPalette& palette);

	void setCarPlate(QString);
	void setSoundPw(double);
	void setCarLaneInfo(QString);
protected:
	void initMembers();
	void initLayout();

private:
	void init();

protected:
	QGridLayout* _pMainLayout_Grid = nullptr;

	QLabel* _pLabelCarPlate_hint = nullptr;
	QLabel* _pLabelSoundPw_hint = nullptr;
	QLabel* _pLabelCarLaneInfo_hint = nullptr;
	QLabel* _pLabelCarPlate = nullptr;
	QLabel* _pLabelSoundPw = nullptr;
	QLabel* _pLabelCarLaneInfo = nullptr;
private:
	Model_CarIdentifyInfoDisplay* _pModel_CarIdentifyInfoDisplay = nullptr;
};

