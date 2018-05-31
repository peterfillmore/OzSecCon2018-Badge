/**
  ******************************************************************************
  * @file    lib_ISO7816_Password.h
  * @author  MMY Application Team
  * @version V1.0.1
  * @date    06-December-2016
  * @brief   This file help to manage password.
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
#ifndef __LIB_ISO7816_PASSWORD_H
#define __LIB_ISO7816_PASSWORD_H

/* Includes ------------------------------------------------------------------*/ 
#include "lib_NDEF.h"	 
	 
uint16_t ISO7816_EnableReadPwd(uint8_t* pCurrentWritePassword, uint8_t* pNewPassword);
uint16_t ISO7816_DisableReadPwd(uint8_t* pCurrentWritePassword);
uint16_t ISO7816_EnableWritePwd(uint8_t* pCurrentWritePassword, uint8_t* pNewPassword);
uint16_t ISO7816_DisableWritePwd(uint8_t* pCurrentWritePassword);
uint16_t ISO7816_DisableAllPassword(uint8_t* pSuperUserPassword);
#endif /* __LIB_ISO7816_PASSWORD_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
