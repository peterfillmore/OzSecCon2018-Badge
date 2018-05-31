/**
  ******************************************************************************
  * @file    LED_config.c 
  * @author  MMY Application Team
  * @version V0.0.1
  * @date    06/12/2012
  * @brief   LED config 
	******************************************************************************
  * @copyright
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "LED_config.h"
#include "stm32f0xx_rcc.h"


/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */

/** @addtogroup HW_Config
 * 	@{
 * @brief      This file defines a set of command to initialize the MCU
 */
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions Prototype -----------------------------------------------*/

/** @addtogroup HW_Config_Private_Functions
 * 	@{
 */
uint32_t get_timer_clock_frequency (void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  uint32_t multiplier;
  if (RCC_Clocks.PCLK_Frequency == RCC_Clocks.SYSCLK_Frequency) {
    multiplier = 1;
  } else {
    multiplier = 2;
  }
  return multiplier * RCC_Clocks.PCLK_Frequency;
}
 
/**
 *	@brief Structure configuration for the Timer2 in ms
 *  @param  None
 *  @retval None
 */
void timer_clock_init(void)
{
    uint32_t TIMER_Frequency = get_timer_clock_frequency();
    //uint32_t COUNTER_Frequency = PWM_Steps * PWM_Frequency;
    uint32_t COUNTER_Frequency = 100 * 100;
    uint32_t PSC_Value = (TIMER_Frequency / COUNTER_Frequency) - 1;
    //uint16_t ARR_Value = PWM_Steps - 1; 
    uint16_t ARR_Value = 100 - 1; 
     /* make sure the peripheral is clocked */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* set everything back to default values */
    TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
    /* only changes from the defaults are needed */
    TIM_TimeBaseStructure.TIM_Period = ARR_Value;
    TIM_TimeBaseStructure.TIM_Prescaler = PSC_Value;
    TIM_TimeBaseInit(TIMER_LED_EYE_RIGHT, &TIM_TimeBaseStructure);

    TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ARR_Value;
    TIM_TimeBaseStructure.TIM_Prescaler = PSC_Value;
    TIM_TimeBaseInit(TIMER_LED_EYE_LEFT, &TIM_TimeBaseStructure);
}

void timer_pwm_init (void)
{
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* always initialise local variables before use */
  TIM_OCStructInit (&TIM_OCInitStructure);
 
  /* Common settings for all channels */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;
 
  /* Channel2 - ORANGE LED*/
  TIM_OC1Init (TIMER_LED_EYE_RIGHT, &TIM_OCInitStructure);
 
  /* Channel3 - RED LED*/
  TIM_OC2Init (TIMER_LED_EYE_RIGHT, &TIM_OCInitStructure);
 
  /* Channel4 - BLUE LED*/
  /* make this the opposite polarity to the other two */
  //TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC3Init (TIMER_LED_EYE_RIGHT, &TIM_OCInitStructure);

  TIM_OCStructInit (&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC2Init (TIMER_LED_EYE_LEFT, &TIM_OCInitStructure);
  TIM_OC3Init (TIMER_LED_EYE_LEFT, &TIM_OCInitStructure);
}

void led_init(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  /* always initialise local variables before use */
  GPIO_StructInit (&GPIO_InitStructure);
 
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOB, ENABLE);
 
  /* these pins will be controlled by the CCRx registers */
  GPIO_InitStructure.GPIO_Pin = LED_EYE_RIGHT_BLUE + LED_EYE_RIGHT_RED + LED_EYE_RIGHT_GREEN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init (LED_EYE_RIGHT_PORT, &GPIO_InitStructure);
 
  /* ensure that the pins all start off in a known state */
  //GPIO_ResetBits (LED_EYE_RIGHT_PORT, LED_EYE_RIGHT_BLUE + LED_EYE_RIGHT_RED + LED_EYE_RIGHT_GREEN);
  GPIO_SetBits (LED_EYE_RIGHT_PORT, LED_EYE_RIGHT_BLUE + LED_EYE_RIGHT_RED + LED_EYE_RIGHT_GREEN);
 
  /* The others get connected to the AF function for the timer */
  GPIO_PinAFConfig (LED_EYE_RIGHT_PORT, LED_EYE_RIGHT_BLUE_Pin_Source, GPIO_AF_2);
  GPIO_PinAFConfig (LED_EYE_RIGHT_PORT, LED_EYE_RIGHT_GREEN_Pin_Source, GPIO_AF_2);
  GPIO_PinAFConfig (LED_EYE_RIGHT_PORT, LED_EYE_RIGHT_RED_Pin_Source, GPIO_AF_2);

  //configure blue left LED
  GPIO_InitStructure.GPIO_Pin = LED_EYE_LEFT_BLUE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
  //GPIO_Init (LED_EYE_RIGHT_PORT, &GPIO_InitStructure); //GPIO A

  GPIO_InitStructure.GPIO_Pin = LED_EYE_LEFT_RED + LED_EYE_LEFT_GREEN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init (LED_EYE_LEFT_PORT, &GPIO_InitStructure);
 
  GPIO_SetBits (LED_EYE_LEFT_PORT, LED_EYE_LEFT_BLUE + LED_EYE_LEFT_RED + LED_EYE_LEFT_GREEN);
  //GPIO_PinAFConfig (LED_EYE_LEFT_PORT, LED_EYE_LEFT_BLUE_Pin_Source, GPIO_AF_2);
  GPIO_PinAFConfig(LED_EYE_LEFT_PORT, LED_EYE_LEFT_GREEN_Pin_Source, GPIO_AF_2);
  GPIO_PinAFConfig(LED_EYE_LEFT_PORT, LED_EYE_LEFT_RED_Pin_Source, GPIO_AF_2);

}

/**
 *	@brief Structure configuration for the Timer2 in us
 *  @param  None
 *  @retval None
 */

/** @addtogroup HW_Config_Public_Functions
 * 	@{
 */

/**
 *	@brief  This function configures the Main system clocks & power
 *  @param  None
 *  @retval None
 */

/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */



/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
