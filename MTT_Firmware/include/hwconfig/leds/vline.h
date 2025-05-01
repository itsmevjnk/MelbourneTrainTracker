/* V/Line lines */

#pragma once

#include "hwconfig/led_matrix.h"

/* Melton line */
#define LMAT_VLINE_MEL                                  LMAT_BASE(1, 10, 0)
#define LMAT_VLINE_MEL_ALT                              LMAT_BASE(1, 10, 1)
#define LMAT_VLINE_TLN                                  LMAT_BASE(1, 10, 2)
#define LMAT_VLINE_TLN_ALT                              LMAT_BASE(1, 10, 3)
#define LMAT_VLINE_RBK                                  LMAT_BASE(1, 10, 4)
#define LMAT_VLINE_RBK_ALT                              LMAT_BASE(1, 10, 5)
#define LMAT_VLINE_CPS                                  LMAT_BASE(1, 11, 0)
#define LMAT_VLINE_CPS_ALT                              LMAT_BASE(1, 11, 1)

/* Wyndham Vale line */
#define LMAT_VLINE_WVL                                  LMAT_BASE(2, 0, 0)
#define LMAT_VLINE_WVL_ALT                              LMAT_BASE(2, 0, 1)
#define LMAT_VLINE_DAV                                  LMAT_BASE(2, 0, 2) // West Tarneit  
#define LMAT_VLINE_DAV_ALT                              LMAT_BASE(2, 0, 3)
#define LMAT_VLINE_TNT                                  LMAT_BASE(2, 0, 4)
#define LMAT_VLINE_TNT_ALT                              LMAT_BASE(2, 0, 5)

/* Western V/Line shared track */
#define LMAT_VLINE_DEK                                  LMAT_BASE(1, 11, 2)
#define LMAT_VLINE_DEK_ALT                              LMAT_BASE(1, 11, 3)
#define LMAT_VLINE_ADR                                  LMAT_BASE(1, 11, 4)
#define LMAT_VLINE_ADR_ALT                              LMAT_BASE(1, 11, 5)
#define LMAT_VLINE_SUN                                  LMAT_BASE(1, 9, 0)
#define LMAT_VLINE_SUN_ALT                              LMAT_BASE(1, 9, 1)
#define LMAT_VLINE_FSY                                  LMAT_BASE(1, 9, 2)
#define LMAT_VLINE_FSY_ALT                              LMAT_BASE(1, 9, 3)
#define LMAT_VLINE_SSS                                  LMAT_BASE(2, 1, 2)
#define LMAT_VLINE_SSS_ALT                              LMAT_BASE(2, 1, 3)

#define LMAT_VLINE_FSS                                  LMAT_BASE(2, 8, 2)

/* Gippsland line */
#define LMAT_VLINE_RMD                                  LMAT_BASE(2, 11, 0)
#define LMAT_VLINE_RMD_ALT                              LMAT_BASE(2, 11, 1)
#define LMAT_VLINE_CFD                                  LMAT_BASE(4, 11, 4)
#define LMAT_VLINE_CFD_ALT                              LMAT_BASE(4, 11, 5)
#define LMAT_VLINE_CLA                                  LMAT_BASE(6, 1, 0)
#define LMAT_VLINE_CLA_ALT                              LMAT_BASE(6, 1, 1)
#define LMAT_VLINE_DNG                                  LMAT_BASE(5, 11, 4)
#define LMAT_VLINE_DNG_ALT                              LMAT_BASE(5, 11, 5)
#define LMAT_VLINE_BEW                                  LMAT_BASE(5, 8, 2)
#define LMAT_VLINE_BEW_ALT                              LMAT_BASE(5, 8, 3)
#define LMAT_VLINE_PKM                                  LMAT_BASE(5, 6, 4)
#define LMAT_VLINE_PKM_ALT                              LMAT_BASE(5, 6, 5)

#define LMAT_VLINE                                      \
    LMAT_VLINE_MEL, LMAT_VLINE_MEL_ALT,                \
    LMAT_VLINE_TLN, LMAT_VLINE_TLN_ALT,                \
    LMAT_VLINE_RBK, LMAT_VLINE_RBK_ALT,                \
    LMAT_VLINE_CPS, LMAT_VLINE_CPS_ALT,                \
    LMAT_VLINE_WVL, LMAT_VLINE_WVL_ALT,                \
    LMAT_VLINE_DAV, LMAT_VLINE_DAV_ALT,                \
    LMAT_VLINE_TNT, LMAT_VLINE_TNT_ALT,                \
    LMAT_VLINE_DEK, LMAT_VLINE_DEK_ALT,                \
    LMAT_VLINE_ADR, LMAT_VLINE_ADR_ALT,                \
    LMAT_VLINE_SUN, LMAT_VLINE_SUN_ALT,                \
    LMAT_VLINE_FSY, LMAT_VLINE_FSY_ALT,                \
    LMAT_VLINE_SSS, LMAT_VLINE_SSS_ALT,                \
    LMAT_VLINE_FSS,                                    \
    LMAT_VLINE_RMD, LMAT_VLINE_RMD_ALT,                \
    LMAT_VLINE_CFD, LMAT_VLINE_CFD_ALT,                \
    LMAT_VLINE_CLA, LMAT_VLINE_CLA_ALT,                \
    LMAT_VLINE_DNG, LMAT_VLINE_DNG_ALT,                \
    LMAT_VLINE_BEW, LMAT_VLINE_BEW_ALT,                \
    LMAT_VLINE_PKM, LMAT_VLINE_PKM_ALT
