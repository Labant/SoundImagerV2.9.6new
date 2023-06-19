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
	
	////读取cvs
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

	////test 插值
	//this->Fine_S = new double[64800 * 36];


	//分配
	p_fre_copy = new double[2400];
	x_fre_copy = new double[2400];
	y_fre_copy = new double[2400];
	z_fre_copy = new double[2400];

	//P_1D_MAX = new double;
	//*P_1D_MAX = 1.0;

	//initLayout
	initLoyout();

	//创建声卡
	//creatSound();

	//初始化相机
	//initCamera();

	////paintCustom 热力图静态读取文件测试
	////paintCustom();

	//初始化谱图图
	this->initSpectrumSequence();

	//初始化并更新数据
	this->initConnectedAndUpData();

	////设置样式表
	this->setCSS();

	////设置相机参数
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

	//停止采集
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

	//读取一帧
	this->read();

	//判断p x y z 都不为0则不执行
	if (this->p[0] == 0 || this->x[0] == 0 || this->y[0] == 0 || this->z[0] == 0)
		return;
	//qDebug() << "1！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	invokeDLL();
	//qDebug() << "2！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	//重绘热力图
	realDraw_plot_heatmap();
	//qDebug() << "3！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840
	//重绘时域图
	realDraw_plot_timeDomain();

	//重绘仰角
	realDraw_plot_eleAngle();

	////重绘方位角
	//realDraw_plot_azimAngle();

	//重绘频域 和 语谱图
	realDraw_plot_freqDomain();
}

void MainViewControl::doFFTData()
{
	//qDebug() << "doFFTData->threadId:" << this->thread()->currentThread();

	//未完成置为false
	isFftDown = 0;

	if (fft_thread == nullptr)
	{ 
		timer_thread_1->stop();
		return;
	}

	//fftw
	fft_change->Time2Frequency(p, x, y, z ,SAMPLE);

	//plot_freqDomain_p
	//传入时域数据 QVector<double>类型
	QVector<double> xData_p_fre, yData_p_fre, xData_x_fre, yData_x_fre, xData_y_fre, yData_y_fre, xData_z_fre, yData_z_fre;
	//用于渲染spectrum的数据
	QVector<double> xData_P_spectrum, xData_X_spectrum, xData_Y_spectrum, xData_Z_spectrum;//xAxis数据
	QVector<double> yData_P_spectrum, yData_X_spectrum, yData_Y_spectrum, yData_Z_spectrum;//yAxis数据

	//获取fft后的数据
	for (int i = 0; i < 2400; i++)
	{
		//对频域x轴数据整合
		xData_p_fre.append(i);
		yData_p_fre.append(fft_change->fft_p[i]);
		xData_x_fre.append(i);
		yData_x_fre.append(fft_change->fft_x[i]);
		xData_y_fre.append(i);
		yData_y_fre.append(fft_change->fft_y[i]);
		xData_z_fre.append(i);
		yData_z_fre.append(fft_change->fft_z[i]);

		//对语谱图y轴数据进行整合
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
	////保存到CVS
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
		//p_fre_copy用于计算评频率最大值
		memcpy(p_fre_copy, fft_change->fft_p, sizeof(double) * 2400);
		memcpy(x_fre_copy, fft_change->fft_x, sizeof(double) * 2400);
		memcpy(y_fre_copy, fft_change->fft_y, sizeof(double) * 2400);
		memcpy(z_fre_copy, fft_change->fft_z, sizeof(double) * 2400);
	}

	//语谱图添加新的数据
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
		//通用
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_p_fre, yData_p_fre);
		//语谱图 setCell()
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
		//通用
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_x_fre, yData_x_fre);
		//语谱图
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
		//通用
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_y_fre, yData_y_fre);
		//语谱图
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
		//通用
		m_freTime_general->m_freqencyDomain_general->graph(0)->setData(xData_z_fre, yData_z_fre);
		//语谱图
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


	//replot需要在同一线程中调用，放在 realDraw_plot_freqDomain()中了

	//完成置为true
	isFftDown = 1;
}

void MainViewControl::doCameraData()
{
	//frames = pipe.wait_for_frames();//等待所有配置的流生成框架

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

	//需在setModel后
	//裁剪阈值变更
	connect(this->_pMainModel->_pModel_HMCutOut, &Model_HMCutOut::xAxisCutOutChanged, this, &MainViewControl::On_setXAxisCutOutValue);//X cutout value
	connect(this->_pMainModel->_pModel_HMCutOut, &Model_HMCutOut::yAxisCutOutChanged, this, &MainViewControl::On_setYAxisCutOutValue);//Y cutout value

	//hotMap factor
	connect(this->_pMainModel->_Model_HmSizeFactor, &Model_HmSizeFactor::On_Signal_FactoeValue, this->_pMainModel->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::On_setHmFactorValue, Qt::QueuedConnection);


	//给予子线程是否显示hm
	connect(this, &MainViewControl::on_signal_P1_DMaxValue, this->_pMainModel->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_setP1_DValue, Qt::QueuedConnection);
	//显示目前最大值
	connect(this, &MainViewControl::on_signal_P1_DMaxValue, this->_mView_TabWidget_Setting->_pView_HMCutOut, &View_HMCutOut::on_showP1_DMaxValue);
}

void MainViewControl::initLoyout()
{
	this->setWindowTitle(QString::fromLocal8Bit("声源定位 | 苏州理声科技"));
	this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/image/logo.png")));
	this->setMinimumSize(1600, 900);//16:9
	//this->setMinimumSize(1600, 1200);
	//plot_heatmap = new QCustomPlot(this);//热力图
	this->_pView_hotMapAndVideoDisplay = new View_hotMapAndVideoDisplay;//热力图
	m_shereLocalation = new GLEWINQT2(this);
	m_freTime_general = new View_FrequencyTimeDomain(this);

	hbox = new QHBoxLayout;//主布局
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

	////热力图网格不可见
	//plot_heatmap->xAxis->grid()->setVisible(false);
	//plot_heatmap->yAxis->grid()->setVisible(false);

	////热力图x y 轴不可见
	//plot_heatmap->xAxis->setVisible(false);
	//plot_heatmap->xAxis2->setVisible(false);
	//plot_heatmap->yAxis->setVisible(false);
	//plot_heatmap->yAxis2->setVisible(false);



	//max[x,y]标签 // 【0】：x;[1]:x-value;[2]:y;[3]:y-value;
	for (int i = 0; i < 4; i++) 
	{
		xylable[i] = new QLabel(QString::fromLocal8Bit(i == 2 ? "方位角:" : "俯仰角"));
		xylable[i]->setToolTip(QString::fromLocal8Bit(i == 2 ? "方位角" : "俯仰角"));
		xylable[i]->setFont(QFont(QString::fromLocal8Bit("黑体"),15));
		//添加进仰角域
		xyBox->addWidget(xylable[i]);
		
	}
	//实时位置放入group中
	mGroupBox_XYPos->setLayout(xyBox);

	//初始化数值
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
	mGroupBox_shere->setTitle(QString::fromLocal8Bit("球体定位"));
	mGroupBox_XYPos->setTitle(QString::fromLocal8Bit("声源实时位置"));
	mGroupBox_freTimeDomain->setTitle(QString::fromLocal8Bit("时域|频域"));
	mGroupBox_videoHotMap->setTitle(QString::fromLocal8Bit("图像定位"));


	//布局添加
	for (int i = 0;i<2;i++)
	{
		vBox[i] = new QVBoxLayout;
		hbox->addLayout(vBox[i]);
	}
	//左右分配布局
	hbox->setStretch(0, 2);
	hbox->setStretch(1, 4);

	//qDebug()<<vbox->stretch(0);

	//添加
	plot_shere_box->addWidget(m_shereLocalation);


	mGroupBox_shere->setLayout(plot_shere_box);
	mGroupBox_freTimeDomain->setLayout(plot_timefreqDomain_box);
	mGroupBox_videoHotMap->setLayout(plot_videoDomain_box);

	//hBox[0]->addLayout(vbox_e_a,1);//三级容器加进二级容器中 左上
	//hBox[0]->addWidget(mGroupBox_shere,1);//球体 右上

	//hBox[1]->addWidget(mGroupBox_freTimeDomain,1);//时域 频域
	//hBox[1]->addWidget(mGroupBox_videoHotMap,1);//热力图


	vBox[0]->addLayout(vbox_e_a, 1);//三级容器加进二级容器中
	vBox[0]->addWidget(mGroupBox_freTimeDomain, 1);//时域频域 
	vBox[1]->addWidget(mGroupBox_shere, 2);//球体
	vBox[1]->addWidget(mGroupBox_videoHotMap, 3);//热力图
	
	//16:9
	vBox[1]->setStretch(0, 225);
	vBox[1]->setStretch(1, 375);

	//plot_timefreqDomain_box->addWidget(mGroupBox_XYPos,1);//文本俯仰角
	plot_timefreqDomain_box->addWidget(m_freTime_general);//,4
	//plot_videoDomain_box->addWidget(plot_heatmap);
	plot_videoDomain_box->addWidget(this->_pView_hotMapAndVideoDisplay);

	//设置边距
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


	//创建FFT QThread obj
	fft_thread = new QThread;
	timer_thread_1 = new QTimer;
	//FFT 模变换
	fft_change = new FFT_change;

	//创建Camera QThread obj
	camera_thread = new QThread;
	timer_thread_2 = new QTimer;

	////opencv 视频流  使用IMX291摄像头
	//m_model_videoDevice = new Model_VideoDevice;
	//m_model_videoDevice->moveToThread(camera_thread);
	//camera_thread->start();

	//频响矫正
	//_pCalibrationDll = new CalibrationDll("I:/WORK/06_Code/test_customPlot_hreatMap_SphereAndHM_V2.9.6/lib/CalibrationDll/rsp.txt", this);//一步计算
	//_pCalibrationDll = new CalibrationDll(":/MainViewControl/lib/CalibrationDll/rsp.txt", this);//一步计算
	_pCalibrationDll = new CalibrationDll(":/MainViewControl/lib/CalibrationDll/rsp_3Step.txt", this);//三部计算
	
	//带通过滤
	_pFiltDll = new filtDll(this);

	//myRawDataPack = new RawDataPack;

	//创建图像
	creatPlot();

}
void MainViewControl::initConnectedAndUpData()
{
	//更新绘图
	connect(&this->timer, &QTimer::timeout, this, &MainViewControl::realDrawplot);
	timer.setInterval(0);//1s钟读取10次，每次4800（实际4810，向前多取）
	//timer.start();

	//开个线程，处理fft数据
	connect(timer_thread_1, &QTimer::timeout, this, &MainViewControl::doFFTData, Qt::DirectConnection);
	timer_thread_1->setInterval(0);
	timer_thread_1->moveToThread(fft_thread);
	fft_thread->start();
	//fft处理-->开启关闭
	connect(this, &MainViewControl::on_signal_fftStart, timer_thread_1, [=]() {
		timer_thread_1->start();
		}, Qt::QueuedConnection);
	connect(this, &MainViewControl::on_signal_fftStop,timer_thread_1, &QTimer::stop, Qt::QueuedConnection);

	////test
	//connect(this->thread(), &QThread::finished, fft_thread, &QThread::quit, Qt::DirectConnection);
	//connect(this->thread(), &QThread::finished, camera_thread, &QThread::quit, Qt::DirectConnection);

	////视频初始化
	//connect(this, &MainViewControl::m_signal_initCamera, m_model_videoDevice, &Model_VideoDevice::init, Qt::QueuedConnection);
	////视频开始初始化
	//connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_camera, this, &MainViewControl::initCamera);

	//设备选择并开始 音频
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_audioStart, this, &MainViewControl::startSound);
	
	//设备停止工作
	//connect(mView_deviceChoose, &View_DeviceChoose::m_signal_stop, this->pSoundCard, &soundCard::stop);
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_stop, this, &MainViewControl::stopWorkSound);

	//设备选择并开始 视频
	connect(_mView_TabWidget_Setting->mView_deviceChoose->m_pushButton[1], &QPushButton::clicked, this, &MainViewControl::initCamera);
	
	//设备类型选择
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_captureType, this, &MainViewControl::m_solt_chooseCaptureType);
	
	//录音
	//connect(mView_deviceChoose, &View_DeviceChoose::m_signak_recording, this, &MainViewControl::m_solt_record);
	//connect(_mView_TabWidget_Setting->m_View_AudioRecord, &View_AudioRecord::m_signak_recording, this, &MainViewControl::m_solt_record);
	connect(this->_mView_TabWidget_Setting, &View_TabWidget_Setting::m_signal_recordding, this, &MainViewControl::m_solt_record);
	//connect(this->_mView_TabWidget_Setting->m_View_AudioRecord, &View_AudioRecord::m_signal_recordding, this, &View_TabWidget_Setting::m_signal_recordding);

	//录音结束 创建wav文件
	connect(m_timer_record, &QTimer::timeout, m_Model_AudioRecord, [=]() {
		m_bool_record = false;
	_mView_TabWidget_Setting->m_View_AudioRecord->m_checkBox->setChecked(0);
		m_Model_AudioRecord->m_slot_WavTimeout(_mView_TabWidget_Setting->m_View_AudioRecord->m_lineEdit_address->text());
		}, Qt::QueuedConnection);

	//Audio not foun just start :waring
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_place_reFoundAudioBtn, this, [=]() {QMessageBox::warning(this, QString::fromLocal8Bit("警告"), "place found for audio devices"); });

	//视频设备查找
	connect(_mView_TabWidget_Setting->mView_deviceChoose, &View_DeviceChoose::m_signal_findVideoDevs, this, &MainViewControl::findVideoDevs);

	//时频域显示相关设置
	//Auto按钮
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->_pPtnAuto, &QPushButton::clicked, this, [=]() {
		//置反
		if (this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)
		{
			this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue = 0;
			//阈值开始工作
			mThresholdVT = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->text().toFloat();
			mThresholdVF = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->text().toFloat();
		}
		else
		{
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue = 1;
		}
		
		//根据自动按钮来设定值
		this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->setEnabled(!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue);
		this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->setEnabled(!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue);
		});
	//带通滤波
	//QLineEdit中当Auto按钮为真并且触发returnPressed信号
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT, &QLineEdit::returnPressed, this, [=]() {
		//阈值变更
		mThresholdVT = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVT->text().toFloat();
		});
	connect(this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF, &QLineEdit::returnPressed, this, [=]() {
		//阈值变更
		mThresholdVF = this->m_freTime_general->_pView_ValueOfTFSetting->pLineVF->text().toFloat();
		});

}

void MainViewControl::setCSS()
{
	QPalette mCostomPal;
	mCostomPal.setBrush(QPalette::Background, QBrush(QPixmap(":/MainViewControl/image/gray.png")));

	//主视图
	this->setPalette(mCostomPal);
	this->setAutoFillBackground(true);    // 这句要加上, 否则可能显示不出背景图.

	//
	_mView_TabWidget_Setting->setPalette(mCostomPal);
	this->_mView_TabWidget_Setting->setAutoFillBackground(true);
	this->_mView_TabWidget_Setting->update();
	
	//时频域设置背景颜色
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
		//去除\r
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
			//p[i] = -p[i];//置反后ni卡ok
		}
		break;
	case CAPTURETYPE::NICaptureCard:
		//p置反
		for (int i = 0; i < 4800; i++)
		{
			//x[i] = -x[i];
			y[i] = -y[i];
			//z[i] = -z[i];
			//p[i] = -p[i];//置反后ni卡ok
		}
		break;
	default:
		break;
	}
	//第一版本的rsp.txt
	//频响校准
	//_pCalibrationDll->invokeDllAndWrite(x, y, z);
	//通带过滤
	//if (isFilt)
	//	_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);

	//第二版本的rsp。滤波-校准-滤波
	//通带过滤
	if (isFilt)
		this->_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);
	//频响校准
	this->_pCalibrationDll->invokeDllAndWrite(x, y, z);
	//通带过滤
	if (isFilt)
		this->_pFiltDll->filtDataAndWrite(p, x, y, z, filt_low, filt_hight, FS);


	//算法
	//direction_spl(p, x, y, z, P_1D); //V_2.9.1
	direction_spl(p, x, y, z, P_1D,&P_1D_MAX_lvs); //V_2.9.1  P_1D_MAX_lvs是P_1D中最大值 未双向双倍插值
	//direction_spl(p, x, y, z, P_1D, &P_1D_MAX_lvs); //V_2.9.1  P_1D_MAX_lvs是P_1D中最大值	双向双倍插值

	//传给子线程用于判断是否显示hm
	emit this->on_signal_P1_DMaxValue(P_1D_MAX_lvs);

	//保存到CVS
	//QFile turnData(QString("CVSData_P1_D_true.csv"));
	//if (!turnData.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//{
	//	qDebug() << "file not open!";
	//	return;
	//}

	//QTextStream stream(&turnData);

	//for (int yIndex = 0; yIndex < 180; ++yIndex) //裁剪第几行 裁剪（行，列）确定对于数值
	//{
	//	for (int xIndex = 0; xIndex < 360; ++xIndex)//裁剪第几列
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


	//热力图中心大小改变
	//changeHotMapcoefficient(P_1D_MAX_lvs);

	////插值数据扩大6*6
	///**
	// * 	extern void interp_imagesc_ver01(const double S[64800], double dx, double dy,
	//	double interp_multiple,
	//	double Fine_S[2332800]);
	// */
	//memset(this->Fine_S, 0, 2332800 * sizeof(double));
	//interp_imagesc_ver01(P_1D, 1, 1, 6, this->Fine_S);

	//qDebug() << "this->Fine_S[2332799]:" << this->Fine_S[2332799];

	////test 写入csv
	//P_1DCSV.setFileName("P_1DData_2_0m.csv");
	//if (!P_1DCSV.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//	qDebug()<<"not open P_1DCSV";
	//QTextStream P_1DCSVFile1(&P_1DCSV);

	//for (int i = 0; i < 180; i++)//行
	//{
	//	for (int j = 0; j < 360; j++)//列
	//	{
	//		P_1DCSVFile1 << QString::number(P_1D[j + i * 360], 'f', 12).toStdString().c_str();
	//		if (j < 359)P_1DCSVFile1 << ",";
	//	}
	//	P_1DCSVFile1 << "\n";

	//}
	//P_1DCSV.close();
	
	//music 算法
	//direction_music(p, x, y, z, P_1D);

	//拷贝 p x y z数据，用于进行排序后对时域y轴设置，避免对频域进行影响
	//memset(p_copy, 0, sizeof(double) * 4800);
	memcpy(p_copy, p, sizeof(double) * 4800);
	//memset(x_copy, 0, sizeof(double) * 4800);
	memcpy(x_copy, x, sizeof(double) * 4800);
	//memset(y_copy, 0, sizeof(double) * 4800);
	memcpy(y_copy, y, sizeof(double) * 4800);
	//memset(z_copy, 0, sizeof(double) * 4800);
	memcpy(z_copy, z, sizeof(double) * 4800);

	//计算方位给球体显示
	//存储数据中的最大值
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

//改为阅读采集线程中的的数据
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

	//获取数据
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



	//转音频文件
	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		//阅读一帧数据
		//_mView_TabWidget_Setting->mView_deviceChoose->pSoundCard->read();





		//方法二：一个循环
					//转存音频raw
		if (m_bool_record)
		{
			for (int i = 0; i < SAMPLE; i++)//共4810*4个点 P通道补偿9.6==10个点
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
				//录音时长 显示
				_mView_TabWidget_Setting->m_View_AudioRecord->m_label_rt->setText(QString("%1").arg(QString::fromLocal8Bit("SoundCard模式")));
			}
		}


		break;
	case CAPTURETYPE::NICaptureCard:

		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
			return;

		//阅读一帧数据
		//_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->read();

					//转存音频raw
		if (m_bool_record)
		{

			{

				QMutexLocker m_lock(&_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->m_lock_pRawData);
				for (int i = 0; i < SAMPLE; i++)//共4810*4个点	P通道补偿9.6==10个点
				{
					////分组取值，不是交错取值
					//p[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 10];
					//x[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 1 * (SAMPLE + 10)];//(FS + 10)
					//y[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 2 * (SAMPLE + 10)];
					//z[i] = _mView_TabWidget_Setting->mView_deviceChoose->m_niDevice->pRawData[i + 3 * (SAMPLE + 10)];

					////交错取值
					//p1[i] = this->mRawData[i*4];
					//x1[i] = this->mRawData[i*4 + 1];
					//y1[i] = this->mRawData[i*4 + 2];
					//z1[i] = this->mRawData[i*4 + 3];


					rawData.append(p[i]);
					rawData.append(x[i]);
					rawData.append(y[i]);
					rawData.append(z[i]);

					this->m_Model_AudioRecord->addRawData(rawData);

					//录音时长 显示
					_mView_TabWidget_Setting->m_View_AudioRecord->m_label_rt->setText(QString("%1").arg(QString::fromLocal8Bit("NI模式")));
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
	//// configure axis rect:配置轴矩形
	//plot_heatmap->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 这也将允许通过拖拽/缩放尺度改变颜色范围
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

	////colorMap->data()->setSize(nx, ny); // 我们希望彩色地图有nx*ny的数据点
	////colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // 并在键（x）和值（y）维上跨越坐标范围

	////初始化 不在此设定具体值
	//switch (mCaptrueType)
	//{
	//case CAPTURETYPE::SoundCard:
	//	colorMap->data()->setSize(90, 60); // 我们希望彩色地图有nx*ny的数据点
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // 并在键（x）和值（y）维上跨越坐标范围
	//	//colorMap->data()->setSize(360, 180); // 我们希望彩色地图有nx*ny的数据点
	//	//colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // 并在键（x）和值（y）维上跨越坐标范围

	//	break;
	//case CAPTURETYPE::NICaptureCard:
	//	//colorMap->data()->setSize(90,60); // 我们希望彩色地图有nx*ny的数据点
	//	colorMap->data()->setSize(this->mXAxisCutOut * 2 * 6, this->mYAxisCutOut * 2 * 6); // 我们希望彩色地图有nx*ny的数据点
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // 并在键（x）和值（y）维上跨越坐标范围
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
	//	colorMap->data()->setSize(90, 60); // 我们希望彩色地图有nx*ny的数据点
	//	colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // 并在键（x）和值（y）维上跨越坐标范围
	//	//colorMap->data()->setSize(360, 180); // 我们希望彩色地图有nx*ny的数据点
	//	//colorMap->data()->setRange(QCPRange(1, 360), QCPRange(1, 180)); // 并在键（x）和值（y）维上跨越坐标范围

	//	break;
	//case CAPTURETYPE::NICaptureCard:
	//	//if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
	//	//	return;
	//	
	//	//Intel D455
	//	//colorMap->data()->setSize(90, 60); // 我们希望彩色地图有nx*ny的数据点
	//	//colorMap->data()->setRange(QCPRange(45, 135), QCPRange(45, 105)); // 并在键（x）和值（y）维上跨越坐标范围
	//	
	//	//IMX291
	//	//manual
	//	//colorMap->data()->setSize(this->mXAxisCutOut * 2 * 6, this->mYAxisCutOut * 2 * 6); // 我们希望彩色地图有nx*ny的数据点
	//	//colorMap->data()->setRange(QCPRange(0, 360), QCPRange(0, 180)); // 并在键（x）和值（y）维上跨越坐标范围

	//	//设置所有
	//	//colorMap->data()->setSize(360*6,180*6); // 我们希望彩色地图有nx*ny的数据点

	//	break;
	//default:
	//	break;
	//}
}

void MainViewControl::realDraw_plot_heatmap()
{
	//qDebug() << "1！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351202 877
	//IMX291相机  设置背景
	{
		//QMutexLocker mLocker(&this->_pMainModel->m_model_videoDevice->m_mutex_frame);
		//if (this->_pMainModel->m_model_videoDevice->frame.empty())return;
		//plot_heatmap->setBackground(QPixmap::fromImage(CVMat2QImage::QCVMat2QImage(this->_pMainModel->m_model_videoDevice->frame)), true, Qt::KeepAspectRatio);

	}
	//qDebug() << "2！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
	switch (mCaptrueType)
	{
	case CAPTURETYPE::SoundCard:
		//nx:65;ny:90;
		for (int xIndex = 45; xIndex < 135; ++xIndex)//x轴  对应行 90
		{
			for (int yIndex = 45; yIndex < 105; ++yIndex) //y轴 对应列  60
			{
				//colorMap->data()->setCell(xIndex - 45, yIndex - 45, P_1D[xIndex + yIndex * 360]);
			}
		}
		break;
	case CAPTURETYPE::NICaptureCard:

		if (_mView_TabWidget_Setting->mView_deviceChoose->m_niDevice == nullptr)
			return;


		/**
		 * IMX291硬件 X:80;Y:70
		 */

		//manual		
		/********************XAxis解析****************************************************/
		/* 在 +70°左右和 -55°左右 算法有问题，增正负60°
		*因此要裁剪0-180数据实际应该裁剪 +65°-> 0°和0 -> -55°的拼凑，共120°
		*或
		* 实际应该裁剪 +60°-> 0°和0 -> -60°的拼凑，共120°即 0->60°+ 360->300°。对称点为0°，左半部：正常加0->60,右半部：0->60°先加300度=>300->360度，逆转裁剪360->300
		*/
		/****************************YAxis解析********************************************/
		/**
		* 可裁剪0->180，手动调节，对称点为90°增正负90°
		 * 
		 ************************************************************************** /
		/**************************************************************************/
		 /**old*/
		//for (int xIndex = 90 - this->mXAxisCutOut; xIndex < 90 + this->mXAxisCutOut; ++xIndex)//x轴  对应行 100  max:360
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //y轴 对应列  80	max:180
		//	{
		//		colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//}
		/**
		 * new
		 */
		//for (int xIndex = 90 - this->mXAxisCutOut; xIndex < 90 + this->mXAxisCutOut; ++xIndex)//x轴  对应行 100  max:360
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //y轴 对应列  80	max:180
		//	{
		//		colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//	 
		//	////左半部 设置一半 比如都裁剪30，则数据应该60*60 则X轴：左半部：0->30；右半部：30->60；abs(0 - this->mYAxisCutOut) - yIndex 区间为：[0，30）;
		//	//for (int yIndex = abs(0 - this->mYAxisCutOut) -1 ; yIndex >= 0; --yIndex) //y轴 对应列  80	max:180
		//	//{
		//	//	colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), abs(0 - this->mYAxisCutOut) - yIndex , P_1D[xIndex + yIndex * 360]);
		//	//}


		//	////右半部	359 - yIndex的区间为：[30,60);
		//	//for (int yIndex = 359 - this->mYAxisCutOut; yIndex > 359 - this->mYAxisCutOut; --yIndex) //y轴 对应列  80	max:180
		//	//{
		//	//	colorMap->data()->setCell(xIndex - (90 - this->mXAxisCutOut), 359 - yIndex , P_1D[xIndex + yIndex * 360]);
		//	//}
		//}

		////目标裁剪：0-30 后半部分
		//for (int xIndex = 0; xIndex < this->mXAxisCutOut; ++xIndex)//裁剪第几列
		//{
		//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //裁剪第几行 裁剪（行，列）确定对于数值
		//	{
		//		colorMap->data()->setCell(xIndex + this->mXAxisCutOut, yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
		//	}
		//}
		////目标裁剪：330-360 前半部分
		//for (int xIndex = 360 - this->mXAxisCutOut; xIndex < 360; ++xIndex)//裁剪第几列
		//{
//	for (int yIndex = 90 - this->mYAxisCutOut; yIndex < 90 + this->mYAxisCutOut; ++yIndex) //裁剪第几行 裁剪（行，列）确定对于数值
//	{
//		colorMap->data()->setCell(xIndex - (360 - this->mXAxisCutOut), yIndex - (90 - this->mYAxisCutOut), P_1D[xIndex + yIndex * 360]);
//	}
//}

////test 插值
////目标裁剪：0-30 ！为后半部分
//for (int xIndex = 0 * 6; xIndex < this->mXAxisCutOut; ++xIndex)//裁剪第几列 数据内容同时扩大6倍
//{
//	for (int yIndex = (90*6 - this->mYAxisCutOut); yIndex < (90*6 + this->mYAxisCutOut); ++yIndex) //裁剪第几行 裁剪（行，列）确定对于数值 数据内容同时扩大6倍
//	{
//		colorMap->data()->setCell(xIndex + this->mXAxisCutOut * 6, yIndex - (90 - this->mYAxisCutOut)*6, this->Fine_S[xIndex + yIndex * 360 * 6]);
//		//colorMap->data()->setCell(xIndex + this->mXAxisCutOut, yIndex - (90*6 - this->mYAxisCutOut), this->Fine_S[yIndex + xIndex * 180 * 6]);
//	}
//}
////目标裁剪：330-360 ！为前半部分
//for (int xIndex = (360 - this->mXAxisCutOut) * 6; xIndex < 360 * 6; ++xIndex)//裁剪第几列
//{
//	for (int yIndex = (90 - this->mYAxisCutOut) * 6; yIndex < (90 + this->mYAxisCutOut) * 6; ++yIndex) //裁剪第几行 裁剪（行，列）确定对于数值
//	{
//		colorMap->data()->setCell(xIndex - (360 - this->mXAxisCutOut) * 6, yIndex - (90 - this->mYAxisCutOut) * 6, this->Fine_S[xIndex + yIndex * 360 * 6]);
//	}
//}

////输出全部数据  以列拼接还原也应以列还原
//for (int xIndex = 0; xIndex < 360 * 6; ++xIndex)//列 全部共360*6列
//{
//	for (int yIndex = 0; yIndex < 180 * 6; ++yIndex) //行 全部共180*6列
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
	//qDebug() << "3！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

	////// 添加色标:
	////QCPColorScale* colorScale = new QCPColorScale(this);
	////this->plotLayout()->addElement(0, 1, colorScale); // 将其添加到主轴矩形的右侧
	////colorScale->setType(QCPAxis::atRight); // 刻度应为垂直条，刻度线/坐标轴标签右侧（实际上，右侧已经是默认值）
	////colorMap->setColorScale(colorScale); // 将颜色图与色标关联
	////colorScale->axis()->setLabel("Magnetic Field Strength");


	////////添加系数改变大小
	////static QCPColorGradient cpColorGradient; 
	////cpColorGradient.clearColorStops();
	//////cpColorGradient.setPeriodic()//周期性
	//////cpColorGradient.setLevelCount(360);//默认值350
	////cpColorGradient.setColorStopAt(0, QColor(Qt::transparent));//透明
	////cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 2.0f - 0.001, QColor(Qt::transparent));//透明
	//////空间比：0.02，0.03，0.01，0.01 共6份
	////
	//////cpColorGradient.setColorStopAt(posColor[0] + this->mHmSizeFactor * 2.0f, hotMapCoefficient > 0 ? QColor(115, 115, 255, 150) : QColor(Qt::transparent));//紫罗兰色
	////cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 3.0f, hotMapCoefficient > 0 ? QColor(77, 255, 255, 150) : QColor(Qt::transparent));//亮蓝色
	////cpColorGradient.setColorStopAt(posColor[2] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 255, 0, 150) : QColor(Qt::transparent));//明黄色
	////cpColorGradient.setColorStopAt(posColor[3] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 71, 26, 150) : QColor(Qt::transparent));//正红色
	////cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(102, 20, 0, 150) : QColor(Qt::transparent));//红黑色
	////
	//////cpColorGradient.setColorStops()
	////cpColorGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciHSV);//ciHSV
	////colorMap->setGradient(cpColorGradient);

	////QColor(1, 0, 0.196078, 1);
	////qDebug() << colorMap->gradient().levelCount();//350
	////qDebug() << colorMap->gradient().colorStops();//QMap((0, QColor(ARGB 1, 0, 0, 0.392157))(0.15, QColor(ARGB 1, 0, 0.196078, 1))(0.35, QColor(ARGB 1, 0, 1, 1))(0.65, QColor(ARGB 1, 1, 1, 0))(0.85, QColor(ARGB 1, 1, 0.117647, 0))(1, QColor(ARGB 1, 0.392157, 0, 0)))
	////qDebug() << colorMap->gradient().colorInterpolation();//颜色插值 0 ciRGB
	////qDebug() << colorMap->gradient().periodic();//bool 定期否 false

	////test
	//colorMap->setGradient(QCPColorGradient::gpHot);

	//// 我们还可以创建一个QCPColorGradient实例并向其中添加自己的颜色
	//// 渐变，请参阅QCPColorGradient的文档以获取可能的效果.

	//// 重新缩放数据维度（颜色），以使所有数据点都位于颜色渐变显示的范围内:
	//colorMap->rescaleDataRange(true);

	//////确保轴rect和色标同步其底边距和顶边距（以便它们对齐）:
	////QCPMarginGroup* marginGroup = new QCPMarginGroup(this);
	////this->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
	////colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

	//// 重新缩放键（x）和值（y）轴，以便可以看到整个颜色图：
	//plot_heatmap->rescaleAxes();

	//plot_heatmap->replot();
	//qDebug() << "4！";
	//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 840

	//拷贝P1_D给子线程渲染
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


	//传入时域数据 QVector<double>类型
	static QVector<double> xData_p, yData_p, xData_x, yData_x, xData_y, yData_y, xData_z, yData_z;



	//重置数据
	xData_p.clear();
	yData_p.clear();
	xData_x.clear();
	yData_x.clear();
	xData_y.clear();
	yData_y.clear();
	xData_z.clear();
	yData_z.clear();

	////判断p x y z 都不为0则添加
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
	//时序制造
	if (!mStructTS.mTimeData_P.isEmpty())
	{
		//移除上一个4800数据点
		mStructTS.mTimeData_P.removeFirst();
		mStructTS.mSequence_P.removeFirst();
		mStructTS.mTimeData_X.removeFirst();
		mStructTS.mSequence_X.removeFirst();
		mStructTS.mTimeData_Y.removeFirst();
		mStructTS.mSequence_Y.removeFirst();
		mStructTS.mTimeData_Z.removeFirst();
		mStructTS.mSequence_Z.removeFirst();

	}

	//添加新的4800数据点	
	mStructTS.mTimeData_P.append(yData_p);
	mStructTS.mSequence_P.append(xData_p);
	mStructTS.mTimeData_X.append(yData_x);
	mStructTS.mSequence_X.append(xData_x);
	mStructTS.mTimeData_Y.append(yData_y);
	mStructTS.mSequence_Y.append(xData_y);
	mStructTS.mTimeData_Z.append(yData_z);
	mStructTS.mSequence_Z.append(xData_z);

	//加入时域序列 可从时域序列中取三组值，每组值都有pxyz四组数据
	if (mTimeSequence.size() > 2)
	{
		mTimeSequence.dequeue();
	}
	mTimeSequence.append(mStructTS);

	//取最大值 //默认是升序但是会破坏原数组
	std::sort(p_copy, p_copy + 4800);
	std::sort(x_copy, x_copy + 4800);
	std::sort(y_copy, y_copy + 4800);
	std::sort(z_copy, z_copy + 4800);

	if (mTimeSequence.size() < 3)
		return;

	//取一组值
	_mStructTS[0] = mTimeSequence.at(0);
	_mStructTS[1] = mTimeSequence.at(1);
	_mStructTS[2] = mTimeSequence.at(2);

	switch (m_freTime_general->_pView_ValueOfTFSetting->m_comBox_chooseDisplay->currentIndex())
	{
	case MYSTRUCT::PXYZ::pxyz::P://0
		////更新y轴范围
		//m_freTime_general->m_timeDomain_general->yAxis->setRange(p_copy[0], p_copy[4799]);
		////更新数据
		//m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_p, yData_p);
		////标签
		//m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));

		//更新y轴范围
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//自动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(p_copy[0], p_copy[4799]);
		}

		//更新数据
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_P.at(0) + _mStructTS[1].mTimeData_P.at(0) + _mStructTS[2].mTimeData_P.at(0));
		//标签
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-TimeDomain"));
		break;
	case MYSTRUCT::PXYZ::pxyz::X:
		//更新y轴范围
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//自动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(x_copy[0], x_copy[4799]);
		}

		//更新数据
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_X.at(0) + _mStructTS[1].mTimeData_X.at(0) + _mStructTS[2].mTimeData_X.at(0));
		//标签
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("X-TimeDomain"));

		break;
	case MYSTRUCT::PXYZ::pxyz::Y:
		//更新y轴范围
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//自动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(y_copy[0], y_copy[4799]);
		}

		//更新数据
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_Y.at(0) + _mStructTS[1].mTimeData_Y.at(0) + _mStructTS[2].mTimeData_Y.at(0));
		//标签
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("Y-TimeDomain"));
		break;
	case MYSTRUCT::PXYZ::pxyz::Z:
		//更新y轴范围
		if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(-abs(mThresholdVT), abs(mThresholdVT));
		}
		else//自动设置值
		{
			m_freTime_general->m_timeDomain_general->yAxis->setRange(z_copy[0], z_copy[4799]);
		}

		//更新数据
		m_freTime_general->m_timeDomain_general->graph(0)->setData(xData_x, _mStructTS[0].mTimeData_Z.at(0) + _mStructTS[1].mTimeData_Z.at(0) + _mStructTS[2].mTimeData_Z.at(0));
		//标签
		m_freTime_general->m_timeDomain_general->graph(0)->setName(QString::fromLocal8Bit("Z-TimeDomain"));
		break;
	default:
		break;
	}

	//重绘
	m_freTime_general->m_timeDomain_general->replot();

}

void MainViewControl::realDraw_plot_freqDomain()
{
	//只对y轴设置数值,x轴的数据点在doFFTData()中处理了
	if (this->thread()->isRunning() && fft_thread->isRunning() )//&& isFftDown
	{
		QMutexLocker m_dataCopyLocker(&m_dataCopy);

		switch (m_freTime_general->_pView_ValueOfTFSetting->m_comBox_chooseDisplay->currentIndex())
		{
		case MYSTRUCT::PXYZ::pxyz::P://0
			//更新y轴范围
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//自动设置值
			{
				//排序
				std::sort(p_fre_copy, p_fre_copy + SAMPLE / 2);
				//通用
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(p_fre_copy, SAMPLE / 2) + 0.2);
			}
			//标签
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("P-FFT"));			//标签
			//m_freTime_general->m_spectrum_general->graph(0)->setName(QString::fromLocal8Bit("P-SPECTRUM"));
			break;
		case MYSTRUCT::PXYZ::pxyz::X:
			//更新y轴范围
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//自动设置值
			{
				//排序
				std::sort(x_fre_copy, x_fre_copy + SAMPLE / 2);
				//通用
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(x_fre_copy, SAMPLE / 2) + 0.2);
			}

			//标签
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("X-FFT"));
			break;
		case MYSTRUCT::PXYZ::pxyz::Y:
			//更新y轴范围
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//自动设置值
			{
				//排序
				std::sort(y_fre_copy, y_fre_copy + SAMPLE / 2);
				//通用
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(y_fre_copy, SAMPLE / 2) + 0.2);
			}

			//标签
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("Y-FFT"));
			break;
		case MYSTRUCT::PXYZ::pxyz::Z:
			//更新y轴范围
			if (!this->m_freTime_general->_pView_ValueOfTFSetting->mAutoValue)//启用手动设置值
			{
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(0, abs(mThresholdVF));
			}
			else//自动设置值
			{
				//排序
				std::sort(z_fre_copy, z_fre_copy + SAMPLE / 2);
				//通用
				m_freTime_general->m_freqencyDomain_general->yAxis->setRange(-0.02, MAX_VALUE(z_fre_copy, SAMPLE / 2) + 0.2);
			}

			//标签
			m_freTime_general->m_freqencyDomain_general->graph(0)->setName(QString::fromLocal8Bit("Z-FFT"));
			break;
		default:
			break;
		}
		//频率重绘
		m_freTime_general->m_freqencyDomain_general->replot();
		//语谱图重绘
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

	////设置X轴坐标范围
	//plot_eleAngle->xAxis->setRange(myTimer-60, myTimer);

	////设置Y轴坐标范围
	//plot_eleAngle->yAxis->setRange(0, 180);

	////更新x轴数据
	//plot_eleAngle->graph(0)->addData(myTimer, maxp.first);
	////plot_eleAngle->graph(0)->re

	////根据图像最高点最低点自动缩放坐标轴
	////plot_eleAngle->graph(0)->rescaleAxes(true);

	//plot_eleAngle->replot();

	//设置 m_model值
	//maxp.second / 6: 60
	//180 - maxp.first: 12
	//qDebug() << "maxp.second:" << maxp.second;//maxp.second / 6: 360
	//qDebug() << "180 - maxp.first:" << 180 - maxp.first;//180 - maxp.first: 17
	m_shereLocalation->m_soundLocation.setX(maxp.second);//左右
	m_shereLocalation->m_soundLocation.setY(180 - maxp.first);//上下

	////设置label值
	//xylable[1]->setText(QString::number(maxp.first));//180°
	//xylable[3]->setText(QString::number(maxp.second));//360°
}

void MainViewControl::realDraw_plot_azimAngle()
{
	static QVector<double> y2_value, x2_value;
	y2_value.clear();
	x2_value.clear();

	static double myTimer2 = QDateTime::currentSecsSinceEpoch();
	myTimer2 = QDateTime::currentSecsSinceEpoch();

	////设置X轴坐标范围
	//plot_azimAngle->xAxis->setRange(myTimer2 - 60, myTimer2);

	////设置Y轴坐标范围
	//plot_azimAngle->yAxis->setRange(0, 360);

	////更新x轴数据
	//plot_azimAngle->graph(0)->addData(myTimer2, maxp.second);

	////根据图像最高点最低点自动缩放坐标轴
	////plot_azimAngle->graph(0)->rescaleAxes(true);

	//plot_azimAngle->replot();
}
void MainViewControl::initCamera(int camIndex)
{
	this->_pMainModel->m_model_videoDevice->setDevice(camIndex);
	//emit this->m_signal_initCamera();
	//是否初始化相机
	isInitCamera = 1;

}
//改变热力图带下的系数
void MainViewControl::changeHotMapcoefficient(double& coefficient)
{
	//hotMapCoefficient 非零则产生热力图

	if (coefficient < 0.0000007)	//无热力图
	{
		hotMapCoefficient = 0; 
	} 
	else if (coefficient > 0.0000007 && coefficient < 0.0000391965)//1级  0.93*1= 0.93  最小;0.98*1 = 0.98;0.99*1 = 0.99
	{				
		hotMapCoefficient = 1;//1 - 0.97 = 0.3; 0.3/5 = 0.06

		posColor[0] = 0.93;
		posColor[1] = 0.95;
		posColor[2] = 0.98;//明黄色
		posColor[3] = 0.99;//红色
		posColor[4] = 1;//小黑色
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
//初始化语谱图数据
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
//		//是否初始化相机
//		isInitCamera = 0;
//
//		QMessageBox::warning(this, "Warning!", "No RS device detected. Is it plugged in?");
//		QApplication::quit();
//		return;
//	}
//	rs2::device dev = list.front();
//
//	//给组件列表添加信息
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
//	cfg.enable_stream(RS2_STREAM_COLOR, camera_frame_width, camera_frame_height, RS2_FORMAT_BGR8, camera_fps);//向配置添加所需的流
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
//		qDebug() << QString::fromLocal8Bit("自动曝光 参数为[") << sen.get_option(RS2_OPTION_EXPOSURE) << QString::fromLocal8Bit("]");
//	}
//	else {
//		sen.set_option(RS2_OPTION_EXPOSURE, camera_exposure);
//		qDebug() << QString::fromLocal8Bit("手动曝光 参数为[") << sen.get_option(RS2_OPTION_EXPOSURE) << QString::fromLocal8Bit("]");
//	}
//
//	//auto depth_stream = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
//	auto color_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
//
//	//以初始化相机
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



	//更改热力图数据范围
	changePlot();

}
//录音
void MainViewControl::m_solt_record()
{
	//设置录音时间
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

	//发送信号给子线程设置范围
	emit this->_pView_hotMapAndVideoDisplay->on_signal_xAxisRange(value);

}
void MainViewControl::On_setYAxisCutOutValue(uInt16 value)
{
	//{
	//	QMutexLocker locker(&this->mYAxis_CO_Mutex);
	//	this->mYAxisCutOut = value;
	//}
	//this->changePlot();

	//发送信号给子线程设置范围
	emit this->_pView_hotMapAndVideoDisplay->on_signal_yAxisRange(value);
}

void MainViewControl::On_setHmFactorValue(uInt16 value)
{
	/**需要大于0.94，可分为：0.95(最小)，0.96，0.97，0.98，0.99（最大）
	 * value值可设为：50->90 =>/1000 = 0.05->0.09;可设基数为0.9
	 */
	this->mHmSizeFactor = (1.0f - (value / 1000.0f + 0.9f)) / 6.0f;

	//this->mHmSizeFactor = 1.05;
	qDebug() << "this->mHmSizeFactor:" << this->mHmSizeFactor;
}

//计算数组中的大小
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
		//开始渲染
		timer.start();
		//开始采集声源
		emit this->_pMainModel->_pModel_AudioCollector->on_signal_startTimer();
		//fft处理数据
		//timer_thread_1->start();
		emit this->on_signal_fftStart();

		//qDebug() << "this.thread:" << this->thread();
		//开始渲染热力图和视频帧
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
			//fft停止
			//timer_thread_1->stop();
			emit this->on_signal_fftStop();
			//主线程渲染
			timer.stop();
			emit this->_pMainModel->_pModel_AudioCollector->on_signal_stopTimer();
			//停止渲染热力图和视频帧
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
