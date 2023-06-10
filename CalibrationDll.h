#pragma once
#include <qobject.h>


/**
 * extern void calibration(const double ux[4800], 
						const double uy[4800],
						const double uz[4800],
						const double fit_mag_rsp[4800],
						const double fit_phase_rsp[4800],
						const double fit_xy_rsp[4800],
						const double fit_zy_rsp[4800],
						double ux_calib[4800],
						double uy_calib[4800],
						double uz_calib[4800]);
 */
class CalibrationDll :public QObject
{
public:
	CalibrationDll(QString rsp,QObject* parent = nullptr);
	~CalibrationDll();
	bool invokeDllAndWrite(double ux[4800], double uy[4800], double uz[4800]);
protected:
	bool readRSPDataAndWrite(QString rspFilePath);
	
private:
	double ux_calib[4800] = {0}, uy_calib[4800] = { 0 }, uz_calib[4800] = { 0 };
	double fit_mag_rsp[4800] = { 0 }, fit_phase_rsp[4800] = { 0 },
		   fit_xy_rsp[4800] = { 0 },  fit_zy_rsp[4800] = { 0 };
};

