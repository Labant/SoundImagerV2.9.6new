#pragma once
#include <qwidget.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
class View_filteringSetting :public QWidget
{
//function
public:
	explicit View_filteringSetting(QWidget* parent = nullptr);
	~View_filteringSetting();
	void setCSS(QPalette& palette);
protected:
	void initLayout();
	void initConnect();

private:
	void init();

//member
public:
	QPushButton* _pPushButtonSure = nullptr;
	QLineEdit* _pLELow = nullptr;
	QLineEdit* _pLEHigh = nullptr;

protected:

private:
	QLabel* _pLabelTxt1 = nullptr;
	QLabel* _pLabelTxt2 = nullptr;
	QHBoxLayout* _pHBoxLayout = nullptr;
};

