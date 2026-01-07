/* Sandringham line */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_SANDRINGHAM_SHM                            LMAT_BASE(2, 6, 3)
#define LMAT_SANDRINGHAM_SHM_ALT                        LMAT_BASE(2, 4, 3)
#define LMAT_SANDRINGHAM_HAM                            LMAT_BASE(2, 2, 3)
#define LMAT_SANDRINGHAM_HAM_ALT                        LMAT_BASE(2, 0, 3)
#define LMAT_SANDRINGHAM_BBH                            LMAT_BASE(2, 1, 3)
#define LMAT_SANDRINGHAM_BBH_ALT                        LMAT_BASE(2, 3, 3)
#define LMAT_SANDRINGHAM_MBN                            LMAT_BASE(2, 5, 3)
#define LMAT_SANDRINGHAM_MBN_ALT                        LMAT_BASE(2, 7, 3)
#define LMAT_SANDRINGHAM_NBN                            LMAT_BASE(2, 9, 3)
#define LMAT_SANDRINGHAM_NBN_ALT                        LMAT_BASE(2, 11, 3)
#define LMAT_SANDRINGHAM_GVE                            LMAT_BASE(2, 10, 4)
#define LMAT_SANDRINGHAM_GVE_ALT                        LMAT_BASE(2, 8, 4)
#define LMAT_SANDRINGHAM_ELS                            LMAT_BASE(2, 6, 4)
#define LMAT_SANDRINGHAM_ELS_ALT                        LMAT_BASE(2, 4, 4)
#define LMAT_SANDRINGHAM_RIP                            LMAT_BASE(2, 2, 4)
#define LMAT_SANDRINGHAM_RIP_ALT                        LMAT_BASE(2, 0, 4)
#define LMAT_SANDRINGHAM_BCV                            LMAT_BASE(2, 1, 4)
#define LMAT_SANDRINGHAM_BCV_ALT                        LMAT_BASE(2, 3, 4)
#define LMAT_SANDRINGHAM_WIN                            LMAT_BASE(2, 5, 4)
#define LMAT_SANDRINGHAM_WIN_ALT                        LMAT_BASE(2, 7, 4)
#define LMAT_SANDRINGHAM_PRA                            LMAT_BASE(2, 9, 4)
#define LMAT_SANDRINGHAM_PRA_ALT                        LMAT_BASE(2, 11, 4)
#define LMAT_SANDRINGHAM_SYR                            LMAT_BASE(3, 7, 2)
#define LMAT_SANDRINGHAM_SYR_ALT                        LMAT_BASE(3, 5, 2)
#define LMAT_SANDRINGHAM_RMD                            LMAT_BASE(3, 5, 3)
#define LMAT_SANDRINGHAM_RMD_ALT                        LMAT_BASE(3, 7, 3)
#define LMAT_SANDRINGHAM_FSS                            LMAT_BASE(3, 0, 1)

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