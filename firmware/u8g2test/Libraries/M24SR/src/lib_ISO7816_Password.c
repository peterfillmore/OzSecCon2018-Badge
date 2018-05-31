/**
  ******************************************************************************
  * @file    lib_ISO7816_Password.c
  * @author  MMY Application Team
  * @version V1.0.1
  * @date    06-December-2016
  * @brief   This file help to manage password in the ISO7816 context.
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
#include "lib_ISO7816_Password.h"

/** @addtogroup NFC_libraries
 * 	@{
 *	@brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @addtogroup libISO7816
  * @{
	*	@brief  This part of the library help to manage TAG with ISO7816 feature.
  */
	
	
/** @defgroup libPassword_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup libPassword_Public_Functions
  * @{
	*	@brief  This file is used to manage password with NDEF file
  */ 

/**
  * @brief  This fonction activate the need of a password for next read access
	* @param	pCurrentWritePassword : Write password is needed to have the right to enable Read Password
	* @param	pNewPassword : The password that will be requiered for next read access
  * @retval SUCCESS : Read password is activated
	* @retval ERROR : operation does not complete  
  */
uint16_t ISO7816_EnableReadPwd(uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = EnableReadPassword( pCurrentWritePassword, pNewPassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction desactivate the need of a password for next read access
	* @param	pCurrentWritePassword : Write password is needed to have the right to disable Read Password
  * @retval SUCCESS : Read password is desactivated
	* @retval ERROR : operation does not complete  
  */
uint16_t ISO7816_DisableReadPwd(uint8_t* pCurrentWritePassword )
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = DisableReadPassword( pCurrentWritePassword );
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction activate the need of a password for next write access
	* @param	pCurrentWritePassword : Write password must be prensented to have the right to modify write Password
	* @param	pNewPassword : The password that will be requiered for next write access
  * @retval SUCCESS : Write password is activated
	* @retval ERROR : operation does not complete   
  */
uint16_t ISO7816_EnableWritePwd(uint8_t* pCurrentWritePassword, uint8_t* pNewPassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = EnableWritePassword( pCurrentWritePassword, pNewPassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}

/**
  * @brief  This fonction desactivate the need of a password for next write access
	* @param	pCurrentWritePassword : Write password must be prensented to have the right to disable it
  * @retval SUCCESS : Write password is desactivated
	* @retval ERROR : operation does not complete   
  */
uint16_t ISO7816_DisableWritePwd(uint8_t* pCurrentWritePassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, ASK_FOR_SESSION);	
	
	status = DisableWritePassword( pCurrentWritePassword);
	
	CloseNDEFSession(FileId);
		
	return status;
}	

/**
  * @brief  This fonction desactivate the need of read and write password for next access
	* @param	pSuperUserPassword : I2C super user password to overwrite read and write password
  * @retval SUCCESS : M24SR access is now free (no password needed)
	* @retval ERROR : operation does not complete   
  */
uint16_t ISO7816_DisableAllPassword(uint8_t* pSuperUserPassword)
{
	uint16_t status;
	uint16_t FileId;
	
	/* Before using Verify command NDEF file must be selected */
	GetNDEFFileId(&FileId);
	OpenNDEFSession(FileId, TAKE_SESSION);	
	
	status = DisableAllPassword( pSuperUserPassword);
	
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

