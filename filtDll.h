#pragma once
#include <qobject.h>
#include "filt.h"

/************************************************************************/
/* extern void filt(const double sig_p[4800], const double sig_x[4800],
	const double sig_y[4800], const double sig_z[4800],
	double f_low, double f_high, double fs,
	double sig_filt_p[4800], double sig_filt_x[4800],
	double sig_filt_y[4800], double sig_filt_z[4800]);                                                                     */
/************************************************************************/

class filtDll :public QObject
{
public:
	explicit filtDll(QObject* parent = nullptr);
	~filtDll();

	void filtDataAndWrite(double *sig_p, double* sig_x, double* sig_y, double* sig_z, double f_low, double f_high, double fs);
protected:

private:
	void init();

public:

protected:

private:
	double sig_filt_p[4800], sig_filt_x[4800],sig_filt_y[4800], sig_filt_z[4800];
};

