#include "View_FrequencyTimeDomain.h"

View_FrequencyTimeDomain::View_FrequencyTimeDomain(QWidget* w)
{

	initLayout();
	initConnect();
	//this->setStyleSheet(QString("border:1xp"));
}

View_FrequencyTimeDomain::~View_FrequencyTimeDomain()
{
}

void View_FrequencyTimeDomain::initLayout()
{
	//����
	m_main_layout = new QVBoxLayout;
	m_dropDownList_layout = new QHBoxLayout;
	m_freTime_layout = new QVBoxLayout;

	//ʵ��
	m_timeDomain_general = new QCustomPlot;
	m_freqencyDomain_general = new QCustomPlot;

	//����ͼ
	m_spectrum_general = new QCustomPlot;
	this->initSpectrum(m_spectrum_general);

	//�߾�
	//m_timeDomain_general->setContentsMargins();


	//����������
	this->setLayout(m_main_layout);
	m_main_layout->addLayout(m_dropDownList_layout);
	m_main_layout->addLayout(m_freTime_layout);
	//���ò����ڲ�������
	m_main_layout->setStretch(0, 1);
	m_main_layout->setStretch(1, 20);
	//�����ֱ߾�
	m_main_layout->setMargin(0);
	m_main_layout->setContentsMargins(0,0,0,0);
	//������߾�
	m_dropDownList_layout->setMargin(0);
	m_freTime_layout->setMargin(0);
	////�����ڱ߾�
	//m_dropDownList_layout->setContentsMargins(0, 0, 0, 0);
	////���� ���뷽ʽ
	//m_dropDownList_layout->setAlignment(Qt::AlignCenter);

	////test
	//qDebug() << m_dropDownList_layout->contentsMargins();

	//ʵ�� -> ����
	//m_dropDownList_layout->addWidget(m_comBox_chooseDisplay,0,Qt::AlignRight);
	//m_dropDownList_layout->setAlignment(Qt::AlignRight);
	//m_dropDownList_layout->setContentsMargins(QMargins(5,0,5,5));

	//��ֵ����
	this->_pView_ValueOfTFSetting = new View_ValueOfTFSetting;
	//m_dropDownList_layout->addWidget(_pView_ValueOfTFSetting);

	m_freTime_layout->addWidget(_pView_ValueOfTFSetting,1);
	m_freTime_layout->addWidget(m_timeDomain_general,10);
	m_freTime_layout->addWidget(m_freqencyDomain_general,10);
	m_freTime_layout->addWidget(m_spectrum_general,10);
	//customplot ��ʼ������
	initCustomPlot();
	//combox ��ʼ��
	initCombox();

}

void View_FrequencyTimeDomain::initConnect()
{
	//connect(this, &View_FrequencyTimeDomain::m_siganl_currentText,)
	//connect(m_comBox_chooseDisplay, SIGNAL(currentIndexChanged(int)), this, SLOT(m_siganl_currentText(int)));

	//btn click->display setting
	//connect(this->_pBtnSetting, &QPushButton::clicked, this, &View_FrequencyTimeDomain::on_btn_display);
}

void View_FrequencyTimeDomain::initCustomPlot()
{
	//��ʼ����������
	// configure axis rect:���������
	m_timeDomain_general->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // ��Ҳ������ͨ����ק/���ų߶ȸı���ɫ��Χ
	m_timeDomain_general->axisRect()->setupFullAxesBox(true);
	m_timeDomain_general->xAxis->setLabel(QString::fromLocal8Bit("���ݵ�"));
	m_timeDomain_general->yAxis->setLabel(QString::fromLocal8Bit("��ѹ"));
	m_timeDomain_general->setBackground(QPixmap::fromImage(QImage(":/test_customPlot_hreatMap/image/gray.png")));
	m_timeDomain_general->xAxis->setVisible(true);
	m_timeDomain_general->yAxis->setVisible(true);
	m_timeDomain_general->xAxis2->setVisible(false);
	m_timeDomain_general->yAxis2->setVisible(false);
	//�趨���Ͻ�ͼ�α�ע�ɼ�
	m_timeDomain_general->legend->setVisible(true);
	//�������
	m_timeDomain_general->addGraph();
	m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));
	m_timeDomain_general->graph(0)->setPen(QPen(QColor(230, 230, 230)));
	//!!ʱ�򲿷�!!
	//����X�����귶Χ
	m_timeDomain_general->xAxis->setRange(0, 4800*3);
	//����Y�����귶Χ
	m_timeDomain_general->yAxis->setRange(-0.02, 0.03);

	//Frequency
	m_freqencyDomain_general->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // ��Ҳ������ͨ����ק/���ų߶ȸı���ɫ��Χ
	m_freqencyDomain_general->axisRect()->setupFullAxesBox(true);
	m_freqencyDomain_general->xAxis->setLabel(QString::fromLocal8Bit("Ƶ�ʵ�(kHz)"));
	m_freqencyDomain_general->yAxis->setLabel(QString::fromLocal8Bit("���"));
	//m_freqencyDomain_general->setBackground(QPixmap::fromImage(QImage(":/test_customPlot_hreatMap/image/gray.png")));
	m_freqencyDomain_general->xAxis->setVisible(true);
	m_freqencyDomain_general->yAxis->setVisible(true);
	m_freqencyDomain_general->xAxis2->setVisible(false);
	m_freqencyDomain_general->yAxis2->setVisible(false);

	//����ɼ�
	m_freqencyDomain_general->legend->setVisible(true);
	//�������
	m_freqencyDomain_general->addGraph();
	m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-FFT"));
	m_freqencyDomain_general->graph(0)->setPen(QPen(QColor(230, 230, 230)));//�Ұ�
	//!!Ƶ�򲿷�!!
	//����X�����귶Χ
	//m_freqencyDomain_general->xAxis->setRange(0, 2400);//24kHz
	//2023.6.28 ����
	this->m_freqencyDomain_general->xAxis->setRange(0, 10, Qt::AlignLeft); //����x�᷶Χ
	this->m_freqencyDomain_general->xAxis->ticker()->setTickCount(11);//11�����̶�

	//����Y�����귶Χ
	m_freqencyDomain_general->yAxis->setRange(-0.02, 0.16);
	//X������̶Ⱦ���
	//m_freqencyDomain_general->xAxis->setNumberPrecision(10);
	//m_freqencyDomain_general->xAxis->setTickLabelRotation(45);
	//���þ�����ʽ
	//m_freqencyDomain_general->xAxis->setNumberFormat("f");
	m_freqencyDomain_general->xAxis->setSubTicks(true);
	//m_freqencyDomain_general->xAxis->ticker()->setTickCount(24);
	m_freqencyDomain_general->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);

}

void View_FrequencyTimeDomain::initCombox()
{
	//if (m_comBox_chooseDisplay != nullptr)
	//{
	//	QStringList items;
	//	items << "P" << "X" << "Y" << "Z";
	//	m_comBox_chooseDisplay->addItems(items);

	//	//���ÿ��
	//	m_comBox_chooseDisplay->setMaximumWidth(50);
	//	////���þ���
	//}

	//if (_pBtnSetting != nullptr)
	//{
	//	//
	//	_pBtnSetting->setText("set");
	//	//���ÿ��
	//	_pBtnSetting->setMaximumWidth(50);
	//}
}

void View_FrequencyTimeDomain::on_btn_display(double _x,double _y)
{
	//this->_pView_ValueOfTFSetting->move(_x, _y);
	//this->_pView_ValueOfTFSetting->show();
}

//void View_FrequencyTimeDomain::m_slot_currentText(pxyz)
//{
//}

void View_FrequencyTimeDomain::initSpectrum(QCustomPlot* p_imag)
{
	//     p_imag = ui->widget_Imag; //����һ����ͼָ��

	//p_imag->installEventFilter(this);  //��װһ���¼�������

	p_imag->clearGraphs();//���֮ǰ����
	//m_fp5->showTracer(true);//ʵ�����ݸ���(!!��Ҫ����)
	p_imag->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//����ק+�ɹ�������
	//p_imag->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);//����ק+�ɹ�������

	//  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//������X��
	// dateTicker->setDateTimeFormat("hh:mm:ss");//���ڸ�ʽ(�ɲο�QDateTime::fromString()����)
	//fp3->xAxis->setTicker(dateTicker);//����X��Ϊʱ����
	//fp3->xAxis->setTicker();


	p_imag->xAxis->setLabel(QString::fromLocal8Bit("Time(ms)"));//x����ʾ����
	p_imag->yAxis->setLabel(QString::fromLocal8Bit("Frequency(kHz)"));//y����ʾ����

	//���û�������ɫ
	p_imag->setBackground(QColor(0, 0, 0, 255));//��ɫ Aiphaͨ��0Ϊ��͸��������0Ϊ��͸����255͸��������ʾ��������ɫ��
	//p_imag->setBackground(QPixmap::fromImage(QImage(":/MainViewControl/image/gray.png")));
	//p_imag->setBackground(QColor(255, 0, 0,50));//��ɫ Aiphaͨ��0Ϊ͸��������0Ϊ��͸����255��͸����

	p_imag->yAxis->setRangeReversed(false);//����Y�����ᷴת��һ��

	p_imag->xAxis->setLabelColor(Qt::white);//������ɫ
	p_imag->yAxis->setLabelColor(Qt::white);
	p_imag->xAxis->setTickLabelColor(Qt::white);//������������ɫ
	p_imag->yAxis->setTickLabelColor(Qt::white);
	p_imag->xAxis->setBasePen(QPen(Qt::white, 1));//��������ɫ�����
	p_imag->yAxis->setBasePen(QPen(Qt::white, 1));
	p_imag->xAxis->setTickPen(QPen(Qt::white, 1));//���̶�
	p_imag->yAxis->setTickPen(QPen(Qt::white, 1));
	p_imag->xAxis->setSubTickPen(QPen(Qt::white, 1));//���̶�
	p_imag->yAxis->setSubTickPen(QPen(Qt::white, 1));

	//ÿ�����߶����ռһ��graph()
	p_imag->addGraph();
	p_imag->graph(0)->setPen(QPen(Qt::blue));//������ɫ

	p_imag->graph(0)->setBrush(QBrush(QColor(0, 255, 255, 20)));//������X���Χ������ɫ

	//ͨ�����ݵ����QCustomPlot����ע��,������QCPColorMap������Ϊ��x�ᣬy��;��ɫ��ֵvalue��
	m_pColorMap = new QCPColorMap(p_imag->xAxis, p_imag->yAxis);
	//m_pColorMap->data()->setSize(100,5000);//��������ͼ��x,y������,X��50���㣬y��4096����
	m_pColorMap->data()->setSize(500, 1000); //X��500�������ʱ�䣬y��1000�������ݵ��Ӧ0-10kHz
	m_pColorMap->data()->setRange(QCPRange(0, 500), QCPRange(0, 10));//setRange������X���Լ�Y��ķ�Χ

	QCPColorScale* colorScale = new QCPColorScale(p_imag);  // ����һ��ɫ��
	colorScale->setType(QCPAxis::atRight);   // ˮƽ��ʾatLeft,atRight,atTop ,atBottom
	p_imag->plotLayout()->addElement(0, 1, colorScale); // ����ɫͼ�ұ���ʾ
	colorScale->setDataRange(QCPRange(0, 10));  //����ɫ������ֵ�����С
	
	//ɫ��������ק
	colorScale->setRangeDrag(true);
	colorScale->setRangeZoom(true);
	//xAxis��yAxis��������ק
	//p_imag->xAxis->s

	colorScale->axis()->setLabelColor(Qt::white);
	colorScale->axis()->setTickLabelColor(Qt::white);
	m_pColorMap->setColorScale(colorScale);

	QCPColorGradient gradient;  // ɫ��ʹ�õ���ɫ����
	gradient.setColorStopAt(0.0, QColor("#f6efa6")); //ǳ�׻�ɫ  // ����ɫ����ʼʱ����ɫ
	gradient.setColorStopAt(1.0, QColor("#bf444c")); //ǳ�׻�ɫ  // ����ɫ������ʱ����ɫ

	m_pColorMap->setGradient(QCPColorGradient::gpJet);//����Ĭ�Ͻ���ɫ�仯������QCPColorGradient�в鿴��
	m_pColorMap->rescaleDataRange(true);


	// ����ˢ��ͼ��
	p_imag->rescaleAxes();//����Ӧ��С
	p_imag->replot();

};