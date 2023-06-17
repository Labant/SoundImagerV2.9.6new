#include "Model_CarPlateIdentify.h"
#include "Model_hotMapAndVideoDisplay.h"
#include <QDebug>

Model_CarPlateIdentify::Model_CarPlateIdentify(QObject* handle)
{
	qDebug() << __FUNCDNAME__ << "this->Thread old:" << this->thread();
	//this->setParent(parent);
	this->moveToThread(&this->mThread);
	this->mThread.start();
	this->_pModel_hotMapAndVideoDisplay = qobject_cast<Model_hotMapAndVideoDisplay*>(handle);
	this->init();
	qDebug() << __FUNCDNAME__ << "this->Thread new:" << this->thread();
}

Model_CarPlateIdentify::~Model_CarPlateIdentify()
{
	if (this->mThread.isRunning())
	{
		this->mThread.quit();
		this->mThread.wait();
	}

}

QString Model_CarPlateIdentify::IdentifyCarPalte(cv::Mat src)
{
	this->plateVec.clear();

	pr.setResultShow(false);

	//qDebug() << __FUNCDNAME__ << "this->Thread:" << this->thread();

	int result = pr.plateRecognize(src, plateVec);

	qDebug() << __FUNCDNAME__ << "Identify is " << ((result == 0) ? "success" : "defail");

	for (int i = 0; i < plateVec.size(); i++)
	{
		CPlate plate = plateVec.at(i);
		Mat plateMat = plate.getPlateMat();
		RotatedRect rrect = plate.getPlatePos();
		string license = plate.getPlateStr();
		//qDebug() << "QString::fromStdString(license.c_str()):" << QString(QString::fromLocal8Bit(license.c_str()));
		return QString(QString::fromLocal8Bit(license.c_str()));
	}
	return " ";
	
}

QPair<QString, QString> Model_CarPlateIdentify::IdentifyCarPalte(cv::Mat src, bool isIndefiy)
{
	this->plateVec.clear();

	pr.setResultShow(false);

	int result = pr.plateRecognize(src, plateVec);

	qDebug() << __FUNCDNAME__ << "Identify is " << ((result == 0) ? "success" : "defail");

	for (int i = 0; i < plateVec.size(); i++)
	{
		CPlate plate = plateVec.at(i);
		Mat plateMat = plate.getPlateMat();
		RotatedRect rrect = plate.getPlatePos();
		string license = plate.getPlateStr();
		//qDebug() << "QString::fromStdString(license.c_str()):" << QString(QString::fromLocal8Bit(license.c_str()));
		return QPair<QString,QString>(QString(QString::fromLocal8Bit(license.c_str())),QString::fromLocal8Bit(this->IdentifyCarlane(rrect.center.x).toLocal8Bit()));
	}

	return QPair<QString, QString>();
}

void Model_CarPlateIdentify::initMembers()
{

	pr.setResultShow(false);
	pr.setLifemode(true);
	pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_CMSER);
	pr.setMaxPlates(1);
	// new in v1.6
	pr.LoadGrayChANN("model/annCh.xml");
	pr.LoadChineseMapping("model/province_mapping");
}

void Model_CarPlateIdentify::initConnects()
{
	qRegisterMetaType<cv::Mat>("cv::Mat");

	//����ͼƬMat��ʶ��
	connect(this->_pModel_hotMapAndVideoDisplay, &Model_hotMapAndVideoDisplay::on_signal_sendImageMat, this, &Model_CarPlateIdentify::IdentifyAndResSend, Qt::QueuedConnection);//Blocking
}

void Model_CarPlateIdentify::init()
{
	this->initMembers();
	this->initConnects();
}

/**
 * �˺����б��λ�á�����ʶ������Ƶ�λ��
 */
QString Model_CarPlateIdentify::IdentifyCarlane(double value)
{
	QString mCarLaneInfo;
	if (value < this->mCarlaneDividingLine)
	{
		mCarLaneInfo = QString::fromLocal8Bit("��һ����");//��
	}
	else if (value < (this->mCarlaneDividingLine * 2) && value >= this->mCarlaneDividingLine)
	{
		mCarLaneInfo = QString::fromLocal8Bit("�ڶ�����");//��
	}
	else if (value < (this->mCarlaneDividingLine * 3) && value >= (this->mCarlaneDividingLine * 2))
	{
		mCarLaneInfo = QString::fromLocal8Bit("��������");//��
	}

	return mCarLaneInfo;
}

void Model_CarPlateIdentify::IdentifyAndResSend(cv::Mat src,double maxValue)
{
	//qDebug() << __FUNCDNAME__ << "this->Thread:" << this->thread();
	
	//����ʶ��
	this->on_signal_sendIdentifyRes(QString::fromLocal8Bit("����ʶ��..."));
	QPair<QString,QString> identifyData = this->IdentifyCarPalte(src, 1);
	QString plate = identifyData.first;
	QString carLane = identifyData.second;
	qDebug() << "plate:" << plate;
	qDebug() << "maxValue:" << maxValue;
	qDebug() << "carLane:" << carLane;
	if (plate.contains(":"))
	{
		this->on_signal_sendIdentifyRes(plate);//����
		this->on_signal_sendSoundPw(maxValue);//������
		this->on_signal_sendCarLane(carLane);//����
	}
	else
	{
		this->on_signal_sendIdentifyRes(QString::fromLocal8Bit("δʶ��"));//����
		this->on_signal_sendSoundPw(0);//������
		this->on_signal_sendCarLane(QString::fromLocal8Bit(" "));//����
	}
	
}
