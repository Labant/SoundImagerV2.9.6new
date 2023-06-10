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
	//友元类
	friend class View_TabWidget_Setting;
	friend class  MainViewControl;

	void setCSS(QPalette& palette);
protected:
	virtual void initLayout() override;
	virtual void initConnect() override;
	void setmodifyEntity();

private:

	//布局 main布局 
	QVBoxLayout* m_mainLayout = nullptr;
	//布局 主布局
	QVBoxLayout* m_vLayout_groupBox = nullptr;
	//实体 主实体
	QGroupBox* m_group_main = nullptr;
	//布局 checkBox
	QVBoxLayout* m_vLayout_checkBox = nullptr;
	//实体 是否录音
	QCheckBox* m_checkBox = nullptr;

	//布局 文字提示+ 录音时间
	QHBoxLayout* m_hLayout_txt_rt = nullptr;
	//实体 提示输入时间的文本
	QLabel* m_label_txt_rt = nullptr;
	//实体 录音时间
	QLineEdit* m_lineEdit_rt = nullptr;

	//布局 录音时间
	QVBoxLayout* m_vLayout_display_rt = nullptr;
	//实体 实时录音时间
	QLabel* m_label_rt = nullptr;

	//布局 文字提示+ wav文件地址
	QHBoxLayout* m_hLayout_txt_address = nullptr;
	//实体 提示存储wav文件的地址
	QLabel* m_label_txt_address = nullptr;
	//实体 存储wav文件的地址
	QLineEdit* m_lineEdit_address = nullptr;

signals:
	//void m_signak_recording();

};

