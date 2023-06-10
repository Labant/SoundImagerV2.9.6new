/*
 * File: filt.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 31-May-2023 11:25:08
 */

#ifndef FILT_H
#define FILT_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void filt(const double sig_p[4800], const double sig_x[4800],
                 const double sig_y[4800], const double sig_z[4800],
                 double f_low, double f_high, double fs,
                 double sig_filt_p[4800], double sig_filt_x[4800],
                 double sig_filt_y[4800], double sig_filt_z[4800]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for filt.h
 *
 * [EOF]
 */
