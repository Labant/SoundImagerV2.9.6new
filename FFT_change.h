#pragma once
#include <qobject.h>
#include "fftw3.h"
#include <QThread>
#include "GlobalConfig.hpp"

class FFT_change :public QObject
{

public:
	//构造函数
	FFT_change();
	//析构函数
	~FFT_change();

public:
	//用于外部访问的接口
	double fft_p[2400] = {0}, fft_x[2400] = { 0 }, fft_y[2400] = { 0 }, fft_z[2400] = { 0 };
private:
	int mSample = SAMPLE / 10;

public:
	//实际计算频域的接口
	void Time2Frequency(double [],double [],double[], double[],int mSample);




};

