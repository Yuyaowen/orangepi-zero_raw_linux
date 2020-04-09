// SPDX-License-Identifier: GPL-2.0
/*
 * Synaptics AS370 pinctrl driver
 *
 * Copyright (C) 2018 Synaptics Incorporated
 *
 * Author: Jisheng Zhang <jszhang@kernel.org>
 */

#include <linux/init.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include "berlin.h"

static const struct berlin_desc_group as370_soc_pinctrl_groups[] = {
	BERLIN_PINCTRL_GROUP("I2S1_BCLKIO", 0x0, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO0 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* BCLKIO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG0 */
	BERLIN_PINCTRL_GROUP("I2S1_LRCKIO", 0x0, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO1 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* LRCKIO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG1 */
	BERLIN_PINCTRL_GROUP("I2S1_DO0", 0x0, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "por"), /* 1P8V RSTB*/
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO0 */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio"), /* GPIO2 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG2 */
	BERLIN_PINCTRL_GROUP("I2S1_DO1", 0x0, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "por"), /* 3P3V RSTB */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO1 */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio"), /* GPIO3 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG3 */
	BERLIN_PINCTRL_GROUP("I2S1_DO2", 0x0, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "por"), /* CORE RSTB */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO2 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM4 */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio"), /* GPIO4 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG4 */
	BERLIN_PINCTRL_GROUP("I2S1_DO3", 0x0, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO5 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* DO3 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM5 */
			BERLIN_PINCTRL_FUNCTION(0x3, "spdifib"), /* SPDIFIB */
			BERLIN_PINCTRL_FUNCTION(0x4, "spdifo"), /* SPDIFO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG5 */
	BERLIN_PINCTRL_GROUP("I2S1_MCLK", 0x0, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO6 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s1"), /* MCLK */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG6 */
	BERLIN_PINCTRL_GROUP("I2S2_BCLKIO", 0x0, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO7 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* BCLKIO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG7 */
	BERLIN_PINCTRL_GROUP("I2S2_LRCKIO", 0x0, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO8 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* LRCKIO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG8 */
	BERLIN_PINCTRL_GROUP("I2S2_DI0", 0x0, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO9 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI0 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM2 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG9 */
	BERLIN_PINCTRL_GROUP("I2S2_DI1", 0x4, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO10 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI1 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM3 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG10 */
	BERLIN_PINCTRL_GROUP("I2S2_DI2", 0x4, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO11 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI2 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM6 */
			BERLIN_PINCTRL_FUNCTION(0x3, "spdific"), /* SPDIFIC */
			BERLIN_PINCTRL_FUNCTION(0x4, "spdifo"), /* SPDIFO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG11 */
	BERLIN_PINCTRL_GROUP("I2S2_DI3", 0x4, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO12 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s2"), /* DI3 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM7 */
			BERLIN_PINCTRL_FUNCTION(0x3, "spdifia"), /* SPDIFIA */
			BERLIN_PINCTRL_FUNCTION(0x4, "spdifo"), /* SPDIFO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG12 */
	BERLIN_PINCTRL_GROUP("PDM_CLKO", 0x4, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO13 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pdm"), /* CLKO */
			BERLIN_PINCTRL_FUNCTION(0x2, "i2s2"), /* MCLK */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG13 */
	BERLIN_PINCTRL_GROUP("PDM_DI0", 0x4, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO14 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pdm"), /* DI0 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG14 */
	BERLIN_PINCTRL_GROUP("PDM_DI1", 0x4, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO15 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pdm"), /* DI1 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG15 */
	BERLIN_PINCTRL_GROUP("PDM_DI2", 0x4, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO16 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pdm"), /* DI2 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM4 */
			BERLIN_PINCTRL_FUNCTION(0x3, "spdifid"), /* SPDIFID */
			BERLIN_PINCTRL_FUNCTION(0x4, "spdifo"), /* SPDIFO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG16 */
	BERLIN_PINCTRL_GROUP("PDM_DI3", 0x4, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO17 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pdm"), /* DI3 */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM5 */
			BERLIN_PINCTRL_FUNCTION(0x3, "spdifi"), /* SPDIFI */
			BERLIN_PINCTRL_FUNCTION(0x4, "spdifo"), /* SPDIFO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG17 */
	BERLIN_PINCTRL_GROUP("NAND_IO0", 0x4, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO0 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* DATA0 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pcie0")), /* MDIO */
	BERLIN_PINCTRL_GROUP("NAND_IO1", 0x4, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO1 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* DATA1 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pcie0")), /* MDC */
	BERLIN_PINCTRL_GROUP("NAND_IO2", 0x8, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO2 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* DATA2 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pcie1")), /* MDIO */
	BERLIN_PINCTRL_GROUP("NAND_IO3", 0x8, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO3 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* DATA3 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pcie1")), /* MDC */
	BERLIN_PINCTRL_GROUP("NAND_IO4", 0x8, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO4 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc")), /* DATA4 */
	BERLIN_PINCTRL_GROUP("NAND_IO5", 0x8, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO5 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc")), /* DATA5 */
	BERLIN_PINCTRL_GROUP("NAND_IO6", 0x8, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO6 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc")), /* DATA6 */
	BERLIN_PINCTRL_GROUP("NAND_IO7", 0x8, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* IO7 */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc")), /* DATA7 */
	BERLIN_PINCTRL_GROUP("NAND_ALE", 0x8, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* ALE */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM6 */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO18 */
	BERLIN_PINCTRL_GROUP("NAND_CLE", 0x8, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* CLE */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM7 */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO19 */
	BERLIN_PINCTRL_GROUP("NAND_WEn", 0x8, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* WEn */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO20 */
	BERLIN_PINCTRL_GROUP("NAND_REn", 0x8, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* REn */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO21 */
	BERLIN_PINCTRL_GROUP("NAND_WPn", 0xc, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* WPn */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* CLK */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO22 */
	BERLIN_PINCTRL_GROUP("NAND_CEn", 0xc, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* CEn */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* RSTn */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO23 */
	BERLIN_PINCTRL_GROUP("NAND_RDY", 0xc, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "nand"), /* RDY */
			BERLIN_PINCTRL_FUNCTION(0x1, "emmc"), /* CMD */
			BERLIN_PINCTRL_FUNCTION(0x3, "gpio")), /* GPIO24 */
	BERLIN_PINCTRL_GROUP("SPI1_SS0n", 0xc, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "spi1"), /* SS0n */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO25 */
	BERLIN_PINCTRL_GROUP("SPI1_SS1n", 0xc, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "spi1"), /* SS1n */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio"), /* GPIO26 */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM2 */
	BERLIN_PINCTRL_GROUP("SPI1_SS2n", 0xc, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "uart0"), /* RXD */
			BERLIN_PINCTRL_FUNCTION(0x1, "spi1"), /* SS2n */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio"), /* GPIO27 */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM3 */
	BERLIN_PINCTRL_GROUP("SPI1_SS3n", 0xc, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "uart0"), /* TXD */
			BERLIN_PINCTRL_FUNCTION(0x1, "spi1"), /* SS3n */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO28 */
	BERLIN_PINCTRL_GROUP("SPI1_SCLK", 0xc, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "spi1"), /* SCLK */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO29 */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM4 */
	BERLIN_PINCTRL_GROUP("SPI1_SDO", 0xc, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "spi1"), /* SDO */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO30 */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM5 */
	BERLIN_PINCTRL_GROUP("SPI1_SDI", 0xc, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "spi1"), /* SDI */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio")), /* GPIO31 */
	BERLIN_PINCTRL_GROUP("USB0_DRV_VBUS", 0x10, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "usb0"), /* VBUS */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO32 */
			BERLIN_PINCTRL_FUNCTION(0x3, "refclko")), /* 25M */
	BERLIN_PINCTRL_GROUP("TW1_SCL", 0x10, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO33 */
			BERLIN_PINCTRL_FUNCTION(0x1, "tw1")), /* SCL */
	BERLIN_PINCTRL_GROUP("TW1_SDA", 0x10, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO34 */
			BERLIN_PINCTRL_FUNCTION(0x1, "tw1")), /* SDA */
	BERLIN_PINCTRL_GROUP("TW0_SCL", 0x10, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO35 */
			BERLIN_PINCTRL_FUNCTION(0x1, "tw0")), /* SCL */
	BERLIN_PINCTRL_GROUP("TW0_SDA", 0x10, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO36 */
			BERLIN_PINCTRL_FUNCTION(0x1, "tw0")), /* SDA */
	BERLIN_PINCTRL_GROUP("TMS", 0x10, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "jtag"), /* TMS */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO37 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pwm")), /* PWM0 */
	BERLIN_PINCTRL_GROUP("TDI", 0x10, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "jtag"), /* TDI */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO38 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pwm")), /* PWM1 */
	BERLIN_PINCTRL_GROUP("TDO", 0x10, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "jtag"), /* TDO */
			BERLIN_PINCTRL_FUNCTION(0x1, "gpio"), /* GPIO39 */
			BERLIN_PINCTRL_FUNCTION(0x4, "pwm")), /* PWM0 */
	BERLIN_PINCTRL_GROUP("PWM6", 0x10, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO40 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM6 */
	BERLIN_PINCTRL_GROUP("PWM7", 0x10, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO41 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM7 */
	BERLIN_PINCTRL_GROUP("PWM0", 0x14, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "por"), /* VDDCPUSOC RSTB */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm"), /* PWM0 */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO42 */
	BERLIN_PINCTRL_GROUP("PWM1", 0x14, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO43 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM1 */
	BERLIN_PINCTRL_GROUP("PWM2", 0x14, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO44 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM2 */
	BERLIN_PINCTRL_GROUP("PWM3", 0x14, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO45 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM3 */
	BERLIN_PINCTRL_GROUP("PWM4", 0x14, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO46 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM4 */
	BERLIN_PINCTRL_GROUP("PWM5", 0x14, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO47 */
			BERLIN_PINCTRL_FUNCTION(0x1, "pwm")), /* PWM5 */
	BERLIN_PINCTRL_GROUP("URT1_RTSn", 0x14, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO48 */
			BERLIN_PINCTRL_FUNCTION(0x1, "uart1"), /* RTSn */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM6 */
			BERLIN_PINCTRL_FUNCTION(0x3, "tw1a"), /* SCL */
			BERLIN_PINCTRL_FUNCTION(0x4, "aio"), /* DBG0 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG18 */
	BERLIN_PINCTRL_GROUP("URT1_CTSn", 0x14, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO49 */
			BERLIN_PINCTRL_FUNCTION(0x1, "uart1"), /* CTSn */
			BERLIN_PINCTRL_FUNCTION(0x2, "pwm"), /* PWM7 */
			BERLIN_PINCTRL_FUNCTION(0x3, "tw1a"), /* SDA */
			BERLIN_PINCTRL_FUNCTION(0x4, "aio"), /* DBG1 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG19 */
	BERLIN_PINCTRL_GROUP("URT1_RXD", 0x14, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO50 */
			BERLIN_PINCTRL_FUNCTION(0x1, "uart1"), /* RXD */
			BERLIN_PINCTRL_FUNCTION(0x4, "aio"), /* DBG2 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG20 */
	BERLIN_PINCTRL_GROUP("URT1_TXD", 0x14, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO51 */
			BERLIN_PINCTRL_FUNCTION(0x1, "uart1"), /* TXD */
			BERLIN_PINCTRL_FUNCTION(0x4, "aio"), /* DBG3 */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG21 */
	BERLIN_PINCTRL_GROUP("I2S3_DI", 0x18, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO52 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s3"), /* DI */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG22 */
	BERLIN_PINCTRL_GROUP("I2S3_DO", 0x18, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO53 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s3"), /* DO */
			BERLIN_PINCTRL_FUNCTION(0x5, "phy")), /* DBG23 */
	BERLIN_PINCTRL_GROUP("I2S3_BCLKIO", 0x18, 0x3, 0x06,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO54 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s3"), /* BCLKIO */
			BERLIN_PINCTRL_FUNCTION(0x5, "clk")), /* DBG */
	BERLIN_PINCTRL_GROUP("I2S3_LRCKIO", 0x18, 0x3, 0x09,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO55 */
			BERLIN_PINCTRL_FUNCTION(0x1, "i2s3")), /* LRCKIO */
	BERLIN_PINCTRL_GROUP("SD0_DAT0", 0x18, 0x3, 0x0c,
			BERLIN_PINCTRL_FUNCTION(0x0, "cpupll"), /* OUT */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* DAT0 */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO56 */
	BERLIN_PINCTRL_GROUP("SD0_DAT1", 0x18, 0x3, 0x0f,
			BERLIN_PINCTRL_FUNCTION(0x0, "syspll"), /* OUT */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* DAT1 */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO57 */
	BERLIN_PINCTRL_GROUP("SD0_CLK", 0x18, 0x3, 0x12,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO58 */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0")), /* CLK */
	BERLIN_PINCTRL_GROUP("SD0_DAT2", 0x18, 0x3, 0x15,
			BERLIN_PINCTRL_FUNCTION(0x0, "mempll"), /* OUT */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* DAT2 */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO59 */
	BERLIN_PINCTRL_GROUP("SD0_DAT3", 0x18, 0x3, 0x18,
			BERLIN_PINCTRL_FUNCTION(0x0, "apll0"), /* OUT */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* DAT3 */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO60 */
	BERLIN_PINCTRL_GROUP("SD0_CMD", 0x18, 0x3, 0x1b,
			BERLIN_PINCTRL_FUNCTION(0x0, "apll1"), /* OUT */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* CMD */
			BERLIN_PINCTRL_FUNCTION(0x2, "gpio")), /* GPIO61 */
	BERLIN_PINCTRL_GROUP("SD0_CDn", 0x1c, 0x3, 0x00,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO62 */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* CDn */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM2 */
	BERLIN_PINCTRL_GROUP("SD0_WP", 0x1c, 0x3, 0x03,
			BERLIN_PINCTRL_FUNCTION(0x0, "gpio"), /* GPIO63 */
			BERLIN_PINCTRL_FUNCTION(0x1, "sd0"), /* WP */
			BERLIN_PINCTRL_FUNCTION(0x3, "pwm")), /* PWM3 */
};

static const struct berlin_pinctrl_desc as370_soc_pinctrl_data = {
	.groups = as370_soc_pinctrl_groups,
	.ngroups = ARRAY_SIZE(as370_soc_pinctrl_groups),
};

static const struct of_device_id as370_pinctrl_match[] = {
	{
		.compatible = "syna,as370-soc-pinctrl",
		.data = &as370_soc_pinctrl_data,
	},
	{}
};

static int as370_pinctrl_probe(struct platform_device *pdev)
{
	const struct of_device_id *match =
		of_match_device(as370_pinctrl_match, &pdev->dev);
	struct regmap_config *rmconfig;
	struct regmap *regmap;
	struct resource *res;
	void __iomem *base;

	rmconfig = devm_kzalloc(&pdev->dev, sizeof(*rmconfig), GFP_KERNEL);
	if (!rmconfig)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base))
		return PTR_ERR(base);

	rmconfig->reg_bits = 32,
	rmconfig->val_bits = 32,
	rmconfig->reg_stride = 4,
	rmconfig->max_register = resource_size(res);

	regmap = devm_regmap_init_mmio(&pdev->dev, base, rmconfig);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	return berlin_pinctrl_probe_regmap(pdev, match->data, regmap);
}

static struct platform_driver as370_pinctrl_driver = {
	.probe	= as370_pinctrl_probe,
	.driver	= {
		.name = "as370-pinctrl",
		.of_match_table = as370_pinctrl_match,
	},
};
builtin_platform_driver(as370_pinctrl_driver);
