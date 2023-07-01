#include "View_DeviceChoose.h"

View_DeviceChoose::View_DeviceChoose(QWidget* p)
{
	//this->setParent(p);

	initLayout();
	initConnect();
	//setCSS();
}

View_DeviceChoose::~View_DeviceChoose()
{
	m_niDevice->deleteLater();
}

void View_DeviceChoose::initLayout()
{
	//!==========布局开始==============
	main_layout = new QVBoxLayout;
	mGroupBox_videoAudioDevice_layout = new QVBoxLayout;
	this->setLayout(main_layout);
	mGroupBox_videoAudioDevice = new QGroupBox;
	mGroupBox_videoAudioDevice->setTitle(QString::fromLocal8Bit("设置选项"));
	mGroupBox_videoAudioDevice->setContentsMargins(0,20,0,20);
	main_layout->addWidget(mGroupBox_videoAudioDevice);
	mGroupBox_videoAudioDevice->setLayout(mGroupBox_videoAudioDevice_layout);
	
	//!==========成员初始化开始=========
	//m_View_AudioRecord = new View_AudioRecord(mGroupBox_videoAudioDevice);
	//mGroupBox_videoAudioDevice_layout->addWidget(m_View_AudioRecord, 1);
	//!==========成员初始化结束=========


	//采集卡选择
	m_captureCardType = new QGroupBox;
	m_hLayout_selectCapture = new QHBoxLayout;
	m_captureCardType->setLayout(m_hLayout_selectCapture);
	m_captureCardType->setTitle(QString::fromLocal8Bit("采集设备选择"));
	for (int i = 0; i < 2; ++i)
	{
		m_radioBtn_captureCard[i] = new QRadioButton(i < 1 ? QString::fromLocal8Bit("SoundCardCapture") : QString::fromLocal8Bit("NiCaptureCard"));
		m_hLayout_selectCapture->addWidget(m_radioBtn_captureCard[i]);
	}
	this->m_radioBtn_captureCard[2] = new QRadioButton(QString::fromLocal8Bit("VK"));
	this->m_hLayout_selectCapture->addWidget(this->m_radioBtn_captureCard[2]);

	mGroupBox_videoAudioDevice_layout->addWidget(m_captureCardType,1);
	m_radioBtn_captureCard[2]->setChecked(true);

	//音频选择
	for (int i = 0; i < 2; ++i)
	{
		m_group[i] = new QGroupBox(i < 1 ? QString::fromLocal8Bit("音频设置:") : QString::fromLocal8Bit("视频设置:"));
		m_hBox[i] = new QHBoxLayout();
		m_label[i] = new QLabel(i < 1 ? QString::fromLocal8Bit("音频设备选择:") : QString::fromLocal8Bit("视频设备选择:"));
		m_comBox[i] = new QComboBox();
		m_pushButton[i] = new QPushButton(QString::fromLocal8Bit("搜索"),this);

		//字体样式
		//m_label[i]->setFont(QFont(QString::fromLocal8Bit("黑体"), 12));
		//m_comBox[i]->setFont(QFont(QString::fromLocal8Bit("黑体"), 12));
		//m_pushButton[i]->setFont(QFont(QString::fromLocal8Bit("黑体"), 12));

		//控件大小
		//m_label[i]->setMinimumHeight(3);
		//m_comBox[i]->setMinimumHeight(3);
		//m_pushButton[i]->setMinimumHeight(3);

		//添加
		m_hBox[i]->addWidget(m_label[i]);
		m_hBox[i]->addWidget(m_comBox[i]);
		m_hBox[i]->addWidget(m_pushButton[i]);
		m_group[i]->setLayout(m_hBox[i]);
		mGroupBox_videoAudioDevice_layout->addWidget(m_group[i],1);
	}
	
	//summit box
	m_hlayout_summit = new QGridLayout;
	mGroupBox_videoAudioDevice_layout->addLayout(m_hlayout_summit, 1);

	//m_hlayout_summit->setAlignment(Qt::AlignRight);
	
	//summit 按钮
	m_btn_summit = new QPushButton(QString::fromLocal8Bit("开始"));
	//summit字体样式
	m_btn_summit->setFont(QFont(QString::fromLocal8Bit("宋体"), 12));
	//summit控件大小
	m_btn_summit->setMaximumWidth(100);
	m_hlayout_summit->addWidget(m_btn_summit,0,2,1,1);

	//summit_back 按钮
	m_btn_summit_back = new QPushButton(QString::fromLocal8Bit("停止"));
	//summit_back字体样式
	m_btn_summit_back->setFont(QFont(QString::fromLocal8Bit("宋体"), 12));
	//summit_back控件大小
	m_btn_summit_back->setMaximumWidth(100);
	m_hlayout_summit->addWidget(m_btn_summit_back,0,3,1,1);

	//2023.6.30新增
    // Tcp连接状态显示
	this->_pLabelTcpState = new QLabel(QString::fromLocal8Bit("Tcp no connect！"));//Tcp no connect
	m_hlayout_summit->addWidget(_pLabelTcpState, 0, 0, 1, 1);
	//!==========布局结束==============
	

}

void View_DeviceChoose::initConnect()
{
	//音视频设备搜索
	for (int i = 0; i < 2; ++i)
	{
		connect(m_pushButton[i], &QPushButton::clicked, this, i < 1 ? &View_DeviceChoose::m_audio_serach : &View_DeviceChoose::m_video_serach);
	}

	//commit
	connect(m_btn_summit, &QPushButton::clicked, this, &View_DeviceChoose::m_serach_commit);

	//commit back
	connect(m_btn_summit_back, &QPushButton::clicked, this, &View_DeviceChoose::m_solt_device_stop);
	
	//设备类型选择
	connect(this, &View_DeviceChoose::m_signal_captureType, this, &View_DeviceChoose::m_solt_chooseCaptureType);

	//采集卡设备选择
	for (int i = 0; i < 2; ++i)
	{
		connect(m_radioBtn_captureCard[i], &QRadioButton::clicked, this, i < 1 ? &View_DeviceChoose::m_slot_soundCardModel : &View_DeviceChoose::m_slot_NiCardModel);
	}
	connect(m_radioBtn_captureCard[2], &QRadioButton::clicked, this, &View_DeviceChoose::m_slot_VKCardModel);

}

void View_DeviceChoose::setCSS(QPalette& palette)
{
	//QPalette pal = this->palette();
	//pal.setBrush(QPalette::Background, QBrush(QPixmap(":/test_customPlot_hreatMap/image/gray.png")));
	this->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.
	this->setPalette(palette);
}

void View_DeviceChoose::m_audio_serach()
{
	//是否搜索过为真
	m_bool_serached = 1;

	switch (m_captrueType)
	{
	case  CAPTURETYPE::SoundCard:

		m_comBox[0]->clear();
		//info
		foreach(QAudioDeviceInfo infos, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
		{
			//QStringList
			m_comBox[0]->addItem(infos.deviceName());

		}
		break;
	case  CAPTURETYPE::NICaptureCard:

		m_comBox[0]->clear();
		if (this->m_niDevice == nullptr)
		{
			m_niDevice = new NiDevice;

		}
		
		m_comBox[0]->addItem(m_niDevice->getNiDeviceName());

		break;

	case CAPTURETYPE::VKCaptureCard:

		m_comBox[0]->clear();
		//if (this->_pTcpDiscoverServer == nullptr)
		//{
		//	this->_pTcpDiscoverServer = new TcpDiscoverServer;
		//	//this->_pTcpDiscoverServer->moveToThread(&this->mThread);
		//	//this->mThread.start(QThread::HighestPriority);
		//	//检测端口
		//	this->_pTcpDiscoverServer->openMonitorPort();
		//}
		m_comBox[0]->addItem(QString("VK"));


		//tell mode AudioCollector to init tcp and open monitor port
		emit this->on_signal_initTcpServerAndMonitor();
		

		QTimer::singleShot(100, this, [=]() {
			emit this->on_signal_initTCPState();
			});

		break;
	default:
		break;
	}



}

void View_DeviceChoose::m_video_serach()
{
	emit this->m_signal_findVideoDevs();
}

void View_DeviceChoose::m_serach_commit()//开始btn
{
	//m_bool_audio_reFound：搜索触发则进行 为真重新搜索设备，为假不提示查找
	//m_bool_serached:为假：不曾搜索，为真：重新搜索过。
	if (m_bool_audio_reFound && !m_bool_serached)
	{
		emit m_signal_place_reFoundAudioBtn();
		return;
	}

	//点击一次后不能点击
	m_btn_summit->setCheckable(0);
	////搜索置为否
	//m_bool_audio_reFound = 0;

	if (m_comBox[0]->currentText().isEmpty())
		return;

	//音频设备选择
	switch (m_captrueType)
	{
	case CAPTURETYPE::SoundCard:
		if (this->pSoundCard == nullptr)
		{
			this->pSoundCard = new soundCard;
			this->pSoundCard->start(m_comBox[0]->currentText());
			emit m_signal_audioStart();
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		if (m_niDevice != nullptr && !m_initNiDev)
		{
			m_niDevice->setChannelNum(CHANNLENUMBER);//通道数为：4ChannelNum
			//m_niDevice->setSamples(4810);//采样数：4810
			m_niDevice->setSamples(SAMPLE);//采样数：4810
			m_niDevice->setRate(FS/1000);//采样率：48k
			m_niDevice->setDevName(QString("%1").arg(NiDevice::getNiDeviceName()));//设备名
			//this->pNiDevice->setDevName(name, mic);//设备名
			m_niDevice->init();
			m_niDevice->start();

			//已初始化
			m_initNiDev = 1;
		}
		else if(m_initNiDev)
		{
			m_niDevice->start();
		}
		emit m_signal_audioStart();
		break;

	case CAPTURETYPE::VKCaptureCard:

		//if(this->_pTcpDiscoverServer == nullptr)
		//	break;
		////开始解析数据
		//this->_pTcpDiscoverServer->start();
		////开启TcpServer应该等待客户端的连接后(内部动作:设置参数)才启动采集，但有个isEmpty的动作
		//emit m_signal_audioStart();//->告知主程序可以采集数据已就绪->主程序启动阅读数据进行渲染。。。

		//tcp 设置客户端参数 -->改为Client连接后，自动设置
		//emit this->on_signal_initTcpClientParameter();
		//tcp 采集数据解析数据
		emit this->on_signal_collectData();//采集数据->AudioCollector
		emit m_signal_audioStart();//->告知主程序可以采集数据已就绪->主程序启动阅读数据进行渲染。。。
		break;
	default:
		break;
	}
	

	//!处理视频选择
	if (m_comBox[1]->currentText().isEmpty())
		return;
	//相机是否初始化过 -->初始化相机在类内已完成
	//if (!m_bool_initCamera )
	//	emit this->m_signal_camera(m_comBox[1]->currentIndex());
	m_bool_initCamera = 1;
	
	//视频子线程开始抓取帧 -->采集视频帧
	emit this->m_signal_modelStart();




	//停止按钮可以点击
	m_btn_summit_back->setCheckable(1);

}

void View_DeviceChoose::m_slot_soundCardModel()
{
	//声卡搜索
	m_bool_audio_reFound = 1;
	//重新搜索 声卡设备
	m_bool_serached = 0;
	emit m_signal_captureType(CAPTURETYPE::SoundCard);
}

void View_DeviceChoose::m_slot_NiCardModel()
{
	//音频搜索置为真
	m_bool_audio_reFound = 1;
	//重新搜索 声卡设备
	m_bool_serached = 0;
	emit m_signal_captureType(CAPTURETYPE::NICaptureCard);
}

void View_DeviceChoose::m_slot_VKCardModel()
{
	//音频搜索置为真
	m_bool_audio_reFound = 1;
	//重新搜索 声卡设备
	m_bool_serached = 0;
	emit m_signal_captureType(CAPTURETYPE::VKCaptureCard);
}

void View_DeviceChoose::m_solt_chooseCaptureType(int type)
{
	switch (type)
	{
	case CAPTURETYPE::SoundCard:
		this->m_captrueType = CAPTURETYPE::SoundCard;
		this->_pLabelTcpState->setText(QString::fromLocal8Bit(" "));
		break;
	case CAPTURETYPE::NICaptureCard:
		this->m_captrueType = CAPTURETYPE::NICaptureCard;
		this->_pLabelTcpState->setText(QString::fromLocal8Bit(" "));
		break;
	case CAPTURETYPE::VKCaptureCard:
		this->m_captrueType = CAPTURETYPE::VKCaptureCard;
		this->_pLabelTcpState->setText(this->mSaveTcpState);
		break;
	default:
		break;
	}

}

void View_DeviceChoose::m_solt_device_stop()
{
	m_btn_summit_back->setCheckable(0);
	emit this->m_signal_stop();//告知主线程以及本线程停止采集
	m_btn_summit->setCheckable(1);
	switch (this->m_captrueType)
	{
	case CAPTURETYPE::SoundCard:
		if (this->pSoundCard != nullptr)
		{
			this->pSoundCard->stop();
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		if (this->m_niDevice != nullptr)
		{
			this->m_niDevice->stop();
		}
		break;
	case CAPTURETYPE::VKCaptureCard:
		//if (this->_pTcpDiscoverServer != nullptr)
		//{
		//	this->_pTcpDiscoverServer->stop();//停止解析数据
		//}

		//tcp 停止采集数据解析数据
		emit this->on_signal_collectData_stop();
		break;
	default:
		break;
	}
}
void View_DeviceChoose::on_slot_tcpStateMonitor(int _state)
{
	if (this->m_captrueType == CAPTURETYPE::VKCaptureCard)
	{
		switch (_state)
		{
		case QAbstractSocket::UnconnectedState:
			break;
		case QAbstractSocket::HostLookupState:
			break;
		case QAbstractSocket::ConnectingState:
			this->_pLabelTcpState->setText(QString::fromLocal8Bit("Tcp State :Connecting!"));
			this->mSaveTcpState = QString::fromLocal8Bit("Tcp State :Connecting!");
			break;
		case QAbstractSocket::ConnectedState:
			this->_pLabelTcpState->setText(QString::fromLocal8Bit("Tcp State :Connected!"));
			this->mSaveTcpState = QString::fromLocal8Bit("Tcp State :Connected!");
			break;
		case QAbstractSocket::BoundState:
			this->_pLabelTcpState->setText(QString::fromLocal8Bit("Tcp State :BoundState!"));
			this->mSaveTcpState = QString::fromLocal8Bit("Tcp State :BoundState!");
			break;
		case QAbstractSocket::ListeningState:
			break;
		case QAbstractSocket::ClosingState:
			this->_pLabelTcpState->setText(QString::fromLocal8Bit("Tcp State :Clinet not online!"));
			this->mSaveTcpState = QString::fromLocal8Bit("Tcp State :Clinet not online!");
			break;
		default:
			break;
		}
	}
}