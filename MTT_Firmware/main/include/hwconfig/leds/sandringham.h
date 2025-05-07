/* Sandringham line */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_SANDRINGHAM_SHM                            LMAT_BASE(7, 5, 1)
#define LMAT_SANDRINGHAM_SHM_ALT                        LMAT_BASE(7, 5, 2)
#define LMAT_SANDRINGHAM_HAM                            LMAT_BASE(7, 6, 0)
#define LMAT_SANDRINGHAM_HAM_ALT                        LMAT_BASE(7, 6, 1)
#define LMAT_SANDRINGHAM_BBH                            LMAT_BASE(7, 6, 2)
#define LMAT_SANDRINGHAM_BBH_ALT                        LMAT_BASE(7, 7, 0)
#define LMAT_SANDRINGHAM_MBN                            LMAT_BASE(7, 7, 1)
#define LMAT_SANDRINGHAM_MBN_ALT                        LMAT_BASE(7, 7, 2)
#define LMAT_SANDRINGHAM_NBN                            LMAT_BASE(7, 8, 0)
#define LMAT_SANDRINGHAM_NBN_ALT                        LMAT_BASE(7, 8, 1)
#define LMAT_SANDRINGHAM_GVE                            LMAT_BASE(7, 8, 2)
#define LMAT_SANDRINGHAM_GVE_ALT                        LMAT_BASE(7, 9, 0)
#define LMAT_SANDRINGHAM_ELS                            LMAT_BASE(7, 9, 1)
#define LMAT_SANDRINGHAM_ELS_ALT                        LMAT_BASE(7, 9, 2)
#define LMAT_SANDRINGHAM_RIP                            LMAT_BASE(7, 9, 3)
#define LMAT_SANDRINGHAM_RIP_ALT                        LMAT_BASE(7, 10, 0)
#define LMAT_SANDRINGHAM_BCV                            LMAT_BASE(7, 10, 1)
#define LMAT_SANDRINGHAM_BCV_ALT                        LMAT_BASE(7, 10, 2)
#define LMAT_SANDRINGHAM_WIN                            LMAT_BASE(7, 11, 0)
#define LMAT_SANDRINGHAM_WIN_ALT                        LMAT_BASE(7, 11, 1)
#define LMAT_SANDRINGHAM_PRA                            LMAT_BASE(7, 11, 2)
#define LMAT_SANDRINGHAM_PRA_ALT                        LMAT_BASE(7, 11, 3)
#define LMAT_SANDRINGHAM_SYR                            LMAT_BASE(2, 9, 2)
#define LMAT_SANDRINGHAM_SYR_ALT                        LMAT_BASE(2, 9, 3)
#define LMAT_SANDRINGHAM_RMD                            LMAT_BASE(2, 9, 4)
#define LMAT_SANDRINGHAM_RMD_ALT                        LMAT_BASE(2, 9, 5)
#define LMAT_SANDRINGHAM_FSS                            LMAT_BASE(2, 8, 0)

#define LMAT_SANDRINGHAM \
    LMAT_SANDRINGHAM_SHM, LMAT_SANDRINGHAM_SHM_ALT, \
    LMAT_SANDRINGHAM_HAM, LMAT_SANDRINGHAM_HAM_ALT, \
    LMAT_SANDRINGHAM_BBH, LMAT_SANDRINGHAM_BBH_ALT, \
    LMAT_SANDRINGHAM_MBN, LMAT_SANDRINGHAM_MBN_ALT, \
    LMAT_SANDRINGHAM_NBN, LMAT_SANDRINGHAM_NBN_ALT, \
    LMAT_SANDRINGHAM_GVE, LMAT_SANDRINGHAM_GVE_ALT, \
    LMAT_SANDRINGHAM_ELS, LMAT_SANDRINGHAM_ELS_ALT, \
    LMAT_SANDRINGHAM_RIP, LMAT_SANDRINGHAM_RIP_ALT, \
    LMAT_SANDRINGHAM_BCV, LMAT_SANDRINGHAM_BCV_ALT, \
    LMAT_SANDRINGHAM_WIN, LMAT_SANDRINGHAM_WIN_ALT, \
    LMAT_SANDRINGHAM_PRA, LMAT_SANDRINGHAM_PRA_ALT, \
    LMAT_SANDRINGHAM_SYR, LMAT_SANDRINGHAM_SYR_ALT, \
    LMAT_SANDRINGHAM_RMD, LMAT_SANDRINGHAM_RMD_ALT, \
    LMAT_SANDRINGHAM_FSS