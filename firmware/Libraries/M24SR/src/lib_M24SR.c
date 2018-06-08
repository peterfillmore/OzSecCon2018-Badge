/**
  ******************************************************************************
  * @file    lib_M24SR.c
  * @author  MMY Application Team
  * @version V2.0.0
  * @date    06-December-2016
  * @brief   This file help to manage M24SR in a NFC forum context.
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
#include "lib_M24SR.h"

/** @addtogroup M24SR_Driver
  * @{
  *	@brief  <b>This folder contains the driver layer of M24SR family (M24SR64, M24SR16, M24SR04, M24SR02)</b> 
  */

/** @addtogroup lib_M24SR
  * @{
	*	@brief  This is the library to interface with the M24SR dynamic tag.
	*         This layer simplify the use of the M24SR driver by sequencing 
	*         some commands.
  */
uint8_t DefaultPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
														 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	

uint8_t I2CPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint32_t NDEFSessionOpenID=NDEF_SESSION_CLOSED;

/* Init NDEF_FileID with bad value in case Init failed */
static uint16_t NDEF_FileID = 0xDEAD;
								

/** @defgroup libM24SR_Private_Functions
  * @{
  */

/**
  * @}
  */


/** @defgroup libM24SR_Public_Functions
  * @{
  */ 

/**
  * @brief  This fonction initialize the M24SR
	* @param	CCBuffer : pointer on the buffer to store CC file
	* @param	size : number of byte of data to read
  * @retval SUCCESS : Initalization done
	* @retval ERROR : Not able to Initialize. 
  */
uint16_t M24SR_Initialization ( uint8_t* CCBuffer, uint8_t size )
{
	uint16_t status = ERROR;
	uint16_t timeout = 1000; /* wait 1sec */
	
	/* Perform HW initialization */
	M24SR_Init();
	
	if(size < 15)
		return ERROR;
	
	/* Read CC file */
	status = M24SR_GetSession();
	while( status != M24SR_ACTION_COMPLETED && timeout)
	{
		delay_ms(1);
		status = M24SR_GetSession();
		timeout--;
	}
	if (status != M24SR_ACTION_COMPLETED)
		return ERROR;
	/*===================================*/
	/* Select the NFC type 4 application */ 
	/*===================================*/
	errorchk( M24SR_SelectApplication() );
		
	/*==================*/		
	/* select a CC file */ 
	/*==================*/
	errorchk (M24SR_SelectCCfile() );
			
	/* read the first 15 bytes of the CC file */
	if( M24SR_ReadData ( 0x0000 , size , CCBuffer )== M24SR_ACTION_COMPLETED)
	{			
		NDEF_FileID = (uint16_t) ((CCBuffer[0x09]<<8) | CCBuffer[0x0A]);
		errorchk( M24SR_Deselect () );
		return SUCCESS;
	}
	else
		errorchk( M24SR_Deselect () );
		
Error:
		return ERROR;
		
}

/**
  * @brief  This fonction initialize the M24SR
	* @param	NumberOfFile : number of file M24SR must managed
  * @retval SUCCESS : Initalization done
	* @retval ERROR : Not able to Initialize. 
  */
uint16_t M24SR_Format ( uint8_t NumberOfFile, uint8_t* pNewReadPassWord, uint8_t* pNewWritePassWord )
{
	uint16_t status = ERROR;
	uint16_t timeout = 1000; /* wait 1sec */
	uint16_t data = 0;
	uint8_t FileId[2];
	uint16_t Private_fileID;
	
	if( NumberOfFile > 2)
		return ERROR;
	
	/* Read CC file */
	status = M24SR_GetSession();
	while( status != M24SR_ACTION_COMPLETED && timeout)
	{
		delay_ms(1);
		status = M24SR_GetSession();
		timeout--;
	}
	if (status != M24SR_ACTION_COMPLETED)
		return ERROR;
	/*===================================*/
	/* Select the NFC type 4 application */ 
	/*===================================*/
	errorchk( M24SR_SelectApplication() );
		
	/*==================*/		
	/* select a CC file */ 
	/*==================*/
	errorchk (M24SR_SelectSystemfile() );
	status = M24SR_WriteData ( 0x0007 , 0x01 , &NumberOfFile );

	if( status == M24SR_ACTION_COMPLETED)
	{	
		/* select CC file to retrieve new file ID */
		errorchk (M24SR_SelectCCfile() );
		
		/* read new file ID */
		M24SR_ReadData ( 0x0011 , 0x02 , FileId );
		
		Private_fileID = (uint16_t)((FileId[0])<<8) + (uint16_t)(FileId[1]);
		
		/* select the new file */
		errorchk( M24SR_SelectNDEFfile(Private_fileID) );
		
		/* Force File length to 0x0000 */
		errorchk( M24SR_WriteData(0x00, 0x02, (uint8_t*) (&data)) );
		
		/* set File property to proprietary */
		errorchk( M24SR_UpdateFileType(0x05) );
		
		/* then set RD and WR PWD */
		M24SR_EnableReadPassword(DefaultPassword, pNewReadPassWord);
		M24SR_EnableWritePassword(DefaultPassword, pNewWritePassWord);
		
		errorchk( M24SR_Deselect () );
		return SUCCESS;
	}
	
Error:
		return ERROR;
		
}

/**
  * @brief  This fonction retrieve the NDEF file ID of NDEF file present in M24SR
	* @param	NDEF_fileID : To store NDEF ID
  * @retval SUCCESS : File ID read
	* @retval ERROR : Not able to read file ID. 
  */
uint16_t M24SR_GetNDEFFileId ( uint16_t *NDEF_fileID )
{
	if( NDEF_FileID != 0xDEAD)
	{
		*NDEF_fileID = NDEF_FileID;
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}


/**
  * @brief  This fonction configure the M24SR to access NDEF message by I2C
	* @param	NDEF_fileID : NDEF identification to select NDEF in M24SR
	* @param  Priority: 2 options: check if M24SR available to open session (no RF session on going)
  *															 Kill RF session and open I2C sesssion.
  * @retval SUCCESS : Session is opened
	* @retval ERROR : Not able to open session. 
  */
uint16_t M24SR_OpenNDEFSession ( uint16_t NDEF_fileID, uint16_t Priority )
{
	uint16_t status = ERROR;
	uint16_t timeout = 1000;
	
	if(NDEFSessionOpenID == NDEF_SESSION_CLOSED)
	{
		if( Priority == TAKE_SESSION)
		{
			status = M24SR_KillSession();
		}
		else
		{
			status = M24SR_GetSession();
			while( status != M24SR_ACTION_COMPLETED && timeout)
			{
				delay_ms(1);
				status = M24SR_GetSession();
				timeout--;
			}
		}
		if (status != M24SR_ACTION_COMPLETED)
			return ERROR;
	
		/*===================================*/
		/* Select the NFC type 4 application */ 
		/*===================================*/
		errorchk( M24SR_SelectApplication() );
		
		/*====================*/
		/* select NDEF file   */
		/*====================*/
		errorchk( M24SR_SelectNDEFfile(NDEF_fileID) );
	
		NDEFSessionOpenID = (uint32_t)(NDEF_fileID);

		return SUCCESS;
	}
	else if(NDEFSessionOpenID == NDEF_fileID)
	{
		/* Session already Open not an issue caller can perform access in NDEF file */
		return SUCCESS;
	}
		
Error:
		return ERROR;	
}

/**
  * @brief  This fonction close the NDEF Session.
	* @param	NDEF_fileID : NDEF identification to select NDEF in M24SR
  * @retval SUCCESS : Session is closed
	* @retval ERROR : Not able to close session. 
  */
uint16_t M24SR_CloseNDEFSession ( uint16_t NDEF_fileID )
{
	uint16_t status = ERROR;
	
	if(NDEFSessionOpenID == (uint32_t)(NDEF_fileID))
	{
		errorchk( M24SR_Deselect () );
		NDEFSessionOpenID = NDEF_SESSION_CLOSED;
	
		return SUCCESS;
	}
	else if(NDEFSessionOpenID == NDEF_SESSION_CLOSED)
	{
		/* Not an error as session is already closed */
		return SUCCESS;
	}
		
Error:
		return ERROR;	
}

/**
  * @brief  This fonction read the data stored in M24SR at defined offset
	* @param	Offset : Offset in the NDEF file in M24SR
	* @param	DataSize : Number of byte to read
	* @param	pData : pointer on buffer to store read data
	* @retval Status (SW1&SW2) : Status of the operation. 
  */
uint16_t M24SR_ReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
	uint16_t status;
	
	if( DataSize > M24SR_READ_MAX_NBBYTE)
	{	
		do
		{
			status = M24SR_ReadBinary ( Offset, M24SR_READ_MAX_NBBYTE , pData);
			Offset += M24SR_READ_MAX_NBBYTE;
			pData += M24SR_READ_MAX_NBBYTE;
			DataSize -= M24SR_READ_MAX_NBBYTE;
		}while( DataSize > M24SR_READ_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
		if( status == M24SR_ACTION_COMPLETED && DataSize)
			status = M24SR_ReadBinary ( Offset, (uint8_t)(DataSize) , pData);
	}
	else
		status = M24SR_ReadBinary ( Offset, (uint8_t)(DataSize) , pData);
	
	return status;
}

/**
  * @brief  This fonction read the data stored in M24SR at defined offset without NDEF concideration
	* @param	Offset : Offset in the NDEF file in M24SR
	* @param	DataSize : Number of byte to read
	* @param	pData : pointer on buffer to store read data
	* @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t M24SR_ForceReadData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
	uint16_t status;
	
	if( DataSize > M24SR_READ_MAX_NBBYTE)
	{	
		do
		{
			status = M24SR_STReadBinary ( Offset, M24SR_READ_MAX_NBBYTE , pData);
			Offset += M24SR_READ_MAX_NBBYTE;
			pData += M24SR_READ_MAX_NBBYTE;
			DataSize -= M24SR_READ_MAX_NBBYTE;
		}while( DataSize > M24SR_READ_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
		if( status == M24SR_ACTION_COMPLETED && DataSize)
			status = M24SR_STReadBinary ( Offset, (uint8_t)(DataSize) , pData);
	}
	else
		status = M24SR_STReadBinary ( Offset, (uint8_t)(DataSize) , pData);
	
	return status;
}

/**
  * @brief  This fonction write data in M24SR at defined offset
	* @param	Offset : Offset in the NDEF file in M24SR
	* @param	DataSize : Number of byte to read
	* @param	pData : pointer on buffer to copy in M24SR
	* @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t M24SR_WriteData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData)
{
	uint16_t status;
	
	if( DataSize > M24SR_WRITE_MAX_NBBYTE)
	{	
		do
		{
			status = M24SR_UpdateBinary ( Offset, M24SR_WRITE_MAX_NBBYTE , pData);
			Offset += M24SR_WRITE_MAX_NBBYTE;
			pData += M24SR_WRITE_MAX_NBBYTE;
			DataSize -= M24SR_WRITE_MAX_NBBYTE;
		}while( DataSize > M24SR_WRITE_MAX_NBBYTE && status == M24SR_ACTION_COMPLETED);
		if( status == M24SR_ACTION_COMPLETED && DataSize)
			status = M24SR_UpdateBinary ( Offset, (uint8_t)(DataSize) , pData);
	}
	else
		status = M24SR_UpdateBinary ( Offset, (uint8_t)(DataSize) , pData);
	
	return status;
}

/**
  * @brief  This fonction read the private data stored in M24SR after gaining access with password
	* @param	Offset : Offset in the NDEF file in M24SR
	* @param	DataSize : Number of byte to read
	* @param	pData : pointer on buffer to store read data
	* @param	pReadPassword : pointer on the read password
	* @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t M24SR_ReadPrivateData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData, uint8_t* pReadPassword)
{
	uint16_t status;
	
	/* Present password to gain read access */
	status = M24SR_Verify( READ_PWD ,0x10 ,pReadPassword );
	if( status == M24SR_PWD_CORRECT)
	{
		status = M24SR_ForceReadData(Offset, DataSize, pData);
	}
	
	return status;
}

/**
  * @brief  This fonction read the private data stored in M24SR after gaining access with password
	* @param	Offset : Offset in the NDEF file in M24SR
	* @param	DataSize : Number of byte to read
	* @param	pData : pointer on buffer to store read data
	* @param	pWritePassword : pointer on the write password
	* @retval Status (SW1&SW2) : Status of the operation.  
  */
uint16_t M24SR_WritePrivateData ( uint16_t Offset , uint16_t DataSize , uint8_t* pData, uint8_t* pWritePassword)
{
	uint16_t status;
	
	/* Present password to gain read access */
	status = M24SR_Verify( WRITE_PWD ,0x10 ,pWritePassword );
	if( status == M24SR_PWD_CORRECT)
	{
		status = M24SR_WriteData(Offset, DataSize, pData);
	}
	
	return status;
}

/**
  * @brief  This fonction activate the need of a password for next read access
	* @param	pCurrentWritePassword : Write password is needed to have the right to enable Read Password
	* @param	pNewPassword : The password that will be requiered for next read access
  * @retval SUCCESS : Read password is activated
	* @retval ERROR : operation does not complete  
  */
uint16_t M24SR_EnableReadPassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
	{				
		/* Set new password */
		M24SR_ChangeReferenceData ( READ_PWD, pNewPassword );
		M24SR_EnableVerificationRequirement( READ_PWD );
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction desactivate the need of a password for next read access
	* @param	pCurrentWritePassword : Write password is needed to have the right to disable Read Password
  * @retval SUCCESS : Read password is desactivated
	* @retval ERROR : operation does not complete  
  */
uint16_t M24SR_DisableReadPassword( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
	{				
		/* Set new password */
		M24SR_DisableVerificationRequirement( READ_PWD );
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction activate the need of a password for next write access
	* @param	pCurrentWritePassword : Write password must be prensented to have the right to modify write Password
	* @param	pNewPassword : The password that will be requiered for next write access
  * @retval SUCCESS : Write password is activated
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_EnableWritePassword( uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
	uint16_t status;

	/* check we have the good password */
	if (M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword )== M24SR_PWD_CORRECT)
	{
		/* Set new password */
		M24SR_ChangeReferenceData ( WRITE_PWD, pNewPassword );
		M24SR_EnableVerificationRequirement( WRITE_PWD );
		status = SUCCESS;			
	}
	else /* we don't have the good password */
	{				
		status = ERROR;
	}
	
	return status;
}	

/**
  * @brief  This fonction desactivate the need of a password for next write access
	* @param	pCurrentWritePassword : Write password must be prensented to have the right to disable it
  * @retval SUCCESS : Write password is desactivated
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_DisableWritePassword( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
	{				
		M24SR_DisableVerificationRequirement( WRITE_PWD );
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction desactivate the need of read and write password for next access
	* @param	pSuperUserPassword : I2C super user password to overwrite read and write password
  * @retval SUCCESS : M24SR access is now free (no password needed)
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_DisableAllPassword( uint8_t* pSuperUserPassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( I2C_PWD ,0x10 ,pSuperUserPassword ) == M24SR_PWD_CORRECT)
	{				
		M24SR_DisablePermanentState( READ_PWD );
		M24SR_DisablePermanentState( WRITE_PWD );
	
		M24SR_DisableVerificationRequirement( READ_PWD );
		M24SR_DisableVerificationRequirement( WRITE_PWD );
	
		/* reset password */
		M24SR_ChangeReferenceData ( READ_PWD, pSuperUserPassword );
		M24SR_ChangeReferenceData ( WRITE_PWD, pSuperUserPassword );
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}

/**
  * @brief  This fonction enable read only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to enable read only mode
  * @retval SUCCESS : M24SR access is now forbidden in write mode
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_EnableReadOnly( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
	{				
		M24SR_EnablePermanentState( WRITE_PWD ); /* lock write to have read only */
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction disable read only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to disable read only mode
  * @retval SUCCESS : M24SR write access is now allowed 
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_DisableReadOnly( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;
														
	if(M24SR_Verify( I2C_PWD ,0x10 ,I2CPassword ) == M24SR_PWD_CORRECT)
	{					
		M24SR_DisablePermanentState( WRITE_PWD ); /* disable write protection to disable read only mode */
		M24SR_DisableVerificationRequirement( WRITE_PWD );
		status = SUCCESS;
	}
	else
	{
		/* we don't have the good I2C password nothing to do anymore */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction enable write only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to enable write only mode
  * @retval SUCCESS : M24SR access is now forbidden in read mode
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_EnableWriteOnly( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( WRITE_PWD ,0x10 ,pCurrentWritePassword ) == M24SR_PWD_CORRECT)
	{				
		M24SR_EnablePermanentState( READ_PWD ); /* disable read access and keep write */
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}	

/**
  * @brief  This fonction disable write only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to disable write only mode
  * @retval SUCCESS : M24SR read access is now allowed 
	* @retval ERROR : operation does not complete   
  */
uint16_t M24SR_DisableWriteOnly( uint8_t* pCurrentWritePassword)
{
	uint16_t status = SUCCESS;

	if(M24SR_Verify( I2C_PWD ,0x10 ,I2CPassword ) == M24SR_PWD_CORRECT)
	{				
		M24SR_DisablePermanentState( READ_PWD ); /* disable write only -> enable write acces */
		M24SR_DisableVerificationRequirement( READ_PWD );
		status = SUCCESS;
	}
	else
	{
		/* M24SR already lock but password not known */
		status = ERROR;
	}
			
	return status;
}

/**
  * @brief  This function configure GPO purpose for RF session
	* @param	GPO_config: GPO configuration to set
	* @param	mode: select RF or I2C, GPO config to update
  * @retval Status : Status of the operation.
  */
uint16_t M24SR_ManageGPO( uc8 GPO_config, uc8 mode)
{
	uint16_t status;
	
	if( mode == RF_GPO)
	{
		status = M24SR_ManageRFGPO ( GPO_config );
	}
	else
	{
		status = M24SR_ManageI2CGPO ( GPO_config );
	}
	return status;
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


