/* V/Line lines */

#pragma once

#include "hwconfig/led_matrix.h"

#define LMAT_VLINE_HAS_REGIONAL 1 // Rev2 board has V/Line regional sections
#define LMAT_VLINE_HAS_BENDIGO 1 // Rev2 board has Bendigo line
#define LMAT_VLINE_HAS_SEYMOUR 1 // Rev2 board also has Seymour line
// NOTE: technically it's possible for a board to have Bendigo/Seymour line but only for the metropolitan section, just like the Wyndham, Melton and part of the Gippsland line on Rev1 board

/* Ararat line */
#define LMAT_ARARAT_ART                                 LMAT_BASE(0, 11, 4)
#define LMAT_ARARAT_ART_ALT                             LMAT_BASE(0, 9, 4)
#define LMAT_ARARAT_BET                                 LMAT_BASE(0, 7, 4)
#define LMAT_ARARAT_BET_ALT                             LMAT_BASE(0, 5, 4)
#define LMAT_ARARAT_WED                                 LMAT_BASE(0, 3, 4)
#define LMAT_ARARAT_WED_ALT                             LMAT_BASE(0, 1, 4)

/* Maryborough line */
#define LMAT_MARYBOROUGH_MYB                            LMAT_BASE(0, 0, 4)
#define LMAT_MARYBOROUGH_MYB_ALT                        LMAT_BASE(0, 2, 4)
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
#define LMAT_GIPPSLAND_GFD                              LMAT_BASE(10, 1, 1)
#define LMAT_GIPPSLAND_GFD_ALT                          LMAT_BASE(10, 3, 1)
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

/* Echuca line */
#define LMAT_ECHUCA_ECH                                 LMAT_BASE(0, 0, 0)
#define LMAT_ECHUCA_ECH_ALT                             LMAT_BASE(0, 2, 0)
#define LMAT_ECHUCA_ROR                                 LMAT_BASE(0, 4, 0)
#define LMAT_ECHUCA_ROR_ALT                             LMAT_BASE(0, 6, 0)
#define LMAT_ECHUCA_EME                                 LMAT_BASE(0, 8, 0)
#define LMAT_ECHUCA_EME_ALT                             LMAT_BASE(0, 10, 0)
#define LMAT_ECHUCA_GRG                                 LMAT_BASE(0, 0, 1)
#define LMAT_ECHUCA_GRG_ALT                             LMAT_BASE(0, 2, 1)
#define LMAT_ECHUCA_HUY                                 LMAT_BASE(0, 4, 1)
#define LMAT_ECHUCA_HUY_ALT                             LMAT_BASE(0, 6, 1)
#define LMAT_ECHUCA_EPM                                 LMAT_BASE(0, 8, 1)
#define LMAT_ECHUCA_EPM_ALT                             LMAT_BASE(0, 10, 1)

/* Swan Hill line */
#define LMAT_SWANHILL_SWL                               LMAT_BASE(0, 1, 0)
#define LMAT_SWANHILL_SWL_ALT                           LMAT_BASE(0, 3, 0)
#define LMAT_SWANHILL_KER                               LMAT_BASE(0, 5, 0)
#define LMAT_SWANHILL_KER_ALT                           LMAT_BASE(0, 7, 0)
#define LMAT_SWANHILL_PYD                               LMAT_BASE(0, 9, 0)
#define LMAT_SWANHILL_PYD_ALT                           LMAT_BASE(0, 11, 0)
#define LMAT_SWANHILL_DGE                               LMAT_BASE(0, 1, 1)
#define LMAT_SWANHILL_DGE_ALT                           LMAT_BASE(0, 3, 1)
#define LMAT_SWANHILL_RAY                               LMAT_BASE(0, 5, 1)
#define LMAT_SWANHILL_RAY_ALT                           LMAT_BASE(0, 7, 1)
#define LMAT_SWANHILL_EHK                               LMAT_BASE(0, 9, 1)
#define LMAT_SWANHILL_EHK_ALT                           LMAT_BASE(0, 11, 1)

/* Bendigo shared track */
#define LMAT_BENDIGO_BGO                                LMAT_BASE(0, 0, 2)
#define LMAT_BENDIGO_BGO_ALT                            LMAT_BASE(0, 2, 2)
#define LMAT_BENDIGO_KFT                                LMAT_BASE(0, 4, 2)
#define LMAT_BENDIGO_KFT_ALT                            LMAT_BASE(0, 6, 2)
#define LMAT_BENDIGO_CME                                LMAT_BASE(0, 8, 2)
#define LMAT_BENDIGO_CME_ALT                            LMAT_BASE(0, 10, 2)
#define LMAT_BENDIGO_MBY                                LMAT_BASE(0, 3, 2)
#define LMAT_BENDIGO_MBY_ALT                            LMAT_BASE(0, 5, 2)
#define LMAT_BENDIGO_KYN                                LMAT_BASE(0, 7, 2)
#define LMAT_BENDIGO_KYN_ALT                            LMAT_BASE(0, 9, 2)
#define LMAT_BENDIGO_WNO                                LMAT_BASE(0, 11, 2)
#define LMAT_BENDIGO_WNO_ALT                            LMAT_BASE(0, 1, 2)
#define LMAT_BENDIGO_MDN                                LMAT_BASE(0, 1, 3)
#define LMAT_BENDIGO_MDN_ALT                            LMAT_BASE(0, 11, 3)
#define LMAT_BENDIGO_GIS                                LMAT_BASE(0, 9, 3)
#define LMAT_BENDIGO_GIS_ALT                            LMAT_BASE(0, 7, 3)
#define LMAT_BENDIGO_RIK                                LMAT_BASE(0, 5, 3)
#define LMAT_BENDIGO_RIK_ALT                            LMAT_BASE(0, 3, 3)
#define LMAT_BENDIGO_CKF                                LMAT_BASE(0, 10, 3)
#define LMAT_BENDIGO_CKF_ALT                            LMAT_BASE(0, 8, 3)
#define LMAT_BENDIGO_SUY                                LMAT_BASE(0, 6, 3)
#define LMAT_BENDIGO_SUY_ALT                            LMAT_BASE(0, 2, 3)
#define LMAT_BENDIGO_WGS                                LMAT_BASE(1, 6, 0)
#define LMAT_BENDIGO_WGS_ALT                            LMAT_BASE(1, 4, 0)
// NOTE: WGS <-> FSY transition for Bendigo line will have two LEDs between the stations: LMAT_BENDIGO_WGS_ALT (WGS side) and LMAT_VLWEST_SUN_ALT (FSY side)

/* Shepparton line */
#define LMAT_SHEPPARTON_SNH                             LMAT_BASE(5, 1, 4)
#define LMAT_SHEPPARTON_SNH_ALT                         LMAT_BASE(5, 5, 4)
#define LMAT_SHEPPARTON_MPA                             LMAT_BASE(5, 7, 4)
#define LMAT_SHEPPARTON_MPA_ALT                         LMAT_BASE(5, 9, 4)
#define LMAT_SHEPPARTON_MST                             LMAT_BASE(5, 11, 4)
#define LMAT_SHEPPARTON_MST_ALT                         LMAT_BASE(5, 3, 4)
#define LMAT_SHEPPARTON_NGE                             LMAT_BASE(5, 0, 4)
#define LMAT_SHEPPARTON_NGE_ALT                         LMAT_BASE(5, 2, 4)

/* Albury line */
#define LMAT_ALBURY_ABY                                 LMAT_BASE(5, 0, 0) // this is in NSW, but luckily there are no Victorian stations with the code ABY
#define LMAT_ALBURY_ABY_ALT                             LMAT_BASE(5, 2, 0)
#define LMAT_ALBURY_WOD                                 LMAT_BASE(5, 4, 0)
#define LMAT_ALBURY_WOD_ALT                             LMAT_BASE(5, 6, 0)
#define LMAT_ALBURY_CHI                                 LMAT_BASE(5, 8, 0)
#define LMAT_ALBURY_CHI_ALT                             LMAT_BASE(5, 10, 0)
#define LMAT_ALBURY_SPTV                                LMAT_BASE(5, 0, 1) // Springhurst station, doesn't show up in GTFS timetable for some reason (guessed station ID)
#define LMAT_ALBURY_SPTV_ALT                            LMAT_BASE(5, 2, 1)
#define LMAT_ALBURY_WRT                                 LMAT_BASE(5, 4, 1)
#define LMAT_ALBURY_WRT_ALT                             LMAT_BASE(5, 6, 1)
#define LMAT_ALBURY_BXA                                 LMAT_BASE(5, 8, 1)
#define LMAT_ALBURY_BXA_ALT                             LMAT_BASE(5, 10, 1)
#define LMAT_ALBURY_VTN                                 LMAT_BASE(5, 0, 5)
#define LMAT_ALBURY_VTN_ALT                             LMAT_BASE(5, 2, 5)
#define LMAT_ALBURY_EOA                                 LMAT_BASE(5, 4, 5)
#define LMAT_ALBURY_EOA_ALT                             LMAT_BASE(5, 6, 5)
#define LMAT_ALBURY_AVL                                 LMAT_BASE(5, 8, 5)
#define LMAT_ALBURY_AVL_ALT                             LMAT_BASE(5, 10, 5)

/* Seymour shared track */
#define LMAT_SEYMOUR_SER                                LMAT_BASE(5, 4, 4)
#define LMAT_SEYMOUR_SER_ALT                            LMAT_BASE(5, 6, 4)
#define LMAT_SEYMOUR_TOK                                LMAT_BASE(5, 8, 4)
#define LMAT_SEYMOUR_TOK_ALT                            LMAT_BASE(5, 10, 4)
#define LMAT_SEYMOUR_BRF                                LMAT_BASE(5, 0, 3)
#define LMAT_SEYMOUR_BRF_ALT                            LMAT_BASE(5, 2, 3)
#define LMAT_SEYMOUR_KET                                LMAT_BASE(5, 4, 3)
#define LMAT_SEYMOUR_KET_ALT                            LMAT_BASE(5, 6, 3)
#define LMAT_SEYMOUR_WDG                                LMAT_BASE(5, 8, 3)
#define LMAT_SEYMOUR_WDG_ALT                            LMAT_BASE(5, 10, 3)
#define LMAT_SEYMOUR_HCJ                                LMAT_BASE(4, 0, 3)
#define LMAT_SEYMOUR_HCJ_ALT                            LMAT_BASE(4, 2, 3)
#define LMAT_SEYMOUR_WAN                                LMAT_BASE(4, 4, 3)
#define LMAT_SEYMOUR_WAN_ALT                            LMAT_BASE(4, 6, 3)
#define LMAT_SEYMOUR_DBK                                LMAT_BASE(4, 8, 3)
#define LMAT_SEYMOUR_DBK_ALT                            LMAT_BASE(4, 10, 3)
#define LMAT_SEYMOUR_CGB                                LMAT_BASE(4, 9, 0)
#define LMAT_SEYMOUR_CGB_ALT                            LMAT_BASE(4, 11, 0)
#define LMAT_SEYMOUR_BMS                                LMAT_BASE(4, 6, 0)
#define LMAT_SEYMOUR_BMS_ALT                            LMAT_BASE(4, 4, 0)
#define LMAT_SEYMOUR_ESD                                LMAT_BASE(4, 4, 2)
#define LMAT_SEYMOUR_ESD_ALT                            LMAT_BASE(4, 7, 2)
#define LMAT_SEYMOUR_NME                                LMAT_BASE(2, 7, 5)
#define LMAT_SEYMOUR_NME_ALT                            LMAT_BASE(2, 5, 5)

#define LMAT_VLINE \
    LMAT_ARARAT_ART, LMAT_ARARAT_ART_ALT, LMAT_ARARAT_BET, LMAT_ARARAT_BET_ALT, \
    LMAT_ARARAT_WED, LMAT_ARARAT_WED_ALT, LMAT_MARYBOROUGH_MYB, LMAT_MARYBOROUGH_MYB_ALT, \
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
    LMAT_GIPPSLAND_TYN_ALT, LMAT_GIPPSLAND_GFD, LMAT_GIPPSLAND_GFD_ALT, LMAT_GIPPSLAND_BYP, \
    LMAT_GIPPSLAND_BYP_ALT, LMAT_GIPPSLAND_LWY, LMAT_GIPPSLAND_LWY_ALT, LMAT_GIPPSLAND_DRN, \
    LMAT_GIPPSLAND_DRN_ALT, LMAT_GIPPSLAND_WGL, LMAT_GIPPSLAND_WGL_ALT, LMAT_GIPPSLAND_YON, \
    LMAT_GIPPSLAND_YON_ALT, LMAT_GIPPSLAND_TAR, LMAT_GIPPSLAND_TAR_ALT, LMAT_GIPPSLAND_MOE, \
    LMAT_GIPPSLAND_MOE_ALT, LMAT_GIPPSLAND_MWL, LMAT_GIPPSLAND_MWL_ALT, LMAT_GIPPSLAND_TRN, \
    LMAT_GIPPSLAND_TRN_ALT, LMAT_GIPPSLAND_ROE, LMAT_GIPPSLAND_ROE_ALT, LMAT_GIPPSLAND_SAE, \
    LMAT_GIPPSLAND_SAE_ALT, LMAT_GIPPSLAND_STD, LMAT_GIPPSLAND_STD_ALT, LMAT_GIPPSLAND_BDE, \
    LMAT_GIPPSLAND_BDE_ALT

