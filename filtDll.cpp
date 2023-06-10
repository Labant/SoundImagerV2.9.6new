#include "filtDll.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

filtDll::filtDll(QObject* parent)
{
	this->init();
}

filtDll::~filtDll()
{
}

void filtDll::filtDataAndWrite(double* sig_p, double* sig_x, double* sig_y, double* sig_z, double f_low, double f_high, double fs)
{
	if (0)
	{
		//ÂË²¨Ç°
		QFile uwsFile;
		uwsFile.setFileName(("uwSFile1.csv"));

		if (!uwsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			qDebug() << "uwsFile is not opened;";
			return ;
		}
		QTextStream uwSFile(&uwsFile);

		for (int i = 0; i < 4800; i++)
		{
			uwSFile << QString::number(sig_p[i], 'f', 16).toStdString().c_str();
			if (i < 4799)uwSFile << ",";
		}
		uwSFile << "\n";

		for (int i = 0; i < 4800; i++)
		{
			uwSFile << QString::number(sig_x[i], 'f', 16).toStdString().c_str();
			if (i < 4799)uwSFile << ",";
		}
		uwSFile << "\n";

		for (int i = 0; i < 4800; i++)
		{
			uwSFile << QString::number(sig_y[i], 'f', 16).toStdString().c_str();
			if (i < 4799)uwSFile << ",";
		}

		uwSFile << "\n";

		for (int i = 0; i < 4800; i++)
		{
			uwSFile << QString::number(sig_z[i], 'f', 16).toStdString().c_str();
			if (i < 4799)uwSFile << ",";
		}

		uwsFile.close();
	}

	filt(sig_p, sig_x, sig_y, sig_z, f_low, f_high, fs, this->sig_filt_p, this->sig_filt_x, this->sig_filt_y, this->sig_filt_z);
	
	memset(sig_p, 0, sizeof(double) * 4800);
	memset(sig_x, 0, sizeof(double) * 4800);
	memset(sig_y, 0, sizeof(double) * 4800);
	memset(sig_z, 0, sizeof(double) * 4800);
	memcpy(sig_p, sig_filt_p, sizeof(double) * 4800);
	memcpy(sig_x, sig_filt_x, sizeof(double) * 4800);
	memcpy(sig_y, sig_filt_y, sizeof(double) * 4800);
	memcpy(sig_z, sig_filt_z, sizeof(double) * 4800);

	if (0)
	{
		//ÂË²¨ºó
		QFile uhSFile;
		uhSFile.setFileName(("uhSFile.csv"));

		if (!uhSFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			qDebug() << "uhSFile is not opened;";
			return ;
		}
		QTextStream hSFile1(&uhSFile);

		for (int i = 0; i < 4800; i++)
		{
			hSFile1 << QString::number(sig_p[i], 'f', 16).toStdString().c_str();
			if (i < 4799)hSFile1 << ",";
		}
		hSFile1 << "\n";

		for (int i = 0; i < 4800; i++)
		{
			hSFile1 << QString::number(sig_x[i], 'f', 16).toStdString().c_str();
			if (i < 4799)hSFile1 << ",";
		}
		hSFile1 << "\n";

		for (int i = 0; i < 4800; i++)
		{
			hSFile1 << QString::number(sig_y[i], 'f', 16).toStdString().c_str();
			if (i < 4799)hSFile1 << ",";
		}

		hSFile1 << "\n";

		for (int i = 0; i < 4800; i++)
		{
			hSFile1 << QString::number(sig_z[i], 'f', 16).toStdString().c_str();
			if (i < 4799)hSFile1 << ",";
		}



		uhSFile.close();
	}
}

void filtDll::init()
{
}
