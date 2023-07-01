#include "TcpDiscoverServer.h"
#include "qdebug.h"
#include <QDateTime>


TcpDiscoverServer::TcpDiscoverServer(QObject* parent)
{
	//qDebug() << calculateVolt("fe9e5e");

	qDebug() << __FUNCDNAME__ << this->thread();//QThread(0xb21d48)
	this->init();
}

TcpDiscoverServer::~TcpDiscoverServer()
{
}

void TcpDiscoverServer::openMonitorPort()
{
	if (!this->_pTcpServer->listen(QHostAddress::Any, PRESETMONITORPORT))
	{
		qDebug() << "No Can Discover 8234 Port!";
	}
	else
	{
		qDebug() << "Discover 8234 Porting!";
	}

	qDebug() << "is listening?" << this->_pTcpServer->isListening();
		
}

void TcpDiscoverServer::clossTheServer()
{
	if (this->_pTcpServer != nullptr)
		this->_pTcpServer->close();
}

QString TcpDiscoverServer::getVKDeviceName()
{

	return QString("VKinging");
}

void TcpDiscoverServer::initMembers()
{
	this->_pTcpServer = new QTcpServer(this);
	////test
	//this->openMonitorPort();


	this->_pTimer = new QTimer;
	this->_pTimer->setInterval(0);
	this->_pTimer->moveToThread(&this->mTimerThread);
	this->mTimerThread.start(QThread::HighestPriority);
	qDebug() << __FUNCDNAME__ << this->_pTimer->thread();//QThread(0x21c1008c)


	//this->_pTcpSocket = new QTcpSocket;
	//this->_pTcpSocket->moveToThread(&this->mThreadSocket);
	//mThreadSocket.start(QThread::HighestPriority);
	//qDebug() << __FUNCDNAME__ << this->_pTcpSocket->thread();
}

void TcpDiscoverServer::initConnect()
{
	connect(this->_pTcpServer, &QTcpServer::newConnection, this, &TcpDiscoverServer::discoverConnect);
	//qDebug() << __FUNCDNAME__ << this->_pTimer->thread();//QThread(0x21c1008c)
	connect(this->_pTimer, &QTimer::timeout, this, &TcpDiscoverServer::dealWithData, Qt::DirectConnection);
	connect(this,SIGNAL(on_signal_startAnalyze()), this->_pTimer, SLOT(start()), Qt::QueuedConnection);
	connect(this, &TcpDiscoverServer::on_signal_stopAnalyze, this->_pTimer, &QTimer::stop, Qt::QueuedConnection);
}

void TcpDiscoverServer::init()
{
	this->initMembers();
	this->initConnect();
}

void TcpDiscoverServer::discoverConnect()
{
	if (this->_pTcpSocket != nullptr)
	{
		this->_pTcpSocket->deleteLater();
		this->_pTcpSocket = nullptr;
	}
	this->_pTcpSocket = this->_pTcpServer->nextPendingConnection();

	qDebug() << __FUNCDNAME__ <<this->_pTcpSocket->thread();


	//设置参数
	//this->setVKParameter(SAMPLEBITS::BITS_16);
	this->setVKParameter(SAMPLEBITS::BITS_24);



	QTimer::singleShot(200, this, [=]() {//防止黏包，设置不成功
		//客户端连接标志
		this->mInitSocketFlag = true;
		//Tcp状态连接 ->View_DeviceChoose
		emit this->on_signal_TcpState(QAbstractSocket::ConnectedState);

		this->initSocketConnect(this->_pTcpSocket);

		qDebug() << "Client Linked~!";
		});
}
void TcpDiscoverServer::initSocketConnect(QTcpSocket* socket)
{


	connect(socket, &QTcpSocket::stateChanged, this, &TcpDiscoverServer::discoverState);
	connect(socket, &QTcpSocket::readyRead, this, &TcpDiscoverServer::readData,Qt::QueuedConnection);//, Qt::QueuedConnection //客户端采集数据

}

void TcpDiscoverServer::discoverState(int state)
{
	switch (state)
	{
	case QAbstractSocket::ConnectingState:
		qDebug() << QString::fromLocal8Bit("正在连接。。。");
		emit this->on_signal_TcpState(QTcpSocket::ConnectingState);
		break;
	case QAbstractSocket::ConnectedState:
		qDebug() << QString::fromLocal8Bit("已连接。。。");
		emit this->on_signal_TcpState(QTcpSocket::ConnectedState);
		break;
	case QAbstractSocket::ClosingState:
		qDebug() << QString::fromLocal8Bit("套接字即将关闭。。。");
		emit this->on_signal_TcpState(QTcpSocket::ClosingState);
		break;
	default:
		//qDebug() << QString::fromLocal8Bit("No Things");
		break;
	}
}
void TcpDiscoverServer::readData()
{		
	QByteArray data = this->_pTcpSocket->readAll().toHex();

	//qDebug()<<QDateTime::currentMSecsSinceEpoch();
	if (data.mid(0, 8).compare("55ee0004", Qt::CaseInsensitive) == 0) //心跳数据开头55ee0004不作数据处理
	{
		qDebug() << QString::fromLocal8Bit("心跳数据开头55ee0004不作数据处理");
		return;
	}
	if (data.mid(0, 8).compare("55ee0014", Qt::CaseInsensitive) == 0) //心跳数据开头55ee0014不作数据处理
	{
		qDebug() << QString::fromLocal8Bit("停止采集数据开头55ee0014不作数据处理");
		return;
	}
	if (data.mid(0, 8).compare("55ee0005", Qt::CaseInsensitive) == 0) //设置参数成功 or 失败后返回值55ee0005。不做处理
	{
		qDebug() << QString::fromLocal8Bit("设置参数成功 or 失败后返回值55ee0005。不做处理");
		return;
	}

	//qint64 tt1 = QDateTime::currentMSecsSinceEpoch();
	{
		QMutexLocker mLocker(&this->mMutexLockForData);
		this->mHexData.append(data);
	}
	//qDebug() << __FUNCDNAME__ << QDateTime::currentMSecsSinceEpoch()-tt1 << " " << QDateTime::currentMSecsSinceEpoch();

}

void TcpDiscoverServer::setVKParameter(SAMPLEBITS _lvs)
{
	//给予局部变量
	this->mBits = _lvs;

	if (this->_pTcpSocket != nullptr)
	{
		switch (this->mBits)
		{
		case BITS_16:
			qDebug() << "!SET_parameter:" << this->_pTcpSocket->write(QByteArray::fromHex("55EE800E10018101010101000100BB8000A3"));
			break;
		case BITS_24:
			qDebug() << "!SET_parameter:" << this->_pTcpSocket->write(QByteArray::fromHex("55EE800E10018101010101000200BB8000A4"));
			break;
		default:
			break;
		}


	}
	else
	{
		qDebug() << "SET_parameter Error!";
	}
}

void TcpDiscoverServer::start()
{
	emit this->on_signal_startAnalyze();


	if (this->_pTcpSocket != nullptr)
	{
		switch (this->mBits)
		{
		case BITS_16:
			qDebug() << "!SEND_start:" << this->_pTcpSocket->write(QByteArray::fromHex("55EE0005100124108D"));
			break;
		case BITS_24:
			qDebug() << "!SEND_start:" << this->_pTcpSocket->write(QByteArray::fromHex("55EE0005100134109D"));
			break;
		default:
			break;
		}
		
	}
	else
	{
		qDebug() << "SEND_start Error!";
	}
		
}

void TcpDiscoverServer::stop()
{
	emit this->on_signal_stopAnalyze();

	if (this->_pTcpSocket != nullptr)
	{
		qDebug() << "!SEND_stop:" << this->_pTcpSocket->write(QByteArray::fromHex("55EE8009100124330000000135"));

	}
	else
	{
		qDebug() << "SEND_stop Error!";
	}

}
void TcpDiscoverServer::dealWithData()
{
	//qDebug() << __FUNCDNAME__ << QThread::currentThread();//QThread(0xb21d48)
	
	//qint64 tt1 = QDateTime::currentMSecsSinceEpoch();
	if (!this->mHexData.isEmpty())
	{
		QMutexLocker mLocker(&this->mMutexLockForData);
		//qDebug() << QString::fromLocal8Bit("this->mHexData not empty,it's has data = ") << this->mHexData;
		//QString temp = this->mHexData;
		this->dealWithPack(this->mHexData, this->mBits);
		this->mHexData.clear();
		//qDebug() << "1:" << QDateTime::currentMSecsSinceEpoch();
	}
	//qDebug() << __FUNCTION__<< QDateTime::currentMSecsSinceEpoch() - tt1;
}

void TcpDiscoverServer::dealWithPack(QString hexDataPack, SAMPLEBITS _lvs)
{
	//qDebug() << __FUNCDNAME__ << QThread::currentThread();//QThread(0xb21d48)


	if (hexDataPack.mid(0, 4).compare("55ee", Qt::CaseInsensitive) != 0) //接收固定头
	{
		qDebug() << QString::fromLocal8Bit("非55ee开头");
		return;
	}
	if (hexDataPack.mid(0, 8).compare("55ee0004", Qt::CaseInsensitive) == 0) //心跳数据开头55ee0004不作数据处理
	{
		qDebug() << QString::fromLocal8Bit("心跳数据开头55ee0004不作数据处理");
		return;
	}
	if (hexDataPack.mid(0, 8).compare("55ee0014", Qt::CaseInsensitive) == 0) //心跳数据开头55ee0014不作数据处理
	{
		qDebug() << QString::fromLocal8Bit("停止采集数据开头55ee0014不作数据处理");
		return;
	}
	if (hexDataPack.mid(0, 8).compare("55ee0005", Qt::CaseInsensitive) == 0) //设置参数成功 or 失败后返回值55ee0005。不做处理
	{
		qDebug() << QString::fromLocal8Bit("设置参数成功 or 失败后返回值55ee0005。不做处理");
		return;
	}


	if (!mCutOffStr.isEmpty())
		hexDataPack.prepend(mCutOffStr);
	mCutOffStr.clear();

	//分解整体数据包
	QStringList mHexSplitData = hexDataPack.split("55ee");
	mPackNums = mHexSplitData.size();

	//qDebug() << "1:" << QDateTime::currentMSecsSinceEpoch();
	//预防不完整数据包
	if (mHexSplitData.size() > 2) {
		//qDebug() << "2:" << QDateTime::currentMSecsSinceEpoch();
		if (mHexSplitData.at(mHexSplitData.length() - 1).capacity() < (PACKLENGTH - 4)) {//最后面数据不完整 ；PACKLENGTH - 4为固定前导且split了2个字节
			mCutOffStr = mHexSplitData.at(mHexSplitData.length() - 1);//移到下一次数据中的最前面
			mPackNums -= 1;//总包数-1
		}
		//qDebug() << "3:" << QDateTime::currentMSecsSinceEpoch();

		if (mHexSplitData.at(1).capacity() < (PACKLENGTH - 4)) {//最前面数据不完整，理论上最前面数据不完整这个包一定不是第一个pack。在非第一个最前面不完整，mCutOffStr会补全
			qDebug() << "No Complate:" << mHexSplitData;
		}
		//qDebug() << "4:" << QDateTime::currentMSecsSinceEpoch();
	}

	//qDebug() << "5:" << QDateTime::currentMSecsSinceEpoch();
	for (int _j = 1; _j < mPackNums; ++_j)//mPackNums.at(0)为空 //debug:2、3ms
	{
		//根据3、4字节计算每个pack中有多少个采样点数
		QString myHexData = mHexSplitData.at(_j);
		myHexData.remove(0, 10);//移除固定字节+数据包大小，直达数据字节。

		//计算包中采样点数量
		int packSample;
		switch (_lvs)
		{
		case BITS_16:
			packSample = (mHexSplitData.at(_j).mid(0, 2).toInt(0, 16) * 256 + mHexSplitData.at(_j).mid(2, 2).toInt(0, 16) - 4) / 8;//单位是字节。 4中包括 2 字节固定编码，1 字节命令字，1 字节效验码，/ 8 = 4通道数* 每通道字节数2。
			break;
		case BITS_24:
			packSample = (mHexSplitData.at(_j).mid(0, 2).toInt(0, 16) * 256 + mHexSplitData.at(_j).mid(2, 2).toInt(0, 16) - 4) / 16;//单位是字节。 4中包括 2 字节固定编码，1 字节命令字，1 字节效验码，/ 16 = 4通道数* 每通道字节数4。
			break;
		default:
			break;
		}

		for (int i = 0; i < packSample; ++i)//转换数据，存入原始数据包
		{
			/**
			 * mingbai
			* //官方：未反转 数据一致性高
				1   2  3  4  5  6  7  8 9  10 11 12 13 14 15 16
				F1 x  F3 x  F5 F6 F7 F8 F9 FA FB FC FD FE x  FF :x为保留
				F0 00 FE 00 87 61 CD FE FE 70 BB 8F 80 FE FF C1
				1通道：0xF3F6F5 ==>取数据：FE 61 87
				2通道：0xF8F7FA ==>FE CD 70
				3通道：0xF9FCFB ==>FE 8F BB
				4通道：0xFEFDFF ==>FE 80 C1
			 */


			switch (_lvs)
			{
			case BITS_16:
				//此处数据应为大端数据解析，但传来的是小端数据，应以两两字节做互换， i * 8 * 2:->以字符为单位。两个字符为一个字节。8字节是四个通道数据为一组，一个通道是2字节也就是4个字符，一组共16字符
				//存入原始数据包
				if (this->has4800 < SAMPLE)//4799
				{
					mRawDataPack.pData[has4800] = calculateVolt(myHexData.mid(i * 8 * 2 + 2, 2) + myHexData.mid(i * 8 * 2 + 0, 2), _lvs);
					mRawDataPack.xData[has4800] = calculateVolt(myHexData.mid(i * 8 * 2 + 6, 2) + myHexData.mid(i * 8 * 2 + 4, 2), _lvs);
					mRawDataPack.yData[has4800] = calculateVolt(myHexData.mid(i * 8 * 2 + 10, 2) + myHexData.mid(i * 8 * 2 + 8, 2), _lvs);
					mRawDataPack.zData[has4800] = calculateVolt(myHexData.mid(i * 8 * 2 + 14, 2) + myHexData.mid(i * 8 * 2 + 12, 2), _lvs);

					this->has4800 += 1;
				}
				else
				{
					mRawDataQueue.append(mRawDataPack);
					this->has4800 = 0;
					memset(mRawDataPack.pData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.xData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.yData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.zData, 0, sizeof(double) * SAMPLE);
					qDebug() << __FUNCTION__ << "16BITS" << QDateTime::currentMSecsSinceEpoch() - tt1;
					tt1 = QDateTime::currentMSecsSinceEpoch();
				}
				//qDebug() << "9:" << QDateTime::currentMSecsSinceEpoch();

				break;
			case BITS_24:
				//i * 16 * 2:->以字符为单位。两个字符为一个字节。Fx = (x - 1) * 2 如： 0xF3F6F5 ;16字节是四个通道数据为一组，一组共32字符
				//存入原始数据包
				if (this->has4800 < SAMPLE)//4799
				{
					mRawDataPack.pData[has4800] = calculateVolt(myHexData.mid(i * 16 * 2 + (3 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (6 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (5 - 1) * 2, 2), _lvs);
					mRawDataPack.xData[has4800] = calculateVolt(myHexData.mid(i * 16 * 2 + (8 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (7 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (10 - 1) * 2, 2),_lvs);
					mRawDataPack.yData[has4800] = calculateVolt(myHexData.mid(i * 16 * 2 + (9 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (12 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (11 - 1) * 2, 2), _lvs);
					mRawDataPack.zData[has4800] = calculateVolt(myHexData.mid(i * 16 * 2 + (14 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (13 - 1) * 2, 2) + myHexData.mid(i * 16 * 2 + (15 - 1 + 1) * 2, 2), _lvs);

					this->has4800 += 1;
				}
				else
				{
					mRawDataQueue.append(mRawDataPack);
					this->has4800 = 0;
					memset(mRawDataPack.pData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.xData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.yData, 0, sizeof(double) * SAMPLE);
					memset(mRawDataPack.zData, 0, sizeof(double) * SAMPLE);
					qDebug() << __FUNCTION__ <<"24BITS" << QDateTime::currentMSecsSinceEpoch() - tt1;
					tt1 = QDateTime::currentMSecsSinceEpoch();
				}
				//qDebug() << "9:" << QDateTime::currentMSecsSinceEpoch();

				break;
			default:
				break;
			}
		}

	}
	//qDebug() << "7:" << QDateTime::currentMSecsSinceEpoch();



}
//Tcp 数据解析
//辅助函数
int TcpDiscoverServer::Convert16BitstoInt(unsigned char lsb, unsigned char msb)
{
	int chdat;
	chdat = (~(msb * 0x1000000 + lsb * 0x10000)) + 1;
	chdat = chdat / 0x10000;
	return chdat;
}
int TcpDiscoverServer::Convert24bitstoInt(unsigned char lsb1, unsigned char lsb, unsigned char msb)
{
	chdat = 0;
	//qDebug() << "chdat1:" << chdat;
	chdat = (~(msb * 0x1000000 + lsb * 0x10000 + lsb1 * 0x100)) + 1;
	//qDebug() << "chdat1:" << chdat;
	chdat = chdat / 0x100;
	//qDebug() << "chdat1:" << chdat;
	return chdat;
};
// qstring2Double
double TcpDiscoverServer::calculateVolt(QString str_val_hex, SAMPLEBITS _lvs) {//str_val_hex:高字节在前
	/**
	 * 24 位转换成电压值公式：
	*V（CH1S1） = [（ADC Value）* 4.000 / 0x800000] / Gain;备注：ADC Value =ADC上传的值、Gain = 0.688
	 */
	//QString tep1 = str_val_hex.mid(0, 2);
	//QString tep2 = str_val_hex.mid(2, 2);
	//QString tep3 = str_val_hex.mid(4, 2);
	//qDebug() << "tep1:" << tep1 << "tep2:" << tep2 << "tep3:" << tep3;

	////int xx = Convert24bitstoInt((unsigned char)(str_val_hex.mid(0, 2).toStdString().c_str())
	////	, (unsigned char)(str_val_hex.mid(2, 2).toStdString().c_str())
	////	, (unsigned char)(str_val_hex.mid(4, 2).toStdString().c_str()));

	//qDebug() << "11:" << (unsigned char)str_val_hex.mid(4, 2).toInt(0, 16);
	//qDebug() << "22:" << (unsigned char)str_val_hex.mid(2, 2).toInt(0, 16);
	//qDebug() << "33:" << (unsigned char)str_val_hex.mid(0, 2).toInt(0, 16);

	////低字节在前高字节在后
	//int xx = Convert24bitstoInt((unsigned char)str_val_hex.mid(4, 2).toInt(0, 16)
	//	, (unsigned char)str_val_hex.mid(2, 2).toInt(0, 16)
	//	, (unsigned char)str_val_hex.mid(0, 2).toInt(0, 16));

	//qDebug() << "xx:" << xx;
	////int cc = xx << 2;

	switch (_lvs)
	{
	case BITS_16:
		return Convert16BitstoInt((unsigned char)str_val_hex.mid(2, 2).toInt(0, 16)////Convert16BitstoInt(低字节，高字节)
			, (unsigned char)str_val_hex.mid(0, 2).toInt(0, 16)) * 4 / NEED_16BITS;
		break;
	case BITS_24:
		return Convert24bitstoInt((unsigned char)str_val_hex.mid(4, 2).toInt(0, 16)//Convert24bitstoInt(低字节，低字节，高字节)
			, (unsigned char)str_val_hex.mid(2, 2).toInt(0, 16)
			, (unsigned char)str_val_hex.mid(0, 2).toInt(0, 16)) * 4 / NEED_24BITS;
		break;
	default:
		break;
}


}
