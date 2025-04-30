/* pin definitions */

#pragma once

#include <driver/gpio.h>
#include <driver/spi_master.h>

/* SPI buses */
#define HSPI_MISO                       GPIO_NUM_12
#define HSPI_MOSI                       GPIO_NUM_13
#define HSPI_SCK                        GPIO_NUM_14
#define VSPI_MISO                       GPIO_NUM_19
#define VSPI_MOSI                       GPIO_NUM_21
#define VSPI_SCK                        GPIO_NUM_18

/* LED drivers */
#define L0_SPI                          HSPI
#define L0_CS                           GPIO_NUM_4
#define L1_SPI                          HSPI
#define L1_CS                           GPIO_NUM_16
#define L2_SPI                          HSPI
#define L2_CS                           GPIO_NUM_17
#define L3_SPI                          HSPI
#define L3_CS                           GPIO_NUM_22
#define L4_SPI                          VSPI
#define L4_CS                           GPIO_NUM_23
#define L5_SPI                          VSPI
#define L5_CS                           GPIO_NUM_25
#define L6_SPI                          VSPI
#define L6_CS                           GPIO_NUM_26
#define L7_SPI                          VSPI
#define L7_CS                           GPIO_NUM_27

#define DRV_EN                          GPIO_NUM_32

/* status LEDs */
#define LED_ACTY                        GPIO_NUM_2
#define LED_ERROR                       GPIO_NUM_33