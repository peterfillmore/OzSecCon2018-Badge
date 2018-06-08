/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

//#define SSD1306_PAGE_PREFIX		0x40

#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

#define SPI1_SCLK   GPIO_Pin_5      //PA5
#define SPI1_MOSI   GPIO_Pin_7      //PA7
#define SPI1_RESET  GPIO_Pin_2      //PA2 reset active high
#define SPI1_DC     GPIO_Pin_1      //PA1 data/command
#define SPI1_CS     GPIO_Pin_3      //PA3 CHIP SELECT active low

#define SPI1_NSS    GPIO_Pin_4      //PA4 
#define SPI1_MISO   GPIO_Pin_6      //PA6

#define SPI1_PORT   GPIOA

#define SET_RST     GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET);
#define CLR_RST     GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET);




static GFXINLINE void init_board(GDisplay *g) {
	//(void) g;
    g->board = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitTypeDefStruct;


	switch(g->controllerdisplay) {
	case 0:											// Set up for Display 0
		/**
		 * Performs the following functions:
		 *		1. initialise the io port used by the display
		 *		2. initialise the reset pin (initial state not-in-reset)
		 *		3. initialise the chip select pin (initial state not-active)
		 *		4. initialise the backlight pin (initial state back-light off)
		 */
                /* GPIO Periph clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

        /* SPI1 Periph clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

        /* Configure I2C pins: SCL and SDA */
        GPIO_InitStructure.GPIO_Pin = SPI1_SCLK | SPI1_MISO | SPI1_MOSI ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
   
        GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_DC | SPI1_RESET | SPI1_CS;
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
        SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
 
        SPI_Init(SPI1, &SPI_InitTypeDefStruct);

        SPI_Cmd(SPI1, ENABLE);

        //reset low
        GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET); //RESET DISPLAY
        GPIO_WriteBit(SPI1_PORT, SPI1_NSS, Bit_RESET); //?
        GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_RESET); //SELECT DISPLAY
        GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET); //RESET DISPLAY
        //backlight pin
        //GPIO_WriteBit(SPI1_PORT, SPI1_BACKLIGHT, Bit_RESET);

		break;
	}}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
	//(void) state;
    if(state)
        SET_RST
    else
        CLR_RST
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
    GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_RESET);
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
    GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_SET);
}

static GFXINLINE void write_cmd(GDisplay *g, uint8_t cmd) {
	(void) g;
    GPIO_WriteBit(SPI1_PORT, SPI1_DC, Bit_RESET); //set command
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
    SPI_SendData8(SPI1, cmd); //send command
    GPIO_WriteBit(SPI1_PORT, SPI1_DC, Bit_SET);
}

static GFXINLINE void write_data(GDisplay *g, uint8_t* data, uint16_t length) {
	(void) g;
	//(void) data;
	//(void) length;
    int i=0;
    GPIO_WriteBit(SPI1_PORT, SPI1_DC, Bit_SET); //set data
    for(i=0; i<length; i++){
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
        SPI_SendData8(SPI1, data[i]);
    }
}

#endif /* _GDISP_LLD_BOARD_H */
