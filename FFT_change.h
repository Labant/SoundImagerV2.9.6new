#pragma once
#include <qobject.h>
#include "fftw3.h"
#include <QThread>
#include "GlobalConfig.hpp"

class FFT_change :public QObject
{

public:
	//���캯��
	FFT_change();
	//��������
	~FFT_change();

public:
	//�����ⲿ���ʵĽӿ�
	double fft_p[2400] = {0}, fft_x[2400] = { 0 }, fft_y[2400] = { 0 }, fft_z[2400] = { 0 };
private:
	int mSample = SAMPLE / 10;

public:
	//ʵ�ʼ���Ƶ��Ľӿ�
	void Time2Frequency(double [],double [],double[], double[],int mSample);




};

