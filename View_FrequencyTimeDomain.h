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

	//�� ����
	QVBoxLayout* m_main_layout = nullptr;
	//����ѡ�� ����
	QHBoxLayout* m_dropDownList_layout = nullptr;
	//fre time domain ����
	QVBoxLayout* m_freTime_layout = nullptr;

public:
	////����ѡ�� ʵ��
	//QComboBox* m_comBox_chooseDisplay = nullptr;
	//fre time domain ʵ��
	QCustomPlot* m_timeDomain_general = nullptr;
	QCustomPlot* m_freqencyDomain_general = nullptr;
	//���� ����ͼ
	QCustomPlot* m_spectrum_general = nullptr;
	////ʱƵ�����ð�ť
	//QPushButton* _pBtnSetting = nullptr;
	//����ͼ��ɫ
	QCPColorMap* m_pColorMap = nullptr;
	//ʱƵ�����ý���
	View_ValueOfTFSetting* _pView_ValueOfTFSetting = nullptr;
};

