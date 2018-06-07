/**
  ******************************************************************************
  * @file    lib_NDEF_SMS.c
  * @author  MMY Application Team
  * @version V1.0.1
  * @date    06-December-2016
  * @brief   This file help to manage NDEF file that represent SMS.
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
#include "lib_NDEF_SMS.h"


/** @addtogroup NFC_libraries
 * 	@{
 *	@brief  <b>This is the library used to manage the content of the TAG (data)
 *          But also the specific feature of the tag, for instance
 *          password, gpo... </b>
 */


/** @addtogroup libNFC_FORUM
  * @{
	*	@brief  This part of the library manage data which follow NFC forum organisation.
  */

/**
 * @brief  This buffer contains the data send/received by TAG
 */
extern uint8_t NDEF_Buffer [NDEF_MAX_SIZE];

/** @defgroup libSMS_Private_Functions
  * @{
  */

static void NDEF_FillSMSStruct( uint8_t* pPayload, uint32_t PayloadSize, sSMSInfo *pSMSStruct);
static void NDEF_ReadURI_SMS ( sRecordInfo *pRecordStruct, sSMSInfo *pSMSStruct );

/**
  * @brief  This fonction fill SMS structure with information of NDEF message
	* @param	pPayload : pointer on the payload data of the NDEF message
	* @param	PayloadSize : number of data in the payload
	* @param	pSMSStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_FillSMSStruct( uint8_t* pPayload, uint32_t PayloadSize, sSMSInfo *pSMSStruct)
{
	uint8_t* pLastByteAdd, *pLook4Word, *pEndString ;
	char* pKeyWord;
	uint32_t SizeOfKeyWord;
	
	pKeyWord = SMS_TYPE_STRING;
	SizeOfKeyWord = SMS_TYPE_STRING_LENGTH;
	
	/* First charactere force to NULL in case not matching found */
	*pSMSStruct->PhoneNumber = 0;
	*pSMSStruct->Message = 0;
	
	/* Interresting information are stored before picture if any */
	/* Moreover picture is not used in this demonstration SW */	
	pLastByteAdd = (uint8_t*)(pPayload + PayloadSize);
		
	pLook4Word = pPayload;
	while( memcmp( pLook4Word, pKeyWord, SizeOfKeyWord) && pLook4Word<pLastByteAdd )
	{
		pLook4Word++;
	}
	
	/* Retrieve phone number */
	if( pLook4Word != pLastByteAdd)
	{	
		pLook4Word += SizeOfKeyWord;
		pEndString = pLook4Word;
		while( memcmp( pEndString, URI_FIRST_DATA_END, URI_FIRST_DATA_END_LENGTH) && pEndString<pLastByteAdd )
		{
			pEndString++;
		}
		if( pEndString != pLastByteAdd)
		{
			memcpy( pSMSStruct->PhoneNumber, pLook4Word, pEndString-pLook4Word);
			/* add end of string charactere */
			pSMSStruct->PhoneNumber[pEndString-pLook4Word] = 0;	
		}
	}	
	pEndString += URI_FIRST_DATA_END_LENGTH;
	pLook4Word = pEndString;
	
	/* check if e-mail subject is present */
	if(!memcmp( pLook4Word, MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH))
	{		
		pEndString += MESSAGE_BEGIN_STRING_LENGTH;
		/* Retrieve message */
		memcpy( pSMSStruct->Message, pEndString, PayloadSize-(pEndString-pPayload));
		/* add end of string charactere */
		pSMSStruct->Message[PayloadSize-(pEndString-pPayload)] = 0;		
	}
}

/**
  * @brief  This fonction read the SMS and store data in a structure
	* @param	pRecordStruct : Pointer on the record structure
	* @param	pSMSStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_ReadURI_SMS ( sRecordInfo *pRecordStruct, sSMSInfo *pSMSStruct )
{
	uint8_t* pPayload;
	uint32_t PayloadSize;
	
	PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
								((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
	
	/* Read record header */
	pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
	
	if( pRecordStruct->NDEF_Type == URI_SMS_TYPE)
		NDEF_FillSMSStruct(pPayload , PayloadSize, pSMSStruct);
		
}

/**
  * @}
  */

/** @defgroup libSMS_Public_Functions
  * @{
  *	@brief  This file is used to manage SMS (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve SMS information if any
	* @param	pRecordStruct : Pointer on the record structure
	* @param	pSMSStruct : pointer on the structure to fill 
  * @retval SUCCESS : SMS information from NDEF have been retrieve
	* @retval ERROR : Not able to retrieve SMS information
  */
uint16_t NDEF_ReadSMS(sRecordInfo *pRecordStruct, sSMSInfo *pSMSStruct)
{
	uint16_t status = ERROR;
	uint16_t FileId=0;
	sRecordInfo *pSPRecordStruct;	
	uint32_t PayloadSize, RecordPosition;
	uint8_t* pData;

	if( pRecordStruct->NDEF_Type == URI_SMS_TYPE )
	{	
		NDEF_ReadURI_SMS(pRecordStruct, pSMSStruct );
		status = SUCCESS;
	}
	else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE)
	{
		for (RecordPosition = 0; RecordPosition<pRecordStruct->NbOfRecordInSPPayload; RecordPosition++)
		{
			pSPRecordStruct = (sRecordInfo *)(pRecordStruct->SPRecordStructAdd[RecordPosition]);
			if(pSPRecordStruct->NDEF_Type == URI_SMS_TYPE )
			{
				NDEF_ReadURI_SMS(pSPRecordStruct, pSMSStruct );
				status = SUCCESS;
			}
			if(pSPRecordStruct->NDEF_Type == TEXT_TYPE )
			{
				PayloadSize = ((uint32_t)(pSPRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pSPRecordStruct->PayloadLength2)<<16) |
										((uint32_t)(pSPRecordStruct->PayloadLength1)<<8)  | pSPRecordStruct->PayloadLength0;
				
				/* The instruction content the UTF-8 language code that is not used here */
				pData = (uint8_t*)pSPRecordStruct->PayloadBufferAdd;
				PayloadSize -= *pData+1; /* remove not usefull data */
				pData += *pData+1; /* set pointer on usefull data */
					
				memcpy(pSMSStruct->Information, pData, PayloadSize);
				/* add end of string charactere */
				pSMSStruct->Information[PayloadSize] = 0;		
			}
		}
	}
	
	CloseNDEFSession(FileId);
	
	return status;
}

/**
  * @brief  This fonction write the NDEF file with the SMS data given in the structure
	* @param	pSMSStruct : pointer on structure that contain the SMS information
  * @retval SUCCESS : the function is succesful
	* @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteSMS ( sSMSInfo *pSMSStruct )
{
	uint16_t status = ERROR;
	uint32_t smsSize, infoSize, totalSize, Offset = 0;
	
	/* SMS is an URI but can be included in a smart poster to add text to give instruction to user for instance */
	
	/* SMS (smart poster) Record Header */
/************************************/	
/*	7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/	
/* MB   ME   CF   SR   IL    TNF    */  /* <---- CF=0, IL=0 and SR=1 TNF=1 NFC Forum Well-known type*/
/*----------------------------------*/	
/*					TYPE LENGTH 						*/
/*----------------------------------*/
/*				PAYLOAD LENGTH 3 					*/	/* <---- Used only if SR=0 */
/*----------------------------------*/
/*			  PAYLOAD LENGTH 2 					*/  /* <---- Used only if SR=0 */
/*----------------------------------*/
/*				PAYLOAD LENGTH 1 					*/  /* <---- Used only if SR=0 */
/*----------------------------------*/	
/*				PAYLOAD LENGTH 0 					*/  
/*----------------------------------*/
/*					ID LENGTH 							*/  /* <---- Not Used  */
/*----------------------------------*/
/*							TYPE 								*/
/*----------------------------------*/
/*							 ID                 */  /* <---- Not Used  */ 
/************************************/
	
	NDEF_Buffer[0] = 0;
	NDEF_Buffer[1] = 0;
	Offset = FIRST_RECORD_OFFSET;
	
	/* SMS : 1+sms:+tel+1+body=+message */
	smsSize = 1+SMS_TYPE_STRING_LENGTH+strlen(pSMSStruct->PhoneNumber)+URI_FIRST_DATA_END_LENGTH+MESSAGE_BEGIN_STRING_LENGTH+strlen(pSMSStruct->Message);
	
	/* Check if a Smart poster is needed */
	if (pSMSStruct->Information[0] != '\0')
	{
		/* Info : 1+2+info */
		infoSize = 1+ISO_ENGLISH_CODE_STRING_LENGTH+strlen(pSMSStruct->Information);
		/* Total */
		totalSize = 4+smsSize+4+infoSize;
		if (smsSize > 255) totalSize+=3; /* Normal Email size */
		if (infoSize > 255) totalSize+=3;  /* Normal Info size */
		
		/* SmartPoster header */
		if (totalSize > 255) 
		{
			NDEF_Buffer[Offset++] = 0xC1;
			NDEF_Buffer[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
			NDEF_Buffer[Offset++] = (totalSize & 0xFF000000)>>24;
			NDEF_Buffer[Offset++] = (totalSize & 0x00FF0000)>>16;
			NDEF_Buffer[Offset++] = (totalSize & 0x0000FF00)>>8;
			NDEF_Buffer[Offset++] = (totalSize & 0x000000FF);
		}
		else
		{
			NDEF_Buffer[Offset++] = 0xD1;
			NDEF_Buffer[Offset++] = SMART_POSTER_TYPE_STRING_LENGTH;
			NDEF_Buffer[Offset++] = (uint8_t)totalSize;
		}
		memcpy(&NDEF_Buffer[Offset], SMART_POSTER_TYPE_STRING, SMART_POSTER_TYPE_STRING_LENGTH);
		Offset+=SMART_POSTER_TYPE_STRING_LENGTH;
	}
	
	/* SMS header */
	NDEF_Buffer[Offset] = 0x81;
	if (smsSize < 256) NDEF_Buffer[Offset] |= 0x10; 										 // Set the SR bit
	if (pSMSStruct->Information[0] == '\0') NDEF_Buffer[Offset] |= 0x40; // Set the ME bit
	Offset++;
	
	NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
	if (smsSize > 255)
	{
		NDEF_Buffer[Offset++] = (smsSize & 0xFF000000)>>24;
		NDEF_Buffer[Offset++] = (smsSize & 0x00FF0000)>>16;
		NDEF_Buffer[Offset++] = (smsSize & 0x0000FF00)>>8;
		NDEF_Buffer[Offset++] = (smsSize & 0x000000FF);
	}
	else
	{
		NDEF_Buffer[Offset++] = (uint8_t)smsSize;
	}
	memcpy(&NDEF_Buffer[Offset], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
	Offset+=URI_TYPE_STRING_LENGTH;
	
	/* SMS payload */
	NDEF_Buffer[Offset++] = URI_ID_0x00;
	memcpy(&NDEF_Buffer[Offset], SMS_TYPE_STRING, SMS_TYPE_STRING_LENGTH);
	Offset += SMS_TYPE_STRING_LENGTH;
	memcpy( &NDEF_Buffer[Offset], pSMSStruct->PhoneNumber,strlen(pSMSStruct->PhoneNumber));
	Offset += strlen(pSMSStruct->PhoneNumber);
	memcpy( &NDEF_Buffer[Offset], URI_FIRST_DATA_END,URI_FIRST_DATA_END_LENGTH);
	Offset += URI_FIRST_DATA_END_LENGTH;
	
	memcpy( &NDEF_Buffer[Offset], MESSAGE_BEGIN_STRING, MESSAGE_BEGIN_STRING_LENGTH);
	Offset += MESSAGE_BEGIN_STRING_LENGTH;
	memcpy( &NDEF_Buffer[Offset], pSMSStruct->Message, strlen(pSMSStruct->Message));
	Offset += strlen(pSMSStruct->Message);
	
	/* Information header */
	if (pSMSStruct->Information[0] != '\0')
	{
		if (infoSize > 255)
		{
			NDEF_Buffer[Offset++] = 0x41;
			NDEF_Buffer[Offset++] = TEXT_TYPE_STRING_LENGTH;
			NDEF_Buffer[Offset++] = (infoSize & 0xFF000000)>>24;
			NDEF_Buffer[Offset++] = (infoSize & 0x00FF0000)>>16;
			NDEF_Buffer[Offset++] = (infoSize & 0x0000FF00)>>8;
			NDEF_Buffer[Offset++] = (infoSize & 0x000000FF);
		}
		else
		{
			NDEF_Buffer[Offset++] = 0x51;
			NDEF_Buffer[Offset++] = TEXT_TYPE_STRING_LENGTH;
			NDEF_Buffer[Offset++] = (uint8_t)infoSize;
		}
		
		memcpy(&NDEF_Buffer[Offset], TEXT_TYPE_STRING, TEXT_TYPE_STRING_LENGTH);
		Offset+=TEXT_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset++] = ISO_ENGLISH_CODE_STRING_LENGTH; /* UTF-8 with x byte language code */
		memcpy(&NDEF_Buffer[Offset], ISO_ENGLISH_CODE_STRING, ISO_ENGLISH_CODE_STRING_LENGTH);
		Offset+=ISO_ENGLISH_CODE_STRING_LENGTH;
	
		/* Information payload */
		memcpy( &NDEF_Buffer[Offset], pSMSStruct->Information,strlen(pSMSStruct->Information));
		Offset += strlen(pSMSStruct->Information);
	}
	
	/* Write NDEF */
	status = WriteData ( 0x00 , Offset , NDEF_Buffer);

	/* Write NDEF size to complete*/
	if( status == NDEF_ACTION_COMPLETED)
	{
		Offset -= 2; /* Must not count the 2 byte that represent the NDEF size */
		NDEF_Buffer[0] = (Offset & 0xFF00)>>8;
		NDEF_Buffer[1] = (Offset & 0x00FF);
	
		status = WriteData ( 0x00 , 2 , NDEF_Buffer);
	}
	
	if( status == NDEF_ACTION_COMPLETED)
		return SUCCESS;
	else
		return ERROR;
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


