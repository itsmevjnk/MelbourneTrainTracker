/* Dandenong (Pakenham & Cranbourne lines) */

#pragma once

#include "hwconfig/led_matrix.h"

/* Pakenham line */
#define LMAT_PAKENHAM_EPH                               LMAT_BASE(5, 6, 0)
#define LMAT_PAKENHAM_EPH_ALT                           LMAT_BASE(5, 6, 1)
#define LMAT_PAKENHAM_PKM                               LMAT_BASE(5, 6, 2)
#define LMAT_PAKENHAM_PKM_ALT                           LMAT_BASE(5, 6, 3)
#define LMAT_PAKENHAM_CDA                               LMAT_BASE(5, 7, 0)
#define LMAT_PAKENHAM_CDA_ALT                           LMAT_BASE(5, 7, 1)
#define LMAT_PAKENHAM_OFC                               LMAT_BASE(5, 7, 2)
#define LMAT_PAKENHAM_OFC_ALT                           LMAT_BASE(5, 7, 3)
#define LMAT_PAKENHAM_BFD                               LMAT_BASE(5, 7, 4)
#define LMAT_PAKENHAM_BFD_ALT                           LMAT_BASE(5, 7, 5)
#define LMAT_PAKENHAM_BEW                               LMAT_BASE(5, 8, 0)
#define LMAT_PAKENHAM_BEW_ALT                           LMAT_BASE(5, 8, 1)
#define LMAT_PAKENHAM_NWA                               LMAT_BASE(5, 8, 4)
#define LMAT_PAKENHAM_NWA_ALT                           LMAT_BASE(5, 8, 5)

/* Cranbourne line */
#define LMAT_CRANBOURNE_CBE                             LMAT_BASE(5, 9, 0)
#define LMAT_CRANBOURNE_CBE_ALT                         LMAT_BASE(5, 9, 1)
#define LMAT_CRANBOURNE_MPK                             LMAT_BASE(5, 9, 2)
#define LMAT_CRANBOURNE_MPK_ALT                         LMAT_BASE(5, 9, 3)
#define LMAT_CRANBOURNE_LBK                             LMAT_BASE(5, 9, 4)
#define LMAT_CRANBOURNE_LBK_ALT                         LMAT_BASE(5, 9, 5)
#define LMAT_CRANBOURNE_HLM                             LMAT_BASE(5, 10, 0)
#define LMAT_CRANBOURNE_HLM_ALT                         LMAT_BASE(5, 10, 1)

/* Dandenong shared track */
#define LMAT_DANDENONG_DNG                              LMAT_BASE(5, 10, 2)
#define LMAT_DANDENONG_DNG_ALT                          LMAT_BASE(5, 10, 3)
#define LMAT_DANDENONG_YMN                              LMAT_BASE(5, 10, 4)
#define LMAT_DANDENONG_YMN_ALT                          LMAT_BASE(5, 10, 5)
#define LMAT_DANDENONG_NPK                              LMAT_BASE(5, 11, 0)
#define LMAT_DANDENONG_NPK_ALT                          LMAT_BASE(5, 11, 1)
#define LMAT_DANDENONG_SNP                              LMAT_BASE(5, 11, 2)
#define LMAT_DANDENONG_SNP_ALT                          LMAT_BASE(5, 11, 3)
#define LMAT_DANDENONG_SPG                              LMAT_BASE(6, 0, 0)
#define LMAT_DANDENONG_SPG_ALT                          LMAT_BASE(6, 0, 1)
#define LMAT_DANDENONG_WTL                              LMAT_BASE(6, 0, 2)
#define LMAT_DANDENONG_WTL_ALT                          LMAT_BASE(6, 0, 3)
#define LMAT_DANDENONG_CLA                              LMAT_BASE(6, 0, 4)
#define LMAT_DANDENONG_CLA_ALT                          LMAT_BASE(6, 0, 5)
#define LMAT_DANDENONG_HUN                              LMAT_BASE(6, 1, 2)
#define LMAT_DANDENONG_HUN_ALT                          LMAT_BASE(6, 1, 3)
#define LMAT_DANDENONG_OAK                              LMAT_BASE(6, 1, 4)
#define LMAT_DANDENONG_OAK_ALT                          LMAT_BASE(6, 1, 5)
#define LMAT_DANDENONG_HUG                              LMAT_BASE(6, 2, 0)
#define LMAT_DANDENONG_HUG_ALT                          LMAT_BASE(6, 2, 1)
#define LMAT_DANDENONG_MRB                              LMAT_BASE(6, 2, 2)
#define LMAT_DANDENONG_MRB_ALT                          LMAT_BASE(6, 2, 3)
#define LMAT_DANDENONG_CNE                              LMAT_BASE(6, 2, 4)
#define LMAT_DANDENONG_CNE_ALT                          LMAT_BASE(6, 2, 5)
#define LMAT_DANDENONG_CFD                              LMAT_BASE(4, 11, 2)
#define LMAT_DANDENONG_CFD_ALT                          LMAT_BASE(4, 11, 3)
#define LMAT_DANDENONG_SYR                              LMAT_BASE(2, 10, 0)
#define LMAT_DANDENONG_SYR_ALT                          LMAT_BASE(2, 10, 1)
#define LMAT_DANDENONG_RMD                              LMAT_BASE(2, 11, 2)
#define LMAT_DANDENONG_RMD_ALT                          LMAT_BASE(2, 11, 3)

/* city stations */
#define LMAT_DANDENONG_SSS                              LMAT_BASE(2, 1, 4)
#define LMAT_DANDENONG_SSS_ALT                          LMAT_BASE(2, 1, 5)
#define LMAT_DANDENONG_FGS                              LMAT_BASE(2, 3, 0)
#define LMAT_DANDENONG_FGS_ALT                          LMAT_BASE(2, 3, 1)
#define LMAT_DANDENONG_MCE                              LMAT_BASE(2, 4, 0)
#define LMAT_DANDENONG_MCE_ALT                          LMAT_BASE(2, 4, 1)
#define LMAT_DANDENONG_PAR                              LMAT_BASE(2, 5, 0)
#define LMAT_DANDENONG_PAR_ALT                          LMAT_BASE(2, 5, 1)
#define LMAT_DANDENONG_FSS                              LMAT_BASE(2, 8, 3)

#define LMAT_DANDENONG \
    LMAT_PAKENHAM_EPH, LMAT_PAKENHAM_EPH_ALT, LMAT_PAKENHAM_PKM, LMAT_PAKENHAM_PKM_ALT, \
    LMAT_PAKENHAM_CDA, LMAT_PAKENHAM_CDA_ALT, LMAT_PAKENHAM_OFC, LMAT_PAKENHAM_OFC_ALT, \
    LMAT_PAKENHAM_BFD, LMAT_PAKENHAM_BFD_ALT, LMAT_PAKENHAM_BEW, LMAT_PAKENHAM_BEW_ALT, \
    LMAT_PAKENHAM_NWA, LMAT_PAKENHAM_NWA_ALT, \
    LMAT_CRANBOURNE_CBE, LMAT_CRANBOURNE_CBE_ALT, LMAT_CRANBOURNE_MPK, LMAT_CRANBOURNE_MPK_ALT, \
    LMAT_CRANBOURNE_LBK, LMAT_CRANBOURNE_LBK_ALT, LMAT_CRANBOURNE_HLM, LMAT_CRANBOURNE_HLM_ALT, \
    LMAT_DANDENONG_DNG, LMAT_DANDENONG_DNG_ALT, LMAT_DANDENONG_YMN, LMAT_DANDENONG_YMN_ALT, \
    LMAT_DANDENONG_NPK, LMAT_DANDENONG_NPK_ALT, LMAT_DANDENONG_SNP, LMAT_DANDENONG_SNP_ALT, \
    LMAT_DANDENONG_SPG, LMAT_DANDENONG_SPG_ALT, LMAT_DANDENONG_WTL, LMAT_DANDENONG_WTL_ALT, \
    LMAT_DANDENONG_CLA, LMAT_DANDENONG_CLA_ALT, LMAT_DANDENONG_HUN, LMAT_DANDENONG_HUN_ALT, \
    LMAT_DANDENONG_OAK, LMAT_DANDENONG_OAK_ALT, LMAT_DANDENONG_HUG, LMAT_DANDENONG_HUG_ALT, \
    LMAT_DANDENONG_MRB, LMAT_DANDENONG_MRB_ALT, LMAT_DANDENONG_CNE, LMAT_DANDENONG_CNE_ALT, \
    LMAT_DANDENONG_CFD, LMAT_DANDENONG_CFD_ALT, LMAT_DANDENONG_SYR, LMAT_DANDENONG_SYR_ALT, \
    LMAT_DANDENONG_RMD, LMAT_DANDENONG_RMD_ALT, LMAT_DANDENONG_SSS, LMAT_DANDENONG_SSS_ALT, \
    LMAT_DANDENONG_FGS, LMAT_DANDENONG_FGS_ALT, LMAT_DANDENONG_MCE, LMAT_DANDENONG_MCE_ALT, \
    LMAT_DANDENONG_PAR, LMAT_DANDENONG_PAR_ALT, LMAT_DANDENONG_FSS