/**
  ******************************************************************************
  * @file    lib_STProprietary_feature.h
  * @author  MMY Application Team
  * @version V2.0.0
  * @date    06-December-2016
  * @brief   This file help to manage some special feature embedded by target
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_STPROPRIETARY_FEATURE_H
#define __LIB_STPROPRIETARY_FEATURE_H

/* Includes ------------------------------------------------------------------*/ 
#include "lib_NDEF.h"	 

/* Number of file to create in tag */
/* By default tag have 1 file, it's possible to manage more files with M24SR */
#define SYS_NUMBER_OF_MANAGED_FILE			 	2
#define SYS_NUMBER_OF_PRIVATE_FILE			 	(SYS_NUMBER_OF_MANAGED_FILE-1)
#define SYS_FORMAT_CONFIG							  	SYS_NUMBER_OF_PRIVATE_FILE

typedef struct
{
	uint8_t TField;
	uint8_t LField;
	uint16_t FileID;
	uint16_t NDEFFileMaxSize;
	uint8_t ReadAccess;
	uint8_t WriteAccess;
}sCCPrivateFileInfo;


uint16_t STProprietary_Format(uint8_t *pNewReadPassword, uint8_t* pNewWritePassword);

uint16_t STProprietary_ReadData(uint8_t FileNumber, uint16_t offset, uint16_t size, uint8_t * pData, uint8_t *pReadPassword);
uint16_t STProprietary_WriteData(uint8_t FileNumber, uint16_t offset, uint16_t size, uint8_t * pData, uint8_t *pWritePassword);
	 
uint16_t STProprietary_EnableReadOnly(uint8_t* pCurrentWritePassword);
uint16_t STProprietary_DisableReadOnly(uint8_t* pCurrentWritePassword);
uint16_t STProprietary_EnableWriteOnly(uint8_t* pCurrentWritePassword);
uint16_t STProprietary_DisableWriteOnly(uint8_t* pCurrentWritePassword);
uint16_t STProprietary_GPOConfig(uc8 GPO_RFconfig, uc8 mode);

#endif /* __LIB_STPROPRIETARY_FEATURE_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
