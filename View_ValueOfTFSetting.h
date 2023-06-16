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
	
	//����ѡ�� ʵ��
	QComboBox* m_comBox_chooseDisplay = nullptr;

	//�Զ����� Auto
	QPushButton* _pPtnAuto = nullptr;
	//vt(valueTimeDomain)����
	QLabel* pLabelVT = nullptr;
	//vt ����ֵ
	QLineEdit* pLineVT = nullptr;
	//vf(value FFT ) ����
	QLabel* pLabelVF = nullptr;
	//vf ����ֵ
	QLineEdit* pLineVF = nullptr;

	//Auto��ťֵ
	bool mAutoValue = 1;

private:
	//QGridLayout* _pGridLayout = nullptr;
	QHBoxLayout* _pHBoxLayout = nullptr;
};

