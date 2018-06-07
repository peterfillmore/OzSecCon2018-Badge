/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MMY Application Team
  * @version V2.0.0
  * @date    06/12/2012
  * @brief   Hardware Configuration & Setup of the MCU
	******************************************************************************
  * @copyright
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
//#include "drv_led.h"


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

static __IO uint16_t					counter_delay_ms;
static __IO uint32_t					timestamp_us;

/* Private functions Prototype -----------------------------------------------*/
static void TimerDelay_us_Config		( void );
static void TimerDelay_ms_Config		( void );
static void TimerTimestamp_us_Config	( void );


/** @addtogroup HW_Config_Private_Functions
 * 	@{
 */

/**
 *	@brief Structure configuration for the Timer2 in ms
 *  @param  None
 *  @retval None
 */
static void TimerDelay_ms_Config( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* --------------------------------------------------------------------------
	 * Delay TIMER configuration
	 * --------------------------------------------------------------------------
	 * 48 MHz / 48  = 1MHz (1us)
	 * 1us * 1000 + 1us ~= 1ms
	 * -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_DELAY_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler			= TIMER_DELAY_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIMER_DELAY, TIM_UpdateSource_Global);

	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);

	/* Enable TIMER Update interrupt */
	TIM_ITConfig(TIMER_DELAY, TIM_IT_Update, ENABLE);

	/* Disable timer	*/
	TIM_Cmd(TIMER_DELAY, DISABLE);
}

/**
 *	@brief Structure configuration for the Timer2 in us
 *  @param  None
 *  @retval None
 */
static void TimerDelay_us_Config( void )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* --------------------------------------------------------------------------
	* Delay TIMER configuration (us)
	* --------------------------------------------------------------------------
	* 72 MHz / 1 = 72MHz (0.013us)
	* 1us * 71 + 1us ~= 1us
	* -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_US_DELAY_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_US_DELAY_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_US_DELAY, &TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIMER_US_DELAY, TIM_UpdateSource_Global);

	TIM_ClearITPendingBit(TIMER_US_DELAY, TIM_IT_Update);

	/* Enable TIMER Update interrupt */
	TIM_ITConfig(TIMER_US_DELAY, TIM_IT_Update, ENABLE);

	/* Disable timer	*/
	TIM_Cmd(TIMER_US_DELAY, DISABLE);

}

/**
 *	@brief Structure configuration for the Timer2 in us
 *  @param  None
 *  @retval None
 */
static void TimerTimestamp_us_Config( void )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* --------------------------------------------------------------------------
	* Delay TIMER configuration (us)
	* --------------------------------------------------------------------------
	* 72 MHz / 1 = 72MHz (0.013us)
	* 0.013us * 71 + 0.013us ~= 1us
	* -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_TIMESTAMP_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_TIMESTAMP_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_TIMESTAMP, &TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIMER_TIMESTAMP, TIM_UpdateSource_Global);

	TIM_ClearITPendingBit(TIMER_TIMESTAMP, TIM_IT_Update);

	/* Enable TIMER Update interrupt */
	TIM_ITConfig(TIMER_TIMESTAMP, TIM_IT_Update, ENABLE);

	/* Disable timer	*/
	TIM_Cmd(TIMER_TIMESTAMP, DISABLE);

}

/**
  * @}
  */


/** @addtogroup HW_Config_Public_Functions
 * 	@{
 */

/**
 *	@brief  This function configures the Main system clocks & power
 *  @param  None
 *  @retval None
 */
void Set_System(void)
{
  /*!  At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */

//#if defined(USB_USE_EXTERNAL_PULLUP)
//  GPIO_InitTypeDef  GPIO_InitStructure;
//#endif /* USB_USE_EXTERNAL_PULLUP */

  /* Enable and Disconnect Line GPIO clock */
  //USB_Disconnect_Config();

	/* the following code allow to be deconnected from USB data */
//#if defined(USB_USE_EXTERNAL_PULLUP)
//  /* Enable the USB disconnect GPIO clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);
//
//  /* USB_DISCONNECT used as USB pull-up */
//  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
//#endif /* USB_USE_EXTERNAL_PULLUP */

	/* PWR and BKP clocks selection ------------------------------------------*/
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
}

/**
 *	@brief  Configures the interrupts
 *  @param  None
 *  @retval None
 */
void Interrupts_Config (void)
{
   NVIC_EnableIRQ(TIM2_IRQn);
   //NVIC_InitTypeDef NVIC_InitStructure;

  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable and set TIMER IRQ used for delays */
	//NVIC_InitStructure.NVIC_IRQChannel 	 						= TIMER_DELAY_IRQ_CHANNEL;
	//NVIC_InitStructure.NVIC_IRQChannelPriority					= 1;

    //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= TIMER_DELAY_SUB_PRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

    /* Enable and set TIMER IRQ used for delays */
	//NVIC_InitStructure.NVIC_IRQChannel 					 		= TIMER_TIMESTAMP_IRQ_CHANNEL;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_TIMESTAMP_PREEMPTION_PRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= TIMER_TIMESTAMP_SUB_PRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	/* Enable UART4 IRQ */
    //NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority 	 			= 1;
    //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitStructure);

  //NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

  //NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

	/* Enable the EXTI9_5 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

}

/**
 *	@brief  This function Connects or disconnects the MCU from the USB
 *  @param  NewState : ENABLE or DISABLE value
 *  @retval None
 */
void USB_Cable_Config (FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

/**
 *	@brief  This function configurates the disconnects pin
 *  @param  None
 *  @retval None
 */
//void USB_Disconnect_Config(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//
//  /* Enable USB_DISCONNECT GPIO clock */
//  RCC_APB2PeriphClockCmd(USB_DISCONNECT_GPIO_CLOCK, ENABLE);
//
//  /* USB_DISCONNECT_PIN used as USB pull-up */
//  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
//}

/**
 *	@brief  This function configures the Timers
 *  @param  None
 *  @retval None
 */
void Timer_Config( void )
{
	Timer_RCC_Config( );
	Timer_Structure_Config( );
}

/**
 *	@brief  This function configures RCC for the Timers
 *  @param  None
 *  @retval None
 */
void Timer_RCC_Config( void )
{
	/*	enable TIM2 & TIM3 */
	//RCC_APB1PeriphClockCmd(		(TIMER_DELAY_CLOCK | TIMER_TIMESTAMP_CLOCK),
	RCC_APB1PeriphClockCmd(TIMER_DELAY_CLOCK,ENABLE);
}

/**
 *	@brief  Structure configuration for the Timers
 *  @param  None
 *  @retval None
 */
void Timer_Structure_Config( void )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* --------------------------------------------------------------------------
	 * Delay TIMER configuration
	 * --------------------------------------------------------------------------
	 * 72 MHz / 72 = 1MHz (1us)
	 * 1us * 1000 + 1us ~= 1ms
	 * -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_DELAY_PERIOD;      /* 1000 */
	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_DELAY_PRESCALER;    /* 72 */
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIMER_DELAY, TIM_UpdateSource_Global);

	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);

	/* Enable TIMER Update interrupt */
	TIM_ITConfig(TIMER_DELAY, TIM_IT_Update, ENABLE);



	/* --------------------------------------------------------------------------
	 * To measure system performance in us
	 * --------------------------------------------------------------------------
	 * 72 MHz / 4 = 18MHz (0.055us)
	 * 0.055us * 18 + 0.055us ~= 1us
	 * -------------------------------------------------------------------------- */
	//TIM_TimeBaseStructure.TIM_Period 			= TIMER_TIMESTAMP_PERIOD;
	//TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_TIMESTAMP_PRESCALER;
	//TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	//TIM_TimeBaseInit(TIMER_TIMESTAMP, &TIM_TimeBaseStructure);
	//
	//TIM_UpdateRequestConfig(TIMER_TIMESTAMP, TIM_UpdateSource_Global);
	//
	//TIM_ClearITPendingBit(TIMER_TIMESTAMP, TIM_IT_Update);
	//
	///* Enable TIMER Update interrupt */
	//TIM_ITConfig(TIMER_TIMESTAMP, TIM_IT_Update, ENABLE);

}

/**
 *	@brief  Time delay in millisecond
 *  @param  delay : delay in ms.
 *  @retval none
 */
void delay_ms(uint16_t delay)
{
	counter_delay_ms = delay;

	TimerDelay_ms_Config ();

	TIM_SetCounter(TIMER_DELAY, 0);
	/* TIM2 enable counter */
    TIM_Cmd(TIMER_DELAY, ENABLE);
	/* Wait for 'delay' milliseconds */
	while(counter_delay_ms != 0);
	/* TIM2 disable counter */
	TIM_Cmd(TIMER_DELAY, DISABLE);
}


/**
 *	@brief  Time delay in microsecond
 *  @param  delay : delay in us.
 *  @retval none
 */
void delay_us(uint16_t delay)
{
	counter_delay_ms = delay;

	TimerDelay_us_Config();

	TIM_SetCounter(TIMER_US_DELAY, 0);
	/* TIM2 enable counter */
    TIM_Cmd(TIMER_US_DELAY, ENABLE);
	/* Wait for 'delay' us */
	while(counter_delay_ms != 0);
	/* TIM2 disable counter */
	TIM_Cmd(TIMER_US_DELAY, DISABLE);
}

/**
 *	@brief  Launch a timer to perform timing measurement
 *  @param  None
 *  @retval none
 */
void StartPerfMeasurement(void)
{
	timestamp_us = 0;

	TimerTimestamp_us_Config ();

	TIM_SetCounter(TIMER_TIMESTAMP, 0);
	/* TIM3 enable counter */
  TIM_Cmd(TIMER_TIMESTAMP, ENABLE);
}

/**
 *	@brief  Get elapsed time since measurement launch
 *  @param  None
 *  @retval none
 */
uint32_t GetElapsed_us(void)
{
	return timestamp_us;
}

/**
 *	@brief  Stop measurement
 *  @param  None
 *  @retval none
 */
void StopPerfMeasurement(void)
{
	timestamp_us = 0;
	/* TIM3 disable counter */
	TIM_Cmd(TIMER_TIMESTAMP, DISABLE);
}



/**
 *	@brief 	Time delay in millisecond. The default priority are changed for this function.
 * 					This function is called by reset function to guarantee the execution time
 *  @param  delay : delay in ms.
 *  @retval none
 */
void delayHighPriority_ms(uint16_t delay)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	counter_delay_ms = delay;

	/* Enable and set TIMER IRQ used for delays. High priority*/
	NVIC_InitStructure.NVIC_IRQChannel	 						= TIMER_DELAY_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPriority                  = 0;
    //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_HIGHPRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 	= TIMER_DELAY_SUB_HIGHPRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd 						=	 ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);
	TIM_SetCounter(TIMER_DELAY, 0);
	/* TIM2 enable counter */
    TIM_Cmd(TIMER_DELAY, ENABLE);
	/* Wait for 'delay' milliseconds */
	while(counter_delay_ms != 0);
	/* TIM2 disable counter */
	TIM_Cmd(TIMER_DELAY, DISABLE);

	/* Enable and set TIMER IRQ used for delays. Default priority */
	NVIC_InitStructure.NVIC_IRQChannel 							= TIMER_DELAY_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPriority                  = 1;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 	= TIMER_DELAY_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd 						=	 ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *	@brief  This function decrements the counter every millisecond used by the function delay_ms
 *  @param  None
 *  @retval None
 */
void decrement_delay(void)
{
	if(counter_delay_ms != 0)
	{
		/* Decrements the counter */
		counter_delay_ms--;
	}
}

/**
 *	@brief  This function decrements the counter every millisecond used by the function delay_ms
 *  @param  None
 *  @retval None
 */
void increment_timestamp(void)
{
	if(timestamp_us <= 0xFFFFFFFE)
	{
		timestamp_us++;
	}
}

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
