#pragma once
#include <qwidget.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>


class View_DistanceSelect :public QWidget
{
public:
	View_DistanceSelect(QWidget* parent = nullptr);
	~View_DistanceSelect();

protected:
	void initMember();
private:
	void init();
public:
protected:
	QGroupBox* _pGroupBox = nullptr;
	QLabel* _pLabel = nullptr;
	QRadioButton _pRadioBtn[4];
private:

};

