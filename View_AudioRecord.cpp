#include "View_AudioRecord.h"

View_AudioRecord::View_AudioRecord(QWidget* w)
{
	initLayout();
	initConnect();
	setmodifyEntity();
	//setCSS();
}

View_AudioRecord::~View_AudioRecord()
{
}

void View_AudioRecord::initLayout()
{

	//布局 main布局 
	m_mainLayout = new QVBoxLayout;
	this->setLayout(m_mainLayout);
	m_mainLayout->setContentsMargins(0, 20, 0, 20);

	//布局 主布局
	m_vLayout_groupBox = new QVBoxLayout;
	//实体 主实体
	m_group_main = new QGroupBox(QString::fromLocal8Bit("录音设置"));
	m_group_main->setLayout(m_vLayout_groupBox);
	m_mainLayout->addWidget(m_group_main);

	//布局 checkBox
	m_vLayout_checkBox = new QVBoxLayout;
	m_vLayout_groupBox->addLayout(m_vLayout_checkBox,1);
	//实体 是否录音
	m_checkBox = new QCheckBox(QString::fromLocal8Bit("record?"));
	m_vLayout_checkBox->addWidget(m_checkBox);


	//布局 文字提示+ 录音时间
	m_hLayout_txt_rt = new QHBoxLayout;
	m_vLayout_groupBox->addLayout(m_hLayout_txt_rt,1);
	//实体 提示输入时间的文本
	m_label_txt_rt = new QLabel(QString::fromLocal8Bit("录音时长(s):"));
	//实体 录音时间
	m_lineEdit_rt = new QLineEdit;
	m_hLayout_txt_rt->addWidget(m_label_txt_rt, 1,Qt::AlignLeft);
	m_hLayout_txt_rt->addWidget(m_lineEdit_rt, 6, Qt::AlignLeft);

	//布局 录音实时时间
	m_vLayout_display_rt = new QVBoxLayout;
	m_vLayout_groupBox->addLayout(m_vLayout_display_rt,1);
	m_label_rt = new QLabel(QString::fromLocal8Bit("我是一只小label"));
	m_vLayout_display_rt->addWidget(m_label_rt);

	//布局 文字提示+ wav文件地址
	m_hLayout_txt_address = new QHBoxLayout;
	m_vLayout_groupBox->addLayout(m_hLayout_txt_address,1);
	//实体 提示存储wav文件的地址
	m_label_txt_address = new QLabel(QString::fromLocal8Bit("文件存储地址:"));
	//实体 存储wav文件的地址
	m_lineEdit_address = new QLineEdit;
	m_hLayout_txt_address->addWidget(m_label_txt_address,1, Qt::AlignLeft);
	m_hLayout_txt_address->addWidget(m_lineEdit_address,6, Qt::AlignLeft);

	//录音设置窗口对齐方式
	this->m_vLayout_groupBox->setAlignment(Qt::AlignTop);
	this->m_hLayout_txt_address->setAlignment(Qt::AlignTop);
	this->m_vLayout_display_rt->setAlignment(Qt::AlignTop);
	
}

void View_AudioRecord::initConnect()
{
}

void View_AudioRecord::setmodifyEntity()
{
	//m_label_rt->setMaximumWidth(200);
	//m_group_main->setMaximumWidth(300);

	//文件默认地址
	m_lineEdit_address->setText(QDir::currentPath() + "/RecordAudio");
}

void View_AudioRecord::setCSS(QPalette& palette)
{
	//QPalette pal = this->palette();
	//pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
	this->setPalette(palette);
}
