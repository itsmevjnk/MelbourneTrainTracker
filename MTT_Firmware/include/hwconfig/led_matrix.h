/* LED matrix configuration */

#pragma once

#define LMAT_SPI_CLOCK                      1000000UL // SPI clock speed for LED matrix drivers

/* number of rows per driver */
#define L0_ROWS                             (3 * 6)
#define L1_ROWS                             (3 * 6)
#define L2_ROWS                             (3 * 6)
#define L3_ROWS                             (3 * 6)
#define L4_ROWS                             (3 * 6)
#define L5_ROWS                             (3 * 6)
#define L6_ROWS                             (3 * 6)
#define L7_ROWS                             (3 * 4)
#define L_ROWS(x)                           L##x##_ROWS

/* offsets into buffer */
#define L0_OFFSET                           0
#define L1_OFFSET                           (L0_OFFSET + 12 * L0_ROWS)
#define L2_OFFSET                           (L1_OFFSET + 12 * L1_ROWS)
#define L3_OFFSET                           (L2_OFFSET + 12 * L2_ROWS)
#define L4_OFFSET                           (L3_OFFSET + 12 * L3_ROWS)
#define L5_OFFSET                           (L4_OFFSET + 12 * L4_ROWS)
#define L6_OFFSET                           (L5_OFFSET + 12 * L5_ROWS)
#define L7_OFFSET                           (L6_OFFSET + 12 * L6_ROWS)
#define L_OFFSET(x)                         L##x##_OFFSET

/* addressing macros */
#define LMAT_BASE(chip, col, row)           (L_OFFSET(chip) + L_ROWS(chip) * (col) + 3 * (row))
#define LMAT_R(base)                        ((base) + 0)
#define LMAT_G(base)                        ((base) + 1)
#define LMAT_B(base)                        ((base) + 2)

/* line LED offsets */
#include "hwconfig/leds/burnley.h"
#include "hwconfig/leds/clifton.h"
#include "hwconfig/leds/crosscity.h"
#include "hwconfig/leds/dandenong.h"
#include "hwconfig/leds/munnel.h"
#include "hwconfig/leds/northern.h"
#include "hwconfig/leds/sandringham.h"
#include "hwconfig/leds/vline.h"
