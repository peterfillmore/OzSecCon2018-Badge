#include <stm32f0xx.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_conf.h>
#include <stm32f0xx_wwdg.h>
#include <string.h>

#include "gfx.h"

#include <hw_config.h>
#include "stm32f0xx_it.h"

#include "lib_TagType4.h"
#include "lib_ISO7816_Password.h"
#include "LED_config.h"

//#include "aura_logo_circle.h"
#include "aura_logo_small.h"

extern volatile uint32_t MSec;

//ugfx
systemticks_t gfxSystemTicks(void)
{

    return MSec;
}

systemticks_t gfxMillisecondsToTicks(delaytime_t ms)
{
    return ms;
}

//extern volatile uint32_t MSec;

void WWDG_IRQHandler(void) {
    //GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
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
static void M24SR_DemoInit ( void )
{
    uint8_t ResetI2CPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    ISO7816_DisableAllPassword(ResetI2CPassword);
}

void flash_led_forever (void)
{
  int brightness = 0;
  int increment = 1;
  while (1) {
    TIM_SetCompare1 (TIM1, 85);
    TIM_SetCompare2 (TIM1, 85);
    TIM_SetCompare3 (TIM1, 120);
    TIM_SetCompare2 (TIM2, 3);
    //for (int i = 0; i < 5; i++) {
    //  brightness += increment;
    //  delay_ms (100);
    //  TIM_SetCompare1 (TIM1, brightness);
    //
    //  //LEFT EYE
    //  delay_ms (100);
    //  TIM_SetCompare2 (TIM2, brightness);
    //  delay_ms (100);
    //  TIM_SetCompare3 (TIM2, brightness);
    //}
    //TIM_SetCompare1 (TIM1, 0);
    //for (int i = 0; i < 5; i++) {
    //  brightness += increment;
    //  delay_ms (100);
    //  TIM_SetCompare2 (TIM1, brightness);
    //
    //  //LEFT EYE
    //  delay_ms (100);
    //  TIM_SetCompare2 (TIM2, brightness);
    //  delay_ms (100);
    //  TIM_SetCompare3 (TIM2, brightness);
    //}
    //TIM_SetCompare2 (TIM1, 0);
    //for (int i = 0; i < 5; i++) {
    //  brightness += increment;
    //  delay_ms (100);
    //  TIM_SetCompare3 (TIM1, brightness);
    //
    //  //LEFT EYE
    //  delay_ms (100);
    //  TIM_SetCompare2 (TIM2, brightness);
    //  delay_ms (100);
    //  TIM_SetCompare3 (TIM2, brightness);
    //}
    //TIM_SetCompare3 (TIM1, 0);

    //GPIO_WriteBit(GPIOB, GPIO_Pin_11, (increment&2) ? Bit_SET : Bit_RESET);
  }
}

int main(void)
{
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
  M24SR_I2CInit();

  gfxInit();
  font_t font = gdispOpenFont("DejaVuSans12");

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
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  //red
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  //blue
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Right Eye
  //green
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);
  //red
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);
  //blue
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);


  //GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
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
  const char *OzSecCon = "OzSecCon 2018\0";
  gdispImage auraLogo;
  coord_t   swidth, sheight;



  swidth = gdispGetWidth();
  sheight = gdispGetHeight();


  //gdisp_lld_display()
  led_init();
  timer_clock_init();
  timer_pwm_init();
  TIM_Cmd(TIM1, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);

  GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);


  //flash_led_forever();
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
    //GPIO_WriteBit(GPIOA, GPIO_Pin_8, (n&6) ? Bit_SET : Bit_RESET);
    //GPIO_WriteBit(GPIOA, GPIO_Pin_9, (n&7) ? Bit_SET : Bit_RESET);
    //GPIO_WriteBit(GPIOA, GPIO_Pin_10, (n&8) ? Bit_SET : Bit_RESET);

    //flash right eye
    //GPIO_WriteBit(GPIOA, GPIO_Pin_15, (n&6) ? Bit_SET : Bit_RESET); //blue
    //GPIO_WriteBit(GPIOB, GPIO_Pin_3, (n&7) ? Bit_SET : Bit_RESET); //RED
    //GPIO_WriteBit(GPIOB, GPIO_Pin_10, (n&8) ? Bit_SET : Bit_RESET); //green

    //GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);// : Bit_RESET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET); //: Bit_RESET);
    //GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET); //: Bit_RESET);

    int counter_1 = 75;
    int counter_2 = 65;
    int counter_3 = 55;
    int counter_4 = 60;
    int counter_5 = 70;
    int i=0;
    int j=0;
    TIM_SetCompare1 (TIM1, 85);
    TIM_SetCompare2 (TIM1, 85);
    TIM_SetCompare3 (TIM1, 120);
    TIM_SetCompare3 (TIM2, 90);
    TIM_SetCompare2 (TIM2, 100);

    for(j=0; j< 127; j++){
       TIM_SetCompare1 (TIM1, counter_1 ^ ((j&74)+i));
       TIM_SetCompare2 (TIM1, counter_2 | ((j&75)-i));
       TIM_SetCompare3 (TIM1, counter_3 % ((j&76)/i));
       TIM_SetCompare2 (TIM2, counter_4 ^ ((j&77)*i));
       TIM_SetCompare3 (TIM2, counter_5 & ((j& 78)+i));

       TIM_SetCompare1 (TIM1, 120);
       TIM_SetCompare2 (TIM1, 110);
       TIM_SetCompare3 (TIM1, 100);
       TIM_SetCompare2 (TIM2, 105);
       TIM_SetCompare3 (TIM2, 115);

       gdispClear(GFX_BLACK);
       gdispDrawString(i, sheight/2, OzSecCon, font, GFX_WHITE);
       delay_ms(5000);
       gdispClear(GFX_BLACK);
       //for(i=0;i<127; i++){
       // //TIM_SetCompare1 (TIM1, 120);
       // //TIM_SetCompare2 (TIM1, 110);
       // //TIM_SetCompare3 (TIM1, 100);
       // //TIM_SetCompare2 (TIM2, 105);
       // //TIM_SetCompare3 (TIM2, 115);
       // GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET); //blue
       // GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET); //RED
       // GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET); //green

       //       gdispDrawString(i, sheight/2, OzSecCon, font, GFX_WHITE);
       //       delay_ms(200);
       //       gdispClear(GFX_BLACK);
       // }
       TIM_SetCompare1 (TIM1, counter_1^((j&94)+i));
       TIM_SetCompare2 (TIM1, counter_2|((j&95)-i));
       TIM_SetCompare3 (TIM1, counter_3%((j&96)/i));
       TIM_SetCompare1 (TIM2, counter_4^((j&97)*i));
       TIM_SetCompare2 (TIM2, counter_5&((j&98)+i));
       gdispImageOpenMemory(&auraLogo, &aura_logo);
       gdispImageDraw(&auraLogo, 0, 0, swidth, sheight, 0,0);
       gdispImageClose(&auraLogo);
       delay_ms(10000);
    }
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

