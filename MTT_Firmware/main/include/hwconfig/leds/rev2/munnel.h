/* Metro Tunnel */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_MUNNEL_ARN                                 LMAT_BASE(3, 11, 4)
#define LMAT_MUNNEL_ARN_ALT                             LMAT_BASE(3, 5, 4)
#define LMAT_MUNNEL_PKV                                 LMAT_BASE(3, 7, 4)
#define LMAT_MUNNEL_PKV_ALT                             LMAT_BASE(3, 9, 4)
#define LMAT_MUNNEL_STL                                 LMAT_BASE(3, 11, 5)
#define LMAT_MUNNEL_STL_ALT                             LMAT_BASE(3, 9, 5)
#define LMAT_MUNNEL_THL                                 LMAT_BASE(3, 7, 5)
#define LMAT_MUNNEL_THL_ALT                             LMAT_BASE(3, 5, 5)
#define LMAT_MUNNEL_AZC                                 LMAT_BASE(3, 9, 3)
#define LMAT_MUNNEL_AZC_ALT                             LMAT_BASE(3, 11, 3)

#define LMAT_MUNNEL \
    LMAT_MUNNEL_ARN, \
    LMAT_MUNNEL_ARN_ALT, \
    LMAT_MUNNEL_PKV, \
    LMAT_MUNNEL_PKV_ALT, \
    LMAT_MUNNEL_STL, \
    LMAT_MUNNEL_STL_ALT, \
    LMAT_MUNNEL_THL, \
    LMAT_MUNNEL_THL_ALT, \
    LMAT_MUNNEL_AZC, \
    LMAT_MUNNEL_AZC_ALT
