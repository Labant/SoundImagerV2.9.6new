#include "MainViewControl.h"
#include "fftw3.h"
#include "complex.h"
#include <algorithm>
#include "Custom_Box.h"
#include "MainModel.h"
#include <QFile>
#include <QDebug>
#include "Model_hotMapAndVideoDisplay.h"

MainViewControl::MainViewControl(QWidget *parent)
    : QWidget(parent)
    //, ui(new Ui::test_customPlot_hreatMapClass())
{
	//ui->setupUi(this);
	
	////��ȡcvs
	//QFile ttFile("P_1DData_1_0m.csv");
	//if (!ttFile.open(QIODevice::ReadOnly | QIODevice::Text))
	//	qDebug() << "not open file";
	//QList<QByteArray> allData1, allData2;
	//
	//for (int i = 0; i < 180;i++)
	//{
	//	allData1.clear();
	//	allData1 = ttFile.readLine().split(',');
	//	allData2.append(allData1);
	//}
	// 
	////ttdouble = new double[180 * 360]
	//static int ii = 0;
	//for (; !allData2.isEmpty();)
	//{
	//	//ttVec.push_back(allData.takeFirst().toDouble());
	//	P_1D[ii++] = allData2.takeFirst().toDouble();
	//}

	////test ��ֵ
	//this->Fine_S = new double[64800 * 36];


	//����
	p_fre_copy = new double[2400];
	x_fre_copy = new double[2400];
	y_fre_copy = new double[2400];
	z_fre_copy = new double[2400];

	//P_1D_MAX = new double;
	//*P_1D_MAX = 1.0;

	//initLayout
	initLoyout();

	//��������
	//creatSound();

	//��ʼ�����
	//initCamera();

	////paintCustom ����ͼ��̬��ȡ�ļ�����
	////paintCustom();

	//��ʼ����ͼͼ
	this->initSpectrumSequence();

	//��ʼ������������
	this->initConnectedAndUpData();

	////������ʽ��
	this->setCSS();

	////�����������
	////m_model_videoDevice->setFrameWidth(1920);
	////m_model_videoDevice->setFrameHeigth(1080);
	////m_model_videoDevice->setFrameRate(60);

	//m_model_videoDevice->setFrameWidth(848);
	//m_model_videoDevice->setFrameHeigth(480);
	//m_model_videoDevice->setFrameRate(60);


}

MainViewControl::~MainViewControl()
{
	//delete ui;

	//ֹͣ�ɼ�
	this->_mView_TabWidget_Setting->mView_deviceChoose->m_solt_device_stop();

	if (_mView_TabWidget_Setting != nullptr && _mView_TabWidget_Setting->mView_deviceChoose->pSoundCard != nullptr)
	{
		_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->deleteLater();
	}
	timer_thread_1->stop();
	timer.stop();

	//timer_thread_1->stop();
	//timer_thread_2->stop();

	if (fft_thread != nullptr)
	{
		fft_thread->quit();
		fft_thread->wait();
		fft_thread->deleteLater();
	}


	if (m_thread_AudioRecord != nullptr)
	{
		m_thread_AudioRecord->quit();
		m_thread_AudioRecord->wait();
		m_thread_AudioRecord->deleteLater();
	}


	//camera_thread->quit();
	//camera_thread = nullptr;
	//camera_thread->deleteLater();
	//delete camera_thread;

	delete p_fre_copy;
	delete x_fre_copy;
	delete y_fre_copy;
	delete z_fre_copy;
	//delete P_1D_MAX; 

}

void MainViewControl::realDrawplot()
{
	//qDebug() << "current_start:" << QDateTime::currentMSecsSinceEpoch();
	
	//if (!isInitCamera) {
	//	return;
	//}

	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		if (_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard == nullptr)
		{
			return;
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
		{
			return;
		}
		break;
	default:
		break;
	}

	//��ȡһ֡
	this->read();

	//�ж�p x y z ����Ϊ0��ִ��
	if (this->p[0] == 0 || this->x[0] == 0 || this->y[0] == 0 || this->z[0] == 0)
		return;
	//qDebug() << "1��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	invokeDLL();
	//qDebug() << "2��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	//�ػ�����ͼ
	realDraw_plot_heatmap();
	//qDebug() << "3��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	//�ػ�ʱ��ͼ
	realDraw_plot_timeDomain();

	//�ػ�����
	realDraw_plot_eleAngle();

	////�ػ淽λ��
	//realDraw_plot_azimAngle();

	//�ػ�Ƶ�� �� ����ͼ
	realDraw_plot_freqDomain();
}

void MainViewControl::doFFTData()
{
	//qDebug() << "doFFTData->threadId:" << this->thread()->currentThread();

	//δ�����Ϊfalse
	isFftDown = 0;

	if (fft_thread == nullptr)
	{ 
		timer_thread_1->stop();
		return;
	}

	//fftw
	fft_change->Time2Frequency(p, x, y, z ,SAMPLE);

	//plot_freqDomain_p
	//����ʱ������ QVector<double>����
	QVector<double> xData_p_fre, yData_p_fre, xData_x_fre, yData_x_fre, xData_y_fre, yData_y_fre, xData_z_fre, yData_z_fre;
	//������Ⱦspectrum������
	QVector<double> xData_P_spectrum, xData_X_spectrum, xData_Y_spectrum, xData_Z_spectrum;//xAxis����
	QVector<double> yData_P_spectrum, yData_X_spectrum, yData_Y_spectrum, yData_Z_spectrum;//yAxis����

	//��ȡfft�������
	for (int i = 0; i < 2400; i++)
	{
		//��Ƶ��x����������
		xData_p_fre.append(i);
		yData_p_fre.append(fft_change->fft_p[i]);
		xData_x_fre.append(i);
		yData_x_fre.append(fft_change->fft_x[i]);
		xData_y_fre.append(i);
		yData_y_fre.append(fft_change->fft_y[i]);
		xData_z_fre.append(i);
		yData_z_fre.append(fft_change->fft_z[i]);

		//������ͼy�����ݽ�������
		if (i >= 1000)continue;;
		xData_P_spectrum.append(i);
		yData_P_spectrum.append(fft_change->fft_p[i]);
		xData_X_spectrum.append(i);
		yData_X_spectrum.append(fft_change->fft_x[i]);
		xData_Y_spectrum.append(i);
		yData_Y_spectrum.append(fft_change->fft_y[i]);
		xData_Z_spectrum.append(i);
		yData_Z_spectrum.append(fft_change->fft_z[i]);
			
	}

	////!-----------------------------------
	////���浽CVS
	//QFile turnData(QString("CVSData_x_fft_check_NI.csv"));
	//if (!turnData.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//{
	//	qDebug() << "file not open!";
	//	return;
	//}
	//QTextStream stream(&turnData);
	//for (int i = 0; i <SAMPLE / 2; i++)
	//{
	//	stream << this->fft_change->fft_x[i];
	//	if (i == SAMPLE / 2 - 1)
	//	{
	//	}
	//	else
	//	{
	//		stream << ",";
	//	}
	//}
	//turnData.close();
	////! ------------------------------------



	{
		QMutexLocker m_dataCopyLocker(&m_dataCopy);
		//p_fre_copy���ڼ�����Ƶ�����ֵ
		memcpy(p_fre_copy, fft_change->fft_p, sizeof(double) * 2400);
		memcpy(x_fre_copy, fft_change->fft_x, sizeof(double) * 2400);
		memcpy(y_fre_copy, fft_change->fft_y, sizeof(double) * 2400);
		memcpy(z_fre_copy, fft_change->fft_z, sizeof(double) * 2400);
	}

	//����ͼ����µ�����
	if (mSpectrumSequence_P.size() >= 500)
	{
		auto aa = mSpectrumSequence_P.takeFirst();
		mSpectrumSequence_P.push_back(yData_P_spectrum);

		auto bb = mSpectrumSequence_X.takeFirst();
		mSpectrumSequence_X.push_back(yData_X_spectrum);

		auto cc = mSpectrumSequence_Y.takeFirst();
		mSpectrumSequence_Y.push_back(yData_Y_spectrum);

		auto dd = mSpectrumSequence_Z.takeFirst();
		mSpectrumSequence_Z.push_back(yData_X_spectrum);
	}

	switch (m_freTime_general->_pView_ValueOfTFSetting->m_comBox_chooseDisplay->currentIndex())
	{
	case MYSTRUCT::PXYZ::pxyz::P://0
		//ͨ��
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_p_fre, yData_p_fre);
		//����ͼ setCell()
		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				//qDebug() << "yData_P_spectrum.at(i).at(j):" << mSpectrumSequence.at(i).at(j);
				m_freTime_general->m_pColorMap->data()->setCell(i, j, mSpectrumSequence_P.at(i).at(j));
			}
		}

		break;
	case MYSTRUCT::PXYZ::pxyz::X:
		//ͨ��
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_x_fre, yData_x_fre);
		//����ͼ
		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				//qDebug() << "yData_P_spectrum.at(i).at(j):" << mSpectrumSequence.at(i).at(j);
				m_freTime_general->m_pColorMap->data()->setCell(i, j, mSpectrumSequence_X.at(i).at(j));
			}
		}
		break;
	case MYSTRUCT::PXYZ::pxyz::Y:
		//ͨ��
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_y_fre, yData_y_fre);
		//����ͼ
		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				//qDebug() << "yData_P_spectrum.at(i).at(j):" << mSpectrumSequence.at(i).at(j);
				m_freTime_general->m_pColorMap->data()->setCell(i, j, mSpectrumSequence_Y.at(i).at(j));
			}
		}
		break;
	case MYSTRUCT::PXYZ::pxyz::Z:
		//ͨ��
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_z_fre, yData_z_fre);
		//����ͼ
		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				//qDebug() << "yData_P_spectrum.at(i).at(j):" << mSpectrumSequence.at(i).at(j);
				m_freTime_general->m_pColorMap->data()->setCell(i, j, mSpectrumSequence_Z.at(i).at(j));
			}
		}
		break;
	default:
		break;
	}


	//replot��Ҫ��ͬһ�߳��е��ã����� realDraw_plot_freqDomain()����

	//�����Ϊtrue
	isFftDown = 1;
}

void MainViewControl::doCameraData()
{
	//frames = pipe.wait_for_frames();//�ȴ��������õ������ɿ��

	//rs2::align align_to_color(RS2_STREAM_COLOR);
	//frames = align_to_color.process(frames);

	//rs2::frame color_frame = frames.get_color_frame();

	//cv::Mat frame_color(cv::Size(camera_frame_width, camera_frame_height), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);

	//plot_heatmap->setBackground(QPixmap::fromImage(CVMat2QImage::QCVMat2QImage(frame_color)), true, Qt::KeepAspectRatioByExpanding);

	
}

void MainViewControl::findVideoDevs()
{
	this->_mView_TabWidget_Setting->mView_deviceChoose->m_comBox[1]->clear();
	this->_mView_TabWidget_Setting->mView_deviceChoose->m_comBox[1]->addItems(this->_pMainModel->m_model_videoDevice->getDeviceList());
	qDebug() << "=======================wei zhi xing wan bi================================";
}

void MainViewControl::setModel(MainModel* hendle)
{
	this->_pMainModel = hendle;

	//filt btn on_Click
	connect(this->_pMainModel->_pModel_filteringSetting, &Model_filteringSetting::on_signal_filtBtn_clicked, this, [=]() {
		isFilt = !isFilt;
	if (isFilt)
	{
		filt_low = this->_pMainModel->_pModel_filteringSetting->getFiltLow();
		filt_hight = this->_pMainModel->_pModel_filteringSetting->getFiltHigh();
		qDebug() << "filt_low:" << filt_low;
		qDebug() << "filt_hight:" << filt_hight;
		this->_pMainModel->_pModel_filteringSetting->setBtnSwitchText(1);
	}
	else
	{
		this->_pMainModel->_pModel_filteringSetting->setBtnSwitchText(0);
	}
		}, Qt::DirectConnection);

	//����setModel��
	//�ü���ֵ���
	connect(this->_pMainModel->_pModel_HMCutOut, &Model_HMCutOut::xAxisCutOutChanged, this, &MainViewControl::On_setXAxisCutOutValue);//X cutout value
	connect(this->_pMainModel->_pModel_HMCutOut, &Model_HMCutOut::yAxisCutOutChanged, this, &MainViewControl::On_setYAxisCutOutValue);//Y cutout value

	//hotMap factor
	connect(this->_pMainModel->_Model_HmSizeFactor, &Model_HmSizeFactor::On_Signal_FactoeValue, this->_pMainModel->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::On_setHmFactorValue, Qt::QueuedConnection);


	//�������߳��Ƿ���ʾhm
	connect(this, &MainViewControl::on_signal_P1_DMaxValue, this->_pMainModel->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_setP1_DValue, Qt::QueuedConnection);
	//��ʾĿǰ���ֵ
	connect(this, &MainViewControl::on_signal_P1_DMaxValue, this->_mView_TabWidget_Setting->_pView_HMCutOut, &View_HMCutOut::on_showP1_DMaxValue);
}

void MainViewControl::initLoyout()
{
	this->setWindowTitle(QString::fromLocal8Bit("��Դ��λ | ���������Ƽ�"));
	this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/image/logo.png")));
	this->setMinimumSize(1600, 900);//16:9
	//this->setMinimumSize(1600, 1200);
	//plot_heatmap = new QCustomPlot(this);//����ͼ
	this->_pView_hotMapAndVideoDisplay = new View_hotMapAndVideoDisplay;//����ͼ
	m_shereLocalation = new GLEWINQT2(this);
	m_freTime_general = new View_FrequencyTimeDomain(this);

	hbox = new QHBoxLayout;//������
	vbox_e_a = new QVBoxLayout;
	vbox_time_pxyz = new QVBoxLayout;
	mGroupBox_XYPos = new QGroupBox;
	mGroupBox_shere = new QGroupBox;
	mGroupBox_freTimeDomain = new QGroupBox;
	mGroupBox_videoHotMap = new QGroupBox;

	_mView_TabWidget_Setting = new View_TabWidget_Setting;

	plot_shere_box = new QVBoxLayout;
	plot_timefreqDomain_box = new QVBoxLayout;
	plot_videoDomain_box = new QVBoxLayout;
	plot_eleAngle_box = new QVBoxLayout;
	plot_azimAngle_box = new QVBoxLayout;
	xyBox = new QHBoxLayout;

	////����ͼ���񲻿ɼ�
	//plot_heatmap->xAxis->grid()->setVisible(false);
	//plot_heatmap->yAxis->grid()->setVisible(false);

	////����ͼx y �᲻�ɼ�
	//plot_heatmap->xAxis->setVisible(false);
	//plot_heatmap->xAxis2->setVisible(false);
	//plot_heatmap->yAxis->setVisible(false);
	//plot_heatmap->yAxis2->setVisible(false);



	//max[x,y]��ǩ // ��0����x;[1]:x-value;[2]:y;[3]:y-value;
	for (int i = 0; i < 4; i++) 
	{
		xylable[i] = new QLabel(QString::fromLocal8Bit(i == 2 ? "��λ��:" : "������"));
		xylable[i]->setToolTip(QString::fromLocal8Bit(i == 2 ? "��λ��" : "������"));
		xylable[i]->setFont(QFont(QString::fromLocal8Bit("����"),15));
		//��ӽ�������
		xyBox->addWidget(xylable[i]);
		
	}
	//ʵʱλ�÷���group��
	mGroupBox_XYPos->setLayout(xyBox);

	//��ʼ����ֵ
	xylable[1]->setText(QString::fromLocal8Bit("0"));
	xylable[3]->setText(QString::fromLocal8Bit("0"));
	//add loyout
	//vbox_e_a->addWidget(mGroupBox_XYPos,1); //, 2
	vbox_e_a->addWidget(_mView_TabWidget_Setting,1);//,3
	m_Model_AudioRecord = new Model_RecordAudio;
	m_Model_AudioRecord->setFs(FS);
	m_Model_AudioRecord->setBitsberSample(BITERSSAMPLE);
	m_Model_AudioRecord->setChannleNumber(CHANNLENUMBER);
	m_timer_record = new QTimer;
	m_thread_AudioRecord = new QThread(this);
	m_Model_AudioRecord->moveToThread(m_thread_AudioRecord);
	m_thread_AudioRecord->start();

	//setGroupBoxName
	mGroupBox_shere->setTitle(QString::fromLocal8Bit("���嶨λ"));
	mGroupBox_XYPos->setTitle(QString::fromLocal8Bit("��Դʵʱλ��"));
	mGroupBox_freTimeDomain->setTitle(QString::fromLocal8Bit("ʱ��|Ƶ��"));
	mGroupBox_videoHotMap->setTitle(QString::fromLocal8Bit("ͼ��λ"));


	//�������
	for (int i = 0;i<2;i++)
	{
		vBox[i] = new QVBoxLayout;
		hbox->addLayout(vBox[i]);
	}
	//���ҷ��䲼��
	hbox->setStretch(0, 2);
	hbox->setStretch(1, 4);

	//qDebug()<<vbox->stretch(0);

	//���
	plot_shere_box->addWidget(m_shereLocalation);


	mGroupBox_shere->setLayout(plot_shere_box);
	mGroupBox_freTimeDomain->setLayout(plot_timefreqDomain_box);
	mGroupBox_videoHotMap->setLayout(plot_videoDomain_box);

	//hBox[0]->addLayout(vbox_e_a,1);//���������ӽ����������� ����
	//hBox[0]->addWidget(mGroupBox_shere,1);//���� ����

	//hBox[1]->addWidget(mGroupBox_freTimeDomain,1);//ʱ�� Ƶ��
	//hBox[1]->addWidget(mGroupBox_videoHotMap,1);//����ͼ


	vBox[0]->addLayout(vbox_e_a, 1);//���������ӽ�����������
	vBox[0]->addWidget(mGroupBox_freTimeDomain, 1);//ʱ��Ƶ�� 
	vBox[1]->addWidget(mGroupBox_shere, 2);//����
	vBox[1]->addWidget(mGroupBox_videoHotMap, 3);//����ͼ
	
	//16:9
	vBox[1]->setStretch(0, 225);
	vBox[1]->setStretch(1, 375);

	//plot_timefreqDomain_box->addWidget(mGroupBox_XYPos,1);//�ı�������
	plot_timefreqDomain_box->addWidget(m_freTime_general);//,4
	//plot_videoDomain_box->addWidget(plot_heatmap);
	plot_videoDomain_box->addWidget(this->_pView_hotMapAndVideoDisplay);

	//���ñ߾�
	plot_timefreqDomain_box->setMargin(0);
	plot_timefreqDomain_box->setContentsMargins(0, 0, 0, 0);
	mGroupBox_freTimeDomain->setContentsMargins(0, 0, 0, 0);
	vBox[0]->setMargin(0);
	vBox[0]->setContentsMargins(0, 0, 0, 0);

	////test
	//qDebug() << plot_timefreqDomain_box->contentsMargins();
	//qDebug() << plot_videoDomain_box->contentsMargins();
	//qDebug() << mGroupBox_freTimeDomain->contentsMargins();
	this->setLayout(hbox);


	//����FFT QThread obj
	fft_thread = new QThread;
	timer_thread_1 = new QTimer;
	//FFT ģ�任
	fft_change = new FFT_change;

	//����Camera QThread obj
	camera_thread = new QThread;
	timer_thread_2 = new QTimer;

	////opencv ��Ƶ��  ʹ��IMX291����ͷ
	//m_model_videoDevice = new Model_VideoDevice;
	//m_model_videoDevice->moveToThread(camera_thread);
	//camera_thread->start();

	//Ƶ�����
	//_pCalibrationDll = new CalibrationDll("I:/WORK/06_Code/test_customPlot_hreatMap_SphereAndHM_V2.9.6/lib/CalibrationDll/rsp.txt", this);//һ������
	//_pCalibrationDll = new CalibrationDll(":/MainViewControl/lib/CalibrationDll/rsp.txt", this);//һ������
	_pCalibrationDll = new CalibrationDll(":/MainViewControl/lib/CalibrationDll/rsp_3Step.txt", this);//��������
	
	//��ͨ����
	_pFiltDll = new filtDll(this);

	//myRawDataPack = new RawDataPack;

	//����ͼ��
	creatPlot();

}
void MainViewControl::initConnectedAndUpData()
{
	//���»�ͼ
	connect(&this->timer, &QTimer::timeout, this, &MainViewControl::realDrawplot);
	timer.setInterval(0);//1s�Ӷ�ȡ10�Σ�ÿ��4800��ʵ��4810����ǰ��ȡ��
	//timer.start();

	//�����̣߳�����fft����
	connect(timer_thread_1, &QTimer::timeout, this, &MainViewControl::doFFTData, Qt::DirectConnection);
	timer_thread_1->setInterval(0);
	timer_thread_1->moveToThread(fft_thread);
	fft_thread->start();
	//fft����-->�����ر�
	connect(this, &MainViewControl::on_signal_fftStart, timer_thread_1, [=]() {
		timer_thread_1->start();
		}, Qt::QueuedConnection);
	connect(this, &MainViewControl::on_signal_fftStop,timer_thread_1, &QTimer::stop, Qt::QueuedConnection);

	////test
	//connect(this->thread(), &QThread::finished, fft_thread, &QThread::quit, Qt::DirectConnection);
	//connect(this->thread(), &QThread::finished, camera_thread, &QThread::quit, Qt::DirectConnection);

	////��Ƶ��ʼ��
	//connect(this, &MainViewControl::m_signal_initCamera, m_model_videoDevice, &Model_VideoDevice::init, Qt::QueuedConnection);
	////��Ƶ��ʼ��ʼ��
	//connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_camera, this, &MainViewControl::initCamera);

	//�豸ѡ�񲢿�ʼ ��Ƶ
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_audioStart, this, &MainViewControl::startSound);
	
	//�豸ֹͣ����
	//connect(mView_deviceChoose, &View_DeviceChoose::m_signal_stop, this->pSoundCard, &soundCard::stop);
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_stop, this, &MainViewControl::stopWorkSound);

	//�豸ѡ�񲢿�ʼ ��Ƶ
	connect(_mView_TabWidget_Setting->mView_deviceChoose->m_pushButton[1], &QPushButton::clicked, this, &MainViewControl::initCamera);
	
	//�豸����ѡ��
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_captureType, this, &MainViewControl::m_solt_chooseCaptureType);
	
	//¼��
	//connect(mView_deviceChoose, &View_DeviceChoose::m_signak_recording, this, &MainViewControl::m_solt_record);
	//connect(_mView_TabWidget_Setting->m_View_AudioRecord, &View_AudioRecord::m_signak_recording, this, &MainViewControl::m_solt_record);
	connect(this->_mView_TabWidget_Setting, &View_TabWidget_Setting::m_signal_recordding, this, &MainViewControl::m_solt_record);
	//connect(this->_mView_TabWidget_Setting->m_View_AudioRecord, &View_AudioRecord::m_signal_recordding, this, &View_TabWidget_Setting::m_signal_recordding);

	//¼������ ����wav�ļ�
	connect(m_timer_record, &QTimer::timeout, m_Model_AudioRecord, [=]() {
		m_bool_record = false;
	_mView_TabWidget_Setting->m_View_AudioRecord->m_checkBox->setChecked(0);
		m_Model_AudioRecord->m_slot_WavTimeout(_mView_TabWidget_Setting->m_View_AudioRecord->m_lineEdit_address->text());
		}, Qt::QueuedConnection);

	//Audio not foun just start :waring
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_place_reFoundAudioBtn, this, [=]() {QMessageBox::warning(this, QString::fromLocal8Bit("����"), "place found for audio devices"); });

	//��Ƶ�豸����
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_findVideoDevs, this, &MainViewControl::findVideoDevs);

	//ʱƵ����ʾ�������
	//Auto��ť
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->_pPtnAuto, &QPushButton::clicked, this, [=]() {
		//�÷�
		if (this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)
		{
			this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue = 0;
			//��ֵ��ʼ����
			mThresholdVT = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->text().toFloat();
			mThresholdVF = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->text().toFloat();
		}
		else
		{
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue = 1;
		}
		
		//�����Զ���ť���趨ֵ
		this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->setEnabled(!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue);
		this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->setEnabled(!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue);
		});
	//��ͨ�˲�
	//QLineEdit�е�Auto��ťΪ�沢�Ҵ���returnPressed�ź�
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT, &QLineEdit::returnPressed, this, [=]() {
		//��ֵ���
		mThresholdVT = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->text().toFloat();
		});
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF, &QLineEdit::returnPressed, this, [=]() {
		//��ֵ���
		mThresholdVF = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->text().toFloat();
		});

}

void MainViewControl::setCSS()
{
	QPalette mCostomPal;
	mCostomPal.setBrush(QPalette::Background, QBrush(QPixmap(":/MainViewControl/image/gray.png")));

	//����ͼ
	this->setPalette(mCostomPal);
	this->setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.

	//
	_mView_TabWidget_Setting->setPalette(mCostomPal);
	this->_mView_TabWidget_Setting->setAutoFillBackground(true);
	this->_mView_TabWidget_Setting->update();
	
	//ʱƵ�����ñ�����ɫ
	this->m_freTime_general->m_freqencyDomain_general->setBackground(QPixmap::fromImage(QImage(":/MainViewControl/image/gray.png")));
	this->m_freTime_general->m_freqencyDomain_general->update();
	this->m_freTime_general->m_timeDomain_general->setBackground(QPixmap::fromImage(QImage(":/MainViewControl/image/gray.png")));
	this->m_freTime_general->m_timeDomain_general->update();
}

void MainViewControl::openFile(int&& i)
{

	switch (i)
	{
	case 1000:
		QDir::setCurrent(QString::fromLocal8Bit("E:/lvs/04_Qt_test/MainViewControl/file_read"));
		if (p_file == nullptr || x_file == nullptr || y_file == nullptr || z_file == nullptr || res_file == nullptr)
		{
			p_file = new QFile(QString::fromLocal8Bit("p_file.txt"), this);
			x_file = new QFile(QString::fromLocal8Bit("x_file.txt"), this);
			y_file = new QFile(QString::fromLocal8Bit("y_file.txt"), this);
			z_file = new QFile(QString::fromLocal8Bit("z_file.txt"), this);
			res_file = new QFile(QString::fromLocal8Bit("res_file.txt"), this);
		}
		//set IODevic
		if (!p_file->isOpen() || !x_file->isOpen() || !y_file->isOpen() || !z_file->isOpen())
		{

			p_file->open(QIODevice::ReadOnly);
			x_file->open(QIODevice::ReadOnly);
			y_file->open(QIODevice::ReadOnly);
			z_file->open(QIODevice::ReadOnly);

		}
		break;


	case 4800:
		QDir::setCurrent(QString::fromLocal8Bit("E:/lvs/04_Qt_test/MainViewControl/file"));
		if (p_file == nullptr || x_file == nullptr || y_file == nullptr || z_file == nullptr || res_file == nullptr)
		{
			p_file = new QFile(QString::fromLocal8Bit("p_file.txt"), this);
			x_file = new QFile(QString::fromLocal8Bit("x_file.txt"), this);
			y_file = new QFile(QString::fromLocal8Bit("y_file.txt"), this);
			z_file = new QFile(QString::fromLocal8Bit("z_file.txt"), this);
			res_file = new QFile(QString::fromLocal8Bit("res_file.txt"), this);
		}
		//set IODevic
		if (!p_file->isOpen() || !x_file->isOpen() || !y_file->isOpen() || !z_file->isOpen() || !res_file->isOpen())
		{
			p_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
			x_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
			y_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
			z_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
			res_file->open(QIODevice::WriteOnly | QIODevice::Truncate);

		}
		break;
	default:
		break;
	}
	


}

void MainViewControl::readFileData()
{

	QList<QByteArray> list_p_file, list_x_file, list_y_file, list_z_file;
	//list_p_file = p_file->readAll().split('\r\n');
	//list_x_file = x_file->readAll().split('\r\n');
	//list_y_file = y_file->readAll().split('\r\n');
	//list_z_file = z_file->readAll().split('\r\n');
	static int flag = 0;
	if (flag == 0 )
	{
		list_p_file = p_file->readAll().split('\n');
		list_x_file = x_file->readAll().split('\n');
		list_y_file = y_file->readAll().split('\n');
		list_z_file = z_file->readAll().split('\n');
		//qDebug() << "flag:" << flag;
		//qDebug() << "list_p_file.size()" << list_p_file.size();
		
	}
	flag += 1;
	

	//qDebug()<<p_file->readAll().split('\n');




	//    qDebug() << "list_p_file:" << list_p_file;
	//    qDebug() << list_p_file.at(0).toDouble(0);
	for (int i = 0; i < 1000; i++)
	{
		//ȥ��\r
		//list_p_file[i] = list_p_file[i].remove(list_p_file[i].length() - 2, 2);
		//list_x_file[i] = list_x_file[i].remove(list_x_file[i].length() - 2, 2);
		//list_y_file[i] = list_y_file[i].remove(list_y_file[i].length() - 2, 2);
		//list_z_file[i] = list_z_file[i].remove(list_z_file[i].length() - 2, 2);

		p[i] = list_p_file.at(i).toDouble(0);
		//qDebug() << list_p_file.at(i).toDouble(0);
		x[i] = list_x_file.at(i).toDouble(0);
		y[i] = list_y_file.at(i).toDouble(0);
		z[i] = list_z_file.at(i).toDouble(0);
		//qDebug() << "list_p_file.size()" << list_p_file.size();
	}


}

void MainViewControl::invokeDLL()
{

	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		for (int i = 0; i < 4800; i++)
		{
			////x[i] = -x[i];
			////y[i] = -y[i];
			z[i] = -z[i];
			//p[i] = -p[i];//�÷���ni��ok
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		//p�÷�
		for (int i = 0; i < 4800; i++)
		{
			//x[i] = -x[i];
			y[i] = -y[i];
			//z[i] = -z[i];
			//p[i] = -p[i];//�÷���ni��ok
		}
		break;
	default:
		break;
	}
	//��һ�汾��rsp.txt
	//Ƶ��У׼
	//_pCalibrationDll->invokeDllAndWrite(x, y, z);
	//ͨ������
	//if (isFilt)
	//	_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);

	//�ڶ��汾��rsp���˲�-У׼-�˲�
	//ͨ������
	if (isFilt)
		this->_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);
	//Ƶ��У׼
	this->_pCalibrationDll->invokeDllAndWrite(x, y, z);
	//ͨ������
	if (isFilt)
		this->_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);


	//�㷨
	//direction_spl(p, x, y, z, P_1D); //V_2.9.1
	direction_spl(p, x, y, z, P_1D,&P_1D_MAX_lvs); //V_2.9.1  P_1D_MAX_lvs��P_1D�����ֵ δ˫��˫����ֵ
	//direction_spl(p, x, y, z, P_1D, &P_1D_MAX_lvs); //V_2.9.1  P_1D_MAX_lvs��P_1D�����ֵ	˫��˫����ֵ

	//�������߳������ж��Ƿ���ʾhm
	emit this->on_signal_P1_DMaxValue(P_1D_MAX_lvs);

	//���浽CVS
	//QFile turnData(QString("CVSData_P1_D_true.csv"));
	//if (!turnData.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//{
	//	qDebug() << "file not open!";
	//	return;
	//}

	//QTextStream stream(&turnData);

	//for (int yIndex = 0; yIndex < 180; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
	//{
	//	for (int xIndex = 0; xIndex < 360; ++xIndex)//�ü��ڼ���
	//	{


	//		//this->_pView_hotMapAndVideoDisplay->colorMap->data()->setCell(xIndex, yIndex, data[xIndex + yIndex * 360 * 6]);
	//		stream << this->P_1D[xIndex + yIndex * 360];
	//		if ((xIndex + yIndex * 360) == 360 - 1 + yIndex * 360)
	//		{
	//			stream << "\n";
	//		}
	//		else
	//		{
	//			stream << ",";
	//		}

	//	}
	//}
	//turnData.close();


	//����ͼ���Ĵ�С�ı�
	//changeHotMapcoefficient(P_1D_MAX_lvs);

	////��ֵ��������6*6
	///**
	// * 	extern void interp_imagesc_ver01(const double S[64800], double dx, double dy,
	//	double interp_multiple,
	//	double Fine_S[2332800]);
	// */
	//memset(this->Fine_S, 0, 2332800 * sizeof(double));
	//interp_imagesc_ver01(P_1D, 1, 1, 6, this->Fine_S);

	//qDebug() << "this->Fine_S[2332799]:" << this->Fine_S[2332799];

	////test д��csv
	//P_1DCSV.setFileName("P_1DData_2_0m.csv");
	//if (!P_1DCSV.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//	qDebug()<<"not open P_1DCSV";
	//QTextStream P_1DCSVFile1(&P_1DCSV);

	//for (int i = 0; i < 180; i++)//��
	//{
	//	for (int j = 0; j < 360; j++)//��
	//	{
	//		P_1DCSVFile1 << QString::number(P_1D[j + i * 360], 'f', 12).toStdString().c_str();
	//		if (j < 359)P_1DCSVFile1 << ",";
	//	}
	//	P_1DCSVFile1 << "\n";

	//}
	//P_1DCSV.close();
	
	//music �㷨
	//direction_music(p, x, y, z, P_1D);

	//���� p x y z���ݣ����ڽ���������ʱ��y�����ã������Ƶ�����Ӱ��
	//memset(p_copy, 0, sizeof(double) * 4800);
	memcpy(p_copy, p, sizeof(double) * 4800);
	//memset(x_copy, 0, sizeof(double) * 4800);
	memcpy(x_copy, x, sizeof(double) * 4800);
	//memset(y_copy, 0, sizeof(double) * 4800);
	memcpy(y_copy, y, sizeof(double) * 4800);
	//memset(z_copy, 0, sizeof(double) * 4800);
	memcpy(z_copy, z, sizeof(double) * 4800);

	//���㷽λ��������ʾ
	//�洢�����е����ֵ
	static double max_;
	max_ = P_1D[0];

	//qDebug() << "current_start:" << QDateTime::currentMSecsSinceEpoch();
	for (int i = 0;i< 64800;i++)
	{	
		if (max_ < P_1D[i])
		{
			max_ = P_1D[i];
			//qDebug() << "i % (180.0 * 6.0) + 1:" << i % (360 * 6) + 1;
			maxp = QPair<int, int>(i / (360) + 1,i % (360) + 1);
		}
	}
}

//��Ϊ�Ķ��ɼ��߳��еĵ�����
void MainViewControl::read()
{
	static QList<double> rawData;

	rawData.clear();

	memset(this->p, 0, sizeof(double) * SAMPLE);
	memset(this->x, 0, sizeof(double) * SAMPLE);
	memset(this->y, 0, sizeof(double) * SAMPLE);
	memset(this->z, 0, sizeof(double) * SAMPLE);

	if (this->_pMainModel->_pModel_AudioCollector->mRawData.isEmpty())
		return;

	//��ȡ����
	{
		QMutexLocker mLocker(&this->_pMainModel->_pModel_AudioCollector->mMutex);

		 //qDebug() << sizeof(RawDataPack);
		 //memcpy(this->myRawDataPack, (void*) & this->_pMainModel->_pModel_AudioCollector->mRawData.takeFirst(), sizeof(RawDataPack));

		memcpy(this->p, this->_pMainModel->_pModel_AudioCollector->mRawData.first().pData, sizeof(double) * SAMPLE);
		memcpy(this->x, this->_pMainModel->_pModel_AudioCollector->mRawData.first().xData, sizeof(double) * SAMPLE);
		memcpy(this->y, this->_pMainModel->_pModel_AudioCollector->mRawData.first().yData, sizeof(double) * SAMPLE);
		memcpy(this->z, this->_pMainModel->_pModel_AudioCollector->mRawData.first().zData, sizeof(double) * SAMPLE);
		if(!this->_pMainModel->_pModel_AudioCollector->mRawData.isEmpty())
			this->_pMainModel->_pModel_AudioCollector->mRawData.removeFirst();

	}



	//ת��Ƶ�ļ�
	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		//�Ķ�һ֡����
		//_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->read();





		//��������һ��ѭ��
					//ת����Ƶraw
		if (m_bool_record)
		{
			for (int i = 0; i < SAMPLE; i++)//��4810*4���� Pͨ������9.6==10����
			{
			//p[i] = _mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i];
			//x[i] = _mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 1 + 40];
			//y[i] = _mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 2 + 40];
			//z[i] = _mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->pRawData[4 * i + 3 + 40];


				rawData.append(p[i]);
				rawData.append(x[i]);
				rawData.append(y[i]);
				rawData.append(z[i]);

				this->m_Model_AudioRecord->addRawData(rawData);
				//¼��ʱ�� ��ʾ
				_mView_TabWidget_Setting->m_View_AudioRecord->m_label_rt->setText(QString("%1").arg(QString::fromLocal8Bit("SoundCardģʽ")));
			}
		}


		break;
	case CAPTURETYPE::NICaptureCard:

		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
			return;

		//�Ķ�һ֡����
		//_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->read();

					//ת����Ƶraw
		if (m_bool_record)
		{

			{

				QMutexLocker m_lock(&_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->m_lock_pRawData);
				for (int i = 0; i < SAMPLE; i++)//��4810*4����	Pͨ������9.6==10����
				{
					////����ȡֵ�����ǽ���ȡֵ
					//p[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 10];
					//x[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 1 * (SAMPLE + 10)];//(FS + 10)
					//y[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 2 * (SAMPLE + 10)];
					//z[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 3 * (SAMPLE + 10)];

					////����ȡֵ
					//p1[i] = this->mRawData[i*4];
					//x1[i] = this->mRawData[i*4 + 1];
					//y1[i] = this->mRawData[i*4 + 2];
					//z1[i] = this->mRawData[i*4 + 3];


					rawData.append(p[i]);
					rawData.append(x[i]);
					rawData.append(y[i]);
					rawData.append(z[i]);

					this->m_Model_AudioRecord->addRawData(rawData);

					//¼��ʱ�� ��ʾ
					_mView_TabWidget_Setting->m_View_AudioRecord->m_label_rt->setText(QString("%1").arg(QString::fromLocal8Bit("NIģʽ")));
				}
			}

		}
		break;
	default:
		break;
	}



}

void MainViewControl::creatPlot()
{
	//// configure axis rect:���������
	//plot_heatmap->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // ��Ҳ������ͨ����ק/���ų߶ȸı���ɫ��Χ
	//plot_heatmap->axisRect()->setupFullAxesBox(true);
	//plot_heatmap->xAxis->setLabel("x");
	//plot_heatmap->yAxis->setLabel("y");
	//plot_heatmap->xAxis->setVisible(false);
	//plot_heatmap->yAxis->setVisible(false);
	//plot_heatmap->xAxis2->setVisible(false);
	//plot_heatmap->yAxis2->setVisible(false);

	


	//// set up the QCPColorMap:
	//colorMap = new QCPColorMap(plot_heatmap->xAxis, plot_heatmap->yAxis);
	//colorMap->setAntialiased(true);

	////colorMap->data()->setSize(nx, ny); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	////colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ

	////��ʼ�� ���ڴ��趨����ֵ
	//switch (mCaptrueType)
	//{
	//case CAPTURETYPE::SoundCard:
	//	colorMap->data()->setSize(90, 60); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ
	//	//colorMap->data()->setSize(360, 180); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	//colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ

	//	break;
	//case CAPTURETYPE::NICaptureCard:
	//	//colorMap->data()->setSize(90,60); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	colorMap->data()->setSize(this->mXAxisCutOut * 2 * 6, this->mYAxisCutOut * 2 * 6); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ
	//	break;
	//default:
	//	break;
	//}


}

void MainViewControl::changePlot()
{
	//switch (mCaptrueType)
	//{
	//case CAPTURETYPE::SoundCard:
	//	//Intel D455
	//	colorMap->data()->setSize(90, 60); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ
	//	//colorMap->data()->setSize(360, 180); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	//colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ

	//	break;
	//case CAPTURETYPE::NICaptureCard:
	//	//if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
	//	//	return;
	//	
	//	//Intel D455
	//	//colorMap->data()->setSize(90, 60); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	//colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ
	//	
	//	//IMX291
	//	//manual
	//	//colorMap->data()->setSize(this->mXAxisCutOut * 2 * 6, this->mYAxisCutOut * 2 * 6); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�
	//	//colorMap->data()->setRange(QCPRange(0, 360), QCPRange(0, 180)); // ���ڼ���x����ֵ��y��ά�Ͽ�Խ���귶Χ

	//	//��������
	//	//colorMap->data()->setSize(360*6,180*6); // ����ϣ����ɫ��ͼ��nx*ny�����ݵ�

	//	break;
	//default:
	//	break;
	//}
}

void MainViewControl::realDraw_plot_heatmap()
{
	//qDebug() << "1��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351202 877
	//IMX291���  ���ñ���
	{
		//QMutexLocker mLocker(&this->_pMainModel->m_model_videoDevice->m_mutex_frame);
		//if (this->_pMainModel->m_model_videoDevice->frame.empty())return;
		//plot_heatmap->setBackground(QPixmap::fromImage(CVMat2QImage::QCVMat2QImage(this->_pMainModel->m_model_videoDevice->frame)), true, Qt::KeepAspectRatio);

	}
	//qDebug() << "2��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		//nx:65;ny:90;
		for (int xIndex = 45; xIndex < 135; ++xIndex)//x��  ��Ӧ�� 90
		{
			for (int yIndex = 45; yIndex < 105; ++yIndex) //y�� ��Ӧ��  60
			{
				//colorMap->data()->setCell(xIndex - 45, yIndex - 45, P_1D[xIndex + yIndex * 360]);
			}
		}
		break;
	case CAPTURETYPE::NICaptureCard:

		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
			return;


		/**
		 * IMX291Ӳ�� X:80;Y:70
		 */

		//manual		
		/********************XAxis����****************************************************/
		/* �� +70�����Һ� -55������ �㷨�����⣬������60��
		*���Ҫ�ü�0-180����ʵ��Ӧ�òü� +65��-> 0���0 -> -55���ƴ�գ���120��
		*��
		* ʵ��Ӧ�òü� +60��-> 0���0 -> -60���ƴ�գ���120�㼴 0->60��+ 360->300�㡣�ԳƵ�Ϊ0�㣬��벿��������0->60,�Ұ벿��0->60���ȼ�300��=>300->360�ȣ���ת�ü�360->300
		*/
		/****************************YAxis����********************************************/
		/**
		* �ɲü�0->180���ֶ����ڣ��ԳƵ�Ϊ90��������90��
		 * 
		 ************************************************************************** /
		/**************************************************************************/
		 /**old*/
		//for (int xIndex = 90 - this->mXAxisCutOut; xIndex < 90 + this->mXAxisCutOut; ++xIndex)//x��  ��Ӧ�� 100  max:360
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //y�� ��Ӧ��  80	max:180
		//	{
		//		colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//}
		/**
		 * new
		 */
		//for (int xIndex = 90 - this->mXAxisCutOut; xIndex < 90 + this->mXAxisCutOut; ++xIndex)//x��  ��Ӧ�� 100  max:360
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //y�� ��Ӧ��  80	max:180
		//	{
		//		colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//	 
		//	////��벿 ����һ�� ���綼�ü�30��������Ӧ��60*60 ��X�᣺��벿��0->30���Ұ벿��30->60��abs(0 - this->mYAxisCutOut) - yIndex ����Ϊ��[0��30��;
		//	//for (int yIndex = abs(0 - this->mYAxisCutOut) -1 ; yIndex >= 0; --yIndex) //y�� ��Ӧ��  80	max:180
		//	//{
		//	//	colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), abs(0 - this->mYAxisCutOut) - yIndex , P_1D[xIndex + yIndex * 360]);
		//	//}


		//	////�Ұ벿	359 - yIndex������Ϊ��[30,60);
		//	//for (int yIndex = 359 - this->mYAxisCutOut; yIndex > 359 - this->mYAxisCutOut; --yIndex) //y�� ��Ӧ��  80	max:180
		//	//{
		//	//	colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), 359 - yIndex , P_1D[xIndex + yIndex * 360]);
		//	//}
		//}

		////Ŀ��ü���0-30 ��벿��
		//for (int xIndex = 0; xIndex < this->mXAxisCutOut; ++xIndex)//�ü��ڼ���
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
		//	{
		//		colorMap->data()->setCell(xIndex + this->mXAxisCutOut, yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//}
		////Ŀ��ü���330-360 ǰ�벿��
		//for (int xIndex = 360 - this->mXAxisCutOut; xIndex < 360; ++xIndex)//�ü��ڼ���
		//{
//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
//	{
//		colorMap->data()->setCell(xIndex - (360 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
//	}
//}

////test ��ֵ
////Ŀ��ü���0-30 ��Ϊ��벿��
//for (int xIndex = 0 * 6; xIndex < this->mXAxisCutOut; ++xIndex)//�ü��ڼ��� ��������ͬʱ����6��
//{
//	for (int yIndex = (90*6 - this->mYAxisCutOut); yIndex < (90*6 + this->mYAxisCutOut); ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ ��������ͬʱ����6��
//	{
//		colorMap->data()->setCell(xIndex + this->mXAxisCutOut * 6, yIndex - (90 - this->mYAxisCutOut)*6, this->Fine_S[xIndex + yIndex * 360 * 6]);
//		//colorMap->data()->setCell(xIndex + this->mXAxisCutOut, yIndex - (90*6 - this->mYAxisCutOut), this->Fine_S[yIndex + xIndex * 180 * 6]);
//	}
//}
////Ŀ��ü���330-360 ��Ϊǰ�벿��
//for (int xIndex = (360 - this->mXAxisCutOut) * 6; xIndex < 360 * 6; ++xIndex)//�ü��ڼ���
//{
//	for (int yIndex = (90 - this->mYAxisCutOut) * 6; yIndex < (90 + this->mYAxisCutOut) * 6; ++yIndex) //�ü��ڼ��� �ü����У��У�ȷ��������ֵ
//	{
//		colorMap->data()->setCell(xIndex - (360 - this->mXAxisCutOut) * 6, yIndex - (90 - this->mYAxisCutOut) * 6, this->Fine_S[xIndex + yIndex * 360 * 6]);
//	}
//}

////���ȫ������  ����ƴ�ӻ�ԭҲӦ���л�ԭ
//for (int xIndex = 0; xIndex < 360 * 6; ++xIndex)//�� ȫ����360*6��
//{
//	for (int yIndex = 0; yIndex < 180 * 6; ++yIndex) //�� ȫ����180*6��
//	{
//		
//		//colorMap->data()->setCell(xIndex, yIndex, this->Fine_S[xIndex + yIndex * 360 * 6]);
//		//qDebug() << xIndex * yIndex + yIndex << " " << QString::fromLocal8Bit("%1").arg(this->Fine_S[xIndex * yIndex + yIndex]);
//	}
//}


break;
	default:
		break;
	}
	//qDebug() << "3��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

	////// ���ɫ��:
	////QCPColorScale* colorScale = new QCPColorScale(this);
	////this->plotLayout()->addElement(0, 1, colorScale); // ������ӵ�������ε��Ҳ�
	////colorScale->setType(QCPAxis::atRight); // �̶�ӦΪ��ֱ�����̶���/�������ǩ�Ҳࣨʵ���ϣ��Ҳ��Ѿ���Ĭ��ֵ��
	////colorMap->setColorScale(colorScale); // ����ɫͼ��ɫ�����
	////colorScale->axis()->setLabel("Magnetic Field Strength");


	////////���ϵ���ı��С
	////static QCPColorGradient cpColorGradient; 
	////cpColorGradient.clearColorStops();
	//////cpColorGradient.setPeriodic()//������
	//////cpColorGradient.setLevelCount(360);//Ĭ��ֵ350
	////cpColorGradient.setColorStopAt(0, QColor(Qt::transparent));//͸��
	////cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 2.0f - 0.001, QColor(Qt::transparent));//͸��
	//////�ռ�ȣ�0.02��0.03��0.01��0.01 ��6��
	////
	//////cpColorGradient.setColorStopAt(posColor[0] + this->mHmSizeFactor * 2.0f, hotMapCoefficient > 0 ? QColor(115, 115, 255, 150) : QColor(Qt::transparent));//������ɫ
	////cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 3.0f, hotMapCoefficient > 0 ? QColor(77, 255, 255, 150) : QColor(Qt::transparent));//����ɫ
	////cpColorGradient.setColorStopAt(posColor[2] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 255, 0, 150) : QColor(Qt::transparent));//����ɫ
	////cpColorGradient.setColorStopAt(posColor[3] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 71, 26, 150) : QColor(Qt::transparent));//����ɫ
	////cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(102, 20, 0, 150) : QColor(Qt::transparent));//���ɫ
	////
	//////cpColorGradient.setColorStops()
	////cpColorGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciHSV);//ciHSV
	////colorMap->setGradient(cpColorGradient);

	////QColor(1, 0, 0.196078, 1);
	////qDebug() << colorMap->gradient().levelCount();//350
	////qDebug() << colorMap->gradient().colorStops();//QMap((0, QColor(ARGB 1, 0, 0, 0.392157))(0.15, QColor(ARGB 1, 0, 0.196078, 1))(0.35, QColor(ARGB 1, 0, 1, 1))(0.65, QColor(ARGB 1, 1, 1, 0))(0.85, QColor(ARGB 1, 1, 0.117647, 0))(1, QColor(ARGB 1, 0.392157, 0, 0)))
	////qDebug() << colorMap->gradient().colorInterpolation();//��ɫ��ֵ 0 ciRGB
	////qDebug() << colorMap->gradient().periodic();//bool ���ڷ� false

	////test
	//colorMap->setGradient(QCPColorGradient::gpHot);

	//// ���ǻ����Դ���һ��QCPColorGradientʵ��������������Լ�����ɫ
	//// ���䣬�����QCPColorGradient���ĵ��Ի�ȡ���ܵ�Ч��.

	//// ������������ά�ȣ���ɫ������ʹ�������ݵ㶼λ����ɫ������ʾ�ķ�Χ��:
	//colorMap->rescaleDataRange(true);

	//////ȷ����rect��ɫ��ͬ����ױ߾�Ͷ��߾ࣨ�Ա����Ƕ��룩:
	////QCPMarginGroup* marginGroup = new QCPMarginGroup(this);
	////this->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
	////colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

	//// �������ż���x����ֵ��y���ᣬ�Ա���Կ���������ɫͼ��
	//plot_heatmap->rescaleAxes();

	//plot_heatmap->replot();
	//qDebug() << "4��";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840

	//����P1_D�����߳���Ⱦ
	//_pP1_DData
	{
		QMutexLocker mMuter(&this->_pMainModel->_pModel_hotMapAndVideoDisplay->mMutex);

		memcpy(this->_pMainModel->_pModel_hotMapAndVideoDisplay->_pP1_DData, this->P_1D, 64800 * sizeof(double));
		//for (int i = 0; i < 64800 * 4; i++)
		//{
		//	//qDebug() << this->P_1D[i];
		//	qDebug()<< i<<":" << this->_pMainModel->_pModel_hotMapAndVideoDisplay->_pP1_DData[i];
		//}
	}
}

void MainViewControl::realDraw_plot_timeDomain()
{


	//����ʱ������ QVector<double>����
	static QVector<double> xData_p, yData_p, xData_x, yData_x, xData_y, yData_y, xData_z, yData_z;



	//��������
	xData_p.clear();
	yData_p.clear();
	xData_x.clear();
	yData_x.clear();
	xData_y.clear();
	yData_y.clear();
	xData_z.clear();
	yData_z.clear();

	////�ж�p x y z ����Ϊ0�����
	//if (this->p[0] == 0 || this->x[0] == 0 || this->y[0] == 0 || this->z[0] == 0)
	//	return;

	for (int i = 0; i < 4800; i++)
	{
		xData_p.append(i);
		yData_p.append(p[i]);
		//xData_x.append(i);
		yData_x.append(x[i]);
		//xData_y.append(i);
		yData_y.append(y[i]);
		//xData_z.append(i);
		yData_z.append(z[i]);
	}
	for (int i = 0; i < 4800 * 3; i++)
	{
		xData_x.append(i);
	}
	//ʱ������
	if (!mStructTS.mTimeData_P.isEmpty())
	{
		//�Ƴ���һ��4800���ݵ�
		mStructTS.mTimeData_P.removeFirst();
		mStructTS.mSequence_P.removeFirst();
		mStructTS.mTimeData_X.removeFirst();
		mStructTS.mSequence_X.removeFirst();
		mStructTS.mTimeData_Y.removeFirst();
		mStructTS.mSequence_Y.removeFirst();
		mStructTS.mTimeData_Z.removeFirst();
		mStructTS.mSequence_Z.removeFirst();

	}

	//����µ�4800���ݵ�	
	mStructTS.mTimeData_P.append(yData_p);
	mStructTS.mSequence_P.append(xData_p);
	mStructTS.mTimeData_X.append(yData_x);
	mStructTS.mSequence_X.append(xData_x);
	mStructTS.mTimeData_Y.append(yData_y);
	mStructTS.mSequence_Y.append(xData_y);
	mStructTS.mTimeData_Z.append(yData_z);
	mStructTS.mSequence_Z.append(xData_z);

	//����ʱ������ �ɴ�ʱ��������ȡ����ֵ��ÿ��ֵ����pxyz��������
	if (mTimeSequence.size() > 2)
	{
		mTimeSequence.dequeue();
	}
	mTimeSequence.append(mStructTS);

	//ȡ���ֵ //Ĭ���������ǻ��ƻ�ԭ����
	std::sort(p_copy, p_copy + 4800);
	std::sort(x_copy, x_copy + 4800);
	std::sort(y_copy, y_copy + 4800);
	std::sort(z_copy, z_copy + 4800);

	if (mTimeSequence.size() < 3)
		return;

	//ȡһ��ֵ
	_mStructTS[0] = mTimeSequence.at(0);
	_mStructTS[1] = mTimeSequence.at(1);
	_mStructTS[2] = mTimeSequence.at(2);

	switch (m_freTime_general->_pView_ValueOfTFSetting->m_comBox_chooseDisplay->currentIndex())
	{
	case MYSTRUCT::PXYZ::pxyz::P://0
		////����y�᷶Χ
		//m_freTime_general->m_timeDomain_general->yAxis->setRange(p_copy[0], p_copy[4799]);
		////��������
		//m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_p, yData_p);
		////��ǩ
		//m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));

		//����y�᷶Χ
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//�Զ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(p_copy[0], p_copy[4799]);
		}

		//��������
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_P.at(0) + _mStructTS[1].mTimeData_P.at(0) + _mStructTS[2].mTimeData_P.at(0));
		//��ǩ
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));
		break;
	case MYSTRUCT::PXYZ::pxyz::X:
		//����y�᷶Χ
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//�Զ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(x_copy[0], x_copy[4799]);
		}

		//��������
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_X.at(0) + _mStructTS[1].mTimeData_X.at(0) + _mStructTS[2].mTimeData_X.at(0));
		//��ǩ
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("X-TimeDomain"));

		break;
	case MYSTRUCT::PXYZ::pxyz::Y:
		//����y�᷶Χ
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//�Զ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(y_copy[0], y_copy[4799]);
		}

		//��������
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_Y.at(0) + _mStructTS[1].mTimeData_Y.at(0) + _mStructTS[2].mTimeData_Y.at(0));
		//��ǩ
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("Y-TimeDomain"));
		break;
	case MYSTRUCT::PXYZ::pxyz::Z:
		//����y�᷶Χ
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//�Զ�����ֵ
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(z_copy[0], z_copy[4799]);
		}

		//��������
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_Z.at(0) + _mStructTS[1].mTimeData_Z.at(0) + _mStructTS[2].mTimeData_Z.at(0));
		//��ǩ
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("Z-TimeDomain"));
		break;
	default:
		break;
	}

	//�ػ�
	m_freTime_general->m_timeDomain_general->replot();

}

void MainViewControl::realDraw_plot_freqDomain()
{
	//ֻ��y��������ֵ,x������ݵ���doFFTData()�д�����
	if (this->thread()->isRunning() && fft_thread->isRunning() )//&& isFftDown
	{
		QMutexLocker m_dataCopyLocker(&m_dataCopy);

		switch (m_freTime_general->_pView_ValueOfTFSetting->m_comBox_chooseDisplay->currentIndex())
		{
		case MYSTRUCT::PXYZ::pxyz::P://0
			//����y�᷶Χ
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//�Զ�����ֵ
			{
				//����
				std::sort(p_fre_copy, p_fre_copy + SAMPLE / 2);
				//ͨ��
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(p_fre_copy, SAMPLE / 2) + 0.2);
			}
			//��ǩ
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-FFT"));			//��ǩ
			//m_freTime_general->m_spectrum_general->graph(0)->setName(QString::fromLocal8Bit("P-SPECTRUM"));
			break;
		case MYSTRUCT::PXYZ::pxyz::X:
			//����y�᷶Χ
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//�Զ�����ֵ
			{
				//����
				std::sort(x_fre_copy, x_fre_copy + SAMPLE / 2);
				//ͨ��
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(x_fre_copy, SAMPLE / 2) + 0.2);
			}

			//��ǩ
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("X-FFT"));
			break;
		case MYSTRUCT::PXYZ::pxyz::Y:
			//����y�᷶Χ
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//�Զ�����ֵ
			{
				//����
				std::sort(y_fre_copy, y_fre_copy + SAMPLE / 2);
				//ͨ��
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(y_fre_copy, SAMPLE / 2) + 0.2);
			}

			//��ǩ
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("Y-FFT"));
			break;
		case MYSTRUCT::PXYZ::pxyz::Z:
			//����y�᷶Χ
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//�����ֶ�����ֵ
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//�Զ�����ֵ
			{
				//����
				std::sort(z_fre_copy, z_fre_copy + SAMPLE / 2);
				//ͨ��
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(z_fre_copy, SAMPLE / 2) + 0.2);
			}

			//��ǩ
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("Z-FFT"));
			break;
		default:
			break;
		}
		//Ƶ���ػ�
		m_freTime_general->m_freqencyDomain_general->replot();
		//����ͼ�ػ�
		m_freTime_general->m_spectrum_general->replot();
	}

}

void MainViewControl::realDraw_plot_eleAngle()
{
	static QVector<double> y1_value,x1_value;
	y1_value.clear();
	x1_value.clear();

	//static double myTimer = QDateTime::currentSecsSinceEpoch();
	//myTimer = QDateTime::currentSecsSinceEpoch();

	////����X�����귶Χ
	//plot_eleAngle->xAxis->setRange(myTimer-60, myTimer);

	////����Y�����귶Χ
	//plot_eleAngle->yAxis->setRange(0, 180);

	////����x������
	//plot_eleAngle->graph(0)->addData(myTimer, maxp.first);
	////plot_eleAngle->graph(0)->re

	////����ͼ����ߵ���͵��Զ�����������
	////plot_eleAngle->graph(0)->rescaleAxes(true);

	//plot_eleAngle->replot();

	//���� m_modelֵ
	//maxp.second / 6: 60
	//180 - maxp.first: 12
	//qDebug() << "maxp.second:" << maxp.second;//maxp.second / 6: 360
	//qDebug() << "180 - maxp.first:" << 180 - maxp.first;//180 - maxp.first: 17
	m_shereLocalation->m_soundLocation.setX(maxp.second);//����
	m_shereLocalation->m_soundLocation.setY(180 - maxp.first);//����

	////����labelֵ
	//xylable[1]->setText(QString::number(maxp.first));//180��
	//xylable[3]->setText(QString::number(maxp.second));//360��
}

void MainViewControl::realDraw_plot_azimAngle()
{
	static QVector<double> y2_value, x2_value;
	y2_value.clear();
	x2_value.clear();

	static double myTimer2 = QDateTime::currentSecsSinceEpoch();
	myTimer2 = QDateTime::currentSecsSinceEpoch();

	////����X�����귶Χ
	//plot_azimAngle->xAxis->setRange(myTimer2 - 60, myTimer2);

	////����Y�����귶Χ
	//plot_azimAngle->yAxis->setRange(0, 360);

	////����x������
	//plot_azimAngle->graph(0)->addData(myTimer2, maxp.second);

	////����ͼ����ߵ���͵��Զ�����������
	////plot_azimAngle->graph(0)->rescaleAxes(true);

	//plot_azimAngle->replot();
}
void MainViewControl::initCamera(int camIndex)
{
	this->_pMainModel->m_model_videoDevice->setDevice(camIndex);
	//emit this->m_signal_initCamera();
	//�Ƿ��ʼ�����
	isInitCamera = 1;

}
//�ı�����ͼ���µ�ϵ��
void MainViewControl::changeHotMapcoefficient(double& coefficient)
{
	//hotMapCoefficient �������������ͼ

	if (coefficient < 0.0000007)	//������ͼ
	{
		hotMapCoefficient = 0; 
	} 
	else if (coefficient > 0.0000007 && coefficient < 0.0000391965)//1��  0.93*1= 0.93  ��С;0.98*1 = 0.98;0.99*1 = 0.99
	{				
		hotMapCoefficient = 1;//1 - 0.97 = 0.3; 0.3/5 = 0.06

		posColor[0] = 0.93;
		posColor[1] = 0.95;
		posColor[2] = 0.98;//����ɫ
		posColor[3] = 0.99;//��ɫ
		posColor[4] = 1;//С��ɫ
	}
	else if (coefficient > 0.0000391965 && coefficient < 0.0000671363)
	{
		hotMapCoefficient = 1;//1 - 0.87 = 0.13;0.13/5 = 0.0126

		posColor[0] = 0.93;
		posColor[1] = 0.95;
		posColor[2] = 0.97;
		posColor[3] = 0.98;
		posColor[4] = 1;
	}
	else if (coefficient > 0.0000671363 && coefficient < 0.000190339)//
	{
		hotMapCoefficient = 1;//1 - 0.77 = 0.23;0.23/5 = 0.046

		posColor[0] = 0.93;
		posColor[1] = 0.95;
		posColor[2] = 0.96;
		posColor[3] = 0.97;
		posColor[4] = 1;
	}
	else if (coefficient > 0.000190339 && coefficient < 0.000393498)//
	{
		hotMapCoefficient = 1;//1 - 0.67 = 0.33;0.33/5 = 0.066

		posColor[0] = 0.93;
		posColor[1] = 0.94;
		posColor[2] = 0.95;
		posColor[3] = 0.96;
		posColor[4] = 1;
	}
	else if (coefficient > 0.000393498 && coefficient < 0.00276308)//
	{
		hotMapCoefficient = 1;//1 - 0.57 = 0.43;0.43/5 = 0.086

		posColor[0] = 0.93;
		posColor[1] = 0.945;
		posColor[2] = 0.95;
		posColor[3] = 0.95;
		posColor[4] = 1;
	}
}
//��ʼ������ͼ����
void MainViewControl::initSpectrumSequence()
{
	QVector <double> _temVec;
	for (int i = 0; i < 1000; i++)
	{
		_temVec.append(0);
	}

	for (int i = 0; i < 500; i++)
	{
		
		this->mSpectrumSequence_P.append(_temVec);
		this->mSpectrumSequence_X.append(_temVec);
		this->mSpectrumSequence_Y.append(_temVec);
		this->mSpectrumSequence_Z.append(_temVec);
	}
		
};
//void MainViewControl::initCamera()
//{
//	rs2::context          ctx;
//	rs2::config           cfg;
//	rs2::sensor           sen;
//	rs2::pipeline_profile profile;
//	auto list = ctx.query_devices(); 
//
//
//
//	if (list.size() <= 0) {
//		//exit(0);
//		//throw std::runtime_error("No device detected. Is it plugged in?");
//		//�Ƿ��ʼ�����
//		isInitCamera = 0;
//
//		QMessageBox::warning(this, "Warning!", "No RS device detected. Is it plugged in?");
//		QApplication::quit();
//		return;
//	}
//	rs2::device dev = list.front();
//
//	//������б������Ϣ
//	_mView_TabWidget_Setting->mView_deviceChoose->m_comBox[1]->addItem(dev.get_info(RS2_CAMERA_INFO_NAME));
//
//	//rs2::device rs_dev = profile.get_device();
//	//qDebug() << "Device Name" << ": " << rs_dev.get_info(RS2_CAMERA_INFO_NAME);
//	//qDebug() << "Firmware Version" << ": " << rs_dev.get_info(RS2_CAMERA_INFO_FIRMWARE_VERSION);
//	//qDebug() << "Serial Number" << ": " << rs_dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
//	//qDebug() << "Product Id" << ": " << rs_dev.get_info(RS2_CAMERA_INFO_PRODUCT_ID);
//
//
//
//	cfg.enable_stream(RS2_STREAM_COLOR, camera_frame_width, camera_frame_height, RS2_FORMAT_BGR8, camera_fps);//����������������
//	//cfg.enable_stream(RS2_STREAM_DEPTH, camera_frame_width, camera_frame_height, RS2_FORMAT_Z16, camera_fps);
//
//	pipe = rs2::pipeline();
//	profile = pipe.start(cfg);
//
//	sen = pipe.get_active_profile().get_device().query_sensors()[1];
//
//
//
//	if (camera_exposure < 0) {
//		sen.set_option(RS2_OPTION_AUTO_EXPOSURE_PRIORITY, true);
//		qDebug() << QString::fromLocal8Bit("�Զ��ع� ����Ϊ[") << sen.get_option(RS2_OPTION_EXPOSURE) << QString::fromLocal8Bit("]");
//	}
//	else {
//		sen.set_option(RS2_OPTION_EXPOSURE, camera_exposure);
//		qDebug() << QString::fromLocal8Bit("�ֶ��ع� ����Ϊ[") << sen.get_option(RS2_OPTION_EXPOSURE) << QString::fromLocal8Bit("]");
//	}
//
//	//auto depth_stream = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
//	auto color_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
//
//	//�Գ�ʼ�����
//	isInitCamera = 1;
//}
void MainViewControl::m_solt_chooseCaptureType(int type)
{
	if (timer.isActive())
	{
		timer.stop();
	}
	switch (type)
	{
	case CAPTURETYPE::SoundCard:
		mCaptrueType = CAPTURETYPE::SoundCard;
		this->_pMainModel->_pModel_AudioCollector->mCaptrueType = this->mCaptrueType;
		break;
	case CAPTURETYPE::NICaptureCard:
		mCaptrueType = CAPTURETYPE::NICaptureCard;
		this->_pMainModel->_pModel_AudioCollector->mCaptrueType = this->mCaptrueType;
		break;
	default:
		break;
	}



	//��������ͼ���ݷ�Χ
	changePlot();

}
//¼��
void MainViewControl::m_solt_record()
{
	//����¼��ʱ��
	m_timer_record->setSingleShot(true);
	m_timer_record->start(_mView_TabWidget_Setting->m_View_AudioRecord->m_lineEdit_rt->text().toInt()*1000);
	m_bool_record = true;



}
void MainViewControl::On_setXAxisCutOutValue(uInt16 value)
{
	//{
	//	QMutexLocker locker(&this->mXAxis_CO_Mutex);
	//	this->mXAxisCutOut = value;
	//}
	//this->changePlot();

	//�����źŸ����߳����÷�Χ
	emit this->_pView_hotMapAndVideoDisplay->on_signal_xAxisRange(value);

}
void MainViewControl::On_setYAxisCutOutValue(uInt16 value)
{
	//{
	//	QMutexLocker locker(&this->mYAxis_CO_Mutex);
	//	this->mYAxisCutOut = value;
	//}
	//this->changePlot();

	//�����źŸ����߳����÷�Χ
	emit this->_pView_hotMapAndVideoDisplay->on_signal_yAxisRange(value);
}

void MainViewControl::On_setHmFactorValue(uInt16 value)
{
	/**��Ҫ����0.94���ɷ�Ϊ��0.95(��С)��0.96��0.97��0.98��0.99�����
	 * valueֵ����Ϊ��50->90 =>/1000 = 0.05->0.09;�������Ϊ0.9
	 */
	this->mHmSizeFactor = (1.0f - (value / 1000.0f + 0.9f)) / 6.0f;

	//this->mHmSizeFactor = 1.05;
	qDebug() << "this->mHmSizeFactor:" << this->mHmSizeFactor;
}

//���������еĴ�С
double MainViewControl::MAX_VALUE(double *value, double len)
{
	static double MAX = NULL;
	MAX = NULL;

	for (size_t i = 0; i < len; i++)
	{
		if (MAX< value[i])
		{
			MAX = value[i];
		}
	}

	return MAX;
}

void MainViewControl::startSound()
{
		//��ʼ��Ⱦ
		timer.start();
		//��ʼ�ɼ���Դ
		emit this->_pMainModel->_pModel_AudioCollector->on_signal_startTimer();
		//fft��������
		//timer_thread_1->start();
		emit this->on_signal_fftStart();

		//qDebug() << "this.thread:" << this->thread();
		//��ʼ��Ⱦ����ͼ����Ƶ֡
		emit this->_pView_hotMapAndVideoDisplay->m_signal_start();

}
void MainViewControl::stopWorkSound()
{


	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		if (_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard != nullptr)
		{
			timer.stop();
			emit this->_pMainModel->_pModel_AudioCollector->on_signal_stopTimer();
			_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->stop();
			_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->deleteLater();
			_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard = nullptr;
			qDebug() << "stopSoundDevice";
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice != nullptr)
		{
			//fftֹͣ
			//timer_thread_1->stop();
			emit this->on_signal_fftStop();
			//���߳���Ⱦ
			timer.stop();
			emit this->_pMainModel->_pModel_AudioCollector->on_signal_stopTimer();
			//ֹͣ��Ⱦ����ͼ����Ƶ֡
			emit this->_mView_TabWidget_Setting->mView_deviceChoose->m_signal_modelStop();
			emit this->_pView_hotMapAndVideoDisplay->m_signal_stop();
			this->_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->stop();
			//mView_deviceChoose->m_niDevice = nullptr;
			qDebug() << "stopSoundDevice";
		}
		break;
	default:
		break;
	}
}
