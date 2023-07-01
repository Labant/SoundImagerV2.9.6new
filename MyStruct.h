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
            // RIFF ͷ
            char RiffName[4];
            unsigned long nRiffLength;

            // �������ͱ�ʶ��
            char WavName[4];

            // ��ʽ���еĿ�ͷ
            char FmtName[4];
            unsigned long nFmtLength;

            // ��ʽ���еĿ�����
            unsigned short nAudioFormat;
            unsigned short nChannleNumber;
            unsigned long nSampleRate;
            unsigned long nBytesPerSecond;
            unsigned short nBytesPerSample;
            unsigned short nBitsPerSample;

            // ���ݿ��еĿ�ͷ
            char    DATANAME[4];
            unsigned long   nDataLength;
        };

    };

    //����plot��ʾ��һͨ������
	namespace PXYZ
	{
		enum pxyz
		{
			P = 0, X, Y, Z
		};
	}

    //ѡ��ʹ�����ֲɼ���
    enum CAPTURETYPE
    {
        SoundCard = 0,
        NICaptureCard,
        VKCaptureCard
    };

    //ʱ��ṹ�塣
    struct TimeSequence
    {

			QQueue<QVector<double>> mTimeData_P; //ʱ�� YAxis
            QQueue<QVector<double>> mSequence_P;//ʱ��flag XAxis

            QQueue<QVector<double>> mTimeData_X; //ʱ�� YAxis
            QQueue<QVector<double>> mSequence_X;//ʱ��flag XAxis

            QQueue<QVector<double>> mTimeData_Y; //ʱ�� YAxis
            QQueue<QVector<double>> mSequence_Y;//ʱ��flag XAxis

            QQueue<QVector<double>> mTimeData_Z; //ʱ�� YAxis
            QQueue<QVector<double>> mSequence_Z;//ʱ��flag XAxis

    };




}