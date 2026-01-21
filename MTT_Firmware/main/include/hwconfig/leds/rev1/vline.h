/* V/Line lines */

#pragma once

#include "hwconfig/led_matrix.h"

/* Melton line */
#define LMAT_BALLARAT_MEL                               LMAT_BASE(1, 10, 0)
#define LMAT_BALLARAT_MEL_ALT                           LMAT_BASE(1, 10, 1)
#define LMAT_BALLARAT_TLN                               LMAT_BASE(1, 10, 2)
#define LMAT_BALLARAT_TLN_ALT                           LMAT_BASE(1, 10, 3)
#define LMAT_BALLARAT_RBK                               LMAT_BASE(1, 10, 4)
#define LMAT_BALLARAT_RBK_ALT                           LMAT_BASE(1, 10, 5)
#define LMAT_BALLARAT_RVH                               LMAT_BASE(1, 11, 0)
#define LMAT_BALLARAT_RVH_ALT                           LMAT_BASE(1, 11, 1)

/* Wyndham Vale line */
#define LMAT_GEELONG_WVL                                LMAT_BASE(2, 0, 0)
#define LMAT_GEELONG_WVL_ALT                            LMAT_BASE(2, 0, 1)
#define LMAT_GEELONG_DAV                                LMAT_BASE(2, 0, 2) // West Tarneit
#define LMAT_GEELONG_DAV_ALT                            LMAT_BASE(2, 0, 3)
#define LMAT_GEELONG_TNT                                LMAT_BASE(2, 0, 4)
#define LMAT_GEELONG_TNT_ALT                            LMAT_BASE(2, 0, 5)

/* Western V/Line shared track */
#define LMAT_VLWEST_DEK                                 LMAT_BASE(1, 11, 2)
#define LMAT_VLWEST_DEK_ALT                             LMAT_BASE(1, 11, 3)
#define LMAT_VLWEST_ARR                                 LMAT_BASE(1, 11, 4)
#define LMAT_VLWEST_ARR_ALT                             LMAT_BASE(1, 11, 5)
#define LMAT_VLWEST_SUN                                 LMAT_BASE(1, 9, 0)
#define LMAT_VLWEST_SUN_ALT                             LMAT_BASE(1, 9, 1)
#define LMAT_VLWEST_FSY                                 LMAT_BASE(1, 9, 2)
#define LMAT_VLWEST_FSY_ALT                             LMAT_BASE(1, 9, 3)
#define LMAT_VLWEST_SSS                                 LMAT_BASE(2, 1, 2)
#define LMAT_VLWEST_SSS_ALT                             LMAT_BASE(2, 1, 3)

#define LMAT_GIPPSLAND_FSS                              LMAT_BASE(2, 8, 2)

/* Gippsland line */
#define LMAT_GIPPSLAND_RMD                              LMAT_BASE(2, 11, 0)
#define LMAT_GIPPSLAND_RMD_ALT                          LMAT_BASE(2, 11, 1)
#define LMAT_GIPPSLAND_CFD                              LMAT_BASE(4, 11, 4)
#define LMAT_GIPPSLAND_CFD_ALT                          LMAT_BASE(4, 11, 5)
#define LMAT_GIPPSLAND_CLA                              LMAT_BASE(6, 1, 0)
#define LMAT_GIPPSLAND_CLA_ALT                          LMAT_BASE(6, 1, 1)
#define LMAT_GIPPSLAND_DNG                              LMAT_BASE(5, 11, 4)
#define LMAT_GIPPSLAND_DNG_ALT                          LMAT_BASE(5, 11, 5)
#define LMAT_GIPPSLAND_BEW                              LMAT_BASE(5, 8, 2)
#define LMAT_GIPPSLAND_BEW_ALT                          LMAT_BASE(5, 8, 3)
#define LMAT_GIPPSLAND_PKM                              LMAT_BASE(5, 6, 4)
#define LMAT_GIPPSLAND_PKM_ALT                          LMAT_BASE(5, 6, 5)

#define LMAT_VLINE                                      \
    LMAT_BALLARAT_MEL, LMAT_BALLARAT_MEL_ALT,           \
    LMAT_BALLARAT_TLN, LMAT_BALLARAT_TLN_ALT,           \
    LMAT_BALLARAT_RBK, LMAT_BALLARAT_RBK_ALT,           \
    LMAT_BALLARAT_RVH, LMAT_BALLARAT_RVH_ALT,           \
    LMAT_GEELONG_WVL, LMAT_GEELONG_WVL_ALT,             \
    LMAT_GEELONG_DAV, LMAT_GEELONG_DAV_ALT,             \
    LMAT_GEELONG_TNT, LMAT_GEELONG_TNT_ALT,             \
    LMAT_VLWEST_DEK, LMAT_VLWEST_DEK_ALT,               \
    LMAT_VLWEST_ARR, LMAT_VLWEST_ARR_ALT,               \
    LMAT_VLWEST_SUN, LMAT_VLWEST_SUN_ALT,               \
    LMAT_VLWEST_FSY, LMAT_VLWEST_FSY_ALT,               \
    LMAT_VLWEST_SSS, LMAT_VLWEST_SSS_ALT,               \
    LMAT_GIPPSLAND_FSS,                                 \
    LMAT_GIPPSLAND_RMD, LMAT_GIPPSLAND_RMD_ALT,         \
    LMAT_GIPPSLAND_CFD, LMAT_GIPPSLAND_CFD_ALT,         \
    LMAT_GIPPSLAND_CLA, LMAT_GIPPSLAND_CLA_ALT,         \
    LMAT_GIPPSLAND_DNG, LMAT_GIPPSLAND_DNG_ALT,         \
    LMAT_GIPPSLAND_BEW, LMAT_GIPPSLAND_BEW_ALT,         \
    LMAT_GIPPSLAND_PKM, LMAT_GIPPSLAND_PKM_ALT
