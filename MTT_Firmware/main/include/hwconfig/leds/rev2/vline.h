/* V/Line lines */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_VLINE_HAS_REGIONAL 1 // Rev2 board has V/Line regional sections

/* Ararat line */
#define LMAT_ARARAT_ART                                 LMAT_BASE(0, 11, 4)
#define LMAT_ARARAT_ART_ALT                             LMAT_BASE(0, 9, 4)
#define LMAT_ARARAT_BET                                 LMAT_BASE(0, 7, 4)
#define LMAT_ARARAT_BET_ALT                             LMAT_BASE(0, 5, 4)
#define LMAT_ARARAT_WED                                 LMAT_BASE(0, 3, 4)
#define LMAT_ARARAT_WED_ALT                             LMAT_BASE(0, 1, 4)

/* Maryborough line */
#define LMAT_MARYBOROUGH_MBY                            LMAT_BASE(0, 0, 4)
#define LMAT_MARYBOROUGH_MBY_ALT                        LMAT_BASE(0, 2, 4)
#define LMAT_MARYBOROUGH_TAT                            LMAT_BASE(0, 4, 4)
#define LMAT_MARYBOROUGH_TAT_ALT                        LMAT_BASE(0, 6, 4)
#define LMAT_MARYBOROUGH_CLU                            LMAT_BASE(0, 8, 4)
#define LMAT_MARYBOROUGH_CLU_ALT                        LMAT_BASE(0, 10, 4)
#define LMAT_MARYBOROUGH_CWK                            LMAT_BASE(0, 0, 5)
#define LMAT_MARYBOROUGH_CWK_ALT                        LMAT_BASE(0, 2, 5)

/* Ballarat shared track */
#define LMAT_BALLARAT_BATV                              LMAT_BASE(0, 4, 5)
#define LMAT_BALLARAT_BATV_ALT                          LMAT_BASE(0, 6, 5)
#define LMAT_BALLARAT_BLN                               LMAT_BASE(0, 8, 5)
#define LMAT_BALLARAT_BLN_ALT                           LMAT_BASE(0, 10, 5)
#define LMAT_BALLARAT_BAH                               LMAT_BASE(0, 1, 5)
#define LMAT_BALLARAT_BAH_ALT                           LMAT_BASE(0, 3, 5)
#define LMAT_BALLARAT_MEL                               LMAT_BASE(0, 5, 5)
#define LMAT_BALLARAT_MEL_ALT                           LMAT_BASE(0, 7, 5)
#define LMAT_BALLARAT_TLN                               LMAT_BASE(0, 9, 5)
#define LMAT_BALLARAT_TLN_ALT                           LMAT_BASE(0, 11, 5)
#define LMAT_BALLARAT_RBK                               LMAT_BASE(1, 11, 5)
#define LMAT_BALLARAT_RBK_ALT                           LMAT_BASE(1, 9, 5)
#define LMAT_BALLARAT_RVH                               LMAT_BASE(1, 7, 5)
#define LMAT_BALLARAT_RVH_ALT                           LMAT_BASE(1, 5, 5)

/* Geelong/Warrnambool line */
#define LMAT_GEELONG_WBL                                LMAT_BASE(2, 10, 3)
#define LMAT_GEELONG_WBL_ALT                            LMAT_BASE(2, 8, 3)
#define LMAT_GEELONG_SDP                                LMAT_BASE(1, 0, 2)
#define LMAT_GEELONG_SDP_ALT                            LMAT_BASE(1, 2, 2)
#define LMAT_GEELONG_TER                                LMAT_BASE(1, 4, 2)
#define LMAT_GEELONG_TER_ALT                            LMAT_BASE(1, 6, 2)
#define LMAT_GEELONG_CPD                                LMAT_BASE(1, 8, 2)
#define LMAT_GEELONG_CPD_ALT                            LMAT_BASE(1, 10, 2)
#define LMAT_GEELONG_COL                                LMAT_BASE(1, 1, 2)
#define LMAT_GEELONG_COL_ALT                            LMAT_BASE(1, 3, 2)
#define LMAT_GEELONG_BGE                                LMAT_BASE(1, 5, 2)
#define LMAT_GEELONG_BGE_ALT                            LMAT_BASE(1, 7, 2)
#define LMAT_GEELONG_WIA                                LMAT_BASE(1, 9, 2)
#define LMAT_GEELONG_WIA_ALT                            LMAT_BASE(1, 11, 2)
#define LMAT_GEELONG_WPP                                LMAT_BASE(1, 0, 1)
#define LMAT_GEELONG_WPP_ALT                            LMAT_BASE(1, 2, 1)
#define LMAT_GEELONG_MAS                                LMAT_BASE(1, 4, 1)
#define LMAT_GEELONG_MAS_ALT                            LMAT_BASE(1, 6, 1)
#define LMAT_GEELONG_SOG                                LMAT_BASE(1, 8, 1)
#define LMAT_GEELONG_SOG_ALT                            LMAT_BASE(1, 10, 1)
#define LMAT_GEELONG_GEL                                LMAT_BASE(1, 1, 1)
#define LMAT_GEELONG_GEL_ALT                            LMAT_BASE(1, 3, 1)
#define LMAT_GEELONG_NGL                                LMAT_BASE(1, 5, 1)
#define LMAT_GEELONG_NGL_ALT                            LMAT_BASE(1, 7, 1)
#define LMAT_GEELONG_NSH                                LMAT_BASE(1, 9, 1)
#define LMAT_GEELONG_NSH_ALT                            LMAT_BASE(1, 11, 1)
#define LMAT_GEELONG_COR                                LMAT_BASE(1, 0, 3)
#define LMAT_GEELONG_COR_ALT                            LMAT_BASE(1, 2, 3)
#define LMAT_GEELONG_LRA                                LMAT_BASE(1, 4, 3)
#define LMAT_GEELONG_LRA_ALT                            LMAT_BASE(1, 6, 3)
#define LMAT_GEELONG_LRR                                LMAT_BASE(1, 8, 3)
#define LMAT_GEELONG_LRR_ALT                            LMAT_BASE(1, 10, 3)
#define LMAT_GEELONG_WVL                                LMAT_BASE(1, 1, 3)
#define LMAT_GEELONG_WVL_ALT                            LMAT_BASE(1, 3, 3)
#define LMAT_GEELONG_DAV                                LMAT_BASE(1, 5, 3) // West Tarneit
#define LMAT_GEELONG_DAV_ALT                            LMAT_BASE(1, 7, 3)
#define LMAT_GEELONG_TNT                                LMAT_BASE(1, 9, 3)
#define LMAT_GEELONG_TNT_ALT                            LMAT_BASE(1, 11, 3)

/* Western V/Line shared track */
#define LMAT_VLWEST_DEK                                 LMAT_BASE(1, 3, 5)
#define LMAT_VLWEST_DEK_ALT                             LMAT_BASE(1, 1, 5)
#define LMAT_VLWEST_ARR                                 LMAT_BASE(1, 0, 5)
#define LMAT_VLWEST_ARR_ALT                             LMAT_BASE(1, 2, 5)
#define LMAT_VLWEST_SUN                                 LMAT_BASE(1, 4, 5)
#define LMAT_VLWEST_SUN_ALT                             LMAT_BASE(1, 6, 5)
#define LMAT_VLWEST_FSY                                 LMAT_BASE(1, 9, 4)
#define LMAT_VLWEST_FSY_ALT                             LMAT_BASE(1, 11, 4)
#define LMAT_VLWEST_SSS                                 LMAT_BASE(3, 2, 0)
#define LMAT_VLWEST_SSS_ALT                             LMAT_BASE(3, 3, 0) // this is actually for Gippsland line; western V/Line trains don't run to FSS

/* Gippsland (Traralgon/Bairnsdale) line */
#define LMAT_GIPPSLAND_FSS                              LMAT_BASE(3, 3, 1)
#define LMAT_GIPPSLAND_RMD                              LMAT_BASE(3, 1, 3)
#define LMAT_GIPPSLAND_RMD_ALT                          LMAT_BASE(3, 10, 3)
#define LMAT_GIPPSLAND_CFD                              LMAT_BASE(8, 2, 5)
#define LMAT_GIPPSLAND_CFD_ALT                          LMAT_BASE(8, 0, 5)
#define LMAT_GIPPSLAND_CLA                              LMAT_BASE(8, 6, 5)
#define LMAT_GIPPSLAND_CLA_ALT                          LMAT_BASE(8, 4, 5)
#define LMAT_GIPPSLAND_DNG                              LMAT_BASE(8, 10, 5)
#define LMAT_GIPPSLAND_DNG_ALT                          LMAT_BASE(8, 8, 5)
#define LMAT_GIPPSLAND_BEW                              LMAT_BASE(8, 11, 3)
#define LMAT_GIPPSLAND_BEW_ALT                          LMAT_BASE(8, 5, 3)
#define LMAT_GIPPSLAND_PKM                              LMAT_BASE(8, 0, 3)
#define LMAT_GIPPSLAND_PKM_ALT                          LMAT_BASE(8, 2, 3)
#define LMAT_GIPPSLAND_NNG                              LMAT_BASE(10, 9, 1) 
#define LMAT_GIPPSLAND_NNG_ALT                          LMAT_BASE(10, 11, 1)
#define LMAT_GIPPSLAND_TYN                              LMAT_BASE(10, 5, 1)
#define LMAT_GIPPSLAND_TYN_ALT                          LMAT_BASE(10, 7, 1)
#define LMAT_GIPPSLAND_GARV                             LMAT_BASE(10, 1, 1)
#define LMAT_GIPPSLAND_GARV_ALT                         LMAT_BASE(10, 3, 1)
#define LMAT_GIPPSLAND_BYP                              LMAT_BASE(10, 2, 1)
#define LMAT_GIPPSLAND_BYP_ALT                          LMAT_BASE(10, 0, 1)
#define LMAT_GIPPSLAND_LWY                              LMAT_BASE(10, 8, 1)
#define LMAT_GIPPSLAND_LWY_ALT                          LMAT_BASE(10, 10, 1)
#define LMAT_GIPPSLAND_DRN                              LMAT_BASE(10, 4, 1)
#define LMAT_GIPPSLAND_DRN_ALT                          LMAT_BASE(10, 6, 1)
#define LMAT_GIPPSLAND_WGL                              LMAT_BASE(10, 9, 0)
#define LMAT_GIPPSLAND_WGL_ALT                          LMAT_BASE(10, 11, 0)
#define LMAT_GIPPSLAND_YON                              LMAT_BASE(10, 5, 0)
#define LMAT_GIPPSLAND_YON_ALT                          LMAT_BASE(10, 7, 0)
#define LMAT_GIPPSLAND_TAR                              LMAT_BASE(10, 1, 0)
#define LMAT_GIPPSLAND_TAR_ALT                          LMAT_BASE(10, 3, 0)
#define LMAT_GIPPSLAND_MOE                              LMAT_BASE(10, 2, 0)
#define LMAT_GIPPSLAND_MOE_ALT                          LMAT_BASE(10, 0, 0)
#define LMAT_GIPPSLAND_MWL                              LMAT_BASE(10, 6, 0)
#define LMAT_GIPPSLAND_MWL_ALT                          LMAT_BASE(10, 4, 0)
#define LMAT_GIPPSLAND_TRN                              LMAT_BASE(10, 10, 0)
#define LMAT_GIPPSLAND_TRN_ALT                          LMAT_BASE(10, 8, 0)
#define LMAT_GIPPSLAND_ROE                              LMAT_BASE(10, 9, 5)
#define LMAT_GIPPSLAND_ROE_ALT                          LMAT_BASE(10, 11, 5)
#define LMAT_GIPPSLAND_SAE                              LMAT_BASE(10, 5, 5)
#define LMAT_GIPPSLAND_SAE_ALT                          LMAT_BASE(10, 7, 5)
#define LMAT_GIPPSLAND_STD                              LMAT_BASE(10, 1, 5)
#define LMAT_GIPPSLAND_STD_ALT                          LMAT_BASE(10, 3, 5)
#define LMAT_GIPPSLAND_BDE                              LMAT_BASE(10, 2, 5)
#define LMAT_GIPPSLAND_BDE_ALT                          LMAT_BASE(10, 0, 5)

#define LMAT_VLINE \
    LMAT_ARARAT_ART, LMAT_ARARAT_ART_ALT, LMAT_ARARAT_BET, LMAT_ARARAT_BET_ALT, \
    LMAT_ARARAT_WED, LMAT_ARARAT_WED_ALT, LMAT_MARYBOROUGH_MBY, LMAT_MARYBOROUGH_MBY_ALT, \
    LMAT_MARYBOROUGH_TAT, LMAT_MARYBOROUGH_TAT_ALT, LMAT_MARYBOROUGH_CLU, LMAT_MARYBOROUGH_CLU_ALT, \
    LMAT_MARYBOROUGH_CWK, LMAT_MARYBOROUGH_CWK_ALT, LMAT_BALLARAT_BATV, LMAT_BALLARAT_BATV_ALT, \
    LMAT_BALLARAT_BLN, LMAT_BALLARAT_BLN_ALT, LMAT_BALLARAT_BAH, LMAT_BALLARAT_BAH_ALT, \
    LMAT_BALLARAT_MEL, LMAT_BALLARAT_MEL_ALT, LMAT_BALLARAT_TLN, LMAT_BALLARAT_TLN_ALT, \
    LMAT_BALLARAT_RBK, LMAT_BALLARAT_RBK_ALT, LMAT_BALLARAT_RVH, LMAT_BALLARAT_RVH_ALT, \
    LMAT_GEELONG_WBL, LMAT_GEELONG_WBL_ALT, LMAT_GEELONG_SDP, LMAT_GEELONG_SDP_ALT, \
    LMAT_GEELONG_TER, LMAT_GEELONG_TER_ALT, LMAT_GEELONG_CPD, LMAT_GEELONG_CPD_ALT, \
    LMAT_GEELONG_COL, LMAT_GEELONG_COL_ALT, LMAT_GEELONG_BGE, LMAT_GEELONG_BGE_ALT, \
    LMAT_GEELONG_WIA, LMAT_GEELONG_WIA_ALT, LMAT_GEELONG_WPP, LMAT_GEELONG_WPP_ALT, \
    LMAT_GEELONG_MAS, LMAT_GEELONG_MAS_ALT, LMAT_GEELONG_SOG, LMAT_GEELONG_SOG_ALT, \
    LMAT_GEELONG_GEL, LMAT_GEELONG_GEL_ALT, LMAT_GEELONG_NGL, LMAT_GEELONG_NGL_ALT, \
    LMAT_GEELONG_NSH, LMAT_GEELONG_NSH_ALT, LMAT_GEELONG_COR, LMAT_GEELONG_COR_ALT, \
    LMAT_GEELONG_LRA, LMAT_GEELONG_LRA_ALT, LMAT_GEELONG_LRR, LMAT_GEELONG_LRR_ALT, \
    LMAT_GEELONG_WVL, LMAT_GEELONG_WVL_ALT, LMAT_GEELONG_DAV, LMAT_GEELONG_DAV_ALT, \
    LMAT_GEELONG_TNT, LMAT_GEELONG_TNT_ALT, LMAT_VLWEST_DEK, LMAT_VLWEST_DEK_ALT, \
    LMAT_VLWEST_ARR, LMAT_VLWEST_ARR_ALT, LMAT_VLWEST_SUN, LMAT_VLWEST_SUN_ALT, \
    LMAT_VLWEST_FSY, LMAT_VLWEST_FSY_ALT, LMAT_VLWEST_SSS, LMAT_VLWEST_SSS_ALT, \
    LMAT_GIPPSLAND_FSS, LMAT_GIPPSLAND_RMD, LMAT_GIPPSLAND_RMD_ALT, LMAT_GIPPSLAND_CFD, \
    LMAT_GIPPSLAND_CFD_ALT, LMAT_GIPPSLAND_CLA, LMAT_GIPPSLAND_CLA_ALT, LMAT_GIPPSLAND_DNG, \
    LMAT_GIPPSLAND_DNG_ALT, LMAT_GIPPSLAND_BEW, LMAT_GIPPSLAND_BEW_ALT, LMAT_GIPPSLAND_PKM, \
    LMAT_GIPPSLAND_PKM_ALT, LMAT_GIPPSLAND_NNG, LMAT_GIPPSLAND_NNG_ALT, LMAT_GIPPSLAND_TYN, \
    LMAT_GIPPSLAND_TYN_ALT, LMAT_GIPPSLAND_GARV, LMAT_GIPPSLAND_GARV_ALT, LMAT_GIPPSLAND_BYP, \
    LMAT_GIPPSLAND_BYP_ALT, LMAT_GIPPSLAND_LWY, LMAT_GIPPSLAND_LWY_ALT, LMAT_GIPPSLAND_DRN, \
    LMAT_GIPPSLAND_DRN_ALT, LMAT_GIPPSLAND_WGL, LMAT_GIPPSLAND_WGL_ALT, LMAT_GIPPSLAND_YON, \
    LMAT_GIPPSLAND_YON_ALT, LMAT_GIPPSLAND_TAR, LMAT_GIPPSLAND_TAR_ALT, LMAT_GIPPSLAND_MOE, \
    LMAT_GIPPSLAND_MOE_ALT, LMAT_GIPPSLAND_MWL, LMAT_GIPPSLAND_MWL_ALT, LMAT_GIPPSLAND_TRN, \
    LMAT_GIPPSLAND_TRN_ALT, LMAT_GIPPSLAND_ROE, LMAT_GIPPSLAND_ROE_ALT, LMAT_GIPPSLAND_SAE, \
    LMAT_GIPPSLAND_SAE_ALT, LMAT_GIPPSLAND_STD, LMAT_GIPPSLAND_STD_ALT, LMAT_GIPPSLAND_BDE, \
    LMAT_GIPPSLAND_BDE_ALT

