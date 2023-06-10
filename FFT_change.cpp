#include "FFT_change.h"
#include <QDebug>
#include "qmath.h"


FFT_change::FFT_change()
{

}

FFT_change::~FFT_change()
{
}

//实际计算频域的接口
void FFT_change::Time2Frequency(double p[] , double x[], double y[], double z[], int mSample)
{

	fftw_plan _1d_fft_p, _1d_fft_x, _1d_fft_y, _1d_fft_z;
	//fftw_plan _1d_fft_y, _1d_fft_z;
	//int test_int = mSample;
	fftw_complex* in_p, * in_x, * in_y, * in_z, * out_p, * out_x, * out_y, * out_z;
	//fftw_complex* in_p, * out_p;
	in_p = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	in_x = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	in_y = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	in_z = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	out_p = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	out_x = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	out_y = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);
	out_z = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * mSample);

	//载入数据
	for (int i = 0; i < mSample; i++)
	{
		//p
		in_p[i][0] = p[i];
		in_p[i][1] = 0;
		//x
		in_x[i][0] = x[i];
		in_x[i][1] = 0;
		//y
		in_y[i][0] = y[i];
		in_y[i][1] = 0;
		//z
		in_z[i][0] = z[i];
		in_z[i][1] = 0;
	}

	_1d_fft_p = fftw_plan_dft_1d(mSample, in_p, out_p, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(_1d_fft_p);
	_1d_fft_x = fftw_plan_dft_1d(mSample, in_x, out_x, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(_1d_fft_x);
	_1d_fft_y = fftw_plan_dft_1d(mSample, in_y, out_y, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(_1d_fft_y);
	_1d_fft_z = fftw_plan_dft_1d(mSample, in_z, out_z, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(_1d_fft_z);

	//分析数据
	for (int i = 0; i < 2401; i++)
	{
		//qDebug() << "out_p[" << QString::number(i) << "].real:" << QString::number(out_p[i][0], 'f', 6);
		//qDebug() << "out_p[" << QString::number(i) << "].image:" << QString::number(out_p[i][1], 'f', 6);

		fft_p[i] = qSqrt(qPow(out_p[i][0], 2) + qPow(out_p[i][1], 2));
		fft_x[i] = qSqrt(qPow(out_x[i][0], 2) + qPow(out_x[i][1], 2));
		fft_y[i] = qSqrt(qPow(out_y[i][0], 2) + qPow(out_y[i][1], 2));
		fft_z[i] = qSqrt(qPow(out_z[i][0], 2) + qPow(out_z[i][1], 2));

	}

	fftw_destroy_plan(_1d_fft_p);
	fftw_destroy_plan(_1d_fft_x);
	fftw_destroy_plan(_1d_fft_y);
	fftw_destroy_plan(_1d_fft_z);
	fftw_free(in_p);
	fftw_free(in_x);
	fftw_free(in_y);
	fftw_free(in_z);
	fftw_free(out_p);
	fftw_free(out_x);
	fftw_free(out_y);
	fftw_free(out_z);
}

