#ifndef SOUNDCARD_H
#define SOUNDCARD_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QMessageBox>
#include <QAudioInput>
#include <QFile>
#include <QThread>
#include "MyStruct.h"

using namespace MYSTRUCT::RAW2WAV;
#define RAW_RECORD_FILENAME "E:/lvs/04_Qt_test/test_customPlot_hreatMap/audio/Hide_48000Hz.raw"         // 录音文件名;
#define WAV_RECORD_FILENAME "E:/lvs/04_Qt_test/test_customPlot_hreatMap/audio/test.wav"         // 录音文件转wav格式文件名;

class soundCard : public QObject
{
    Q_OBJECT
public:
    explicit soundCard(QObject *parent = nullptr);
    ~soundCard();

public:
     void read();
     void stop();
     void start(QString);
     void setDevName(QString );
     void init();
     void destroy();
     void initFmt();
private://fun


    void initConnected();
    QAudioDeviceInfo setDefaultInputAudioDeviceInfo();
    QString getDevName();


public:
    float* pRawData;
    QIODevice* io;
    //用于检测声卡是否可用
    bool soundCardVaild = true;
private:
    QAudioFormat format;
    QAudioDeviceInfo info;
    QAudioInput *mAudioInput;

    QString mDevName;
    QByteArray  mDevNameArray;

    int     mReadSize;
//    float   mRate = 25600.0;       //25.6kHz
    //int     mChannels = 2;
    int     mChannels = 4;
    int     mSampleSize = 32;
//    int     mSamples = 1024;
    //int     mSamples = 4800;
    int     mSamples = 4810;
//    int     mSamples = 25600;
    int     size = 0;

    //WAV头文件
    WAVFILEHEADER wavFileHeader;
    QFile rawFile;
    QFile WAVFile;
signals:

public slots:
};

#endif // SOUNDCARD_H
