/**
  ******************************************************************************
  * @file    lib_TagType4.h
  * @author  MMY Application Team
  * @version V1.0.1
  * @date    06-December-2016
  * @brief   This file help to manage TagType4.
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
#ifndef __LIB_TAGTYPE4_H
#define __LIB_TAGTYPE4_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_URI.h"
#include "lib_NDEF_SMS.h"
#include "lib_NDEF_Email.h"
#include "lib_NDEF_Vcard.h"
#include "lib_NDEF_Geo.h"
#include "lib_NDEF_MyApp.h"	 
#include "lib_NDEF_AAR.h"
  
typedef struct
{
	uint16_t NumberCCByte;
	uint8_t Version;
	uint16_t MaxReadByte;
	uint16_t MaxWriteByte;
	uint8_t TField;
	uint8_t LField;
	uint16_t FileID;
	uint16_t NDEFFileMaxSize;
	uint8_t ReadAccess;
	uint8_t WriteAccess;
}sCCFileInfo;

typedef enum 
{
	UNKNOWN_TAG = 0,
  VCARD_TAG,
	URI_TAG,
	SMS_TAG,
	GEO_TAG,
	EMAIL_TAG,
	TEXT_TAG,
	BT_TAG,
	WIFI_TAG
} Tag_TypeDef;


uint16_t TT4_Init( void );
uint16_t TT4_ReadNDEF(uint8_t *pNDEF);
uint16_t TT4_WriteNDEF(uint8_t *pNDEF);
uint16_t TT4_ReadURI(sURI_Info *pURI);
uint16_t TT4_WriteURI(sURI_Info *pURI);
uint16_t TT4_ReadSMS(sSMSInfo *pSMS);
uint16_t TT4_WriteSMS(sSMSInfo *pSMS);
uint16_t TT4_ReadEmail(sEmailInfo *pEmailStruct);
uint16_t TT4_WriteEmail(sEmailInfo *pEmailStruct);
uint16_t TT4_ReadVcard(sVcardInfo *pVcard);
uint16_t TT4_WriteVcard(sVcardInfo *pVcard);
uint16_t TT4_ReadGeo(sGeoInfo *pGeo);
uint16_t TT4_WriteGeo(sGeoInfo *pGeo);
uint16_t TT4_ReadMyApp(sMyAppInfo *pMyAppStruct);
uint16_t TT4_WriteMyApp(sMyAppInfo *pMyAppStruct);
uint16_t TT4_AddAAR(sAARInfo *pAAR);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_TAGTYPE4_H */


/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
