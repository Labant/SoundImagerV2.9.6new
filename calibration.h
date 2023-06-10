/*
 * File: calibration.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 16-May-2023 18:01:16
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
/*
*
%CALIBRATION 频响修正
%   修正了0-12790Hz的频响
%   频响修正的分辨率df = 10Hz
%   输入的点数应为N=0.1*fs
% rsp = readmatrix("rsp.txt");
% fit_mag_rsp = rsp(1,:);
% fit_phase_rsp = rsp(2, :);
% fit_xy_rsp = rsp(3, :);
% fit_zy_rsp = rsp(4, :);
*/
#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void calibration(const double ux[4800], const double uy[4800],
                        const double uz[4800], const double fit_mag_rsp[4800],
                        const double fit_phase_rsp[4800],
                        const double fit_xy_rsp[4800],
                        const double fit_zy_rsp[4800], double ux_calib[4800],
                        double uy_calib[4800], double uz_calib[4800]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for calibration.h
 *
 * [EOF]
 */
