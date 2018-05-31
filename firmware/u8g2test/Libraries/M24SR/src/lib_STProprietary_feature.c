/**
  ******************************************************************************
  * @file    lib_STProprietary_feature.c
  * @author  MMY Application Team
  * @version V2.0.0
  * @date    06-December-2016
  * @brief   This file help to manage some proprietary feature.
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
#include "lib_STProprietary_feature.h"

/** @addtogroup NFC_libraries
 * 	@{
 *	@brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @addtogroup libSTProprietary
  * @{
	*	@brief  This part of the library help to manage TAG with proprietary feature.
  */


/** @defgroup libSpecialFeature_Private_Functions
  * @{
  */

/**
 * @brief  This structure contains the data of the CC private file
 */
//sCCPrivateFileInfo CCPrivateFileStruct;

uint8_t CCPrivateFile[8*SYS_NUMBER_OF_MANAGED_FILE];
/**
  * @}
  */


/** @defgroup libSpecialFeature_Public_Functions
  * @{
	*	@brief  This file is used to manage private feature of the tag
  */ 

	
/**
  * @brief  This fonction format M24SR has expected by application if not already done
	* @param	pNewReadPassword pointer on the read password to use to lock the private file
	* @param	pNewWritePassword pointer on the write password to use to lock the private file
  * @retval SUCCESS : M24SR is now formatted
	* @retval ERROR : operation does not complete   
  */	
uint16_t STProprietary_Format(uint8_t *pNewReadPassword, uint8_t* pNewWritePassword)
{
	uint16_t status = SUCCESS;
	uint8_t CCBuffer[(15+8*SYS_NUMBER_OF_MANAGED_FILE)];
	uint16_t Nbr_File = 0;
//	sCCPrivateFileInfo *pCCPrivateFile;
	
	//pCCPrivateFile = &CCPrivateFileStruct;

	/* HW initialization and CC file information retrieved */
	TagT4Init( CCBuffer, 15);
	
	/* Check if M24SR is formated with correct file number */
	/* By default CC file size is 7+8*Nbr_File in M24SR */
	Nbr_File = (uint16_t) ((CCBuffer[0x00]<<8) | CCBuffer[0x01]);
	Nbr_File = (Nbr_File-7)/8;
	
	if( Nbr_File != SYS_NUMBER_OF_MANAGED_FILE )
	{
		/* This is the first boot, so format the M24SR */
		status = FormatTag( SYS_FORMAT_CONFIG, pNewReadPassword, pNewWritePassword);
	}
	else
	{
		/* The M24SR is already well formated */
		status = SUCCESS;
	}
	
	if(status == SUCCESS)
	{		
		/* Keep Information about File Id, file size... */
		TagT4Init( CCBuffer, (15+8*SYS_NUMBER_OF_PRIVATE_FILE));
		
		memcpy(CCPrivateFile,&CCBuffer[0x0F],8*SYS_NUMBER_OF_PRIVATE_FILE);
	
//		pCCPrivateFile->TField = CCBuffer[0x0F];
//		pCCPrivateFile->LField = CCBuffer[0x10];
//		pCCPrivateFile->FileID = (uint16_t) ((CCBuffer[0x11]<<8) | CCBuffer[0x12]);
//		pCCPrivateFile->NDEFFileMaxSize = (uint16_t) ((CCBuffer[0x13]<<8) | CCBuffer[0x14]);
//		pCCPrivateFile->ReadAccess = CCBuffer[0x15];
//		pCCPrivateFile->WriteAccess = CCBuffer[0x16];	
	}
	
	return status;
}

/**
  * @brief  This fonction read data in the private file
	* @param	offset : offset where data must be read
	* @param	size : number of byte of data to read
	* @param	pData : pointer on buffer to fill with NDEF data
	* @param	pReadPassword : pointer on read password
  * @retval SUCCESS : Private data have been retrieved
	* @retval ERROR : Not able to get NDEF file data
  */
uint16_t STProprietary_ReadData(uint8_t FileNumber, uint16_t offset, uint16_t size, uint8_t * pData, uint8_t* pReadPassword)
{
	uint16_t status = ERROR;
	uint16_t FileID;
	
	FileID = (uint16_t) ((CCPrivateFile[8*FileNumber+2]<<8) | CCPrivateFile[8*FileNumber+3]);
	
	/* Password */
	
	if(OpenNDEFSession(FileID, ASK_FOR_SESSION) == SUCCESS)
	{
		status = ReadPrivateData(offset, size, pData, pReadPassword);
		CloseNDEFSession(FileID);
	}
	
	return status;
}

/**
  * @brief  This fonction write NDEF file
	* @param	offset : offset where data must be write
	* @param	size : number of byte of data to write
	* @param	pData : pointer on buffer to copy in memory
  * @param	pWritePassword : pointer on write password
  * @retval SUCCESS : NDEF file data have been stored
	* @retval ERROR : Not able to store NDEF file
  */
uint16_t STProprietary_WriteData(uint8_t FileNumber, uint16_t offset, uint16_t size, uint8_t * pData, uint8_t* pWritePassword)
{
	uint16_t status = ERROR;
	uint16_t FileID;

	FileID = (uint16_t) ((CCPrivateFile[8*FileNumber+2]<<8) | CCPrivateFile[8*FileNumber+3]);
	
  if(OpenNDEFSession(FileID, ASK_FOR_SESSION) == SUCCESS)
	{
		status = WritePrivateData(offset, size, pData, pWritePassword);
		CloseNDEFSession(FileID);
	}
	
	return status;
}


/**
  * @brief  This fonction enable read only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to enable read only mode
  * @retval SUCCESS : M24SR access is now forbidden in write mode
	* @retval ERROR : operation does not complete   
  */
uint16_t STProprietary_EnableReadOnly(uint8_t* pCurrentWritePassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = EnableReadOnly( pCurrentWritePassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction disable read only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to disable read only mode
  * @retval SUCCESS : M24SR write access is now allowed 
	* @retval ERROR : operation does not complete   
  */
uint16_t STProprietary_DisableReadOnly(uint8_t* pCurrentWritePassword )
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = DisableReadOnly( pCurrentWritePassword );
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction enable write only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to enable write only mode
  * @retval SUCCESS : M24SR access is now forbidden in read mode
	* @retval ERROR : operation does not complete   
  */
uint16_t STProprietary_EnableWriteOnly(uint8_t* pCurrentWritePassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = EnableWriteOnly( pCurrentWritePassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction disable write only mode
	* @param	pCurrentWritePassword : Write password is needed to have right to disable write only mode
  * @retval SUCCESS : M24SR read access is now allowed 
	* @retval ERROR : operation does not complete   
  */
uint16_t STProprietary_DisableWriteOnly(uint8_t* pCurrentWritePassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = DisableWriteOnly( pCurrentWritePassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}	

/**
  * @brief  This function configure GPO purpose for RF or I2C session
	* @param	GPO_config: GPO configuration to set
	* @param	mode: select RF or I2C, GPO config to update
  * @retval Status : Status of the operation.
  */
uint16_t STProprietary_GPOConfig(uc8 GPO_config, uc8 mode)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	status = GPO_Config(GPO_config, mode);
	CloseNDEFSession(FileId);
		
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

