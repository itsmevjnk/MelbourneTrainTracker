/* pin definitions */

#pragma once

#include <driver/gpio.h>
#include <driver/spi_master.h>

/* SPI buses */
#define SPI2_MISO                       GPIO_NUM_12
#define SPI2_MOSI                       GPIO_NUM_13
#define SPI2_SCK                        GPIO_NUM_14
#define SPI3_MISO                       GPIO_NUM_19
#define SPI3_MOSI                       GPIO_NUM_21
#define SPI3_SCK                        GPIO_NUM_18

/* LED drivers */
#define L0_SPI                          SPI2_HOST
#define L0_CS                           GPIO_NUM_4
#define L1_SPI                          SPI2_HOST
#define L1_CS                           GPIO_NUM_16
#define L2_SPI                          SPI2_HOST
#define L2_CS                           GPIO_NUM_17
#define L3_SPI                          SPI2_HOST
#define L3_CS                           GPIO_NUM_22
#define L4_SPI                          SPI3_HOST
#define L4_CS                           GPIO_NUM_23
#define L5_SPI                          SPI3_HOST
#define L5_CS                           GPIO_NUM_25
#define L6_SPI                          SPI3_HOST
#define L6_CS                           GPIO_NUM_26
#define L7_SPI                          SPI3_HOST
#define L7_CS                           GPIO_NUM_27

#define DRV_EN                          GPIO_NUM_32

/* status LEDs */
#define LED_ACTY                        GPIO_NUM_2
#define LED_ERROR                       GPIO_NUM_33