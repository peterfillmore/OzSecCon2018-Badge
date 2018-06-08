/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    boards/addons/gdisp/board_SSD1289_stm32f4discovery.h
 * @brief   GDISP Graphic Driver subsystem board interface for the SSD1289 display.
 *
 * @note	This file contains a mix of hardware specific and operating system specific
 *			code. You will need to change it for your CPU and/or operating system.
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

// For a multiple display configuration we would put all this in a structure and then
//	set g->board to that structure.
#define GDISP_REG              ((volatile uint16_t *) 0x60000000)[0] /* RS = 0 */
#define GDISP_RAM              ((volatile uint16_t *) 0x60020000)[0] /* RS = 1 */
#define GDISP_DMA_STREAM		STM32_DMA2_STREAM6
#define FSMC_BANK				0

#define SPI1_DC_A0  GPIO_Pin_1      //PA1 
#define SPI1_RESET  GPIO_Pin_2      //PA2 
#define SPI1_BACKLIGHT    GPIO_Pin_3      //PA3
#define SPI1_NSS    GPIO_Pin_4      //PA4 
#define SPI1_SCLK   GPIO_Pin_5      //PA5
#define SPI1_MISO   GPIO_Pin_6      //PA6
#define SPI1_MOSI   GPIO_Pin_7      //PA7

#define SET_RST     GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET);
#define CLR_RST     GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET);


#define SPI1_PORT   GPIOA

/* PWM configuration structure. We use timer 3 channel 3 */
//static const PWMConfig pwmcfg = {
//	100000,       /* 100 kHz PWM clock frequency. */
//	100,          /* PWM period is 100 cycles. */
//	0,
//	{
//		{PWM_OUTPUT_DISABLED, 0},
//		{PWM_OUTPUT_DISABLED, 0},
//		{PWM_OUTPUT_ACTIVE_HIGH, 0},
//		{PWM_OUTPUT_DISABLED, 0}
//	},
//	0,
//	0
//};

static GFXINLINE void init_board(GDisplay *g) {

	// As we are not using multiple displays we set g->board to NULL as we don't use it.
	g->board = 0;

	switch(g->controllerdisplay) {
	case 0:											// Set up for Display 0
		/**
		 * Performs the following functions:
		 *		1. initialise the io port used by the display
		 *		2. initialise the reset pin (initial state not-in-reset)
		 *		3. initialise the chip select pin (initial state not-active)
		 *		4. initialise the backlight pin (initial state back-light off)
		 */
        GPIO_InitTypeDef  GPIO_InitStructure;
        I2C_InitTypeDef   I2C_InitStructure;

        /* GPIO Periph clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

        /* SPI1 Periph clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1)

        /* Configure I2C pins: SCL and SDA */
        GPIO_InitStructure.GPIO_Pin = SPI1_SCLK | SPI1_MISO | SPI1_MOSI ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
   
        GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_DC_A0 | SPI1_RESET | SPI1_BACKLIGHT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
 
        /* Connect SPI pins to AF */
        GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource5, GPIO_AF_0);
        GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource6, GPIO_AF_0);
        GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource7, GPIO_AF_0);

        /*SPI init */        
        SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_1Line_Tx;
        SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
        SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
        SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
        SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
        SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
 
        SPI_Init(SPI1, &SPI_InitTypeDefStruct);

        SPI_Cmd(SPI1, ENABLE);

        //reset low
        GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET);
        //chip select pin
        GPIO_WriteBit(SPI1_PORT, SPI1_NSS, Bit_RESET);
        //backlight pin
        GPIO_WriteBit(SPI1_BACKLIGHT, SPI1_NSS, Bit_RESET);

		break;
	}
}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
    if(state)
        CLR_RST
    else
        SET_RST
}

static GFXINLINE void set_backlight(GDisplay *g, uint8_t percent) {
	(void) g;
    //pwmEnableChannel(&PWMD3, 2, percent);
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
}

static GFXINLINE void write_index(GDisplay *g, uint16_t index) {
	(void) g;
	//GDISP_REG = index;
}

static GFXINLINE void write_data(GDisplay *g, uint16_t data) {
	(void) g;
	//GDISP_RAM = data;
}

static GFXINLINE void setreadmode(GDisplay *g) {
	(void) g;
	//FSMC_Bank1->BTCR[FSMC_BANK+1] = FSMC_BTR1_ADDSET_3 | FSMC_BTR1_DATAST_3 | FSMC_BTR1_BUSTURN_0;		/* FSMC timing */
}

static GFXINLINE void setwritemode(GDisplay *g) {
	(void) g;
	//FSMC_Bank1->BTCR[FSMC_BANK+1] = FSMC_BTR1_ADDSET_0 | FSMC_BTR1_DATAST_2 | FSMC_BTR1_BUSTURN_0;		/* FSMC timing */
}

static GFXINLINE uint16_t read_data(GDisplay *g) {
	(void) g;
	//return GDISP_RAM;
}

#endif /* _GDISP_LLD_BOARD_H */

