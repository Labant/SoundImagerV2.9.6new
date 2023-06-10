#pragma once
/**!!基于Qt的封装音频
 * Qt vision:5.15.2
 * 
 * 
 * @parameter
 * 
 * @for:数据一次性转
 * 
 */

#include <qobject.h>
#include <QDir>
#include <QFile>
#include <QDebug>

class EncapsloationWAVAudio :public QObject
{
public:
	EncapsloationWAVAudio();
	~EncapsloationWAVAudio();
private:

	static void setWavHeader(int Fs, int bitsber_sample, int channle_number);
	static void setFilePath(QString generate_wavfile_path);

public:
	static void RAWDATA2WAVFIlE(QByteArray raw_data, int Fs, int bitsber_sample, int channle_number, QString generate_wavfile_path, QString generate_file_name);
};

