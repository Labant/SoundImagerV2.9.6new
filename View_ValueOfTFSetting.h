#pragma once
#include <qwidget.h>
#include <QObject>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPalette>
#include <QLineEdit>

#define HEIGHT 25

class View_ValueOfTFSetting :public QWidget
{
	Q_OBJECT

public:
	explicit View_ValueOfTFSetting(QWidget* parent = nullptr);
	~View_ValueOfTFSetting();

	void init();
	void initLayout();
	void initConnect();
	void initSheetStyle();
	void initCombox();
public:
	
	//下拉选择 实体
	QComboBox* m_comBox_chooseDisplay = nullptr;

	//自动描述 Auto
	QPushButton* _pPtnAuto = nullptr;
	//vt(valueTimeDomain)描述
	QLabel* pLabelVT = nullptr;
	//vt 输入值
	QLineEdit* pLineVT = nullptr;
	//vf(value FFT ) 描述
	QLabel* pLabelVF = nullptr;
	//vf 输入值
	QLineEdit* pLineVF = nullptr;

	//Auto按钮值
	bool mAutoValue = 1;

private:
	//QGridLayout* _pGridLayout = nullptr;
	QHBoxLayout* _pHBoxLayout = nullptr;
};

