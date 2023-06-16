#include "EncapsloationWAVAudio.h"

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
}WavFileHeader;


EncapsloationWAVAudio::EncapsloationWAVAudio()
{
}

EncapsloationWAVAudio::~EncapsloationWAVAudio()
{
}

void EncapsloationWAVAudio::setWavHeader(int Fs, int bitsber_sample, int channle_number)
{
	//Wavͷ
	qstrcpy(WavFileHeader.RiffName, "RIFF");
	qstrcpy(WavFileHeader.WavName, "WAVE");
	qstrcpy(WavFileHeader.FmtName, "fmt ");//�ӿո���Ϊ����4�ֽڶ���
	qstrcpy(WavFileHeader.DATANAME, "data");

	// ��ʾ FMT�� �ĳ���
	WavFileHeader.nFmtLength = 16;//16��ʾ���ݿ���û�и�����Ϣ��18���и�����Ϣ
	// ��ʾ ����PCM ����; 
	WavFileHeader.nAudioFormat = 1;//����1ʱ��ʾ��ѹ��
	// ������Ŀ;
	WavFileHeader.nChannleNumber = channle_number;//1Ϊ��ͨ������������2˫ͨ����������
	// ����Ƶ��;
	WavFileHeader.nSampleRate = Fs;//ÿ���������

	// nBytesPerSample �� nBytesPerSecond������ֵͨ�����õĲ�������õ�;
	// ���ݿ���뵥λ(ÿ��������Ҫ���ֽ��� = ͨ���� �� ÿ�β����õ�����������λ�� / 8 )
	WavFileHeader.nBytesPerSample = channle_number * bitsber_sample / 8;//���ݿ���뵥λ(ÿ��������Ҫ���ֽ���)

	// �������ݴ�������
	// (ÿ��ƽ���ֽ��� = ����Ƶ�� �� ͨ���� �� ÿ�β����õ�����������λ�� / 8  = ����Ƶ�� �� ÿ��������Ҫ���ֽ��� )
	WavFileHeader.nBytesPerSecond = Fs * channle_number * bitsber_sample / 8;//�������ݴ������ʣ�ÿ��ƽ���ֽ�����

	// ÿ�β����õ�����������λ��;
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

	//����ͷ
	setWavHeader(Fs, bitsber_sample, channle_number);
	int nSize = sizeof(WavFileHeader);
	WavFileHeader.nRiffLength = raw_data.size() - 8 + nSize;//Riffͷ֮�����������,��riffͷռ8B,��ͷ�����ݳ���-8
	WavFileHeader.nDataLength = raw_data.size();	//dataLength

	//����Ŀ¼
	setFilePath(generate_wavfile_path);
	//д���ļ�
	QFile wav_file(generate_wavfile_path + "/" + generate_file_name);
	Q_ASSERT(wav_file.open(QIODevice::WriteOnly | QIODevice::Truncate));
	// �Ƚ�wav�ļ�ͷ��Ϣд�룬�ٽ���Ƶ����д��;
	int writeLen = 0;
	writeLen = wav_file.write((const char*)&WavFileHeader, nSize);


	writeLen += wav_file.write(raw_data);
	qDebug() << "writeLen:" << writeLen;
	wav_file.close();

}
