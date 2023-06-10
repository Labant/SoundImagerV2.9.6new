#pragma once
#include <qwidget.h>
#include "qcustomplot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include "BaseWidget.h"
#include "View_ValueOfTFSetting.h"
//#include "MyStruct.h"

//using namespace MYSTRUCT::PXYZ;

class View_FrequencyTimeDomain :public BaseWidget
{
	Q_OBJECT
public:
	View_FrequencyTimeDomain(QWidget* w = nullptr);
	~View_FrequencyTimeDomain();

	void on_btn_display(double _x, double _y);

protected:

	void initLayout() override;
	void initConnect() override;
private:
	void initCustomPlot();
	void initCombox();
	void initSpectrum(QCustomPlot * _CP);

signals:
	void m_siganl_currentText(int);
public:
	//void m_slot_currentText(pxyz);

private:
	QTimer mTimer;
	QList<QVector<double>> data_lofar;

protected:

	//主 布局
	QVBoxLayout* m_main_layout = nullptr;
	//下拉选择 布局
	QHBoxLayout* m_dropDownList_layout = nullptr;
	//fre time domain 布局
	QVBoxLayout* m_freTime_layout = nullptr;

public:
	////下拉选择 实体
	//QComboBox* m_comBox_chooseDisplay = nullptr;
	//fre time domain 实体
	QCustomPlot* m_timeDomain_general = nullptr;
	QCustomPlot* m_freqencyDomain_general = nullptr;
	//新增 语谱图
	QCustomPlot* m_spectrum_general = nullptr;
	////时频域设置按钮
	//QPushButton* _pBtnSetting = nullptr;
	//语谱图颜色
	QCPColorMap* m_pColorMap = nullptr;
	//时频域设置界面
	View_ValueOfTFSetting* _pView_ValueOfTFSetting = nullptr;
};

