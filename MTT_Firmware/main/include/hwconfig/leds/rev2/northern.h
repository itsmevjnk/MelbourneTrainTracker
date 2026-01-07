/* Northern group (Craigieburn, Upfield, Sunbury) */

#pragma once

#include "hwconfig/led_matrix.h"

/* Craigieburn line */
#define LMAT_CRAIGIEBURN_CGB                            LMAT_BASE(4, 7, 0)
#define LMAT_CRAIGIEBURN_CGB_ALT                        LMAT_BASE(4, 5, 0)
#define LMAT_CRAIGIEBURN_RXP                            LMAT_BASE(4, 3, 0)
#define LMAT_CRAIGIEBURN_RXP_ALT                        LMAT_BASE(4, 1, 0)
#define LMAT_CRAIGIEBURN_CLO                            LMAT_BASE(4, 0, 0)
#define LMAT_CRAIGIEBURN_CLO_ALT                        LMAT_BASE(4, 2, 0)
#define LMAT_CRAIGIEBURN_BMS                            LMAT_BASE(4, 8, 0)
#define LMAT_CRAIGIEBURN_BMS_ALT                        LMAT_BASE(4, 10, 0)
#define LMAT_CRAIGIEBURN_JAC                            LMAT_BASE(4, 11, 1)
#define LMAT_CRAIGIEBURN_JAC_ALT                        LMAT_BASE(4, 9, 1)
#define LMAT_CRAIGIEBURN_GRY                            LMAT_BASE(4, 7, 1)
#define LMAT_CRAIGIEBURN_GRY_ALT                        LMAT_BASE(4, 5, 1)
#define LMAT_CRAIGIEBURN_OKP                            LMAT_BASE(4, 3, 1)
#define LMAT_CRAIGIEBURN_OKP_ALT                        LMAT_BASE(4, 1, 1)
#define LMAT_CRAIGIEBURN_PVL                            LMAT_BASE(4, 0, 1)
#define LMAT_CRAIGIEBURN_PVL_ALT                        LMAT_BASE(4, 2, 1)
#define LMAT_CRAIGIEBURN_SME                            LMAT_BASE(4, 4, 1)
#define LMAT_CRAIGIEBURN_SME_ALT                        LMAT_BASE(4, 6, 1)
#define LMAT_CRAIGIEBURN_GBV                            LMAT_BASE(4, 8, 1)
#define LMAT_CRAIGIEBURN_GBV_ALT                        LMAT_BASE(4, 10, 1)
#define LMAT_CRAIGIEBURN_ESD                            LMAT_BASE(4, 0, 2)
#define LMAT_CRAIGIEBURN_ESD_ALT                        LMAT_BASE(4, 2, 2)
#define LMAT_CRAIGIEBURN_MPD                            LMAT_BASE(4, 8, 2)
#define LMAT_CRAIGIEBURN_MPD_ALT                        LMAT_BASE(4, 10, 2)
#define LMAT_CRAIGIEBURN_ASV                            LMAT_BASE(4, 6, 2)
#define LMAT_CRAIGIEBURN_ASV_ALT                        LMAT_BASE(4, 11, 2)

/* Flemington Racecourse line */
#define LMAT_FLEMINGTON_RCE                             LMAT_BASE(4, 5, 2)
#define LMAT_FLEMINGTON_RCE_ALT                         LMAT_BASE(4, 3, 2)
#define LMAT_FLEMINGTON_SGS                             LMAT_BASE(4, 9, 2)
#define LMAT_FLEMINGTON_SGS_ALT                         LMAT_BASE(4, 1, 2)

/* Craigieburn (continued - shared with Flemington Racecourse line) */
#define LMAT_CRAIGIEBURN_NKT                            LMAT_BASE(4, 1, 3)
#define LMAT_CRAIGIEBURN_NKT_ALT                        LMAT_BASE(4, 3, 3)
#define LMAT_CRAIGIEBURN_KEN                            LMAT_BASE(4, 5, 3)
#define LMAT_CRAIGIEBURN_KEN_ALT                        LMAT_BASE(4, 7, 3)

/* North Melbourne station */
#define LMAT_NORTHERN_NME                               LMAT_BASE(2, 3, 5)
#define LMAT_NORTHERN_NME_ALT                           LMAT_BASE(2, 1, 5) // towards SSS

/* Sunbury line */
#define LMAT_SUNBURY_SUY                                LMAT_BASE(0, 4, 3)
#define LMAT_SUNBURY_SUY_ALT                            LMAT_BASE(0, 0, 3)
#define LMAT_SUNBURY_DRT                                LMAT_BASE(1, 10, 0)
#define LMAT_SUNBURY_DRT_ALT                            LMAT_BASE(1, 8, 0)
#define LMAT_SUNBURY_WGS                                LMAT_BASE(1, 2, 0)
#define LMAT_SUNBURY_WGS_ALT                            LMAT_BASE(1, 0, 0)
#define LMAT_SUNBURY_KPL                                LMAT_BASE(1, 1, 0)
#define LMAT_SUNBURY_KPL_ALT                            LMAT_BASE(1, 3, 0)
#define LMAT_SUNBURY_SAB                                LMAT_BASE(1, 5, 0)
#define LMAT_SUNBURY_SAB_ALT                            LMAT_BASE(1, 7, 0)
#define LMAT_SUNBURY_GIN                                LMAT_BASE(1, 9, 0)
#define LMAT_SUNBURY_GIN_ALT                            LMAT_BASE(1, 11, 0)
#define LMAT_SUNBURY_ALB                                LMAT_BASE(1, 10, 5)
#define LMAT_SUNBURY_ALB_ALT                            LMAT_BASE(1, 8, 5)
#define LMAT_SUNBURY_SUN                                LMAT_BASE(1, 10, 4)
#define LMAT_SUNBURY_SUN_ALT                            LMAT_BASE(1, 8, 4)
#define LMAT_SUNBURY_TOT                                LMAT_BASE(1, 6, 4)
#define LMAT_SUNBURY_TOT_ALT                            LMAT_BASE(1, 4, 4)
#define LMAT_SUNBURY_WFY                                LMAT_BASE(1, 2, 4)
#define LMAT_SUNBURY_WFY_ALT                            LMAT_BASE(1, 0, 4)
#define LMAT_SUNBURY_MFY                                LMAT_BASE(1, 1, 4)
#define LMAT_SUNBURY_MFY_ALT                            LMAT_BASE(1, 3, 4)
#define LMAT_SUNBURY_FSY                                LMAT_BASE(1, 5, 4)
#define LMAT_SUNBURY_FSY_ALT                            LMAT_BASE(1, 7, 4) // towards NME

/* Upfield line */
#define LMAT_UPFIELD_UFD                                LMAT_BASE(4, 11, 5)
#define LMAT_UPFIELD_UFD_ALT                            LMAT_BASE(4, 9, 5)
#define LMAT_UPFIELD_GOW                                LMAT_BASE(4, 7, 5)
#define LMAT_UPFIELD_GOW_ALT                            LMAT_BASE(4, 5, 5)
#define LMAT_UPFIELD_FAK                                LMAT_BASE(4, 3, 5)
#define LMAT_UPFIELD_FAK_ALT                            LMAT_BASE(4, 1, 5)
#define LMAT_UPFIELD_MYN                                LMAT_BASE(4, 0, 5)
#define LMAT_UPFIELD_MYN_ALT                            LMAT_BASE(4, 2, 5)
#define LMAT_UPFIELD_BAT                                LMAT_BASE(4, 4, 5)
#define LMAT_UPFIELD_BAT_ALT                            LMAT_BASE(4, 6, 5)
#define LMAT_UPFIELD_COB                                LMAT_BASE(4, 8, 5)
#define LMAT_UPFIELD_COB_ALT                            LMAT_BASE(4, 10, 5)
#define LMAT_UPFIELD_MLD                                LMAT_BASE(4, 10, 4)
#define LMAT_UPFIELD_MLD_ALT                            LMAT_BASE(4, 8, 4)
#define LMAT_UPFIELD_ASY                                LMAT_BASE(4, 6, 4)
#define LMAT_UPFIELD_ASY_ALT                            LMAT_BASE(4, 4, 4)
#define LMAT_UPFIELD_BWK                                LMAT_BASE(4, 2, 4)
#define LMAT_UPFIELD_BWK_ALT                            LMAT_BASE(4, 0, 4)
#define LMAT_UPFIELD_JWL                                LMAT_BASE(4, 1, 4)
#define LMAT_UPFIELD_JWL_ALT                            LMAT_BASE(4, 3, 4)
#define LMAT_UPFIELD_RPK                                LMAT_BASE(4, 5, 4)
#define LMAT_UPFIELD_RPK_ALT                            LMAT_BASE(4, 7, 4)
#define LMAT_UPFIELD_FBD                                LMAT_BASE(4, 9, 4)
#define LMAT_UPFIELD_FBD_ALT                            LMAT_BASE(4, 11, 4)
#define LMAT_UPFIELD_MAC                                LMAT_BASE(4, 11, 3)
#define LMAT_UPFIELD_MAC_ALT                            LMAT_BASE(4, 9, 3)

/* city stations */
#define LMAT_NORTHERN_SSS                               LMAT_BASE(3, 10, 0)
#define LMAT_NORTHERN_SSS_ALT                           LMAT_BASE(3, 11, 0)
#define LMAT_NORTHERN_FGS                               LMAT_BASE(3, 3, 4)
#define LMAT_NORTHERN_FGS_ALT                           LMAT_BASE(3, 10, 4)
#define LMAT_NORTHERN_MCE                               LMAT_BASE(3, 3, 5)
#define LMAT_NORTHERN_MCE_ALT                           LMAT_BASE(3, 6, 5)
#define LMAT_NORTHERN_PAR                               LMAT_BASE(3, 6, 2)
#define LMAT_NORTHERN_PAR_ALT                           LMAT_BASE(3, 3, 2)
#define LMAT_NORTHERN_FSS                               LMAT_BASE(3, 11, 1)
#define LMAT_NORTHERN_FSS_ALT                           LMAT_BASE(3, 6, 1)

#define LMAT_NORTHERN                                   \
    LMAT_CRAIGIEBURN_CGB, LMAT_CRAIGIEBURN_CGB_ALT,    \
    LMAT_CRAIGIEBURN_RXP, LMAT_CRAIGIEBURN_RXP_ALT,    \
    LMAT_CRAIGIEBURN_CLO, LMAT_CRAIGIEBURN_CLO_ALT,    \
    LMAT_CRAIGIEBURN_BMS, LMAT_CRAIGIEBURN_BMS_ALT,    \
    LMAT_CRAIGIEBURN_JAC, LMAT_CRAIGIEBURN_JAC_ALT,    \
    LMAT_CRAIGIEBURN_GRY, LMAT_CRAIGIEBURN_GRY_ALT,    \
    LMAT_CRAIGIEBURN_OKP, LMAT_CRAIGIEBURN_OKP_ALT,    \
    LMAT_CRAIGIEBURN_PVL, LMAT_CRAIGIEBURN_PVL_ALT,    \
    LMAT_CRAIGIEBURN_SME, LMAT_CRAIGIEBURN_SME_ALT,    \
    LMAT_CRAIGIEBURN_GBV, LMAT_CRAIGIEBURN_GBV_ALT,    \
    LMAT_CRAIGIEBURN_ESD, LMAT_CRAIGIEBURN_ESD_ALT,    \
    LMAT_CRAIGIEBURN_MPD, LMAT_CRAIGIEBURN_MPD_ALT,    \
    LMAT_CRAIGIEBURN_ASV, LMAT_CRAIGIEBURN_ASV_ALT,    \
    LMAT_CRAIGIEBURN_NKT, LMAT_CRAIGIEBURN_NKT_ALT,    \
    LMAT_CRAIGIEBURN_KEN, LMAT_CRAIGIEBURN_KEN_ALT,    \
    LMAT_NORTHERN_NME, LMAT_NORTHERN_NME_ALT,          \
    LMAT_SUNBURY_SUY, LMAT_SUNBURY_SUY_ALT,            \
    LMAT_SUNBURY_DRT, LMAT_SUNBURY_DRT_ALT,            \
    LMAT_SUNBURY_WGS, LMAT_SUNBURY_WGS_ALT,            \
    LMAT_SUNBURY_KPL, LMAT_SUNBURY_KPL_ALT,            \
    LMAT_SUNBURY_SAB, LMAT_SUNBURY_SAB_ALT,            \
    LMAT_SUNBURY_GIN, LMAT_SUNBURY_GIN_ALT,            \
    LMAT_SUNBURY_ALB, LMAT_SUNBURY_ALB_ALT,            \
    LMAT_SUNBURY_SUN, LMAT_SUNBURY_SUN_ALT,            \
    LMAT_SUNBURY_TOT, LMAT_SUNBURY_TOT_ALT,            \
    LMAT_SUNBURY_WFY, LMAT_SUNBURY_WFY_ALT,            \
    LMAT_SUNBURY_MFY, LMAT_SUNBURY_MFY_ALT,            \
    LMAT_SUNBURY_FSY, LMAT_SUNBURY_FSY_ALT,            \
    LMAT_UPFIELD_UFD, LMAT_UPFIELD_UFD_ALT,            \
    LMAT_UPFIELD_GOW, LMAT_UPFIELD_GOW_ALT,            \
    LMAT_UPFIELD_FAK, LMAT_UPFIELD_FAK_ALT,            \
    LMAT_UPFIELD_MYN, LMAT_UPFIELD_MYN_ALT,            \
    LMAT_UPFIELD_BAT, LMAT_UPFIELD_BAT_ALT,            \
    LMAT_UPFIELD_COB, LMAT_UPFIELD_COB_ALT,            \
    LMAT_UPFIELD_MLD, LMAT_UPFIELD_MLD_ALT,            \
    LMAT_UPFIELD_ASY, LMAT_UPFIELD_ASY_ALT,            \
    LMAT_UPFIELD_BWK, LMAT_UPFIELD_BWK_ALT,            \
    LMAT_UPFIELD_JWL, LMAT_UPFIELD_JWL_ALT,            \
    LMAT_UPFIELD_RPK, LMAT_UPFIELD_RPK_ALT,            \
    LMAT_UPFIELD_FBD, LMAT_UPFIELD_FBD_ALT,            \
    LMAT_UPFIELD_MAC, LMAT_UPFIELD_MAC_ALT,            \
    LMAT_NORTHERN_SSS, LMAT_NORTHERN_SSS_ALT,          \
    LMAT_NORTHERN_FGS, LMAT_NORTHERN_FGS_ALT,          \
    LMAT_NORTHERN_MCE, LMAT_NORTHERN_MCE_ALT,          \
    LMAT_NORTHERN_PAR, LMAT_NORTHERN_PAR_ALT,          \
    LMAT_NORTHERN_FSS, LMAT_NORTHERN_FSS_ALT

#define LMAT_FLEMINGTON                                 \
    LMAT_FLEMINGTON_RCE, LMAT_FLEMINGTON_RCE_ALT,      \
    LMAT_FLEMINGTON_SGS, LMAT_FLEMINGTON_SGS_ALT
