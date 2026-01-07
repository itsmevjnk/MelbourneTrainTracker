/* Cross City group (Werribee, Williamstown and Frankston lines) - also includes Stony Point */

#pragma once

#include "hwconfig/led_matrix.h"

/* Werribee line */
#define LMAT_WERRIBEE_WER                               LMAT_BASE(2, 10, 0)
#define LMAT_WERRIBEE_WER_ALT                           LMAT_BASE(2, 8, 0)
#define LMAT_WERRIBEE_HCG                               LMAT_BASE(2, 6, 0)
#define LMAT_WERRIBEE_HCG_ALT                           LMAT_BASE(2, 4, 0)
#define LMAT_WERRIBEE_WLD                               LMAT_BASE(2, 2, 0)
#define LMAT_WERRIBEE_WLD_ALT                           LMAT_BASE(2, 0, 0)
#define LMAT_WERRIBEE_ACF                               LMAT_BASE(2, 1, 0)
#define LMAT_WERRIBEE_ACF_ALT                           LMAT_BASE(2, 3, 0)
#define LMAT_WERRIBEE_LAV                               LMAT_BASE(2, 5, 0)
#define LMAT_WERRIBEE_LAV_ALT                           LMAT_BASE(2, 7, 0) // on Altona Loop - towards WTO
#define LMAT_WERRIBEE_WTO                               LMAT_BASE(2, 10, 1)
#define LMAT_WERRIBEE_WTO_ALT                           LMAT_BASE(2, 8, 1)
#define LMAT_WERRIBEE_ALT                               LMAT_BASE(2, 6, 1)
#define LMAT_WERRIBEE_ALT_ALT                           LMAT_BASE(2, 4, 1)
#define LMAT_WERRIBEE_SHE                               LMAT_BASE(2, 2, 1)
#define LMAT_WERRIBEE_SHE_ALT                           LMAT_BASE(2, 0, 1)
#define LMAT_WERRIBEE_LAV_EX1                           LMAT_BASE(2, 9, 0)  // Altona Loop bypass <-- Laverton end
#define LMAT_WERRIBEE_LAV_EX2                           LMAT_BASE(2, 11, 0) //                    <-- Newport end

/* Williamstown line */
#define LMAT_WILLIAMSTOWN_WIL                           LMAT_BASE(2, 1, 1)
#define LMAT_WILLIAMSTOWN_WIL_ALT                       LMAT_BASE(2, 3, 1)
#define LMAT_WILLIAMSTOWN_WBH                           LMAT_BASE(2, 5, 1)
#define LMAT_WILLIAMSTOWN_WBH_ALT                       LMAT_BASE(2, 7, 1)
#define LMAT_WILLIAMSTOWN_NWN                           LMAT_BASE(2, 9, 1)
#define LMAT_WILLIAMSTOWN_NWN_ALT                       LMAT_BASE(2, 11, 1)

/* Newport group shared track */
#define LMAT_NEWPORT_NPT                                LMAT_BASE(2, 0, 2)
#define LMAT_NEWPORT_NPT_ALT                            LMAT_BASE(2, 2, 2)
#define LMAT_NEWPORT_SPT                                LMAT_BASE(2, 4, 2)
#define LMAT_NEWPORT_SPT_ALT                            LMAT_BASE(2, 6, 2)
#define LMAT_NEWPORT_YVE                                LMAT_BASE(2, 8, 2)
#define LMAT_NEWPORT_YVE_ALT                            LMAT_BASE(2, 10, 2)
#define LMAT_NEWPORT_SEN                                LMAT_BASE(2, 1, 2)
#define LMAT_NEWPORT_SEN_ALT                            LMAT_BASE(2, 3, 2)
#define LMAT_NEWPORT_FSY                                LMAT_BASE(2, 5, 2)
#define LMAT_NEWPORT_FSY_ALT                            LMAT_BASE(2, 7, 2)
#define LMAT_NEWPORT_SKN                                LMAT_BASE(2, 9, 2)
#define LMAT_NEWPORT_SKN_ALT                            LMAT_BASE(2, 11, 2)
#define LMAT_NEWPORT_NME                                LMAT_BASE(2, 11, 5)
#define LMAT_NEWPORT_NME_ALT                            LMAT_BASE(2, 9, 5)
#define LMAT_NEWPORT_SSS                                LMAT_BASE(3, 0, 0)
#define LMAT_NEWPORT_SSS_ALT                            LMAT_BASE(3, 1, 0) // for backward compatibility, this will be the Newport -> Frankston SSS light
#define LMAT_NEWPORT_SSS_ALT_SHM                        LMAT_BASE(3, 2, 1) // and this will be the Newport -> Sandringham one
// TODO: implement switching mechanism between going to Frankston and Sandringham

/* Stony Point line */
#define LMAT_STONY_STY                                  LMAT_BASE(9, 11, 5)
#define LMAT_STONY_STY_ALT                              LMAT_BASE(9, 9, 5)
#define LMAT_STONY_CPT                                  LMAT_BASE(9, 7, 5)
#define LMAT_STONY_CPT_ALT                              LMAT_BASE(9, 5, 5)
#define LMAT_STONY_MRO                                  LMAT_BASE(9, 1, 5)
#define LMAT_STONY_MRO_ALT                              LMAT_BASE(9, 3, 5)
#define LMAT_STONY_BIT                                  LMAT_BASE(9, 0, 5)
#define LMAT_STONY_BIT_ALT                              LMAT_BASE(9, 2, 5)
#define LMAT_STONY_HST                                  LMAT_BASE(9, 4, 5)
#define LMAT_STONY_HST_ALT                              LMAT_BASE(9, 6, 5)
#define LMAT_STONY_TAB                                  LMAT_BASE(9, 8, 5)
#define LMAT_STONY_TAB_ALT                              LMAT_BASE(9, 10, 5)
#define LMAT_STONY_SVE                                  LMAT_BASE(9, 11, 4)
#define LMAT_STONY_SVE_ALT                              LMAT_BASE(9, 9, 4)
#define LMAT_STONY_BXR                                  LMAT_BASE(9, 7, 4)
#define LMAT_STONY_BXR_ALT                              LMAT_BASE(9, 5, 4)
#define LMAT_STONY_LWA                                  LMAT_BASE(9, 3, 4)
#define LMAT_STONY_LWA_ALT                              LMAT_BASE(9, 1, 4)
#define LMAT_STONY_FKN                                  LMAT_BASE(9, 0, 4)

/* Frankston line */
#define LMAT_FRANKSTON_FKN                              LMAT_BASE(9, 2, 4)
#define LMAT_FRANKSTON_FKN_ALT                          LMAT_BASE(9, 4, 4)
#define LMAT_FRANKSTON_KAN                              LMAT_BASE(9, 6, 4)
#define LMAT_FRANKSTON_KAN_ALT                          LMAT_BASE(9, 8, 4)
#define LMAT_FRANKSTON_SEA                              LMAT_BASE(9, 10, 3)
#define LMAT_FRANKSTON_SEA_ALT                          LMAT_BASE(9, 8, 3)
#define LMAT_FRANKSTON_CAR                              LMAT_BASE(9, 6, 3)
#define LMAT_FRANKSTON_CAR_ALT                          LMAT_BASE(9, 4, 3)
#define LMAT_FRANKSTON_BON                              LMAT_BASE(9, 2, 3)
#define LMAT_FRANKSTON_BON_ALT                          LMAT_BASE(9, 0, 3)
#define LMAT_FRANKSTON_CSA                              LMAT_BASE(9, 1, 3)
#define LMAT_FRANKSTON_CSA_ALT                          LMAT_BASE(9, 3, 3)
#define LMAT_FRANKSTON_EDI                              LMAT_BASE(9, 5, 3)
#define LMAT_FRANKSTON_EDI_ALT                          LMAT_BASE(9, 7, 3)
#define LMAT_FRANKSTON_ASP                              LMAT_BASE(9, 9, 3)
#define LMAT_FRANKSTON_ASP_ALT                          LMAT_BASE(9, 11, 3)
#define LMAT_FRANKSTON_MOR                              LMAT_BASE(9, 10, 2)
#define LMAT_FRANKSTON_MOR_ALT                          LMAT_BASE(9, 8, 2)
#define LMAT_FRANKSTON_PKD                              LMAT_BASE(9, 6, 2)
#define LMAT_FRANKSTON_PKD_ALT                          LMAT_BASE(9, 4, 2)
#define LMAT_FRANKSTON_MEN                              LMAT_BASE(9, 2, 2)
#define LMAT_FRANKSTON_MEN_ALT                          LMAT_BASE(9, 0, 2)
#define LMAT_FRANKSTON_CTM                              LMAT_BASE(9, 1, 2)
#define LMAT_FRANKSTON_CTM_ALT                          LMAT_BASE(9, 3, 2)
#define LMAT_FRANKSTON_SOU                              LMAT_BASE(9, 5, 2)
#define LMAT_FRANKSTON_SOU_ALT                          LMAT_BASE(9, 7, 2)
#define LMAT_FRANKSTON_HIG                              LMAT_BASE(9, 9, 2)
#define LMAT_FRANKSTON_HIG_ALT                          LMAT_BASE(9, 11, 2)
#define LMAT_FRANKSTON_MRN                              LMAT_BASE(9, 10, 1)
#define LMAT_FRANKSTON_MRN_ALT                          LMAT_BASE(9, 8, 1)
#define LMAT_FRANKSTON_PAT                              LMAT_BASE(9, 6, 1)
#define LMAT_FRANKSTON_PAT_ALT                          LMAT_BASE(9, 4, 1)
#define LMAT_FRANKSTON_BEN                              LMAT_BASE(9, 2, 1)
#define LMAT_FRANKSTON_BEN_ALT                          LMAT_BASE(9, 0, 1)
#define LMAT_FRANKSTON_MCK                              LMAT_BASE(9, 1, 1)
#define LMAT_FRANKSTON_MCK_ALT                          LMAT_BASE(9, 3, 1)
#define LMAT_FRANKSTON_OMD                              LMAT_BASE(9, 5, 1)
#define LMAT_FRANKSTON_OMD_ALT                          LMAT_BASE(9, 7, 1)
#define LMAT_FRANKSTON_GHY                              LMAT_BASE(9, 9, 1)
#define LMAT_FRANKSTON_GHY_ALT                          LMAT_BASE(9, 11, 1)
#define LMAT_FRANKSTON_CFD                              LMAT_BASE(8, 9, 3)
#define LMAT_FRANKSTON_CFD_ALT                          LMAT_BASE(8, 7, 3)
#define LMAT_FRANKSTON_MAL                              LMAT_BASE(8, 3, 3)
#define LMAT_FRANKSTON_MAL_ALT                          LMAT_BASE(8, 1, 3)
#define LMAT_FRANKSTON_ARM                              LMAT_BASE(2, 1, 5)
#define LMAT_FRANKSTON_ARM_ALT                          LMAT_BASE(2, 3, 5)
#define LMAT_FRANKSTON_TOR                              LMAT_BASE(2, 5, 5)
#define LMAT_FRANKSTON_TOR_ALT                          LMAT_BASE(2, 7, 5)
#define LMAT_FRANKSTON_HKN                              LMAT_BASE(2, 9, 5)
#define LMAT_FRANKSTON_HKN_ALT                          LMAT_BASE(2, 11, 5)
#define LMAT_FRANKSTON_SYR                              LMAT_BASE(3, 9, 2)
#define LMAT_FRANKSTON_SYR_ALT                          LMAT_BASE(3, 11, 2)
#define LMAT_FRANKSTON_RMD                              LMAT_BASE(3, 8, 3)
#define LMAT_FRANKSTON_RMD_ALT                          LMAT_BASE(3, 3, 3)
#define LMAT_FRANKSTON_FSS                              LMAT_BASE(3, 1, 1)

#define LMAT_CROSSCITY \
    LMAT_WERRIBEE_WER, LMAT_WERRIBEE_WER_ALT, LMAT_WERRIBEE_HCG, LMAT_WERRIBEE_HCG_ALT, \
    LMAT_WERRIBEE_WLD, LMAT_WERRIBEE_WLD_ALT, LMAT_WERRIBEE_ACF, LMAT_WERRIBEE_ACF_ALT, \
    LMAT_WERRIBEE_LAV, LMAT_WERRIBEE_LAV_ALT, LMAT_WERRIBEE_WTO, LMAT_WERRIBEE_WTO_ALT, \
    LMAT_WERRIBEE_ALT, LMAT_WERRIBEE_ALT_ALT, LMAT_WERRIBEE_SHE, LMAT_WERRIBEE_SHE_ALT, \
    LMAT_WERRIBEE_LAV_EX1, LMAT_WERRIBEE_LAV_EX2, \
    LMAT_WILLIAMSTOWN_WIL, LMAT_WILLIAMSTOWN_WIL_ALT, LMAT_WILLIAMSTOWN_WBH, LMAT_WILLIAMSTOWN_WBH_ALT, \
    LMAT_WILLIAMSTOWN_NWN, LMAT_WILLIAMSTOWN_NWN_ALT, \
    LMAT_NEWPORT_NPT, LMAT_NEWPORT_NPT_ALT, LMAT_NEWPORT_SPT, LMAT_NEWPORT_SPT_ALT, \
    LMAT_NEWPORT_YVE, LMAT_NEWPORT_YVE_ALT, LMAT_NEWPORT_SEN, LMAT_NEWPORT_SEN_ALT, \
    LMAT_NEWPORT_FSY, LMAT_NEWPORT_FSY_ALT, LMAT_NEWPORT_SKN, LMAT_NEWPORT_SKN_ALT, \
    LMAT_NEWPORT_NME, LMAT_NEWPORT_NME_ALT, LMAT_NEWPORT_SSS, LMAT_NEWPORT_SSS_ALT, LMAT_NEWPORT_SSS_ALT_SHM, \
    LMAT_STONY_STY, LMAT_STONY_STY_ALT, LMAT_STONY_CPT, LMAT_STONY_CPT_ALT, \
    LMAT_STONY_MRO, LMAT_STONY_MRO_ALT, LMAT_STONY_BIT, LMAT_STONY_BIT_ALT, \
    LMAT_STONY_HST, LMAT_STONY_HST_ALT, LMAT_STONY_TAB, LMAT_STONY_TAB_ALT, \
    LMAT_STONY_SVE, LMAT_STONY_SVE_ALT, LMAT_STONY_BXR, LMAT_STONY_BXR_ALT, \
    LMAT_STONY_LWA, LMAT_STONY_LWA_ALT, LMAT_STONY_FKN, \
    LMAT_FRANKSTON_FKN, LMAT_FRANKSTON_FKN_ALT, LMAT_FRANKSTON_KAN, LMAT_FRANKSTON_KAN_ALT, \
    LMAT_FRANKSTON_SEA, LMAT_FRANKSTON_SEA_ALT, LMAT_FRANKSTON_CAR, LMAT_FRANKSTON_CAR_ALT, \
    LMAT_FRANKSTON_BON, LMAT_FRANKSTON_BON_ALT, LMAT_FRANKSTON_CSA, LMAT_FRANKSTON_CSA_ALT, \
    LMAT_FRANKSTON_EDI, LMAT_FRANKSTON_EDI_ALT, LMAT_FRANKSTON_ASP, LMAT_FRANKSTON_ASP_ALT, \
    LMAT_FRANKSTON_MOR, LMAT_FRANKSTON_MOR_ALT, LMAT_FRANKSTON_PKD, LMAT_FRANKSTON_PKD_ALT, \
    LMAT_FRANKSTON_MEN, LMAT_FRANKSTON_MEN_ALT, LMAT_FRANKSTON_CTM, LMAT_FRANKSTON_CTM_ALT, \
    LMAT_FRANKSTON_SOU, LMAT_FRANKSTON_SOU_ALT, LMAT_FRANKSTON_HIG, LMAT_FRANKSTON_HIG_ALT, \
    LMAT_FRANKSTON_MRN, LMAT_FRANKSTON_MRN_ALT, LMAT_FRANKSTON_PAT, LMAT_FRANKSTON_PAT_ALT, \
    LMAT_FRANKSTON_BEN, LMAT_FRANKSTON_BEN_ALT, LMAT_FRANKSTON_MCK, LMAT_FRANKSTON_MCK_ALT, \
    LMAT_FRANKSTON_OMD, LMAT_FRANKSTON_OMD_ALT, LMAT_FRANKSTON_GHY, LMAT_FRANKSTON_GHY_ALT, \
    LMAT_FRANKSTON_CFD, LMAT_FRANKSTON_CFD_ALT, LMAT_FRANKSTON_MAL, LMAT_FRANKSTON_MAL_ALT, \
    LMAT_FRANKSTON_ARM, LMAT_FRANKSTON_ARM_ALT, LMAT_FRANKSTON_TOR, LMAT_FRANKSTON_TOR_ALT, \
    LMAT_FRANKSTON_HKN, LMAT_FRANKSTON_HKN_ALT, \
    LMAT_FRANKSTON_SYR, LMAT_FRANKSTON_SYR_ALT, LMAT_FRANKSTON_RMD, LMAT_FRANKSTON_RMD_ALT, \
    LMAT_FRANKSTON_FSS
