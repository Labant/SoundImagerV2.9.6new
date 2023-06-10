#pragma once
#include <qwidget.h>
#include <QSlider> 
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class View_HmSizeFactor :public QWidget
{
public:
	View_HmSizeFactor(QWidget* parent = nullptr);
	~View_HmSizeFactor();

protected:
	void initLayout();
	void initObject();
private:
	void init();
public:
	QSlider* _pSlider = nullptr;
protected:

private:
	QGridLayout* _pGridLayout = nullptr;
	QLabel* _pLabel[2];
	
	QLabel* _pLabelTitle = nullptr;
};

