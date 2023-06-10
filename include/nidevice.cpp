#include "nidevice.h"
#include <stdio.h>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QDateTime>

//######################################
int32       error = 0;

#define DAQmxErrChk(functionCall) DAQmxFailed(error=(functionCall)) //if( DAQmxFailed(error=(functionCall)) ) goto Error; else;
//if( DAQmxFailed(error=(functionCall))) goto Error; else;

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

void NiDevice::init()
{
    this->mReadSize = this->mChannels * this->mSamples; //每次采集的数据量
    this->pRawData = new float64[this->mReadSize];
    //this->pRawData = new float64[2401];

    qDebug()<<"NI:"<<this->mSamples<<","<<this->mReadSize;

//    qDebug()<<"NiDevice init start";

    qDebug() << DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
//    DAQmxErrChk(DAQmxCreateAIMicrophoneChan(taskHandle,this->mDevNameArray.data(), "", DAQmx_Val_Cfg_Default, DAQmx_Val_Pascals, 50, 120.0, DAQmx_Val_Internal, 0.004, NULL)); //
    qDebug() << DAQmxErrChk (DAQmxCreateAIVoltageChan(taskHandle,this->mDevNameArray.data(),"",DAQmx_Val_Cfg_Default,-5.0,5.0,DAQmx_Val_Volts,NULL));  //采样的通道数据存放在mNiRawData，所以mNiRawData需要匹配
    //qDebug() << DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3,cDAQ9185-1F29BF5Mod2/ai0:3,cDAQ9185-1F29BF5Mod3/ai0:3", "", DAQmx_Val_Cfg_Default, -5.0, 5.0, DAQmx_Val_Volts, NULL));  //采样的通道数据存放在mNiRawData，所以mNiRawData需要匹配
    //qDebug() << DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3", "", DAQmx_Val_Cfg_Default, -5.0, 5.0, DAQmx_Val_Volts, NULL));  //采样的通道数据存放在mNiRawData，所以mNiRawData需要匹配

	qDebug() << DAQmxErrChk( DAQmxSetAICoupling (taskHandle,this->mDevNameArray.data(),DAQmx_Val_AC));   //设置通道采集AC信号,默认采集DC信号
    //qDebug() << DAQmxErrChk(DAQmxSetAICoupling(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3,cDAQ9185-1F29BF5Mod2/ai0:3,cDAQ9185-1F29BF5Mod3/ai0:3", DAQmx_Val_AC));   //设置通道采集AC信号,默认采集DC信号
    //qDebug() << DAQmxErrChk(DAQmxSetAICoupling(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3", DAQmx_Val_AC));   //设置通道采集AC信号,默认采集DC信号

    qDebug() << DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", this->mRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, this->mReadSize));

    qDebug() << DAQmxErrChk (DAQmxSetAITermCfg(taskHandle,this->mDevNameArray.data(),DAQmx_Val_PseudoDiff));
    //qDebug() << DAQmxErrChk(DAQmxSetAITermCfg(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3,cDAQ9185-1F29BF5Mod2/ai0:3,cDAQ9185-1F29BF5Mod3/ai0:3", DAQmx_Val_PseudoDiff));
    //qDebug() << DAQmxErrChk(DAQmxSetAITermCfg(taskHandle, "cDAQ9185-1F29BF5Mod1/ai0:3", DAQmx_Val_PseudoDiff));

    /*********************************************/
    // DAQmx Start Code
    /*********************************************/
//    DAQmxErrChk(DAQmxStartTask(taskHandle));

    //qDebug()<<DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
    //qDebug() << DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle,
    //    this->mDevNameArray.data(),
    //    "",
    //    DAQmx_Val_Cfg_Default,
    //    -5.0,
    //    5.0,
    //    DAQmx_Val_Volts,
    //    NULL));  //采样的通道数据存放在mNiRawData，所以mNiRawData需要匹配

    //qDebug() << DAQmxErrChk(DAQmxCreateAIMicrophoneChan(taskHandle,
    //    m_strMicArray.data(),
    //    "",
    //    DAQmx_Val_PseudoDiff,
    //    DAQmx_Val_Pascals,
    //    50,
    //    120,
    //    DAQmx_Val_Internal,
    //    0.002,
    //    NULL));  //采样的通道数据存放在mNiRawData，所以mNiRawData需要匹配

    ////DAQmxErrChk(DAQmxSetAIMicrophoneSensitivity(taskHandle, m_strMicArray, 1500));
    //qDebug() << DAQmxErrChk(DAQmxSetAICoupling(taskHandle, this->mDevNameArray.data(), DAQmx_Val_AC));   //设置通道采集AC信号,默认采集DC信号
    //qDebug() << DAQmxErrChk(DAQmxSetAICoupling(taskHandle, this->m_strMicArray.data(), DAQmx_Val_AC));   //设置通道采集AC信号,默认采集DC信号
    //qDebug() << DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", this->mRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, this->mSamples));
    //qDebug() << DAQmxErrChk(DAQmxSetAITermCfg(taskHandle, this->mDevNameArray.data(), DAQmx_Val_PseudoDiff));
}

void NiDevice::start()
{
    /*********************************************/
    // DAQmx Start Code
    /*********************************************/
    qDebug() << DAQmxErrChk(DAQmxStartTask(taskHandle));
}

void NiDevice::stop()
{
    /*********************************************/
    // DAQmx Stop Code
    /*********************************************/
    DAQmxStopTask(taskHandle);
}

NiDevice::NiDevice()
{

}

NiDevice::~NiDevice()
{
    this->destroy(); //销毁资源
    if(this->pRawData != nullptr)
    {
        delete [] this->pRawData;
        this->pRawData = nullptr;
    }
}

void NiDevice::setDevName(const QString& name)
{
    this->mDevNameArray = name.toLatin1();

    //m_strMicArray = mic.toLatin1();
    qDebug() << "mDevNameArray:" << mDevNameArray;
    //qDebug() << "m_strMicArray:" << m_strMicArray;
}

void NiDevice::setChannelNum(int num)
{
    this->mChannels = num;
}

void NiDevice::setSamples(int sample)
{
    this->mSamples = sample;
}

void NiDevice::setRate(double rate)
{
    this->mRate = rate*1000;

}

void NiDevice::read()
{

    //qDebug() << "前：this->pNiDevice->pRawData:" << this->pRawData[21650];
    QMutexLocker m_lockerHelper(&m_lock_pRawData);
    DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, mSamples, 1, DAQmx_Val_GroupByChannel, this->pRawData, mSamples * mChannels, &sampsPerChanRead, NULL));//DAQmx_Val_GroupByChannel,DAQmx_Val_GroupByScanNumber

	//qDebug() << "后：this->pNiDevice->pRawData:" << this->pRawData[21650];


}

void NiDevice::destroy()
{
    if (taskHandle != 0)
    {
        /*********************************************/
        // DAQmx Stop Code
        /*********************************************/
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
        taskHandle = 0;
    }
}

QString NiDevice::getNiDeviceName()
{
	QStringList devLists;


	constexpr int charLen = 500; //获取的字符创长度
	char devstr[charLen];
	QStringList devBuffList; //缓存设备
	DAQmxGetSysDevNames(devstr, charLen);
	QString tempStr = QString(QLatin1String(devstr)).remove(QRegExp("\\s")); //去除字符串中的空格

	devBuffList = tempStr.split(",");

	QString strNiDeviceName;
	QString strDevName;


	for (int i = 0; i < devBuffList.size(); i++)
	{
		if (devBuffList.at(i).contains("Mod", Qt::CaseSensitive))
		{
			devLists << "devBuffList:" << devBuffList.at(i);
		}
	}

	if (devLists.size())
	{
		if (devLists.size() == 1)
		{
			strNiDeviceName = devLists.at(0) + "/ai0:3";
			strDevName = strNiDeviceName;
		}
		else
		{
			for (int i = 0; i < devLists.size(); ++i)
			{

				strNiDeviceName += QString("%1/ai0:3").arg(devLists.at(i));
				strDevName += QString("%1/ai0:3").arg(devLists.at(i));
				if (i != devLists.size() - 1)
				{
					strNiDeviceName += ",";
					strDevName += ",";
				}
			}

		}
		qDebug() << "strNiDeviceName:" << strNiDeviceName;
		qDebug() << "strDevName:" << strDevName;
	}
	else
	{
		//        return -1;
	}

	qDebug() << "devBuffList:" << devBuffList;

	if (devBuffList.length() > 1)
	{
		//qDebug()<< strNiDeviceName;

		return strNiDeviceName.remove("devBuffList:/ai0:3,", Qt::CaseSensitive);
	}

	//creatSound(devBuffList.at(1)+"/ai0:4", devBuffList.at(1) + "/ai0:4");


	return "";
}



