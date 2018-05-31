#include <stm32f0xx.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_conf.h>
#include <stm32f0xx_wwdg.h>
#include <string.h>

//#include "gfx.h"
#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"

#include <hw_config.h>
#include "stm32f0xx_it.h"

//#include "lib_TagType4.h"
//#include "lib_ISO7816_Password.h"

#include "aura_logo_small.h"

extern volatile uint32_t MSec;

/*=======================================================================*/
/* external functions */
uint8_t u8x8_byte_stm32f0_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay_ozseccon(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


//ugfx
//systemticks_t gfxSystemTicks(void)
//{
//    return MSec;
//}
// 
//systemticks_t gfxMillisecondsToTicks(delaytime_t ms)
//{
//    return ms;
//}

//extern volatile uint32_t MSec;

void WWDG_IRQHandler(void) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_11, (n&1) ? Bit_SET : Bit_RESET);

}
void USART2_IRQHandler(void) {
}
void USART1_IRQHandler(void) {
}
void TS_IRQHandler(void) {
}

//void delay_ms(uint32_t Time);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t SELStatus = 0;

//static __IO uint16_t                    counter_delay_ms;

uint8_t I2C_Cmd_Flag = 0;
uint8_t NFCT4_SS_Flag = 0;
uint8_t NFCT4_CC_Flag = 0;
uint8_t NFCT4_NDEF_Flag = 0;
uint8_t ISO7816_Flag = 0;
uint8_t ST_Flag = 0;


/**
  * @brief  Initializes the M24SR for demonstration application.
  * @param  None
  * @retval None
  */
//static void M24SR_DemoInit ( void )
//{
//    uint8_t ResetI2CPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
//    ISO7816_DisableAllPassword(ResetI2CPassword);
//}


int main(void)
{
  //setup the graphic struct
  u8g2_t u8g2;

   
  //setup systick handler tick every 1ms
  SysTick_Config(SystemCoreClock/1000);
 
  //DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;

  //__HAL_DBGMCU_FREEZE_IWDG()
  //__HAL_RCC_DBGMCU_CLK_ENABLE();
 
  GPIO_InitTypeDef  GPIO_InitStructure;
  int n = 0;

  // Initialize GPIO clock  
  // see stm32f0xx_rcc.h
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);


  Timer_Config(); 
  Interrupts_Config(); 
  //M24SR_I2CInit();

  //gfxInit();
  //font_t font = gdispOpenFont("DejaVuSans12");

  //gdisp_lld_display();
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);   // WWDG clock enable
  //WWDG_SetWindowValue(0xff)
  
  // Initialize LED pins
  // see stm32f0xx_gpio.h

  GPIO_StructInit(&GPIO_InitStructure);

  //RCC_APB1ENR ^= 0x400
  // Pin PC9

  //tooth 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //tooth 2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //tooth 3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  //tooth 4 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //tooth 5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Left Eye
  //green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //red
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //blue 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Right Eye
  //green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //red
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //blue 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);

  //init the display  
  u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_stm32f0_hw_spi, u8x8_gpio_and_delay_ozseccon);
  //u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_lpc11u3x);  // init u8g2 structure
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0); // wake up display
  //u8g2_SetPowerSave(&u8g2, 1); // wake up display

  //u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 128, 64);
  //u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
  //u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);

  while(1){
    for(uint8_t i=0;i<(128-20);i++){
      u8g2_ClearBuffer(&u8g2);
      u8g2_DrawBox(&u8g2, i,0,20,64);
      u8g2_SendBuffer(&u8g2);
    }
  } 
  //u8g2_ClearBuffer(&u8g2);
  // Here, you can call other code to actually draw things, such as
  // existing application code to generate a screenshot
  //u8g2_DrawStr(&u8g2, 10, 30, "Hello, world!");
  //u8g2_SendBuffer(&u8g2);
  //u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "small.tga");

  // This changes size, but also resets other stuff like font
  //u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 128, 128);
  //u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  //u8g2_ClearBuffer(&u8g2);
  //// Here, you can call other code to actually draw things, such as
  //// existing application code to generate a screenshot
  //u8g2_DrawStr(&u8g2, 10, 30, "Hello, world!");
  //u8g2_DrawStr(&u8g2, 10, 100, "Hello, larger world!");
  //u8g2_SendBuffer(&u8g2);
  //u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "large.tga");
 
  //initialize the M24 NFC
  //TT4_Init();
  //M24SR_DemoInit();
  //M24SR_RFConfig(1);

  //RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC
						//(RM0091 lists this as IOPCEN, not GPIOCEN)
  //GPIOC->MODER = (1 << 16);
	
  //SysTick_Config(SystemCoreClock/100);

  // Pin PC8

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  //GPIO_Init(GPIOC, &GPIO_InitStructure);
  //const char *OzSecCon = "OzSecCon\0";
  //gdispImage auraLogo;
  //coord_t   swidth, sheight;
  //
  //swidth = gdispGetWidth();
  //sheight = gdispGetHeight();
 
  //gdispImageOpenMemory(&auraLogo, &aura_logo);
  //gdispImageDraw(&auraLogo, 0, 0, swidth, sheight, 0,0);
  
  while(1)
  {
    n++;
    delay_ms(1000);
    //gdispDrawLine(1, 1, 20, 20, GFX_WHITE);
    //gdispDrawString(1, 20, OzSecCon, font, GFX_RED);

    //flash teeth
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (n&1) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, (n&2) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, (n&3) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, (n&4) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, (n&5) ? Bit_SET : Bit_RESET);
   
    //flash left eye 
    GPIO_WriteBit(GPIOA, GPIO_Pin_8, (n&6) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_9, (n&7) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_10, (n&8) ? Bit_SET : Bit_RESET);
 
    //flash right eye 
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, (n&6) ? Bit_SET : Bit_RESET); //blue
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (n&7) ? Bit_SET : Bit_RESET); //RED
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (n&8) ? Bit_SET : Bit_RESET); //green
  
    //GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);// : Bit_RESET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET); //: Bit_RESET);
    //GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET); //: Bit_RESET);
  }
}



//Standard delay function as described in one 
//of my previous tutorials! 
//All it does is operate a nop instruction 
//until "Time" amount of milliseconds has passed. 
//void delay_ms(uint32_t Time){ 
//    volatile uint32_t MSStart = MSec; 
//    while((MSec-MSStart)<Time) __asm__ volatile("nop"); 
//}

/*
 * Debug help -- if asserts are enabled, assertion failure
 * from standard peripheral  library ends up here 
 */


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1)
  {
  }
}
#endif

