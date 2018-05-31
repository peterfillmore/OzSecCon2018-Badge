/**
  ******************************************************************************
  * @file    drv_I2C_M24SR.c
  * @author  MMY Application Team
  * @version V4.5.0
  * @date    08-May-2013
  * @brief   This file provides a set of functions needed to manage the I2C of
	*				   the M24SR device.
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_I2C_M24SR.h"

/** @addtogroup M24SR_Driver
  * @{
  *	@brief  <b>This folder contains the driver layer of M24SR family (M24SR64, M24SR16, M24SR04, M24SR02)</b> 
  */

/** @addtogroup M24SR_I2C
 * 	@{
 *  @brief  This file includes the I2C driver used by M24SR family to communicate with the MCU.  
 */

/** @defgroup M24SR_I2C_Private_Functions
 *  @{
 */

static uint8_t						uSynchroMode = M24SR_WAITINGTIME_POLLING;
//static uint8_t						uSynchroMode = M24SR_WAITINGTIME_TIMEOUT;


/**
  * @brief  This functions polls the I2C interface
  * @retval M24SR_STATUS_SUCCESS : the function is succesful
	* @retval M24SR_ERROR_I2CTIMEOUT : The I2C timeout occured. 
	* @retval M24SR_ERROR_NACK : the M24SR doesn't acknowledge
	* @retval M24SR_ERROR_NBATEMPT  
  */
//static int8_t M24SR_PollI2C ( void )
int8_t M24SR_PollI2C ( void )
{
uint32_t	M24SR_Timeout = 0;
uint16_t	NbAtempt = M24SR_I2C_POLLING,
			uISRregister= 0x0000;

  /*---------------------------- Transmission Phase ---------------------------*/
	 do{	  
		 
		/* In the loop avoid to send START just after STOP */
		/* At 400kHz see issue where start condition was sent but not the 7bits addr... */ 
		delay_us (1); 
		
        //Wait until I2C isn't busy
        while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_BUSY) == SET);
 
		M24SR_Timeout = 0x40;  
		 /*!< Clear the M24SR_I2C NACK flag */
		I2C_ClearFlag(M24SR_I2C, I2C_FLAG_NACKF);

		/*!< Enable M24SR_I2C acknowledgement if it is already disabled by other function */
		I2C_AcknowledgeConfig(M24SR_I2C, DISABLE);
		
		/*!< Send M24SR_I2C START condition */
		I2C_GenerateSTART(M24SR_I2C, ENABLE);

		/*!< Test on M24SR_I2C EV5 and clear it */
		//while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_SB)) && --M24SR_Timeout)  /*!< EV5 */
		//{
		//}
				
	    /*!< Send M24SR64-Y slave address for write */
        I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 0);
		//I2C_Send7bitAddress(M24SR_I2C, M24SR_ADDR, I2C_Direction_Transmitter);
		M24SR_Timeout = 0x40; 
		
		uISRregister = I2C_ReadRegister(M24SR_I2C, I2C_Register_ISR);
	
        	
        /* Note: ADDR is not set after a NACK reception */
		while ( ((uISRregister != I2C_FLAG_NACKF) && ((uISRregister & I2C_FLAG_BUSY) != I2C_FLAG_BUSY))&& --M24SR_Timeout)/*BUSY & MSL & ADDR */
		{
			/*reads the I2C ISR register */ 
			uISRregister = I2C_ReadRegister(M24SR_I2C, I2C_Register_ISR);
			//uISRregister &= 0x0482;
			uISRregister &= I2C_FLAG_TXE | I2C_FLAG_ADDR;
		}	
		
		/* Send STOP Condition */
		I2C_GenerateSTOP(M24SR_I2C, ENABLE);
				
		if (M24SR_Timeout == 0)
		{
			return M24SR_ERROR_NBATEMPT;
		}
	    else {
            I2C_Cmd(M24SR_I2C, DISABLE); //wait till we receive a successful address?
			do
			{
				uISRregister = I2C_ReadRegister(M24SR_I2C, I2C_Register_ISR);
			}
			while( (uISRregister & I2C_FLAG_ADDR) == I2C_FLAG_ADDR);
			I2C_Cmd(M24SR_I2C, ENABLE);
			return M24SR_STATUS_SUCCESS;		
        }
	} while ( (uISRregister != 0x0000) && (--NbAtempt > 0) );
	
	if (NbAtempt == 0)
	{
			return M24SR_ERROR_NBATEMPT;
	}
	return M24SR_ERROR_NACK;
}

/**
  * @}
  */


/** @defgroup M24SR_I2C_Public_Functions
  * @{
  */

/**
  * @brief  This function initializes the M24SR_I2C interface
	* @retval None  
  */
void M24SR_I2CInit ( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef   I2C_InitStructure;

  /* GPIO Periph clock enable */
    RCC_AHBPeriphClockCmd(M24SR_GPIO_BLOCK_CLK, ENABLE);
	
  /* I2C Periph clock enable */
    RCC_APB1PeriphClockCmd(M24SR_I2C_BLOCK_CLK, ENABLE);
	
  /* Configure I2C pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  M24SR_SCL_PIN | M24SR_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(M24SR_SDA_PIN_PORT, &GPIO_InitStructure);

	/* Connect I2C pins to AF */
  GPIO_PinAFConfig(M24SR_SCL_PIN_PORT, M24SR_SCL_PIN_SOURCE, GPIO_AF_1);
  GPIO_PinAFConfig(M24SR_SDA_PIN_PORT, M24SR_SDA_PIN_SOURCE, GPIO_AF_1);

  /* Configure GPIO for M24SR GPO as Input pull-up, used as Interuption comming from the M24SR  */
  GPIO_InitStructure.GPIO_Pin = M24SR_GPO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/* Configure GPIO for M24SR RF Disable signal */
  GPIO_InitStructure.GPIO_Pin = M24SR_RFDIS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(M24SR_RFDIS_PIN_PORT, &GPIO_InitStructure);

  I2C_DeInit(M24SR_I2C);

  /* I2C Init */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 
  //I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  //I2C_InitStructure.I2C_ClockSpeed = M24SR_I2C_CLOCKSPEED;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  //i2c 400kHz, 8MHz
  //I2C_InitStructure.I2C_Timing = 0x0010020A;
  //i2c 400kHz, 48MHz
  I2C_InitStructure.I2C_Timing = 0x00901850;
  //i2c 100kHz 8MHz 
  //I2C_InitStructure.I2C_Timing = 0x00201D2B;
  //i2c 100kHz 48MHz 
  //I2C_InitStructure.I2C_Timing = 0x10805e89;

  I2C_Init(M24SR_I2C, &I2C_InitStructure);

  /* I2C Init */
  I2C_Cmd(M24SR_I2C, ENABLE);
	
}

/**
  * @brief  this functions configure I2C synchronization mode
	* @param  mode : 
  * @retval None
  */
void M24SR_SetI2CSynchroMode( uc8 mode )
{
#ifdef I2C_GPO_SYNCHRO_ALLOWED
	uSynchroMode = mode;
#else
	if( mode == M24SR_WAITINGTIME_GPO)
		uSynchroMode = M24SR_WAITINGTIME_POLLING;
	else
		uSynchroMode = mode;
#endif /*  I2C_GPO_SYNCHRO_ALLOWED */
}

/**
  * @brief  This function generates an I2C Token release
  * @retval M24SR_STATUS_SUCCESS : the function is succesful
	* @retval M24SR_ERROR_I2CTIMEOUT : The I2C timeout occured. 
  */
int8_t M24SR_I2CTokenRelease ( void )
{
  uint32_t M24SR_Timeout = M24SR_I2C_TIMEOUT;
	uint8_t	NthByte= 0;

    //Wait until I2C isn't busy
    while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_BUSY) == SET);
  	
  /*!< Clear the M24SR_I2C AF flag */
  //I2C_ClearFlag(M24SR_I2C, I2C_FLAG_AF);

  /*!< Enable M24SR_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(M24SR_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /*!< Send M24SR_I2C START condition */
  I2C_GenerateSTART(M24SR_I2C, ENABLE);

  /*!< Test on M24SR_I2C EV5 and clear it */
  //while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_SB)) && M24SR_Timeout)  /*!< EV5 */
  //{
  // if (M24SR_Timeout-- == 0) goto Error;
  //}
  
  //M24SR_Timeout = M24SR_I2C_TIMEOUT;

  //send device select
  /*!< Send M24SR** slave address  */
  //I2C_Send7bitAddress(M24SR_I2C, M24SR_ADDR, I2C_Direction_Transmitter);
  //if(direction == I2C_Direction_Transmitter) 
  I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 0);
  //else if(direction == I2C_Direction_Receiver) 
  //I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 1);

    // check if slave acknowledged his address within timeout
    //if(direction == I2C_Direction_Transmitter) 
    //    while(!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //else if(direction == I2C_Direction_Receiver) 
    //    while(!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)

  //while ((!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && M24SR_Timeout)/* EV6 */
  //{
  // if (--M24SR_Timeout == 0) goto Error;
  //}
	/* check the AF a flag*/
  
  delay_ms(40);
  
  I2C_GenerateSTOP(M24SR_I2C, ENABLE);

	return M24SR_STATUS_SUCCESS;
  
Error :
	/* Send STOP Condition */
  I2C_GenerateSTOP(M24SR_I2C, ENABLE);
  
	return M24SR_ERROR_I2CTIMEOUT;
}

/**
  * @brief  This functions sends the command buffer 
	* @param  NbByte : Number of byte to send
  * @param  pBuffer : pointer to the buffer to send to the M24SR
  * @retval M24SR_STATUS_SUCCESS : the function is succesful
	* @retval M24SR_ERROR_I2CTIMEOUT : The I2C timeout occured. 
  */
int8_t M24SR_SendI2Ccommand ( uc8 NbByte , uc8 *pBuffer )
{
	uint32_t M24SR_Timeout = M24SR_I2C_TIMEOUT;
	uint8_t	NthByte= 0;

  //Wait until I2C isn't busy
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_BUSY) == SET);

  /*!< Clear the M24SR_I2C AF flag */
  //I2C_ClearFlag(M24SR_I2C, I2C_FLAG_AF);

  /*!< Enable M24SR_I2C acknowledgement if it is already disabled by other function */
  //I2C_AcknowledgeConfig(M24SR_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /*!< Send M24SR_I2C START condition */
  //I2C_GenerateSTART(M24SR_I2C, ENABLE);

  /*!< Test on M24SR_I2C EV5 and clear it */
  //while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_SB)) && M24SR_Timeout)  /*!< EV5 */
  //{
  // if (M24SR_Timeout-- == 0) goto Error;
  //}
  
  //M24SR_Timeout = M24SR_I2C_TIMEOUT;

  /*!< Send M24SR** slave address  */
  //I2C_Send7bitAddress(M24SR_I2C, M24SR_ADDR, I2C_Direction_Transmitter);
  //I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 0);

  //I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
  I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, NbByte, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

  //Ensure the transmit interrupted flag is set
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_TXIS) == RESET);
 
  //while ((!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && M24SR_Timeout)/* EV6 */
  //{
  // if (--M24SR_Timeout == 0) goto Error;
  //}
	/* check the AF a flag*/
	
	for (NthByte = 0; NthByte < NbByte ; NthByte ++)
	{
		M24SR_Timeout = M24SR_FLAG_TIMEOUT;
		/* Send the device's internal address to write to */
		while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_TXE))) //&& (!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_BTF)))  
		{
		 if (M24SR_Timeout-- == 0) goto Error;
		}
        I2C_SendData(M24SR_I2C, pBuffer[NthByte]);  
	}
    
    while (I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_STOPF) == RESET);
    I2C_ClearFlag(M24SR_I2C, I2C_FLAG_STOPF); 
	
    /* Send STOP Condition */
    //I2C_GenerateSTOP(M24SR_I2C, ENABLE);
	
    //M24SR_Timeout = M24SR_I2C_TIMEOUT;
  /*!< Test on M24SR_I2C EV7 and clear it */
  //while (( (M24SR_I2C->CR1 & 0x0200) != 0x0000)  && M24SR_Timeout--)  /*!< EV7 */
  //{
  //}
	
	//if( (M24SR_I2C->SR1 & 0x0400) )
	//{
	//	/* No acknowledgement from M24SR */
	//	return  M24SR_ERROR_NOACKNOWLEDGE;
	//}
	
	return M24SR_STATUS_SUCCESS;
	
Error :
	/* Send STOP Condition */
  I2C_GenerateSTOP(M24SR_I2C, ENABLE);
	return M24SR_ERROR_I2CTIMEOUT;
}


/**
  * @brief  This functions reads a response of the M24SR device
	* @param  NbByte : Number of byte to read (shall be >= 5)
  * @param  pBuffer : Pointer to the buffer to send to the M24SR
  * @retval M24SR_STATUS_SUCCESS : The function is succesful
	* @retval M24SR_ERROR_I2CTIMEOUT : The I2C timeout occured. 
  */
int8_t M24SR_ReceiveI2Cresponse ( uint8_t NbByte , uint8_t *pBuffer )
{
    uint32_t M24SR_Timeout = M24SR_I2C_TIMEOUT/2;
    uint8_t i=0,
    				NthByte;

  //wait till i2c not busy
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_BUSY) == SET);
 
  I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write); 
 
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_TXIS) == RESET);

  I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, NbByte, I2C_AutoEnd_Mode, I2C_Generate_Start_Read); 
 
  /*!< Clear the M24SR_I2C AF flag */
  //I2C_ClearFlag(M24SR_I2C, I2C_FLAG_AF);

  /*---------------------------- Transmission Phase ---------------------------*/

	/* &&&& MUST NOT BE INTERRUPTED TIMING IS CRITICAL INSIDE THIS SECTION &&&&& */
	
  /*!< Send M24SR_I2C START condition */
  //I2C_GenerateSTART(M24SR_I2C, ENABLE);
	
	  /*!< Enable M24SR_I2C acknowledgement if it is already disabled by other function */
  //I2C_AcknowledgeConfig(M24SR_I2C, DISABLE);

  /*!< Test on M24SR_I2C EV5 and clear it */
  //while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_SB)) && M24SR_Timeout)  /*!< EV5 */
  //{
  // if (M24SR_Timeout-- == 0) goto Error;
  //}
  
  /*!< Send M24SR_I2C slave address for write */
  //I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 1);
  //I2C_Send7bitAddress(M24SR_I2C, M24SR_ADDR, I2C_Direction_Receiver);

  /* Test on ADDR Flag */
  //M24SR_Timeout = M24SR_FLAG_TIMEOUT;
  //while (!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  //{
  //  if((M24SR_Timeout--) == 0) 
  //  		goto Error;
  //}
	
	//I2C_AcknowledgeConfig(M24SR_I2C, ENABLE);
	
	/* &&&& MUST NOT BE INTERRUPTED TIMING IS CRITICAL INSIDE THIS SECTION &&&&& */
	
	/* recover the device select byte */
	//I2C_ReceiveData(M24SR_I2C); 

	/* Test on RXE FLag (data sent) */
	//M24SR_Timeout = M24SR_FLAG_TIMEOUT;
	//while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_RXNE)) )  
	//{
	// if (M24SR_Timeout-- == 0)
	//	 goto Error;
	//}

	/* read the NthByte bytes*/
	for (NthByte = NbByte; NthByte >1 ; NthByte --)
	{	
        while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_RXNE) == RESET);				
		/* deactive the ACKbit*/
		//if (NthByte == 2)
		//{
		//	//M24SR_I2C->CR1 &= ~(0x0400); //flip the ACK 
		//	I2C_GenerateSTOP(M24SR_I2C, ENABLE);	
		//}
				
		/* Send the device's internal address to write to */
		pBuffer[i++] = I2C_ReceiveData(M24SR_I2C);  

		/* Test on RXE FLag (data sent) */
		M24SR_Timeout = M24SR_FLAG_TIMEOUT;
		while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_RXNE)) )  
		{
			if (M24SR_Timeout-- == 0) goto Error;
		}
			
	}
	//pBuffer[i++] = I2C_ReceiveData(M24SR_I2C);

    //Wait for the stop condition to be sent
    while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_STOPF) == RESET);

    //Clear the stop flag for next transfers
    I2C_ClearFlag(M24SR_I2C, I2C_FLAG_STOPF);

//	M24SR_Timeout = M24SR_I2C_TIMEOUT;
  /*!< Test on M24SR_I2C EV7 and clear it */
//  while (( (M24SR_I2C->CR1 & 0x0200) != 0x0000)  && M24SR_Timeout--)  /*!< EV7 */
//  {
//  }

	return M24SR_STATUS_SUCCESS;
	
Error :
	/* Send STOP Condition */
  I2C_GenerateSTOP(M24SR_I2C, ENABLE);
	return M24SR_ERROR_I2CTIMEOUT;
}



/**
  * @brief  This function reads a number of byte through the I2C interface
	* @param  NbByte : Number of byte to read
  * @param  pBuffer : pointer to the buffer to send to the M24SR
  * @retval M24SR_STATUS_SUCCESS : the function is succesful
	* @retval M24SR_ERROR_I2CTIMEOUT : The I2C timeout occured. 
  */
int8_t M24SR_ReceiveI2Cbuffer ( uint8_t NbByte , uint8_t *pBuffer )
{
    uint32_t M24SR_Timeout = M24SR_I2C_TIMEOUT/2;
    uint8_t i=0,
				NthByte;

  //wait till i2c not busy
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_BUSY) == SET);
 
  I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write); 
 
  while(I2C_GetFlagStatus(M24SR_I2C, I2C_FLAG_TXIS) == RESET);

  I2C_TransferHandling(M24SR_I2C, M24SR_ADDR, NbByte, I2C_AutoEnd_Mode, I2C_Generate_Start_Read); 

  /*!< Clear the M24SR_I2C AF flag */
  //I2C_ClearFlag(M24SR_I2C, I2C_FLAG_AF);

  /*!< Enable M24SR_I2C acknowledgement if it is already disabled by other function */
  //I2C_AcknowledgeConfig(M24SR_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /*!< Send M24SR_I2C START condition */
  //I2C_GenerateSTART(M24SR_I2C, ENABLE);

  /*!< Test on M24SR_I2C EV5 and clear it */
  //while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_SB)) && M24SR_Timeout)  /*!< EV5 */
  //{
  // if (M24SR_Timeout-- == 0) goto Error;
  //}
  
  /*!< Send M24SR_I2C slave address for write */
  //I2C_SendData(M24SR_I2C, (M24SR_ADDR << 1) | 1);
  //I2C_Send7bitAddress(M24SR_I2C, M24SR_ADDR, I2C_Direction_Receiver);

  /* Test on ADDR Flag */
  //M24SR_Timeout = M24SR_FLAG_TIMEOUT;
  //while (!I2C_CheckEvent(M24SR_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  //{
  //  if((M24SR_Timeout--) == 0) goto Error;
  //}
	
	/* recover the device select byte */
	I2C_ReceiveData(M24SR_I2C);  
	/* Test on RXE FLag (data sent) */
	M24SR_Timeout = M24SR_FLAG_TIMEOUT;
	while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_RXNE)) )  
	{
	 if (M24SR_Timeout-- == 0) goto Error;
	}
	
	/* Read the buffer  */ 
	for (NthByte = NbByte ; NthByte >1 ; NthByte --)
	{	
	
		//if (NthByte == 2)
		//{
		//	/* generate the Stop byte*/
		//	I2C_GenerateSTOP(M24SR_I2C, ENABLE);	
		//	/* deactive the ACKbit*/
		//	M24SR_I2C->CR1 &= ~(0x0400);
		//}
		
		/* Send the device's internal address to write to */
		pBuffer[i++] = I2C_ReceiveData(M24SR_I2C);  

		/* Test on RXE FLag (data sent) */
		if (NthByte > 1)
		{
			M24SR_Timeout = M24SR_FLAG_TIMEOUT;
			while ((!I2C_GetFlagStatus(M24SR_I2C,I2C_FLAG_RXNE))  )  
			{
			 if (M24SR_Timeout-- == 0) goto Error;
			}
		}
		
	}

	//pBuffer[i++] = I2C_ReceiveData(M24SR_I2C);

//	M24SR_Timeout = M24SR_I2C_TIMEOUT;
  /*!< Test on M24SR_I2C EV7 and clear it */
  //while (( (M24SR_I2C->CR1 & 0x0200) != 0x0000)  && M24SR_Timeout--)  /*!< EV7 */
  //{
  //}
	
	return M24SR_STATUS_SUCCESS;
	
Error :
	/* Send STOP Condition */
  I2C_GenerateSTOP(M24SR_I2C, ENABLE);
	return M24SR_ERROR_I2CTIMEOUT;
}

/**
  * @brief  This functions returns M24SR_STATUS_SUCCESS when a response is ready
  * @retval M24SR_STATUS_SUCCESS : a response of the M24LR is ready
	* @retval M24SR_ERROR_DEFAULT : the response of the M24LR is not ready
  */
int8_t M24SR_IsAnswerReady ( void )
{
	int8_t status;
  uint32_t retry = 0x3FFFF;
	uint8_t stable = 0;
	
		switch (uSynchroMode)
		{
			case M24SR_WAITINGTIME_POLLING :
				errchk(M24SR_PollI2C ( ));
				return M24SR_STATUS_SUCCESS;
			
			case M24SR_WAITINGTIME_TIMEOUT :
				// M24SR FWI=5 => (256*16/fc)*2^5=9.6ms but M24SR ask for extended time to program up to 246Bytes.
				delay_ms (80);	
				return M24SR_STATUS_SUCCESS;
			
			case M24SR_WAITINGTIME_GPO :
				/* Here we check if I2C answer is ready, so if GPO of M24SR is low */
			  /* Mapping an interrupt was impossible because EXTI_line 6 was already taken by Joystick SEL */
			  /* HW: GPO --> PA6 and Joystick SEL --> PB6 */
				/* Idealy use an interrupt here */
				do
				{
					if( GPIO_ReadInputDataBit(M24SR_GPO_PIN_PORT,M24SR_GPO_PIN) == Bit_RESET)
					{
						stable ++;						
					}
					retry --;						
				}
				while(stable <5 && retry>0);
				if(!retry)
					goto Error;
				
				return M24SR_STATUS_SUCCESS;
			
			default : 
				return M24SR_ERROR_DEFAULT;
		}

Error :
		return M24SR_ERROR_DEFAULT;
}

/**
  * @brief  This function enable or disable RF communication
	* @param	OnOffChoice: GPO configuration to set
  * @retval Status (SW1&SW2) : Status of the operation to complete.
  */
void M24SR_RFConfig_Hard( uc8 OnOffChoice)
{
	/* Disable RF */
	if ( OnOffChoice != 0 )
	{	
		M24SR_RFDIS_PIN_PORT->BRR = M24SR_RFDIS_PIN; 
	}
	else
	{	
		M24SR_RFDIS_PIN_PORT->BSRR = M24SR_RFDIS_PIN;		
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

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/

