#include "CalibrationDll.h"
#include <QFile>
#include <QDebug>
#include "calibration.h"

CalibrationDll::CalibrationDll(QString rsp, QObject* parent)
{
	readRSPDataAndWrite(rsp);
}

CalibrationDll::~CalibrationDll()
{
}


bool CalibrationDll::readRSPDataAndWrite(QString rspFilePath)
{
	QFile mRspFile(rspFilePath);
	if (!mRspFile.open(QIODevice::ReadOnly))
		return false;

	QByteArray allData = mRspFile.readAll();

	QStringList dataList = QString::fromLocal8Bit(allData).split("\r\n");
	QStringList fit_magList, fit_phaseList, fit_xyList, fit_zyList;
	//qDebug() << "dataList.len:" << dataList.length();

	fit_magList = dataList.at(0).split(" ");
	fit_phaseList = dataList.at(1).split(" ");
	fit_xyList = dataList.at(2).split(" ");
	fit_zyList = dataList.at(3).split(" ");


	for (int i = 0;i < 4800;i++)
	{
		//qDebug() <<QString::number(i)<<" " << fit_magList.at(i);

		fit_mag_rsp[i] = fit_magList.at(i).toDouble();
		fit_phase_rsp[i] = fit_phaseList.at(i).toDouble();
		fit_xy_rsp[i] = fit_xyList.at(i).toDouble();
		fit_zy_rsp[i] = fit_zyList.at(i).toDouble();

	}
	//qDebug() << "fit_mag_rsp[0]:" << fit_mag_rsp[0];
	//qDebug() << "fit_phase_rsp[0]:" << fit_phase_rsp[0];
	//qDebug() << "fit_xy_rsp[0]:" << fit_xy_rsp[0];
	//qDebug() << "fit_zy_rsp[0]:" << fit_zy_rsp[0];
	//qDebug() << "fit_mag_rsp[4799]:" << fit_mag_rsp[4799];
	//qDebug() << "fit_phase_rsp[4799]:" << fit_phase_rsp[4799];
	//qDebug() << "fit_xy_rsp[4799]:" << fit_xy_rsp[4799];
	//qDebug() << "fit_zy_rsp[4799]:" << fit_zy_rsp[4799];
	////qDebug() << "allData:" << QString::fromLocal8Bit(allData).split(" ").at(0).toDouble();
	
	mRspFile.close();
	return false;
}

bool CalibrationDll::invokeDllAndWrite(double ux[4800], double uy[4800], double uz[4800])
{
	if(0)
{
	//矫正前
	QFile uwFile;
	uwFile.setFileName(("uwFile1.csv"));

	if (!uwFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		qDebug() << "uwFile is not opened;";
		return false;
	}
	QTextStream wFile1(&uwFile);

	for (int i = 0; i < 4800; i++)
	{
		wFile1 << QString::number(ux[i], 'f', 12).toStdString().c_str();
		if (i < 4799)wFile1 << ",";
	}
	wFile1 << "\n";

	for (int i = 0; i < 4800; i++)
	{
		wFile1 << QString::number(uy[i], 'f', 12).toStdString().c_str();
		if (i < 4799)wFile1 << ",";
	}
	wFile1 << "\n";

	for (int i = 0; i < 4800; i++)
	{
		wFile1 << QString::number(uz[i], 'f', 12).toStdString().c_str();
		if (i < 4799)wFile1 << ",";
	}

	uwFile.close();
	}
	
	//调用频响矫正
	calibration(ux, uy, uz, fit_mag_rsp, fit_phase_rsp, fit_xy_rsp, fit_zy_rsp, ux_calib, uy_calib, uz_calib);
	
	memset(ux, 0, sizeof(double) * 4800);
	memset(uy, 0, sizeof(double) * 4800);
	memset(uz, 0, sizeof(double) * 4800);

	memcpy(ux, ux_calib, sizeof(double) * 4800);
	memcpy(uy, uy_calib, sizeof(double) * 4800);
	memcpy(uz, uz_calib, sizeof(double) * 4800);

	if (0)
	{
		//矫正后
		QFile uhFile;
		uhFile.setFileName(("uhFile.csv"));

		if (!uhFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			qDebug() << "uhFile is not opened;";
			return false;
		}
		QTextStream hFile1(&uhFile);

		for (int i = 0; i < 4800; i++)
		{
			hFile1 << QString::number(ux[i], 'f', 12).toStdString().c_str();
			if (i < 4799)hFile1 << ",";
		}
		hFile1 << "\n";

		for (int i = 0; i < 4800; i++)
		{
			hFile1 << QString::number(uy[i], 'f', 12).toStdString().c_str();
			if (i < 4799)hFile1 << ",";
		}
		hFile1 << "\n";

		for (int i = 0; i < 4800; i++)
		{
			hFile1 << QString::number(uz[i], 'f', 12).toStdString().c_str();
			if (i < 4799)hFile1 << ",";
		}




		uhFile.close();
	}
	return 1;
}
