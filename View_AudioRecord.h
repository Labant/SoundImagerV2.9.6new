#pragma once
#include "BaseWidget.h"
#include <QObject>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QWidget>
#include <QDir>


class View_AudioRecord :public BaseWidget
{
	Q_OBJECT
public:
	View_AudioRecord(QWidget* w = nullptr);
	~View_AudioRecord();
	//��Ԫ��
	friend class View_TabWidget_Setting;
	friend class  MainViewControl;

	void setCSS(QPalette& palette);
protected:
	virtual void initLayout() override;
	virtual void initConnect() override;
	void setmodifyEntity();

private:

	//���� main���� 
	QVBoxLayout* m_mainLayout = nullptr;
	//���� ������
	QVBoxLayout* m_vLayout_groupBox = nullptr;
	//ʵ�� ��ʵ��
	QGroupBox* m_group_main = nullptr;
	//���� checkBox
	QVBoxLayout* m_vLayout_checkBox = nullptr;
	//ʵ�� �Ƿ�¼��
	QCheckBox* m_checkBox = nullptr;

	//���� ������ʾ+ ¼��ʱ��
	QHBoxLayout* m_hLayout_txt_rt = nullptr;
	//ʵ�� ��ʾ����ʱ����ı�
	QLabel* m_label_txt_rt = nullptr;
	//ʵ�� ¼��ʱ��
	QLineEdit* m_lineEdit_rt = nullptr;

	//���� ¼��ʱ��
	QVBoxLayout* m_vLayout_display_rt = nullptr;
	//ʵ�� ʵʱ¼��ʱ��
	QLabel* m_label_rt = nullptr;

	//���� ������ʾ+ wav�ļ���ַ
	QHBoxLayout* m_hLayout_txt_address = nullptr;
	//ʵ�� ��ʾ�洢wav�ļ��ĵ�ַ
	QLabel* m_label_txt_address = nullptr;
	//ʵ�� �洢wav�ļ��ĵ�ַ
	QLineEdit* m_lineEdit_address = nullptr;

signals:
	//void m_signak_recording();

};

