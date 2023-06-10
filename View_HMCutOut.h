#pragma once
#include <qwidget.h>
#include <QSlider> 
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include "View_HmSizeFactor.h"

class View_HMCutOut :public QWidget
{
public:
	View_HMCutOut(QWidget* parent = nullptr);
	~View_HMCutOut();
	void setCSS(QPalette& pal);
	void on_showP1_DMaxValue(double value);
protected:
	void initLayout();
	void initConnect();
private:
	void init();

public:
	QLineEdit* _pLineEdit[2];
	QSlider* _pSlider[2];
	QSlider* _pSliderHmSizeFactor = nullptr;
	View_HmSizeFactor* _pView_HmSizeFactor = nullptr;
	//2023.6.9新增
	//阈值设置
	QLabel* _pHmthresholdDisplay = nullptr;
	QLineEdit* _pHmthresholdValue = nullptr;
	//当前最大值显示
	QLabel* _pHm_P1D_Display = nullptr;
	QLineEdit* _pHmHm_P1D_DisplayValue = nullptr;

protected:
	QLabel* _pLabel[2];
private:
	QGridLayout* _pGridLayout = nullptr;
};

