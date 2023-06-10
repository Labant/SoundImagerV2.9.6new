/*
 * File: direction_spl.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 06-Apr-2023 16:40:24
 */

#ifndef DIRECTION_SPL_H
#define DIRECTION_SPL_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void direction_spl(const double P[4800], const double X[4800],
                          const double Y[4800], const double Z[4800],
                          double P_1D[64800], double *P_1D_MAX_lvs);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for direction_spl.h
 *
 * [EOF]
 */
