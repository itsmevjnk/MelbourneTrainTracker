/* Metro Tunnel */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_MUNNEL_AEN                                 LMAT_BASE(1, 2, 2)
#define LMAT_MUNNEL_AEN_ALT                             LMAT_BASE(1, 2, 3)
#define LMAT_MUNNEL_PKV                                 LMAT_BASE(1, 2, 4)
#define LMAT_MUNNEL_PKV_ALT                             LMAT_BASE(1, 2, 5)
#define LMAT_MUNNEL_TNC                                 LMAT_BASE(2, 7, 0)
#define LMAT_MUNNEL_TNC_ALT                             LMAT_BASE(2, 7, 1)
#define LMAT_MUNNEL_TSC                                 LMAT_BASE(2, 7, 2)
#define LMAT_MUNNEL_TSC_ALT                             LMAT_BASE(2, 7, 3)
#define LMAT_MUNNEL_DMA                                 LMAT_BASE(2, 7, 4)
#define LMAT_MUNNEL_DMA_ALT                             LMAT_BASE(2, 7, 5)

#define LMAT_MUNNEL \
    LMAT_MUNNEL_AEN, \
    LMAT_MUNNEL_AEN_ALT, \
    LMAT_MUNNEL_PKV, \
    LMAT_MUNNEL_PKV_ALT, \
    LMAT_MUNNEL_TNC, \
    LMAT_MUNNEL_TNC_ALT, \
    LMAT_MUNNEL_TSC, \
    LMAT_MUNNEL_TSC_ALT, \
    LMAT_MUNNEL_DMA, \
    LMAT_MUNNEL_DMA_ALT
