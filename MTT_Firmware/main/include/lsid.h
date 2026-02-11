/* line and station IDs */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include "hwconfig/led_matrix.h"
#include "colours.h"

/* infrastructure (line/station) ID */
typedef uint32_t infraid_t;

#define INFRAID(str)                                ((infraid_t)str[0] | ((infraid_t)str[1] << 8) | ((infraid_t)str[2] << 16) | ((infraid_t)str[3] << 24)) // convert line/station 4-letter code into unique 32-bit ID
#define INFRAID2STR_FMT                             "0x%08lx (%c%c%c%c)" 
#define INFRAID2STR(id)                             (uint32_t)id, (char)(((id) >> 0) & 0x7F), (char)(((id) >> 8) & 0x7F), (char)(((id) >> 16) & 0x7F), (char)(((id) >> 24) & 0x7F) // format line/station ID for printf

#define INFRAID_NULL                                0
#define INFRAID_HLM                                 INFRAID("HLM ")
#define INFRAID_MCH                                 INFRAID("MCH ")
#define INFRAID_MRN                                 INFRAID("MRN ")
#define INFRAID_TAB                                 INFRAID("TAB ")
#define INFRAID_GAR                                 INFRAID("GAR ")
#define INFRAID_TGA                                 INFRAID("TGA ")
#define INFRAID_OFC                                 INFRAID("OFC ")
#define INFRAID_MCK                                 INFRAID("MCK ")
#define INFRAID_MLK                                 INFRAID("MLK ")
#define INFRAID_HIG                                 INFRAID("HIG ")
#define INFRAID_WIL                                 INFRAID("WIL ")
#define INFRAID_SYN                                 INFRAID("SYN ")
#define INFRAID_SYR                                 INFRAID("SYR ")
#define INFRAID_CDN                                 INFRAID("CDN ")
#define INFRAID_NWN                                 INFRAID("NWN ")
#define INFRAID_ERM                                 INFRAID("ERM ")
#define INFRAID_RVH                                 INFRAID("RVH ")
#define INFRAID_SPG                                 INFRAID("SPG ")
#define INFRAID_SPT                                 INFRAID("SPT ")
#define INFRAID_CBE                                 INFRAID("CBE ")
#define INFRAID_WAT                                 INFRAID("WAT ")
#define INFRAID_NWA                                 INFRAID("NWA ")
#define INFRAID_SMG                                 INFRAID("SMG ")
#define INFRAID_NBN                                 INFRAID("NBN ")
#define INFRAID_EPH                                 INFRAID("EPH ")
#define INFRAID_EMV                                 INFRAID("EMV ")
#define INFRAID_CPT                                 INFRAID("CPT ")
#define INFRAID_HEY                                 INFRAID("HEY ")
#define INFRAID_RWE                                 INFRAID("RWE ")
#define INFRAID_BOR                                 INFRAID("BOR ")
#define INFRAID_SHM                                 INFRAID("SHM ")
#define INFRAID_JWL                                 INFRAID("JWL ")
#define INFRAID_MEN                                 INFRAID("MEN ")
#define INFRAID_WGS                                 INFRAID("WGS ")
#define INFRAID_WTO                                 INFRAID("WTO ")
#define INFRAID_PRA                                 INFRAID("PRA ")
#define INFRAID_DLG                                 INFRAID("DLG ")
#define INFRAID_ESD                                 INFRAID("ESD ")
#define INFRAID_ELS                                 INFRAID("ELS ")
#define INFRAID_BEN                                 INFRAID("BEN ")
#define INFRAID_RCE                                 INFRAID("RCE ")
#define INFRAID_KEN                                 INFRAID("KEN ")
#define INFRAID_LIL                                 INFRAID("LIL ")
#define INFRAID_FTG                                 INFRAID("FTG ")
#define INFRAID_SEA                                 INFRAID("SEA ")
#define INFRAID_MCD                                 INFRAID("MCD ")
#define INFRAID_WRM                                 INFRAID("WRM ")
#define INFRAID_NWG                                 INFRAID("NWG ")
#define INFRAID_CLO                                 INFRAID("CLO ")
#define INFRAID_KYG                                 INFRAID("KYG ")
#define INFRAID_BAY                                 INFRAID("BAY ")
#define INFRAID_LWA                                 INFRAID("LWA ")
#define INFRAID_UNN                                 INFRAID("UNN ")
#define INFRAID_FBD                                 INFRAID("FBD ")
#define INFRAID_CGB                                 INFRAID("CGB ")
#define INFRAID_NPT                                 INFRAID("NPT ")
#define INFRAID_WFY                                 INFRAID("WFY ")
#define INFRAID_ARR                                 INFRAID("ARR ")
#define INFRAID_WTT                                 INFRAID("WTT ")
#define INFRAID_DEN                                 INFRAID("DEN ")
#define INFRAID_SVE                                 INFRAID("SVE ")
#define INFRAID_TSN                                 INFRAID("TSN ")
#define INFRAID_COB                                 INFRAID("COB ")
#define INFRAID_WVL                                 INFRAID("WVL ")
#define INFRAID_MLD                                 INFRAID("MLD ")
#define INFRAID_BEG                                 INFRAID("BEG ")
#define INFRAID_CDA                                 INFRAID("CDA ")
#define INFRAID_STY                                 INFRAID("STY ")
#define INFRAID_GHY                                 INFRAID("GHY ")
#define INFRAID_CAM                                 INFRAID("CAM ")
#define INFRAID_ELT                                 INFRAID("ELT ")
#define INFRAID_BIT                                 INFRAID("BIT ")
#define INFRAID_MBN                                 INFRAID("MBN ")
#define INFRAID_MDD                                 INFRAID("MDD ")
#define INFRAID_TOT                                 INFRAID("TOT ")
#define INFRAID_KPL                                 INFRAID("KPL ")
#define INFRAID_CWD                                 INFRAID("CWD ")
#define INFRAID_CLA                                 INFRAID("CLA ")
#define INFRAID_RIP                                 INFRAID("RIP ")
#define INFRAID_TCM                                 INFRAID("TCM ")
#define INFRAID_HST                                 INFRAID("HST ")
#define INFRAID_REG                                 INFRAID("REG ")
#define INFRAID_HUN                                 INFRAID("HUN ")
#define INFRAID_SSS                                 INFRAID("SSS ")
#define INFRAID_MYN                                 INFRAID("MYN ")
#define INFRAID_HKN                                 INFRAID("HKN ")
#define INFRAID_WTG                                 INFRAID("WTG ")
#define INFRAID_PKD                                 INFRAID("PKD ")
#define INFRAID_ASH                                 INFRAID("ASH ")
#define INFRAID_OAK                                 INFRAID("OAK ")
#define INFRAID_BFD                                 INFRAID("BFD ")
#define INFRAID_RIV                                 INFRAID("RIV ")
#define INFRAID_BBN                                 INFRAID("BBN ")
#define INFRAID_WBH                                 INFRAID("WBH ")
#define INFRAID_EAG                                 INFRAID("EAG ")
#define INFRAID_FGS                                 INFRAID("FGS ")
#define INFRAID_CTM                                 INFRAID("CTM ")
#define INFRAID_EDI                                 INFRAID("EDI ")
#define INFRAID_MMR                                 INFRAID("MMR ")
#define INFRAID_SNP                                 INFRAID("SNP ")
#define INFRAID_NKT                                 INFRAID("NKT ")
#define INFRAID_LAL                                 INFRAID("LAL ")
#define INFRAID_CSA                                 INFRAID("CSA ")
#define INFRAID_BEL                                 INFRAID("BEL ")
#define INFRAID_DRT                                 INFRAID("DRT ")
#define INFRAID_ALP                                 INFRAID("ALP ")
#define INFRAID_TOR                                 INFRAID("TOR ")
#define INFRAID_OKP                                 INFRAID("OKP ")
#define INFRAID_GIR                                 INFRAID("GIR ")
#define INFRAID_BWK                                 INFRAID("BWK ")
#define INFRAID_HDB                                 INFRAID("HDB ")
#define INFRAID_SAB                                 INFRAID("SAB ")
#define INFRAID_FSY                                 INFRAID("FSY ")
#define INFRAID_YVE                                 INFRAID("YVE ")
#define INFRAID_SOU                                 INFRAID("SOU ")
#define INFRAID_PVL                                 INFRAID("PVL ")
#define INFRAID_LAB                                 INFRAID("LAB ")
#define INFRAID_BON                                 INFRAID("BON ")
#define INFRAID_CAR                                 INFRAID("CAR ")
#define INFRAID_MFY                                 INFRAID("MFY ")
#define INFRAID_NCE                                 INFRAID("NCE ")
#define INFRAID_AUB                                 INFRAID("AUB ")
#define INFRAID_ALT                                 INFRAID("ALT ")
#define INFRAID_SEN                                 INFRAID("SEN ")
#define INFRAID_KAN                                 INFRAID("KAN ")
#define INFRAID_GRN                                 INFRAID("GRN ")
#define INFRAID_BAT                                 INFRAID("BAT ")
#define INFRAID_VPK                                 INFRAID("VPK ") // Victoria Park
#define INFRAID_CXT                                 INFRAID("CXT ")
#define INFRAID_CBY                                 INFRAID("CBY ")
#define INFRAID_JLI                                 INFRAID("JLI ")
#define INFRAID_TNT                                 INFRAID("TNT ")
#define INFRAID_SUY                                 INFRAID("SUY ")
#define INFRAID_MPD                                 INFRAID("MPD ")
#define INFRAID_KPK                                 INFRAID("KPK ")
#define INFRAID_BWD                                 INFRAID("BWD ")
#define INFRAID_SGS                                 INFRAID("SGS ")
#define INFRAID_DCK                                 INFRAID("DCK ")
#define INFRAID_LAV                                 INFRAID("LAV ")
#define INFRAID_HWS                                 INFRAID("HWS ")
#define INFRAID_MAL                                 INFRAID("MAL ")
#define INFRAID_MER                                 INFRAID("MER ")
#define INFRAID_MRB                                 INFRAID("MRB ")
#define INFRAID_DNG                                 INFRAID("DNG ")
#define INFRAID_CNE                                 INFRAID("CNE ")
#define INFRAID_GWY                                 INFRAID("GWY ")
#define INFRAID_EPP                                 INFRAID("EPP ")
#define INFRAID_DBN                                 INFRAID("DBN ")
#define INFRAID_PAT                                 INFRAID("PAT ")
#define INFRAID_IVA                                 INFRAID("IVA ")
#define INFRAID_WER                                 INFRAID("WER ")
#define INFRAID_HCG                                 INFRAID("HCG ")
#define INFRAID_BMS                                 INFRAID("BMS ")
#define INFRAID_GOW                                 INFRAID("GOW ")
#define INFRAID_GVE                                 INFRAID("GVE ")
#define INFRAID_CHM                                 INFRAID("CHM ")
#define INFRAID_WSN                                 INFRAID("WSN ")
#define INFRAID_RPK                                 INFRAID("RPK ")
#define INFRAID_ROS                                 INFRAID("ROS ")
#define INFRAID_UFD                                 INFRAID("UFD ")
#define INFRAID_HBE                                 INFRAID("HBE ")
#define INFRAID_BXR                                 INFRAID("BXR ")
#define INFRAID_HTD                                 INFRAID("HTD ")
#define INFRAID_TBY                                 INFRAID("TBY ")
#define INFRAID_MWY                                 INFRAID("MWY ")
#define INFRAID_HMT                                 INFRAID("HMT ")
#define INFRAID_ALM                                 INFRAID("ALM ")
#define INFRAID_BEW                                 INFRAID("BEW ")
#define INFRAID_ASY                                 INFRAID("ASY ")
#define INFRAID_FAK                                 INFRAID("FAK ")
#define INFRAID_PRE                                 INFRAID("PRE ")
#define INFRAID_HAW                                 INFRAID("HAW ")
#define INFRAID_ECM                                 INFRAID("ECM ")
#define INFRAID_RES                                 INFRAID("RES ")
#define INFRAID_WTL                                 INFRAID("WTL ")
#define INFRAID_SKN                                 INFRAID("SKN ")
#define INFRAID_RUT                                 INFRAID("RUT ")
#define INFRAID_WIN                                 INFRAID("WIN ")
#define INFRAID_HOL                                 INFRAID("HOL ")
#define INFRAID_FSS                                 INFRAID("FSS ")
#define INFRAID_BBH                                 INFRAID("BBH ")
#define INFRAID_RWD                                 INFRAID("RWD ")
#define INFRAID_BOX                                 INFRAID("BOX ")
#define INFRAID_MPK                                 INFRAID("MPK ")
#define INFRAID_CHL                                 INFRAID("CHL ")
#define INFRAID_RXP                                 INFRAID("RXP ")
#define INFRAID_JAC                                 INFRAID("JAC ")
#define INFRAID_LBK                                 INFRAID("LBK ")
#define INFRAID_UFG                                 INFRAID("UFG ")
#define INFRAID_GRY                                 INFRAID("GRY ")
#define INFRAID_RBK                                 INFRAID("RBK ")
#define INFRAID_TLN                                 INFRAID("TLN ")
#define INFRAID_ARM                                 INFRAID("ARM ")
#define INFRAID_HWL                                 INFRAID("HWL ")
#define INFRAID_MCE                                 INFRAID("MCE ")
#define INFRAID_NME                                 INFRAID("NME ")
#define INFRAID_HAM                                 INFRAID("HAM ")
#define INFRAID_GFE                                 INFRAID("GFE ")
#define INFRAID_FFD                                 INFRAID("FFD ")
#define INFRAID_RUS                                 INFRAID("RUS ")
#define INFRAID_NRM                                 INFRAID("NRM ")
#define INFRAID_MEL                                 INFRAID("MEL ")
#define INFRAID_MAC                                 INFRAID("MAC ")
#define INFRAID_BCV                                 INFRAID("BCV ")
#define INFRAID_ASV                                 INFRAID("ASV ")
#define INFRAID_GBV                                 INFRAID("GBV ")
#define INFRAID_WLD                                 INFRAID("WLD ")
#define INFRAID_ASP                                 INFRAID("ASP ")
#define INFRAID_OMD                                 INFRAID("OMD ")
#define INFRAID_SHE                                 INFRAID("SHE ")
#define INFRAID_MMY                                 INFRAID("MMY ")
#define INFRAID_NPK                                 INFRAID("NPK ")
#define INFRAID_SUN                                 INFRAID("SUN ")
#define INFRAID_YMN                                 INFRAID("YMN ")
#define INFRAID_UPW                                 INFRAID("UPW ")
#define INFRAID_MRO                                 INFRAID("MRO ")
#define INFRAID_ALB                                 INFRAID("ALB ")
#define INFRAID_BLY                                 INFRAID("BLY ")
#define INFRAID_FKN                                 INFRAID("FKN ")
#define INFRAID_PAR                                 INFRAID("PAR ")
#define INFRAID_DEK                                 INFRAID("DEK ")
#define INFRAID_PKM                                 INFRAID("PKM ")
#define INFRAID_CFD                                 INFRAID("CFD ")
#define INFRAID_RMD                                 INFRAID("RMD ")
#define INFRAID_ACF                                 INFRAID("ACF ")
#define INFRAID_JOR                                 INFRAID("JOR ")
#define INFRAID_GIN                                 INFRAID("GIN ")
#define INFRAID_HUG                                 INFRAID("HUG ")
#define INFRAID_SME                                 INFRAID("SME ")
#define INFRAID_MOR                                 INFRAID("MOR ")
#define INFRAID_DAV                                 INFRAID("DAV ") // West Tarneit station (under construction?)
#define INFRAID_CCL                                 INFRAID("CCL ")
#define INFRAID_ART                                 INFRAID("ART ")
#define INFRAID_BDE                                 INFRAID("BDE ")
#define INFRAID_MBY                                 INFRAID("MBY ") // Maryborough line/Malmsbury station
#define INFRAID_MYB                                 INFRAID("MYB ") // Maryborough station
#define INFRAID_TRN                                 INFRAID("TRN ")
#define INFRAID_GEL                                 INFRAID("GEL ")
#define INFRAID_WBL                                 INFRAID("WBL ")
#define INFRAID_BET                                 INFRAID("BET ")
#define INFRAID_WED                                 INFRAID("WED ")
#define INFRAID_BATV                                INFRAID("BATV")
#define INFRAID_BLN                                 INFRAID("BLN ")
#define INFRAID_BAH                                 INFRAID("BAH ")
#define INFRAID_TAT                                 INFRAID("TAT ")
#define INFRAID_CLU                                 INFRAID("CLU ")
#define INFRAID_CWK                                 INFRAID("CWK ")
#define INFRAID_SDP                                 INFRAID("SDP ")
#define INFRAID_TER                                 INFRAID("TER ")
#define INFRAID_CPD                                 INFRAID("CPD ")
#define INFRAID_COL                                 INFRAID("COL ")
#define INFRAID_BGE                                 INFRAID("BGE ")
#define INFRAID_WIA                                 INFRAID("WIA ")
#define INFRAID_WPP                                 INFRAID("WPP ")
#define INFRAID_MAS                                 INFRAID("MAS ")
#define INFRAID_SOG                                 INFRAID("SOG ")
#define INFRAID_NGL                                 INFRAID("NGL ")
#define INFRAID_NSH                                 INFRAID("NSH ")
#define INFRAID_COR                                 INFRAID("COR ")
#define INFRAID_LRA                                 INFRAID("LRA ")
#define INFRAID_LRR                                 INFRAID("LRR ")
#define INFRAID_STD                                 INFRAID("STD ")
#define INFRAID_SAE                                 INFRAID("SAE ")
#define INFRAID_ROE                                 INFRAID("ROE ")
#define INFRAID_MWL                                 INFRAID("MWL ")
#define INFRAID_MOE                                 INFRAID("MOE ")
#define INFRAID_TAR                                 INFRAID("TAR ")
#define INFRAID_YON                                 INFRAID("YON ")
#define INFRAID_WGL                                 INFRAID("WGL ")
#define INFRAID_DRN                                 INFRAID("DRN ")
#define INFRAID_LWY                                 INFRAID("LWY ")
#define INFRAID_BYP                                 INFRAID("BYP ")
#define INFRAID_GARV                                INFRAID("GARV")
#define INFRAID_TYN                                 INFRAID("TYN ")
#define INFRAID_NNG                                 INFRAID("NNG ")
#define INFRAID_SWL                                 INFRAID("SWL ")
#define INFRAID_KER                                 INFRAID("KER ")
#define INFRAID_PYD                                 INFRAID("PYD ")
#define INFRAID_DGE                                 INFRAID("DGE ")
#define INFRAID_RAY                                 INFRAID("RAY ")
#define INFRAID_EAGV                                INFRAID("EAGV")
#define INFRAID_BGO                                 INFRAID("BGO ")
#define INFRAID_KFT                                 INFRAID("KFT ")
#define INFRAID_CME                                 INFRAID("CME ")
#define INFRAID_MMYV                                INFRAID("MMYV")
#define INFRAID_KYN                                 INFRAID("KYN ")
#define INFRAID_WNO                                 INFRAID("WNO ")
#define INFRAID_MDN                                 INFRAID("MDN ")
#define INFRAID_GIS                                 INFRAID("GIS ")
#define INFRAID_RIK                                 INFRAID("RIK ")
#define INFRAID_CKF                                 INFRAID("CKF ")
#define INFRAID_ECH                                 INFRAID("ECH ")
#define INFRAID_ROR                                 INFRAID("ROR ")
#define INFRAID_EME                                 INFRAID("EME ")
#define INFRAID_GRG                                 INFRAID("GRG ")
#define INFRAID_HUY                                 INFRAID("HUY ")
#define INFRAID_EPM                                 INFRAID("EPM ")
#define INFRAID_SNH                                 INFRAID("SNH ")
#define INFRAID_MPA                                 INFRAID("MPA ")
#define INFRAID_MST                                 INFRAID("MST ")
#define INFRAID_NGE                                 INFRAID("NGE ")
#define INFRAID_SER                                 INFRAID("SER ")
#define INFRAID_TOK                                 INFRAID("TOK ")
#define INFRAID_BRF                                 INFRAID("BRF ")
#define INFRAID_KET                                 INFRAID("KET ")
#define INFRAID_WDG                                 INFRAID("WDG ")
#define INFRAID_HCJ                                 INFRAID("HCJ ")
#define INFRAID_WAN                                 INFRAID("WAN ")
#define INFRAID_DBK                                 INFRAID("DBK ")
#define INFRAID_ABY                                 INFRAID("ABY ")
#define INFRAID_WOD                                 INFRAID("WOD ")
#define INFRAID_CHI                                 INFRAID("CHI ")
#define INFRAID_SPTV                                INFRAID("SPTV")
#define INFRAID_WRT                                 INFRAID("WRT ")
#define INFRAID_BXA                                 INFRAID("BXA ")
#define INFRAID_VTN                                 INFRAID("VTN ")
#define INFRAID_EOA                                 INFRAID("EOA ")
#define INFRAID_AVL                                 INFRAID("AVL ")

/* NEW: Metro Tunnel stations */
#define INFRAID_ARN                                 INFRAID("ARN ") // Arden
#define INFRAID_PKV                                 INFRAID("PKV ") // Parkville
#define INFRAID_STL                                 INFRAID("STL ") // State Library
#define INFRAID_THL                                 INFRAID("THL ") // Town Hall
#define INFRAID_AZC                                 INFRAID("AZC ") // Anzac

/* lines via Metro Tunnel */
// originating from Metro Tunnel (i.e. Down direction)
#define INFRAID_SUYM                                INFRAID("SUYM")
#define INFRAID_CBEM                                INFRAID("CBEM")
#define INFRAID_PKMM                                INFRAID("PKMM")
// heading into Metro Tunnel (i.e. Up direction)
#define INFRAID_SUYm                                INFRAID("SUYm")
#define INFRAID_CBEm                                INFRAID("CBEm")
#define INFRAID_PKMm                                INFRAID("PKMm")

/* info structure for a station on a line */
typedef struct {
    uint16_t led; // LED index for this station

    /* LED index between this station and the next (up) station */
    uint16_t nextLED; // 0xFFFF if this is index 0 (first station in the list - up end)
} station_t;

class LSID {
public:
    static uint16_t getLED(infraid_t line, infraid_t code);
    static size_t getLEDsBetween(infraid_t line, infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    static colour_t getLineColour(infraid_t line);

    static inline bool isValidLine(infraid_t line) {
        return line == INFRAID_SHM || line == INFRAID_MDD || line == INFRAID_HBE || line == INFRAID_CCL ||
            line == INFRAID_PKM || line == INFRAID_CBE || line == INFRAID_BEG || line == INFRAID_LIL ||
            line == INFRAID_GWY || line == INFRAID_ALM || line == INFRAID_STY || line == INFRAID_FKN ||
            line == INFRAID_WIL || line == INFRAID_WER || line == INFRAID_CGB || line == INFRAID_SUY ||
            line == INFRAID_RCE || line == INFRAID_UFD || line == INFRAID_ART || line == INFRAID_BAT ||
            line == INFRAID_MBY || line == INFRAID_BDE || line == INFRAID_TRN || line == INFRAID_GEL || 
#if LMAT_VLINE_HAS_BENDIGO
            line == INFRAID_BGO || line == INFRAID_SWL || line == INFRAID_ECH ||
#endif
#if LMAT_VLINE_HAS_SEYMOUR
            line == INFRAID_SER || line == INFRAID_SNH || line == INFRAID_ABY ||
#endif
#ifndef CONFIG_MUNNEL_COLOUR_NONE
            line == INFRAID_SUYM || line == INFRAID_CBEM || line == INFRAID_PKMM ||
#endif
#ifdef CONFIG_MUNNEL_COLOUR_DEST
            line == INFRAID_SUYm || line == INFRAID_CBEm || line == INFRAID_PKMm ||
#endif
            line == INFRAID_WBL;
    }
private:
    static const char* kTag;

    /* helpers */
    static bool stationExists(const infraid_t* codes, size_t count, infraid_t code);
    static uint16_t getLEDStub(const station_t** stations, const infraid_t* codes, size_t count, infraid_t code);
    static size_t getLEDsBetweenCodes(const station_t** stations, const infraid_t* codes, size_t count, infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    static size_t getLEDsBetweenIndices(const station_t** stations, const infraid_t* codes, size_t count, int fromIndex, int toIndex, uint16_t* buffer, size_t maxLength);
    static inline bool isCityStation(infraid_t code) {
        return (code == INFRAID_FSS || code == INFRAID_SSS || code == INFRAID_FGS || code == INFRAID_MCE || code == INFRAID_PAR);
    } // excluding Metro Tunnel
    static inline bool isCityLoopStation(infraid_t code) {
        return (code == INFRAID_FGS || code == INFRAID_MCE || code == INFRAID_PAR);
    }
    static inline bool isMunnelStation(infraid_t code) {
        return (code == INFRAID_ARN || code == INFRAID_PKV || code == INFRAID_STL || code == INFRAID_THL || code == INFRAID_AZC);
    }

    /* Sandringham line */
    static uint16_t shmGetLED(infraid_t code);
    static size_t shmGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    
    /* Ararat/Ballarat/Melton line */
    static uint16_t artGetLED(infraid_t code);
    static size_t artGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Maryborough line */
#if LMAT_VLINE_HAS_REGIONAL
    static uint16_t mbyGetLED(infraid_t code);
    static size_t mbyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
#else // since we're effectively only using the Melton part
#define mbyGetLED artGetLED
#define mbyGetLEDsBetween artGetLEDsBetween 
#endif

    /* Warrnambool/Geelong/Wyndham Vale line */
    static uint16_t gelGetLED(infraid_t code);
    static size_t gelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Gippsland/Pakenham (V/Line) line */
    static uint16_t gplGetLED(infraid_t code);
    static size_t gplGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* city stations entering/exiting Jolimont (Clifton Hill) */
    static size_t chlGetLEDsBetween(
        const station_t** stations, const infraid_t* codes, size_t count,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    );

    /* Mernda line */
    static uint16_t mddGetLED(infraid_t code);
    static size_t mddGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Hurstbridge line */
    static uint16_t hbeGetLED(infraid_t code);
    static size_t hbeGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* City Circle line */
    static uint16_t cclGetLED(infraid_t code);
    static size_t cclGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* city stations entering/exiting Richmond */
    static size_t rmdCityGetLEDsBetween(
        const infraid_t* ccwCodes, const station_t** ccwStations, const station_t** cwStations,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    ); // assumes that ccwCodes, ccwStations and cwStations have a size of 5 (ccwStations in the order of PAR, MCE, FGS, SSS, FSS)
    // counterclockwise services enter PAR and exit from FSS, while clockwise services enter FSS and exit from PAR
    static size_t rmdGetLEDsBetween(
        const infraid_t* cityCCWCodes, const station_t** cityCCWStations, const station_t** cityCWStations,
        const station_t** stations, const infraid_t* codes, size_t count,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    ); // asserts that the last element of stations is RMD

    /* Dandenong lines */
    static uint16_t dngCityGetLED(infraid_t code);
    static size_t dngGetLEDsBetween(
        const station_t** stations, const infraid_t* codes, size_t count,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    ); // asserts that the last element of stations is RMD

    /* Pakenham line */
    static uint16_t pkmGetLED(infraid_t code);
    static size_t pkmGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Cranbourne line */
    static uint16_t cbeGetLED(infraid_t code);
    static size_t cbeGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* hack for Pakenham/Cranbourne via Metro Tunnel - not needed for Rev2 */
#ifndef LMAT_DANDENONG_HAS_MAL
    static size_t cbeMunnelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    static size_t pkmMunnelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
#else // rename by macro so we don't end up cluttering lsid.cpp too much
#define cbeMunnelGetLEDsBetween cbeGetLEDsBetween
#define pkmMunnelGetLEDsBetween pkmGetLEDsBetween
#endif

    /* Burnley city stations */
    static uint16_t blyCityGetLED(infraid_t code);

    /* Belgrave line */
    static uint16_t begGetLED(infraid_t code);
    static size_t begGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Lilydale line */
    static uint16_t lilGetLED(infraid_t code);
    static size_t lilGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Glen Waverley line */
    static uint16_t gwyGetLED(infraid_t code);
    static size_t gwyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Alamein line */
    static uint16_t almGetLED(infraid_t code);
    static size_t almGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Stony Point line */
    static uint16_t styGetLED(infraid_t code);
    static size_t styGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Frankston line */
    static uint16_t fknGetLED(infraid_t code);
    static size_t fknGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Williamstown line */
    static uint16_t wilGetLED(infraid_t code);
    static size_t wilGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Werribee line */
    static uint16_t werGetLED(infraid_t code);
    static size_t werGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Northern city stations */
    static uint16_t nmeCityGetLED(infraid_t code);
    static size_t nmeCityGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    // counterclockwise services enter SSS and exit from FGS, while clockwise services enter FGS and exit from SSS
    static size_t nmeGetLEDsBetween(
        const station_t** stations, const infraid_t* codes, size_t count,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    );

    /* Craigieburn line */
    static uint16_t cgbGetLED(infraid_t code);
    static size_t cgbGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Sunbury line */
    static uint16_t suyGetLED(infraid_t code);
    static size_t suyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Flemington Racecourse line */
    static uint16_t rceGetLED(infraid_t code);
    static size_t rceGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Upfield line */
    static uint16_t ufdGetLED(infraid_t code);
    static size_t ufdGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

    /* Metro Tunnel */
    static uint16_t mtGetLED(infraid_t code);
    static size_t mtGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);

#if LMAT_VLINE_HAS_BENDIGO
    static uint16_t swlGetLED(infraid_t code);
    static size_t swlGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
    static size_t bgoGetLEDsBetweenStub(
        const station_t** stations, const infraid_t* codes, size_t count,
        infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
    );
#if LMAT_VLINE_HAS_REGIONAL
    static uint16_t echGetLED(infraid_t code);
    static size_t echGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
#else
#define echGetLED swlGetLED
#define echGetLEDsBetween swlGetLEDsBetween
#endif
#endif

#if LMAT_VLINE_HAS_SEYMOUR
    static uint16_t snhGetLED(infraid_t code);
    static size_t snhGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
#if LMAT_VLINE_HAS_REGIONAL
    static uint16_t abyGetLED(infraid_t code);
    static size_t abyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength);
#else
#define abyGetLED snhGetLED
#define abyGetLEDsBetween snhGetLEDsBetween
#endif
#endif
};

