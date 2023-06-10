#include "soundcard.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
soundCard::soundCard(QObject *parent) : QObject(parent)
{
//    this->mReadSize = this->mChannels * this->mSamples * sizeof(float);//8192 ////48000*2 *4 = 384000
    //this->mReadSize = this->mChannels * this->mSamples * sizeof(float);// 4*4800*4 = 76800
    this->mReadSize = this->mChannels * this->mSamples * sizeof(float);// 4*4810*4 = 76960

    this->pRawData = new float[mSamples * mChannels];//4810*4
//    qDebug()<<"mSamples:"<<mSamples;//1024
//    qDebug()<<"mChannels:"<<mChannels;//2
//    qDebug()<<"mSamples * mChannels:"<<mSamples * mChannels;//2048

    this->initConnected();


}
void soundCard:: init()
{

//    this->initFmt(); // format
//    this->setDefaultInputAudioDeviceInfo();//device info

}
void soundCard:: initFmt()
{

}

void soundCard:: initConnected()
{
//    connect(io, &QIODevice::readyRead, this, &soundCard::read);

}
QAudioDeviceInfo soundCard:: setDefaultInputAudioDeviceInfo()
{
//    //��ȡ�����豸������Ϣ����
//    QList<QAudioDeviceInfo> deviceInfoes = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

//    foreach(info, deviceInfoes)
//    {
//     /************************************************************************/
//     /* "Line In (2- iRig Stream)"
//     "���������� (Realtek(R) Audio)"
//     "���������� (Realtek(R) Audio)"
//     "Line In (2- iRig Stream)"                                                                     */
//     /************************************************************************/
//       qDebug() << info.deviceName();
//       if (info.deviceName() == QString::fromLocal8Bit("Line In (2- iRig Stream)"))
//         {
//           /**
//            "SampleRatesed:" (8000, 11025, 16000, 22050, 32000, 44100, 48000, 88200, 96000, 192000)采样率
//             "Codecsed:" ("audio/pcm")//解码方式
//             "ByteOrders:" (LittleEndian)//字节顺序
//             "ChannelCounts:" (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)//通道数
//             "SampleSizes:" (8, 16, 24, 32, 48, 64) //采样大小
//             "SampleTypes:" (SignedInt, UnSignedInt, Float)//采样类型
//           */
//            return info;
//            };

//       }
        return QAudioDeviceInfo();


}
void soundCard:: start(QString devName)
{
    //info
    foreach(QAudioDeviceInfo infos,QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    {
        if (infos.deviceName() == devName)
          {

             info = infos;
             qDebug()<<"1:"<<info.deviceName();
             qDebug()<<"2:"<<info.supportedSampleRates();
             qDebug()<<"3:"<<info.supportedSampleSizes();
             qDebug()<<"4:"<<info.supportedSampleTypes();
             qDebug()<<"5:"<<info.supportedByteOrders();
             qDebug()<<"6:"<<info.supportedChannelCounts();
             qDebug()<<"7:"<<info.supportedCodecs();
             soundCardVaild = true;
             break;
        };
        soundCardVaild = false;
        //qDebug()<<QString::fromLocal8Bit("wei huoqu shebei xinxi ");
        qDebug()<<"infos.deviceName():" << infos.deviceName();
    }



	//format
	format.setSampleRate(48000);
	format.setSampleType(QAudioFormat::Float);
	format.setChannelCount(4);              //4
	format.setSampleSize(32);              //32
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec(QString::fromLocal8Bit("audio/pcm"));

	if (!(info.isFormatSupported(format)))
	{
		format = info.nearestFormat(format);
		//QMessageBox::warning(this, QString::fromLocal8Bit("SOUNDCARD"), QString::fromLocal8Bit("format invaild"));
		qDebug() << QString::fromLocal8Bit("cai qu jiu jin shebei shuxing");
		soundCardVaild = false;
	}
	else
	{
		soundCardVaild = true;
	}

	qDebug() << this->format.channelCount();
	qDebug() << this->format.isValid();
	qDebug() << this->info.deviceName();

	mAudioInput = new QAudioInput(info, format);//
	//设置缓存区*2
	mAudioInput->setBufferSize(48000 << 3);
	io = mAudioInput->start();
}

void soundCard:: read()
{
//    qDebug()<<"mReadSize:"<<mReadSize;// 204800
//    mReadSize = 48000;
    char *buf = new char[mReadSize];

//    qDebug()<<mAudioInput->bufferSize();

    for (;;)
    {
        //qDebug() << mAudioInput->bytesReady() ;//192000

        if (mAudioInput->bytesReady() < mReadSize) //mAudioInput->bytesReady():max 76800 then resize 96000
        {
            //qDebug()<<"mAudioInput->bytesReady():"<<mAudioInput->bytesReady();
            QThread::msleep(1);
            continue;
        }
        size = 0;
        while (size != mReadSize)
        {
            int len = io->read(buf + size, mReadSize - size);
            if (len < 0) break;
            //qDebug()<<"buf[0]:"<<buf[0];
            size += len;
            //qDebug()<<"size:"<<size;
        }
        if (size != mReadSize)continue;
        break;
    }

    //mReadSize:8192   mSamples:1024   mChannels:2  buf:8192  pRawData = mSamples * mChannels = 1024 * 2 = 2048

    memcpy(pRawData, buf, mReadSize);


	//for (int i = 2; i < 76800 -4; i+=4)
	//{
	//	qDebug() << "pRawData[" << QString::number(i) << "] y:" << (float)pRawData[(i)];
	//}
    //qDebug() << "mAudioInput:" << mAudioInput->format();
//    qDebug()<<"pRawData[0]:"<<pRawData[0];
    delete buf;

}
void soundCard::setDevName(QString name)
{
    this->mDevName = name;
    this->mDevNameArray = name.toLatin1();
    qDebug()<<"mDevName"<<mDevName;
}
void soundCard::stop()
{
    mAudioInput->stop();

}
void soundCard::destroy()
{

}
soundCard::~soundCard()
{
    if (rawFile.isOpen()) { rawFile.close(); };
   

    this->destroy();
    if(this->pRawData != nullptr)
    {
        delete [] this->pRawData;
        this->pRawData = nullptr;
    }
}
QString soundCard::getDevName()
{
    return this->mDevName;

};
