#include "Model_RecordAudio.h"

Model_RecordAudio::Model_RecordAudio(QObject* o)
{
}

Model_RecordAudio::~Model_RecordAudio()
{
}

void Model_RecordAudio::initLayout()
{
	//m_timer_record = new QTimer;

}

void Model_RecordAudio::addRawData(QList<double>&  data)
{
	mmmRawData.append(data);
}

void Model_RecordAudio::generateWAVFile(int Fs, int bitsber_sample, int channle_number, QString generate_wavfile_path, QString generate_file_name)
{
	qDebug()<<"111:" << double2Int(mmmRawData.at(0).at(0));

	//文件地址
	QString _filePath = generateFileName();
	QString _specific = generate_wavfile_path + "/" + _filePath;
	//make dir
	QDir().mkdir(_specific);

	//方案一 借助写入文件固定2字节位数，可用
	QFile file(_specific +"/RawData.raw");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
		qDebug() << "mmmRawData.txt open failed!";

	while (!mmmRawData.isEmpty())
	{
		QList<double> tem = mmmRawData.dequeue();
		for (int i = 0; i < tem.length(); ++i)
		{
			int ii = double2Int(tem.at(i)) * 60;//放大音量
			file.write((char*)&ii, 2);

		}
	}
	file.close();
	if (!file.open(QIODevice::ReadWrite))
		qDebug() << "mmmRawData.txt open failed!";
	QByteArray alldata = file.readAll();
	file.close();
	EncapsloationWAVAudio::RAWDATA2WAVFIlE(alldata, Fs, bitsber_sample, channle_number, _specific, _filePath + ".wav");

	////方案二 直接存储，不好用。
	//while (!mmmRawData.isEmpty())
	//{
	//	QList<double> tem = mmmRawData.dequeue();
	//	for (int i = 0; i < tem.length(); ++i)
	//	{
	//		int iii = double2Int(tem.at(i));
	//		mRawData.append((char*)&iii);
	// 
	// 
	//	}
	//}
	//EncapsloationWAVAudio::RAWDATA2WAVFIlE(mRawData, 48000, 16, 4, generateFileName(), generateFileName() + ".wav");



	//test
	//mRawData.clear();
	//QFile pppFile("E:\\lvs\\04_Qt_test\\test_customPlot_hreatMap_sphere\\audio\\test_Ppp.raw");
	//if (!pppFile.open(QIODevice::ReadOnly))
	//	qDebug() << "pppFile.txt open failed!";
	//QByteArray temByt_P = pppFile.readAll();
	//pppFile.close();
	//EncapsloationWAVAudio::RAWDATA2WAVFIlE(temByt_P, 25600, 16, 1, QDir::currentPath() + "/audio_", QString("temByt_P_lvs_PWAV.wav"));

}

void Model_RecordAudio::setFs(int Fs)
{
	fs = Fs;
}

void Model_RecordAudio::setBitsberSample(int bitsber_sample)
{
	bitsberSample = bitsber_sample;
}

void Model_RecordAudio::setChannleNumber(int channle_number)
{
	channleNumber = channle_number;
}

void Model_RecordAudio::setWavFilePath(QString generate_wavfile_path)
{
	wavFilePath = generate_wavfile_path;
}

void Model_RecordAudio::setWavFileName(QString generate_file_name)
{
	wavFileName = wavFileName;
}

QString Model_RecordAudio::generateFileName()
{
	return QString("%1%2%3%4%5%6").arg(QDate::currentDate().year(), 2, 10, QLatin1Char('0')).arg(QDate::currentDate().month(), 2, 10, QLatin1Char('0'))
		.arg(QDate::currentDate().day(), 2, 10, QLatin1Char('0')).arg(QTime::currentTime().hour(), 2, 10, QLatin1Char('0'))
		.arg(QTime::currentTime().minute(), 2, 10, QLatin1Char('0')).arg(QTime::currentTime().second(), 2, 10, QLatin1Char('0'));
}
void Model_RecordAudio::m_slot_WavTimeout(QString generate_wavfile_path)
{
	//qDebug() << "m_slot_WavTimeout:" << this->thread();
	generateWAVFile(fs, bitsberSample, channleNumber, generate_wavfile_path, "");
}