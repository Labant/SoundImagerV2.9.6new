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

	//���� main���� 
	m_mainLayout = new QVBoxLayout;
	this->setLayout(m_mainLayout);
	m_mainLayout->setContentsMargins(0, 20, 0, 20);

	//���� ������
	m_vLayout_groupBox = new QVBoxLayout;
	//ʵ�� ��ʵ��
	m_group_main = new QGroupBox(QString::fromLocal8Bit("¼������"));
	m_group_main->setLayout(m_vLayout_groupBox);
	m_mainLayout->addWidget(m_group_main);

	//���� checkBox
	m_vLayout_checkBox = new QVBoxLayout;
	m_vLayout_groupBox->addLayout(m_vLayout_checkBox,1);
	//ʵ�� �Ƿ�¼��
	m_checkBox = new QCheckBox(QString::fromLocal8Bit("record?"));
	m_vLayout_checkBox->addWidget(m_checkBox);


	//���� ������ʾ+ ¼��ʱ��
	m_hLayout_txt_rt = new QHBoxLayout;
	m_vLayout_groupBox->addLayout(m_hLayout_txt_rt,1);
	//ʵ�� ��ʾ����ʱ����ı�
	m_label_txt_rt = new QLabel(QString::fromLocal8Bit("¼��ʱ��(s):"));
	//ʵ�� ¼��ʱ��
	m_lineEdit_rt = new QLineEdit;
	m_hLayout_txt_rt->addWidget(m_label_txt_rt, 1,Qt::AlignLeft);
	m_hLayout_txt_rt->addWidget(m_lineEdit_rt, 6, Qt::AlignLeft);

	//���� ¼��ʵʱʱ��
	m_vLayout_display_rt = new QVBoxLayout;
	m_vLayout_groupBox->addLayout(m_vLayout_display_rt,1);
	m_label_rt = new QLabel(QString::fromLocal8Bit("����һֻСlabel"));
	m_vLayout_display_rt->addWidget(m_label_rt);

	//���� ������ʾ+ wav�ļ���ַ
	m_hLayout_txt_address = new QHBoxLayout;
	m_vLayout_groupBox->addLayout(m_hLayout_txt_address,1);
	//ʵ�� ��ʾ�洢wav�ļ��ĵ�ַ
	m_label_txt_address = new QLabel(QString::fromLocal8Bit("�ļ��洢��ַ:"));
	//ʵ�� �洢wav�ļ��ĵ�ַ
	m_lineEdit_address = new QLineEdit;
	m_hLayout_txt_address->addWidget(m_label_txt_address,1, Qt::AlignLeft);
	m_hLayout_txt_address->addWidget(m_lineEdit_address,6, Qt::AlignLeft);

	//¼�����ô��ڶ��뷽ʽ
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

	//�ļ�Ĭ�ϵ�ַ
	m_lineEdit_address->setText(QDir::currentPath() + "/RecordAudio");
}

void View_AudioRecord::setCSS(QPalette& palette)
{
	//QPalette pal = this->palette();
	//pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
	this->setPalette(palette);
}
