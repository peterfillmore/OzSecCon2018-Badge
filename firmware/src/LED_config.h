/**
	****************** (C) COPYRIGHT 2017 OzSecCon **********************
	* @file          LED_config.h
	* @author        MMY Application Team
	* @version       V0.0.1
	* @date          22/04/2018
	* @brief         Setup of LEDs
	********************************************************************************
	* @attention
	*********************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_CONFIG_H
#define __LED_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stm32f0xx_tim.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/**
 * @brief LED Pin Definitions
 */
#define LED_TOOTH_0 GPIO_Pin_11
#define LED_TOOTH_1 GPIO_Pin_12
#define LED_TOOTH_2 GPIO_Pin_13
#define LED_TOOTH_3 GPIO_Pin_14
#define LED_TOOTH_4 GPIO_Pin_15

#define LED_EYE_RIGHT_PORT      GPIOA
#define LED_EYE_RIGHT_BLUE      GPIO_Pin_8  //PA8   TIM1_CH1 29
#define LED_EYE_RIGHT_BLUE_Pin_Source      GPIO_PinSource8  //PA8   TIM1_CH1 29
#define LED_EYE_RIGHT_RED       GPIO_Pin_9  //PA9   TIM1_CH2 30
#define LED_EYE_RIGHT_RED_Pin_Source      GPIO_PinSource9  //PA9   TIM1_CH2 30
#define LED_EYE_RIGHT_GREEN     GPIO_Pin_10 //PA10  TIM1_CH3 31
#define LED_EYE_RIGHT_GREEN_Pin_Source     GPIO_PinSource10 //PA10  TIM1_CH3 31

#define LED_EYE_LEFT_PORT               GPIOB
#define LED_EYE_LEFT_BLUE               GPIO_Pin_15 //PA15  TIM2_CH1_ETR 38
#define LED_EYE_LEFT_BLUE_Pin_Source    GPIO_PinSource15 //PA15  TIM2_CH1_ETR 38
#define LED_EYE_LEFT_RED                GPIO_Pin_3  //PB3   TIM2_CH2 39
#define LED_EYE_LEFT_RED_Pin_Source     GPIO_PinSource3  //PB3   TIM2_CH2 39
#define LED_EYE_LEFT_GREEN              GPIO_Pin_10 //PB10  TIM2_CH3 21
#define LED_EYE_LEFT_GREEN_Pin_Source   GPIO_PinSource10 //PB10  TIM2_CH3 21

/**
 * @brief  TIMER definitions
 */

/* --------------------------------------------------------------------------
* PWM RIGHT configuration
* --------------------------------------------------------------------------
* 48 MHz / 48 = 1MHz (1us)
* 1us * 1000 + 1us ~= 1ms
* -------------------------------------------------------------------------- */
#define TIMER_LED_EYE_RIGHT										TIM1
#define TIMER_LED_EYE_LEFT										TIM2
#define TIMER_LED_EYE_FREQ									    48000000
#define TIMER_DELAY_PRESCALER									(TIMER_LED_EVE_FREQ/10000)-1
#define TIMER_DELAY_CLOCK										RCC_APB1Periph_TIM2

/* --------------------------------------------------------------------------
* Delay TIMER configuration (us)
* --------------------------------------------------------------------------
* 48 MHz / 1 = 48MHz (0.0138us)
* 0.020833us * 48 ~= 1us
* -------------------------------------------------------------------------- */
#define TIMER_US_DELAY											TIM2
#define TIMER_US_DELAY_PERIOD									48
#define TIMER_US_DELAY_PRESCALER							    1
#define TIMER_US_DELAY_CLOCK									RCC_APB1Periph_TIM2

/* --------------------------------------------------------------------------
* TIMESTAMP configuration (us)
* --------------------------------------------------------------------------
* 72 MHz / 72 = 1MHz (1us)
* 1us * 1 + 1us ~= 2us
* -------------------------------------------------------------------------- */
#define TIMER_TIMESTAMP												TIM3
/* in us need to increase value by 2 in interruption */
//#define TIMER_TIMESTAMP_PERIOD								1
//#define TIMER_TIMESTAMP_PRESCALER							72
#define TIMER_TIMESTAMP_PERIOD								1000
#define TIMER_TIMESTAMP_PRESCALER							72
#define TIMER_TIMESTAMP_CLOCK									RCC_APB1Periph_TIM3

/**
 * @brief  NVIC definitions
 */
#define NVIC_PRIORITY_GROUP										NVIC_PriorityGroup_2

#define TIMER_DELAY_PREEMPTION_PRIORITY				2
#define TIMER_DELAY_SUB_PRIORITY					2
#define TIMER_DELAY_IRQ_CHANNEL						TIM2_IRQn

#define TIMER_DELAY_PREEMPTION_HIGHPRIORITY	    	0
#define TIMER_DELAY_SUB_HIGHPRIORITY				0

#define TIMER_TIMESTAMP_PREEMPTION_PRIORITY			2
#define TIMER_TIMESTAMP_SUB_PRIORITY				2
#define TIMER_TIMESTAMP_IRQ_CHANNEL					TIM3_IRQn

#define TIMER_TIMESTAMP_PREEMPTION_HIGHPRIORITY		0
#define TIMER_TIMESTAMP_SUB_HIGHPRIORITY			0


/* Exported functions ------------------------------------------------------- */
void timer_clock_init (void);
void led_init(void);
void timer_pwm_init(void);

/* Exported functions ------------------------------------------------------- */

#endif  /*__LED_CONFIG_H*/

/************************END OF FILE****/
