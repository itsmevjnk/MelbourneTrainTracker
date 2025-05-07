/* registers for the AW20216S LED matrix driver */

#pragma once

/* page 0 */
#define AW20216S_REG_GCR                            0x000
#define AW20216S_REG_GCCR                           0x001
#define AW20216S_REG_DGCR                           0x002
#define AW20216S_REG_OSR(x)                         (0x003 + (x)) // x: 0 -> 35
#define AW20216S_REG_OTCR                           0x027
#define AW20216S_REG_SSCR                           0x028
#define AW20216S_REG_PCCR                           0x029
#define AW20216S_REG_UVCR                           0x02A
#define AW20216S_REG_SRCR                           0x02B
#define AW20216S_REG_RSTN                           0x02F
#define AW20216S_REG_PWMH(x)                        (0x030 + (x)) // x: 0 -> 2
#define AW20216S_REG_PWML(x)                        (0x032 + (x)) // x: 0 -> 2
#define AW20216S_REG_PAT0T(x)                       (0x036 + (x)) // x: 0 -> 3
#define AW20216S_REG_PAT1T(x)                       (0x03A + (x)) // x: 0 -> 3
#define AW20216S_REG_PAT2T(x)                       (0x03D + (x)) // x: 0 -> 3
#define AW20216S_REG_PAT0CFG                        0x042
#define AW20216S_REG_PAT1CFG                        0x043
#define AW20216S_REG_PAT2CFG                        0x044
#define AW20216S_REG_PATGO                          0x045
#define AW20216S_REG_MIXCR                          0x046
#define AW20216S_REG_SDCR                           0x04D

/* page 1 */
#define AW20216S_LED(row, col)                      ((col) * 18 + (row))
#define AW20216S_REG_PWM(x)                         (0x100 + (x)) // x: 0 -> 215

/* page 2 */
#define AW20216S_REG_SL(x)                          (0x200 + (x)) // x: 0 -> 215

/* page 3 */
#define AW20216S_REG_PATG(x)                        (0x300 + (x)) // x: 0 -> 71

/* page 4 */
#define AW20216S_REG_PWM_SL(x)                      (0x400 + (x)) // x: 0 -> 215