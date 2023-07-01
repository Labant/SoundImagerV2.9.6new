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
	//!==========���ֿ�ʼ==============
	main_layout = new QVBoxLayout;
	mGroupBox_videoAudioDevice_layout = new QVBoxLayout;
	this->setLayout(main_layout);
	mGroupBox_videoAudioDevice = new QGroupBox;
	mGroupBox_videoAudioDevice->setTitle(QString::fromLocal8Bit("����ѡ��"));
	mGroupBox_videoAudioDevice->setContentsMargins(0,20,0,20);
	main_layout->addWidget(mGroupBox_videoAudioDevice);
	mGroupBox_videoAudioDevice->setLayout(mGroupBox_videoAudioDevice_layout);
	
	//!==========��Ա��ʼ����ʼ=========
	//m_View_AudioRecord = new View_AudioRecord(mGroupBox_videoAudioDevice);
	//mGroupBox_videoAudioDevice_layout->addWidget(m_View_AudioRecord, 1);
	//!==========��Ա��ʼ������=========


	//�ɼ���ѡ��
	m_captureCardType = new QGroupBox;
	m_hLayout_selectCapture = new QHBoxLayout;
	m_captureCardType->setLayout(m_hLayout_selectCapture);
	m_captureCardType->setTitle(QString::fromLocal8Bit("�ɼ��豸ѡ��"));
	for (int i = 0; i < 2; ++i)
	{
		m_radioBtn_captureCard[i] = new QRadioButton(i < 1 ? QString::fromLocal8Bit("SoundCardCapture") : QString::fromLocal8Bit("NiCaptureCard"));
		m_hLayout_selectCapture->addWidget(m_radioBtn_captureCard[i]);
	}
	this->m_radioBtn_captureCard[2] = new QRadioButton(QString::fromLocal8Bit("VK"));
	this->m_hLayout_selectCapture->addWidget(this->m_radioBtn_captureCard[2]);

	mGroupBox_videoAudioDevice_layout->addWidget(m_captureCardType,1);
	m_radioBtn_captureCard[2]->setChecked(true);

	//��Ƶѡ��
	for (int i = 0; i < 2; ++i)
	{
		m_group[i] = new QGroupBox(i < 1 ? QString::fromLocal8Bit("��Ƶ����:") : QString::fromLocal8Bit("��Ƶ����:"));
		m_hBox[i] = new QHBoxLayout();
		m_label[i] = new QLabel(i < 1 ? QString::fromLocal8Bit("��Ƶ�豸ѡ��:") : QString::fromLocal8Bit("��Ƶ�豸ѡ��:"));
		m_comBox[i] = new QComboBox();
		m_pushButton[i] = new QPushButton(QString::fromLocal8Bit("����"),this);

		//������ʽ
		//m_label[i]->setFont(QFont(QString::fromLocal8Bit("����"), 12));
		//m_comBox[i]->setFont(QFont(QString::fromLocal8Bit("����"), 12));
		//m_pushButton[i]->setFont(QFont(QString::fromLocal8Bit("����"), 12));

		//�ؼ���С
		//m_label[i]->setMinimumHeight(3);
		//m_comBox[i]->setMinimumHeight(3);
		//m_pushButton[i]->setMinimumHeight(3);

		//���
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
	
	//summit ��ť
	m_btn_summit = new QPushButton(QString::fromLocal8Bit("��ʼ"));
	//summit������ʽ
	m_btn_summit->setFont(QFont(QString::fromLocal8Bit("����"), 12));
	//summit�ؼ���С
	m_btn_summit->setMaximumWidth(100);
	m_hlayout_summit->addWidget(m_btn_summit,0,2,1,1);

	//summit_back ��ť
	m_btn_summit_back = new QPushButton(QString::fromLocal8Bit("ֹͣ"));
	//summit_back������ʽ
	m_btn_summit_back->setFont(QFont(QString::fromLocal8Bit("����"), 12));
	//summit_back�ؼ���С
	m_btn_summit_back->setMaximumWidth(100);
	m_hlayout_summit->addWidget(m_btn_summit_back,0,3,1,1);

	//2023.6.30����
    // Tcp����״̬��ʾ
	this->_pLabelTcpState = new QLabel(QString::fromLocal8Bit("Tcp no connect��"));//Tcp no connect
	m_hlayout_summit->addWidget(_pLabelTcpState, 0, 0, 1, 1);
	//!==========���ֽ���==============
	

}

void View_DeviceChoose::initConnect()
{
	//����Ƶ�豸����
	for (int i = 0; i < 2; ++i)
	{
		connect(m_pushButton[i], &QPushButton::clicked, this, i < 1 ? &View_DeviceChoose::m_audio_serach : &View_DeviceChoose::m_video_serach);
	}

	//commit
	connect(m_btn_summit, &QPushButton::clicked, this, &View_DeviceChoose::m_serach_commit);

	//commit back
	connect(m_btn_summit_back, &QPushButton::clicked, this, &View_DeviceChoose::m_solt_device_stop);
	
	//�豸����ѡ��
	connect(this, &View_DeviceChoose::m_signal_captureType, this, &View_DeviceChoose::m_solt_chooseCaptureType);

	//�ɼ����豸ѡ��
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
	this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
	this->setPalette(palette);
}

void View_DeviceChoose::m_audio_serach()
{
	//�Ƿ�������Ϊ��
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
		//	//���˿�
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

void View_DeviceChoose::m_serach_commit()//��ʼbtn
{
	//m_bool_audio_reFound��������������� Ϊ�����������豸��Ϊ�ٲ���ʾ����
	//m_bool_serached:Ϊ�٣�����������Ϊ�棺������������
	if (m_bool_audio_reFound && !m_bool_serached)
	{
		emit m_signal_place_reFoundAudioBtn();
		return;
	}

	//���һ�κ��ܵ��
	m_btn_summit->setCheckable(0);
	////������Ϊ��
	//m_bool_audio_reFound = 0;

	if (m_comBox[0]->currentText().isEmpty())
		return;

	//��Ƶ�豸ѡ��
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
			m_niDevice->setChannelNum(CHANNLENUMBER);//ͨ����Ϊ��4ChannelNum
			//m_niDevice->setSamples(4810);//��������4810
			m_niDevice->setSamples(SAMPLE);//��������4810
			m_niDevice->setRate(FS/1000);//�����ʣ�48k
			m_niDevice->setDevName(QString("%1").arg(NiDevice::getNiDeviceName()));//�豸��
			//this->pNiDevice->setDevName(name, mic);//�豸��
			m_niDevice->init();
			m_niDevice->start();

			//�ѳ�ʼ��
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
		////��ʼ��������
		//this->_pTcpDiscoverServer->start();
		////����TcpServerӦ�õȴ��ͻ��˵����Ӻ�(�ڲ�����:���ò���)�������ɼ������и�isEmpty�Ķ���
		//emit m_signal_audioStart();//->��֪��������Բɼ������Ѿ���->�����������Ķ����ݽ�����Ⱦ������

		//tcp ���ÿͻ��˲��� -->��ΪClient���Ӻ��Զ�����
		//emit this->on_signal_initTcpClientParameter();
		//tcp �ɼ����ݽ�������
		emit this->on_signal_collectData();//�ɼ�����->AudioCollector
		emit m_signal_audioStart();//->��֪��������Բɼ������Ѿ���->�����������Ķ����ݽ�����Ⱦ������
		break;
	default:
		break;
	}
	

	//!������Ƶѡ��
	if (m_comBox[1]->currentText().isEmpty())
		return;
	//����Ƿ��ʼ���� -->��ʼ����������������
	//if (!m_bool_initCamera )
	//	emit this->m_signal_camera(m_comBox[1]->currentIndex());
	m_bool_initCamera = 1;
	
	//��Ƶ���߳̿�ʼץȡ֡ -->�ɼ���Ƶ֡
	emit this->m_signal_modelStart();




	//ֹͣ��ť���Ե��
	m_btn_summit_back->setCheckable(1);

}

void View_DeviceChoose::m_slot_soundCardModel()
{
	//��������
	m_bool_audio_reFound = 1;
	//�������� �����豸
	m_bool_serached = 0;
	emit m_signal_captureType(CAPTURETYPE::SoundCard);
}

void View_DeviceChoose::m_slot_NiCardModel()
{
	//��Ƶ������Ϊ��
	m_bool_audio_reFound = 1;
	//�������� �����豸
	m_bool_serached = 0;
	emit m_signal_captureType(CAPTURETYPE::NICaptureCard);
}

void View_DeviceChoose::m_slot_VKCardModel()
{
	//��Ƶ������Ϊ��
	m_bool_audio_reFound = 1;
	//�������� �����豸
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
	emit this->m_signal_stop();//��֪���߳��Լ����߳�ֹͣ�ɼ�
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
		//	this->_pTcpDiscoverServer->stop();//ֹͣ��������
		//}

		//tcp ֹͣ�ɼ����ݽ�������
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