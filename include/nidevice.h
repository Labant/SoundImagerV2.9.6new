#include <QThread>
#include <QMutex>
#include "NIDAQmx.h"
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QMutexLocker>
extern float64  nidata[];

//class NiDevice:public QThread
class NiDevice:public QObject
{
    Q_OBJECT
public:
    NiDevice();
    ~NiDevice();

    void setDevName(const QString& name);
    void setChannelNum(int num);     //设置通道个数
    void setSamples(int sample);     //设置采样数
    void setRate(double rate);        //设置采样率
    void init();                     //设置好上面的参数后再调用init

    void start();
    void stop();
    void read();
    void destroy();
    static QString getNiDeviceName();
public:
    float64 *pRawData = nullptr;         //采集的原始数据
    QMutex m_lock_pRawData;
signals:
    void updateRawData();

private:
    int     mChannels = 0;
    int32   mSamples = 0;
    float64 mRate = 0;
    uInt32  mReadSize = 0;
    int32   sampsPerChanRead;

    QMutex m_lock;
    TaskHandle  taskHandle;
    QString     mDevName = "";
    QByteArray  mDevNameArray;
    char        *mDevNameChar;
    QByteArray  m_strMicArray;
  
public slots:
};


