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
	//布局
	m_main_layout = new QVBoxLayout;
	m_dropDownList_layout = new QHBoxLayout;
	m_freTime_layout = new QVBoxLayout;

	//实体
	m_timeDomain_general = new QCustomPlot;
	m_freqencyDomain_general = new QCustomPlot;

	//语谱图
	m_spectrum_general = new QCustomPlot;
	this->initSpectrum(m_spectrum_general);

	//边距
	//m_timeDomain_general->setContentsMargins();


	//设置主布局
	this->setLayout(m_main_layout);
	m_main_layout->addLayout(m_dropDownList_layout);
	m_main_layout->addLayout(m_freTime_layout);
	//设置布局内布局伸缩
	m_main_layout->setStretch(0, 1);
	m_main_layout->setStretch(1, 20);
	//主布局边距
	m_main_layout->setMargin(0);
	m_main_layout->setContentsMargins(0,0,0,0);
	//设置外边距
	m_dropDownList_layout->setMargin(0);
	m_freTime_layout->setMargin(0);
	////设置内边距
	//m_dropDownList_layout->setContentsMargins(0, 0, 0, 0);
	////设置 对齐方式
	//m_dropDownList_layout->setAlignment(Qt::AlignCenter);

	////test
	//qDebug() << m_dropDownList_layout->contentsMargins();

	//实体 -> 布局
	//m_dropDownList_layout->addWidget(m_comBox_chooseDisplay,0,Qt::AlignRight);
	//m_dropDownList_layout->setAlignment(Qt::AlignRight);
	//m_dropDownList_layout->setContentsMargins(QMargins(5,0,5,5));

	//数值设置
	this->_pView_ValueOfTFSetting = new View_ValueOfTFSetting;
	//m_dropDownList_layout->addWidget(_pView_ValueOfTFSetting);

	m_freTime_layout->addWidget(_pView_ValueOfTFSetting,1);
	m_freTime_layout->addWidget(m_timeDomain_general,10);
	m_freTime_layout->addWidget(m_freqencyDomain_general,10);
	m_freTime_layout->addWidget(m_spectrum_general,10);
	//customplot 初始化设置
	initCustomPlot();
	//combox 初始化
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
	//初始化各个曲线
	// configure axis rect:配置轴矩形
	m_timeDomain_general->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 这也将允许通过拖拽/缩放尺度改变颜色范围
	m_timeDomain_general->axisRect()->setupFullAxesBox(true);
	m_timeDomain_general->xAxis->setLabel(QString::fromLocal8Bit("数据点"));
	m_timeDomain_general->yAxis->setLabel(QString::fromLocal8Bit("电压"));
	m_timeDomain_general->setBackground(QPixmap::fromImage(QImage(":/test_customPlot_hreatMap/image/gray.png")));
	m_timeDomain_general->xAxis->setVisible(true);
	m_timeDomain_general->yAxis->setVisible(true);
	m_timeDomain_general->xAxis2->setVisible(false);
	m_timeDomain_general->yAxis2->setVisible(false);
	//设定右上角图形标注可见
	m_timeDomain_general->legend->setVisible(true);
	//添加曲线
	m_timeDomain_general->addGraph();
	m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));
	m_timeDomain_general->graph(0)->setPen(QPen(QColor(230, 230, 230)));
	//!!时域部分!!
	//设置X轴坐标范围
	m_timeDomain_general->xAxis->setRange(0, 4800*3);
	//设置Y轴坐标范围
	m_timeDomain_general->yAxis->setRange(-0.02, 0.03);

	//Frequency
	m_freqencyDomain_general->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 这也将允许通过拖拽/缩放尺度改变颜色范围
	m_freqencyDomain_general->axisRect()->setupFullAxesBox(true);
	m_freqencyDomain_general->xAxis->setLabel(QString::fromLocal8Bit("频率点(kHz)"));
	m_freqencyDomain_general->yAxis->setLabel(QString::fromLocal8Bit("振幅"));
	//m_freqencyDomain_general->setBackground(QPixmap::fromImage(QImage(":/test_customPlot_hreatMap/image/gray.png")));
	m_freqencyDomain_general->xAxis->setVisible(true);
	m_freqencyDomain_general->yAxis->setVisible(true);
	m_freqencyDomain_general->xAxis2->setVisible(false);
	m_freqencyDomain_general->yAxis2->setVisible(false);

	//标题可见
	m_freqencyDomain_general->legend->setVisible(true);
	//添加曲线
	m_freqencyDomain_general->addGraph();
	m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-FFT"));
	m_freqencyDomain_general->graph(0)->setPen(QPen(QColor(230, 230, 230)));//灰白
	//!!频域部分!!
	//设置X轴坐标范围
	//m_freqencyDomain_general->xAxis->setRange(0, 2400);//24kHz
	//2023.6.28 更改
	this->m_freqencyDomain_general->xAxis->setRange(0, 10, Qt::AlignLeft); //设置x轴范围
	this->m_freqencyDomain_general->xAxis->ticker()->setTickCount(11);//11个主刻度

	//设置Y轴坐标范围
	m_freqencyDomain_general->yAxis->setRange(-0.02, 0.16);
	//X轴坐标刻度精度
	//m_freqencyDomain_general->xAxis->setNumberPrecision(10);
	//m_freqencyDomain_general->xAxis->setTickLabelRotation(45);
	//设置精度样式
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

	//	//设置宽度
	//	m_comBox_chooseDisplay->setMaximumWidth(50);
	//	////设置居中
	//}

	//if (_pBtnSetting != nullptr)
	//{
	//	//
	//	_pBtnSetting->setText("set");
	//	//设置宽度
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
	//     p_imag = ui->widget_Imag; //创建一个画图指针

	//p_imag->installEventFilter(this);  //安装一个事件过滤器

	p_imag->clearGraphs();//清楚之前设置
	//m_fp5->showTracer(true);//实现数据跟踪(!!重要函数)
	p_imag->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//可拖拽+可滚轮缩放
	//p_imag->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);//可拖拽+可滚轮缩放

	//  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
	// dateTicker->setDateTimeFormat("hh:mm:ss");//日期格式(可参考QDateTime::fromString()函数)
	//fp3->xAxis->setTicker(dateTicker);//设置X轴为时间轴
	//fp3->xAxis->setTicker();


	p_imag->xAxis->setLabel(QString::fromLocal8Bit("Time(ms)"));//x轴显示文字
	p_imag->yAxis->setLabel(QString::fromLocal8Bit("Frequency(kHz)"));//y轴显示文字

	//设置画布背景色
	p_imag->setBackground(QColor(0, 0, 0, 255));//白色 Aipha通道0为不透明，大于0为半透明，255透明，能显示真正的颜色。
	//p_imag->setBackground(QPixmap::fromImage(QImage(":/MainViewControl/image/gray.png")));
	//p_imag->setBackground(QColor(255, 0, 0,50));//白色 Aipha通道0为透明，大于0为半透明，255不透明。

	p_imag->yAxis->setRangeReversed(false);//设置Y坐标轴反转了一下

	p_imag->xAxis->setLabelColor(Qt::white);//文字颜色
	p_imag->yAxis->setLabelColor(Qt::white);
	p_imag->xAxis->setTickLabelColor(Qt::white);//坐标轴数字颜色
	p_imag->yAxis->setTickLabelColor(Qt::white);
	p_imag->xAxis->setBasePen(QPen(Qt::white, 1));//坐标轴颜色及宽度
	p_imag->yAxis->setBasePen(QPen(Qt::white, 1));
	p_imag->xAxis->setTickPen(QPen(Qt::white, 1));//主刻度
	p_imag->yAxis->setTickPen(QPen(Qt::white, 1));
	p_imag->xAxis->setSubTickPen(QPen(Qt::white, 1));//副刻度
	p_imag->yAxis->setSubTickPen(QPen(Qt::white, 1));

	//每条曲线都会独占一个graph()
	p_imag->addGraph();
	p_imag->graph(0)->setPen(QPen(Qt::blue));//曲线颜色

	p_imag->graph(0)->setBrush(QBrush(QColor(0, 255, 255, 20)));//曲线与X轴包围区的颜色

	//通过传递的轴的QCustomPlot进行注册,简洁理解QCPColorMap的数据为（x轴，y轴;颜色，值value）
	m_pColorMap = new QCPColorMap(p_imag->xAxis, p_imag->yAxis);
	//m_pColorMap->data()->setSize(100,5000);//设置整个图（x,y）点数,X轴50个点，y轴4096个点
	m_pColorMap->data()->setSize(500, 1000); //X轴500个点代表时间，y轴1000代表数据点对应0-10kHz
	m_pColorMap->data()->setRange(QCPRange(0, 500), QCPRange(0, 10));//setRange是设置X轴以及Y轴的范围

	QCPColorScale* colorScale = new QCPColorScale(p_imag);  // 构造一个色条
	colorScale->setType(QCPAxis::atRight);   // 水平显示atLeft,atRight,atTop ,atBottom
	p_imag->plotLayout()->addElement(0, 1, colorScale); // 在颜色图右边显示
	colorScale->setDataRange(QCPRange(0, 10));  //设置色条的数值最大最小
	
	//色条允许拖拽
	colorScale->setRangeDrag(true);
	colorScale->setRangeZoom(true);
	//xAxis、yAxis不允许拖拽
	//p_imag->xAxis->s

	colorScale->axis()->setLabelColor(Qt::white);
	colorScale->axis()->setTickLabelColor(Qt::white);
	m_pColorMap->setColorScale(colorScale);

	QCPColorGradient gradient;  // 色条使用的颜色渐变
	gradient.setColorStopAt(0.0, QColor("#f6efa6")); //浅米黄色  // 设置色条开始时的颜色
	gradient.setColorStopAt(1.0, QColor("#bf444c")); //浅米黄色  // 设置色条结束时的颜色

	m_pColorMap->setGradient(QCPColorGradient::gpJet);//设置默认渐进色变化（可在QCPColorGradient中查看）
	m_pColorMap->rescaleDataRange(true);


	// 立即刷新图像
	p_imag->rescaleAxes();//自适应大小
	p_imag->replot();

};