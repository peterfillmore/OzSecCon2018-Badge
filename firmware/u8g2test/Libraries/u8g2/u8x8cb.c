/*
  u8x8cb.c
  
  OzSecConBadge 
 
  
    #define SPI1_SCLK   GPIO_Pin_5      //PA5
    #define SPI1_MOSI   GPIO_Pin_7      //PA7
    #define SPI1_RESET  GPIO_Pin_2      //PA2 reset active high
    #define SPI1_DC     GPIO_Pin_1      //PA1 data/command
    #define SPI1_CS     GPIO_Pin_3      //PA3 CHIP SELECT active low
    
    #define SPI1_NSS    GPIO_Pin_4      //PA4 
    #define SPI1_MISO   GPIO_Pin_6      //PA6
    
    #define SPI1_PORT   GPIOA

     
*/

#define SPI1_SCLK   GPIO_Pin_5      //PA5
#define SPI1_MOSI   GPIO_Pin_7      //PA7
#define SPI1_RESET  GPIO_Pin_2      //PA2 reset active high
#define SPI1_DC     GPIO_Pin_1      //PA1 data/command
#define SPI1_CS     GPIO_Pin_3      //PA3 CHIP SELECT active low

#define SPI1_NSS    GPIO_Pin_4      //PA4 
#define SPI1_MISO   GPIO_Pin_6      //PA6

#define SPI1_PORT   GPIOA

#include <stm32f0xx.h>
#include "stm32f0xx_gpio.h"
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

//#include "stm32l031xx.h"
//#include "delay.h"
#include "u8x8.h"
#include <hw_config.h>

uint8_t u8x8_gpio_and_delay_ozseccon(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitTypeDefStruct;

  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      //setup the SPI pins for the display 
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
        SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
        SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
        SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
        SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;

        SPI_Init(SPI1, &SPI_InitTypeDefStruct);

        SPI_Cmd(SPI1, ENABLE);
        //GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET);

        //reset low
        //GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET); //RESET DISPLAY
        //GPIO_WriteBit(SPI1_PORT, SPI1_NSS, Bit_RESET); //?
        //GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_RESET); //SELECT DISPLAY
        //GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET); //RESET DISPLAY
        //backlight pin
        //GPIO_WriteBit(SPI1_PORT, SPI1_BACKLIGHT, Bit_RESET);
 
      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;
    
    case U8X8_MSG_DELAY_10MICRO:
      delay_us(arg_int*10);   
      break;
    
    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;
   
    case U8X8_MSG_DELAY_MILLI:
      delay_ms(arg_int);   
    //delay_micro_seconds(arg_int*1000UL);
      break;
    //case U8X8_MSG_DELAY_I2C:
    //  /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
    //  delay_micro_seconds(arg_int<=2?5:1);
    //  break;
   
    case U8X8_MSG_GPIO_CS:             // CS (chip select) pin: Output level in arg_int
        if(arg_int ==0){
            GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_RESET);
        } else {
            GPIO_WriteBit(SPI1_PORT, SPI1_CS, Bit_SET);
        }
        break;
    case U8X8_MSG_GPIO_DC:              // DC (data/cmd, A0, register select) pin: Output level in arg_int
        GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET);
        if(arg_int ==0){
            GPIO_WriteBit(SPI1_PORT, SPI1_DC, Bit_RESET);
        } else {
 
            GPIO_WriteBit(SPI1_PORT, SPI1_DC, Bit_SET);
        } 
      break;
    case U8X8_MSG_GPIO_RESET:              // DC (data/cmd, A0, register select) pin: Output level in arg_int
        if(arg_int ==0){
            GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_SET);
        } else {
 
            GPIO_WriteBit(SPI1_PORT, SPI1_RESET, Bit_RESET);
        } 
      break;
 
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

uint8_t u8x8_byte_stm32f0_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *data;
  //uint8_t internal_spi_mode; 
  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      //u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level); //acquire bus
      //u8x8_gpio_SetReset(u8x8, u8x8->display_info->chip_enable_level); //acquire bus
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 ) {
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
        SPI_SendData8(SPI1, (uint8_t) *data); 
        data++;
        arg_int--;
      }  
      break;
    case U8X8_MSG_BYTE_INIT:
      u8x8_gpio_SetReset(u8x8, u8x8->display_info->chip_enable_level); //acquire bus
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level); //acquire bus
      while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); //wait till we can transfer?
      break;
    case U8X8_MSG_BYTE_SET_DC:
      //u8x8_gpio_SetReset(u8x8, u8x8->display_info->chip_enable_level); //acquire bus
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level); //acquire bus
      //u8x8_gpio_SetReset(u8x8, u8x8->display_info->chip_enable_level); //acquire bus
      //u8x8_gpio_SetDC(u8x8, u8x8->display_info->chip_disable_level);  
      //u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);  
      //while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      //while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); //wait till we can transfer?
      //SPI.beginTransaction(SPISettings(u8x8->display_info->sck_clock_hz, MSBFIRST, internal_spi_mode));
      //u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
      //u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:      
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level); //acquire bus
      //u8x8_gpio_SetReset(u8x8, u8x8->display_info->chip_enable_level); //acquire bus
      //u8x8_gpio_SetDC(u8x8, u8x8->display_info->chip_enable_level);  
      while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
      //u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      //u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      //SPI.endTransaction();
      break;
    default:
      return 0;
  }  
  return 1;
}
