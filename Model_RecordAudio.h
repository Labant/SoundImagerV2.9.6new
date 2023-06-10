#pragma once
#include <qobject.h>
#include <QSharedPointer>
#include "EncapsloationWAVAudio.h"
#include "GenerateLvsFloat64ToInt.hh"
#include <QTime>
#include <QDateTime>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QFile>

class Model_RecordAudio :public QObject
{
public:
	Model_RecordAudio(QObject *o = nullptr);
	~Model_RecordAudio();
	//友元
	friend class MainViewControl;

private:
	void initLayout();
	void addRawData(QList<double>&);
	void generateWAVFile(int Fs, int bitsber_sample, int channle_number, QString generate_wavfile_path, QString generate_file_name);
	void setFs(int);
	void setBitsberSample(int);
	void setChannleNumber(int);
	void setWavFilePath(QString);
	void setWavFileName(QString);
	QString generateFileName();
public slots:
	void m_slot_WavTimeout(QString generate_wavfile_path);
private:
	//存储原始数据
	QByteArray mRawData;
	QList<double> mmRawData;
	QQueue<QList<double>> mmmRawData;

	int fs;
	int bitsberSample;
	int channleNumber;
	QString wavFilePath;
	QString wavFileName;
	////录音定时器
	//QTimer* m_timer_record = nullptr;
};

