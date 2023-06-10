#include "EncapsloationWAVAudio.h"

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
}WavFileHeader;


EncapsloationWAVAudio::EncapsloationWAVAudio()
{
}

EncapsloationWAVAudio::~EncapsloationWAVAudio()
{
}

void EncapsloationWAVAudio::setWavHeader(int Fs, int bitsber_sample, int channle_number)
{
	//Wav头
	qstrcpy(WavFileHeader.RiffName, "RIFF");
	qstrcpy(WavFileHeader.WavName, "WAVE");
	qstrcpy(WavFileHeader.FmtName, "fmt ");//加空格是为满足4字节对齐
	qstrcpy(WavFileHeader.DATANAME, "data");

	// 表示 FMT块 的长度
	WavFileHeader.nFmtLength = 16;//16表示数据块中没有附加信息，18是有附加信息
	// 表示 按照PCM 编码; 
	WavFileHeader.nAudioFormat = 1;//大于1时表示有压缩
	// 声道数目;
	WavFileHeader.nChannleNumber = channle_number;//1为单通道（声道），2双通道（声道）
	// 采样频率;
	WavFileHeader.nSampleRate = Fs;//每秒的样本数

	// nBytesPerSample 和 nBytesPerSecond这两个值通过设置的参数计算得到;
	// 数据块对齐单位(每个采样需要的字节数 = 通道数 × 每次采样得到的样本数据位数 / 8 )
	WavFileHeader.nBytesPerSample = channle_number * bitsber_sample / 8;//数据块对齐单位(每个采样需要的字节数)

	// 波形数据传输速率
	// (每秒平均字节数 = 采样频率 × 通道数 × 每次采样得到的样本数据位数 / 8  = 采样频率 × 每个采样需要的字节数 )
	WavFileHeader.nBytesPerSecond = Fs * channle_number * bitsber_sample / 8;//波形数据传输速率（每秒平均字节数）

	// 每次采样得到的样本数据位数;
	WavFileHeader.nBitsPerSample = bitsber_sample;

}


void EncapsloationWAVAudio::setFilePath(QString generate_wavfile_path)
{
	if (!QDir(generate_wavfile_path).exists())
	{
		QDir().mkdir(generate_wavfile_path);
	}

}

void EncapsloationWAVAudio::RAWDATA2WAVFIlE(QByteArray raw_data, int Fs, int bitsber_sample, int channle_number, QString generate_wavfile_path, QString generate_file_name)
{
	qDebug() << "raw_data.len:" << raw_data.length();

	//设置头
	setWavHeader(Fs, bitsber_sample, channle_number);
	int nSize = sizeof(WavFileHeader);
	WavFileHeader.nRiffLength = raw_data.size() - 8 + nSize;//Riff头之后的所有数据,即riff头占8B,总头加数据长度-8
	WavFileHeader.nDataLength = raw_data.size();	//dataLength

	//创建目录
	setFilePath(generate_wavfile_path);
	//写入文件
	QFile wav_file(generate_wavfile_path + "/" + generate_file_name);
	Q_ASSERT(wav_file.open(QIODevice::WriteOnly | QIODevice::Truncate));
	// 先将wav文件头信息写入，再将音频数据写入;
	int writeLen = 0;
	writeLen = wav_file.write((const char*)&WavFileHeader, nSize);


	writeLen += wav_file.write(raw_data);
	qDebug() << "writeLen:" << writeLen;
	wav_file.close();

}
