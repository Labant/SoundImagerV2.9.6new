#pragma once
#include <QVector>
#include <QQueue>
#include "GlobalConfig.hpp"


namespace MYSTRUCT
{
    namespace RAW2WAV
    {
        struct WAVFILEHEADER
        {
            // RIFF 头
            char RiffName[4];
            unsigned long nRiffLength;

            // 数据类型标识符
            char WavName[4];

            // 格式块中的块头
            char FmtName[4];
            unsigned long nFmtLength;

            // 格式块中的块数据
            unsigned short nAudioFormat;
            unsigned short nChannleNumber;
            unsigned long nSampleRate;
            unsigned long nBytesPerSecond;
            unsigned short nBytesPerSample;
            unsigned short nBitsPerSample;

            // 数据块中的块头
            char    DATANAME[4];
            unsigned long   nDataLength;
        };

    };

    //用于plot显示那一通道数据
	namespace PXYZ
	{
		enum pxyz
		{
			P = 0, X, Y, Z
		};
	}

    //选择使用那种采集卡
    enum CAPTURETYPE
    {
        SoundCard = 0,
        NICaptureCard,
        VKCaptureCard
    };

    //时序结构体。
    struct TimeSequence
    {

			QQueue<QVector<double>> mTimeData_P; //时域 YAxis
            QQueue<QVector<double>> mSequence_P;//时域flag XAxis

            QQueue<QVector<double>> mTimeData_X; //时域 YAxis
            QQueue<QVector<double>> mSequence_X;//时域flag XAxis

            QQueue<QVector<double>> mTimeData_Y; //时域 YAxis
            QQueue<QVector<double>> mSequence_Y;//时域flag XAxis

            QQueue<QVector<double>> mTimeData_Z; //时域 YAxis
            QQueue<QVector<double>> mSequence_Z;//时域flag XAxis

    };




}