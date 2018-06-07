/**
  ******************************************************************************
  * @file    lib_NDEF_Geo.c
  * @author  MMY Application Team
  * @version V1.0.1
  * @date    06-December-2016
  * @brief   This file help to manage NDEF file that represent geolocation.
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
#include "lib_NDEF_Geo.h"

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

/** @defgroup libGeo_Private_Functions
  * @{
  */

static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct);
static void NDEF_ReadURI_Geo ( sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct );

/**
  * @brief  This fonction fill Geo structure with information of NDEF message
	* @param	pPayload : pointer on the payload data of the NDEF message
	* @param	PayloadSize : number of data in the payload
	* @param	pGeoStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_FillGeoStruct( uint8_t* pPayload, uint32_t PayloadSize, sGeoInfo *pGeoStruct)
{
	uint8_t* pLastByteAdd, *pLook4Word, *pEndString ;
	char* pKeyWord;
	uint32_t SizeOfKeyWord;
	
	pKeyWord = GEO_TYPE_STRING;
	SizeOfKeyWord = GEO_TYPE_STRING_LENGTH;
	
	/* First charactere force to NULL in case not matching found */
	*pGeoStruct->Latitude = 0;
	*pGeoStruct->Longitude = 0;
	
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
		while( memcmp( pEndString, URI_LATITUDE_END, URI_LATITUDE_END_LENGTH) && pEndString<pLastByteAdd )
		{
			pEndString++;
		}
		if( pEndString != pLastByteAdd)
		{
			memcpy( pGeoStruct->Latitude, pLook4Word, pEndString-pLook4Word);
			/* add end of string charactere */
			pGeoStruct->Latitude[pEndString-pLook4Word] = 0;	
		}
	}	
	pEndString += URI_LATITUDE_END_LENGTH;
	pLook4Word = pEndString;
	
	memcpy( pGeoStruct->Longitude, pEndString, PayloadSize-(pEndString-pPayload));
	/* add end of string charactere */
	pGeoStruct->Longitude[PayloadSize-(pEndString-pPayload)] = 0;		
	
}

/**
  * @brief  This fonction read the geoloccation information and store data in a structure
	* @param	pRecordStruct : Pointer on the record structure
	* @param	pGeoStruct : pointer on the structure to fill
  * @retval NONE 
  */
static void NDEF_ReadURI_Geo ( sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct )
{
	uint8_t* pPayload;
	uint32_t PayloadSize;
	
	PayloadSize = ((uint32_t)(pRecordStruct->PayloadLength3)<<24) | ((uint32_t)(pRecordStruct->PayloadLength2)<<16) |
								((uint32_t)(pRecordStruct->PayloadLength1)<<8)  | pRecordStruct->PayloadLength0;
	
	/* Read record header */
	pPayload = (uint8_t*)(pRecordStruct->PayloadBufferAdd);
	
	if( pRecordStruct->NDEF_Type == URI_GEO_TYPE)
		NDEF_FillGeoStruct(pPayload , PayloadSize, pGeoStruct);
		
}

/**
  * @}
  */

/** @defgroup libGeo_Public_Functions
  * @{
  *	@brief  This file is used to manage geolocation (stored or loaded in tag)
  */ 

/**
  * @brief  This fonction read NDEF and retrieve Geo information if any
	* @param	pRecordStruct : Pointer on the record structure
	* @param	pGeoStruct : pointer on the structure to fill 
  * @retval SUCCESS : Geolocation information from NDEF have been retrieved
	* @retval ERROR : Not able to retrieve geolocation information
  */
uint16_t NDEF_ReadGeo(sRecordInfo *pRecordStruct, sGeoInfo *pGeoStruct)
{
	uint16_t status = ERROR;
	uint16_t FileId=0;
	sRecordInfo *pSPRecordStruct;	
	uint32_t PayloadSize, RecordPosition;
	uint8_t* pData;

	if( pRecordStruct->NDEF_Type == URI_GEO_TYPE )
	{	
		NDEF_ReadURI_Geo(pRecordStruct, pGeoStruct );
		status = SUCCESS;
	}
	else if( pRecordStruct->NDEF_Type == SMARTPOSTER_TYPE)
	{
		for (RecordPosition = 0; RecordPosition<pRecordStruct->NbOfRecordInSPPayload; RecordPosition++)
		{
			pSPRecordStruct = (sRecordInfo *)(pRecordStruct->SPRecordStructAdd[RecordPosition]);
			if(pSPRecordStruct->NDEF_Type == URI_GEO_TYPE )
			{
				NDEF_ReadURI_Geo(pSPRecordStruct, pGeoStruct );
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
					
				memcpy(pGeoStruct->Information, pData, PayloadSize);
				/* add end of string charactere */
				pGeoStruct->Information[PayloadSize] = 0;		
			}
		}
	}
	
	CloseNDEFSession(FileId);
	
	return status;
}

/**
  * @brief  This fonction write the NDEF file with the geolocation data given in the structure
	* @param	pGeoStruct : pointer on structure that contain the geolocation information
  * @retval SUCCESS : the function is succesful
	* @retval ERROR : Not able to store NDEF file inside tag.
  */
uint16_t NDEF_WriteGeo ( sGeoInfo *pGeoStruct )
{
	uint16_t status = ERROR;
	uint16_t DataSize;
	uint32_t PayloadSP = 0, PayloadURI = 0, PayloadText = 0, Offset = 0;
	
	/* SMS is an URI but can be included in a smart poster to add text to give instruction to user for instance */
	
	/* SMS (smart poster) Record Header */
/************************************/	
/*	7 |  6 |  5 |  4 |  3 | 2  1  0 */
/*----------------------------------*/	
/* MB   ME   CF   SR   IL    TNF    */  /* <---- CF=0, IL=0 and SR=1 TNF=1 NFC Forum Well-known type*/
/*----------------------------------*/	
/*					TYPE LENGTH 						*/
/*----------------------------------*/
/*				PAYLOAD LENGTH 3 					*/	/* <---- Not Used  */
/*----------------------------------*/
/*			  PAYLOAD LENGTH 2 					*/  /* <---- Not Used  */
/*----------------------------------*/
/*				PAYLOAD LENGTH 1 					*/  /* <---- Not Used  */
/*----------------------------------*/	
/*				PAYLOAD LENGTH 0 					*/  
/*----------------------------------*/
/*					ID LENGTH 							*/  /* <---- Not Used  */
/*----------------------------------*/
/*							TYPE 								*/
/*----------------------------------*/
/*							 ID                 */  /* <---- Not Used  */ 
/************************************/
	
	/* SMS is a smart poster with URI record containing the phone number and Text record for SMS data */
	
	/* NDEF file must be written in 2 phases, first phase NDEF size is Null */
	NDEF_Buffer[NDEF_SIZE_OFFSET] = 0x00;
	NDEF_Buffer[NDEF_SIZE_OFFSET+1] = 0x00;
	
	/* we need to format it as Smart Poster */
	if( strlen(pGeoStruct->Information) != 0)
	{	
		/* fill smart poster record header */
		NDEF_Buffer[FIRST_RECORD_OFFSET] = 0xD1;   /* Record Flag */
		NDEF_Buffer[FIRST_RECORD_OFFSET+1] = SMART_POSTER_TYPE_STRING_LENGTH;
		NDEF_Buffer[FIRST_RECORD_OFFSET+2] = 0x00; /* Will be filled at the end when payload size is known */

		memcpy(&NDEF_Buffer[FIRST_RECORD_OFFSET+3], SMART_POSTER_TYPE_STRING, SMART_POSTER_TYPE_STRING_LENGTH);
	
	
		/* fill URI record header */
		Offset = FIRST_RECORD_OFFSET+3+SMART_POSTER_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset++] = 0x91;   /* Record Flag */
		NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset++] = 0x00; /* Will be filled at the end when payload size is known */

		memcpy(&NDEF_Buffer[Offset++], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
	
		NDEF_Buffer[Offset] = 0x00; /* URI identifier no abbreviation */
	  PayloadURI = 1; /* URI identifier */
	
		/* fill URI payload */
		/* "geo:latitude,longitude" */
		memcpy(&NDEF_Buffer[Offset + PayloadURI], GEO_TYPE_STRING, GEO_TYPE_STRING_LENGTH);
		PayloadURI += GEO_TYPE_STRING_LENGTH;
		memcpy( &NDEF_Buffer[Offset + PayloadURI], pGeoStruct->Latitude,strlen(pGeoStruct->Latitude));
		PayloadURI += strlen(pGeoStruct->Latitude);
		memcpy( &NDEF_Buffer[Offset + PayloadURI], URI_LATITUDE_END,URI_LATITUDE_END_LENGTH);
		PayloadURI += URI_LATITUDE_END_LENGTH;
		memcpy( &NDEF_Buffer[Offset + PayloadURI], pGeoStruct->Longitude, strlen(pGeoStruct->Longitude));
		PayloadURI += strlen(pGeoStruct->Longitude);
		
		
		NDEF_Buffer[(Offset-2)] = (uint8_t)PayloadURI;
	
		Offset = Offset + PayloadURI;
		/* fill Text record header */
		NDEF_Buffer[Offset++] = 0x51;   /* Record Flag */
		NDEF_Buffer[Offset++] = TEXT_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset++] = 0x00; /* Will be filled at the end when payload size is known */

		memcpy(&NDEF_Buffer[Offset], TEXT_TYPE_STRING, TEXT_TYPE_STRING_LENGTH);
		Offset += TEXT_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset] = strlen(ISO_ENGLISH_CODE_STRING); /* UTF-8 with x byte language code */
		PayloadText += 1; /* byte to specify number of byte to code language */
		memcpy(&NDEF_Buffer[Offset+PayloadText], ISO_ENGLISH_CODE_STRING, strlen(ISO_ENGLISH_CODE_STRING));
		PayloadText += strlen(ISO_ENGLISH_CODE_STRING);
	
	
		/* fill Text payload */
		/* "instruction to follow" */
		memcpy( &NDEF_Buffer[Offset+PayloadText], pGeoStruct->Information,strlen(pGeoStruct->Information));
		PayloadText += strlen(pGeoStruct->Information);
	
		NDEF_Buffer[(Offset-2)] = PayloadText;
	
	
		DataSize = Offset + PayloadText; /* Must not count the 2 byte that represent the NDEF size */
		PayloadSP = DataSize - (FIRST_RECORD_OFFSET+3+SMART_POSTER_TYPE_STRING_LENGTH);
	
	  /* SR payload must be less than 0xFF bytes */
		if(PayloadSP < 0x100)
			NDEF_Buffer[FIRST_RECORD_OFFSET+2] = (uint8_t)PayloadSP;
		else
			return status;
	}
	/* it's a simple URI */
	else
	{
		/* fill URI record header */
		Offset = FIRST_RECORD_OFFSET;
		NDEF_Buffer[Offset++] = 0xD1;   /* Record Flag */
		NDEF_Buffer[Offset++] = URI_TYPE_STRING_LENGTH;
		NDEF_Buffer[Offset++] = 0x00; /* Will be filled at the end when payload size is known */

		memcpy(&NDEF_Buffer[Offset++], URI_TYPE_STRING, URI_TYPE_STRING_LENGTH);
	
		NDEF_Buffer[Offset] = 0x00; /* URI identifier no abbreviation */
	  PayloadURI = 1; /* URI identifier */
		
		/* fill URI payload */
		/* "geo:latitude,longitude" */
		memcpy(&NDEF_Buffer[Offset + PayloadURI], GEO_TYPE_STRING, GEO_TYPE_STRING_LENGTH);
		PayloadURI += GEO_TYPE_STRING_LENGTH;
		memcpy( &NDEF_Buffer[Offset + PayloadURI], pGeoStruct->Latitude,strlen(pGeoStruct->Latitude));
		PayloadURI += strlen(pGeoStruct->Latitude);
		memcpy( &NDEF_Buffer[Offset + PayloadURI], URI_LATITUDE_END,URI_LATITUDE_END_LENGTH);
		PayloadURI += URI_LATITUDE_END_LENGTH;
		memcpy( &NDEF_Buffer[Offset + PayloadURI], pGeoStruct->Longitude, strlen(pGeoStruct->Longitude));
		PayloadURI += strlen(pGeoStruct->Longitude);
	
	
		NDEF_Buffer[(Offset-2)] = (uint8_t)PayloadURI;
	
		DataSize = Offset + PayloadURI;
	}		
	
	/* Write NDEF */
	status = WriteData ( 0x00 , DataSize , NDEF_Buffer);
	
	/* Write NDEF size to complete*/
	if( status == NDEF_ACTION_COMPLETED)
	{
		DataSize -= 2; /* Must not count the 2 byte that represent the NDEF size */
		NDEF_Buffer[0] = (DataSize & 0xFF00)>>8;
		NDEF_Buffer[1] = (DataSize & 0x00FF);
	
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


