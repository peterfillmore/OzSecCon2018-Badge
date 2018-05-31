/**
	****************** (C) COPYRIGHT 2012 STMicroelectronics **********************
	* @file          hw_config.h
	* @author        MMY Application Team
	* @version       V1.0.0
	* @date          12/20/2013
	* @brief         Hardware Configuration & Setup
	********************************************************************************
	* @attention
	*
	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
	* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
	* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
	* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
	* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED 
	* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
	*
	* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
	*********************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h" 
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/** 
 * @brief  USB disconnect pin B14
 */
#define USB_DISCONNECT                    		GPIOB  
#define USB_DISCONNECT_PIN                		GPIO_Pin_0
//#define USB_DISCONNECT_GPIO_CLOCK   			RCC_APBPeriph_GPIOB
	

/** 
 * @brief  TIMER definitions 
 */

/* -------------------------------------------------------------------------- 
* Delay TIMER configuration (ms)
* --------------------------------------------------------------------------
* 48 MHz / 48 = 1MHz (1us)
* 1us * 1000 + 1us ~= 1ms	
* -------------------------------------------------------------------------- */ 
#define TIMER_DELAY												TIM2
#define TIMER_DELAY_PERIOD										1000
#define TIMER_DELAY_PRESCALER									48
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
* Delay TIMER configuration (ns)
* --------------------------------------------------------------------------
* 48 MHz / 1 = 48MHz (0.0138us)
* 0.020833us * 48 ~= 1us	
* -------------------------------------------------------------------------- */ 
#define TIMER_NS_DELAY											TIM2
#define TIMER_NS_DELAY_PERIOD									48
#define TIMER_NS_DELAY_PRESCALER							    1
#define TIMER_NS_DELAY_CLOCK									RCC_APB1Periph_TIM2


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


/** 
 * @brief  IRQ functions names 
 */
#define USB_HP_IRQ_HANDLER							USB_HP_CAN1_TX_IRQHandler
#define USB_LP_IRQ_HANDLER							USB_LP_CAN1_RX0_IRQHandler
#define TIMER_DELAY_IRQ_HANDLER						TIM2_IRQHandler
#define TIMER_TIMESTAMP_IRQ_HANDLER			    	TIM3_IRQHandler


/* Exported functions ------------------------------------------------------- */
void Set_System							( void );
void Interrupts_Config					( void );
void USB_Cable_Config 					(FunctionalState NewState);
void USB_Disconnect_Config				( void );

void Timer_Config						( void );
void Timer_RCC_Config					( void );
void Timer_Structure_Config				( void );
void delay_ms							( uint16_t delay );
void delay_us							( uint16_t delay );
void delayHighPriority_ms	    		( uint16_t delay );
void decrement_delay					( void );
void StartPerfMeasurement(void);
uint32_t GetElapsed_us(void);
void StopPerfMeasurement(void);
void increment_timestamp(void);


/* Exported functions ------------------------------------------------------- */

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
