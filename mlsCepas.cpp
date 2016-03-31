//#include "stdafx.h"
#include "mlsCepas.h"
#include "mlsCepasTest.h"
#include "mlsEzSAM.h"
#include "mlsCrypto.h"
//#include <mbstring.h>
#include "mlsDebug.h"
#include "mlsLtaSAM.h"
#include "mlsNetSAM.h"
#include "mlsDateTime.h"

#undef MLS_TEST

/**************************************************************************/
/****************************** CSC validation rules ********************/
#define PURSE_ENABLE_STATUS 0x01
#define AUTOLOAD_STATUS 0x02
//const UINT16 crc16_tab[] = {0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
//							0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
//							0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
//							0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
//							0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
//							0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
//							0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
//							0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
//							0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
//							0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
//							0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
//							0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
//							0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
//							0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
//							0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
//							0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
//							0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
//							0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
//							0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
//							0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
//							0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
//							0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
//							0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
//							0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
//							0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
//							0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
//							0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
//							0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
//							0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
//							0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
//							0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
//							0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};
//
//
//UINT16 LIB_Crc16(void *pStart,int iLen)
//{
//	UINT8 *pS = (UINT8 *)pStart;
//	UINT16 crc16;
//	UINT16 polinomial = 0xB408;
//	while(iLen--)
//	{
//		crc16 = crc16_tab[(*pS++^(UINT8)crc16)]^(crc16>>8);
//	}
//	return crc16;
//}

int checkNegativeNum(UINT8 *pNumber,int iLen)
{
	for(int i = 0;i < iLen-1;i++)
	{
		if(pNumber[i] =='-')
			return 1;
	}
	return 0;
}
void setNegativeNum(UINT8 *pNumber,int iLen)
{
	for(int i = 1;i < iLen;i++)
	{
		if(pNumber[i] !='0')
		{
			pNumber[i- 1] = '-';
			break;
		}
	}
}
void removeNegativeTypeAndFirstNumZero(UINT8 *pNumber,int &iLen)
{
	//remove negative type
	int i = 0;
	for(i = 0 ; i < iLen - 1;i++)
	{
		if(pNumber[i] == '-')
			pNumber[i] = '0';
	}
	//remove all first number 0
	for(i = 0 ; i < iLen - 1;i++)
	{
		if(pNumber[i] != '0')
			break;
	}
	
	if(i > 0)
	{
		for(int j = 0 ;j < (iLen - i);j++)
		{
			pNumber[j] = pNumber[j+i];
		}
		iLen = iLen - i;
	}
}
int compareTwoNumber(UINT8 *pFirstNum,int iFirstNumLen,UINT8 *pSecondNum,int iSecondNumLen)
{
	
	if(iFirstNumLen > iSecondNumLen)
		return 1;
	else
	{
		if(iFirstNumLen < iSecondNumLen)
			return -1;
		else{
			for(int i = 0 ;i < iFirstNumLen;i++)
				if(pFirstNum[i]>pSecondNum[i])
					return 1;
				else if(pFirstNum[i]<pSecondNum[i])
					return -1;
		}
	}
	return 0;
}
void convertNumbertoArrChar(int iNumber,UINT8 *pBuffer,int Length)
{
	CHAR arrData[10];
	itoa(iNumber,arrData,10);
	int iLen = elementOfNumber(iNumber);
	memcpy((&pBuffer[0])+(Length-iLen),arrData,iLen);
	if(iLen < Length)
		memset(pBuffer,'0',Length - iLen);
}
void mlsConvertToIntKeepNegative(int *iData,unsigned char*pArrBigEndian,int begin)
{
	/*todo:mls@dev01 to review*/
	unsigned char temp;
	int iThree = pArrBigEndian[begin+0];
	int flag =iThree &0x7E;// check negative number

	int iSecond = pArrBigEndian[begin+1];
	int iFirst = pArrBigEndian[begin +2];
	if(flag != 0)
	{
		iThree = ~iThree;
		iThree = iThree &0x000000FF;
		iSecond = ~iSecond;
		iSecond = iSecond &0x000000FF;
		iFirst = ~iFirst;
		iFirst = iFirst &0x000000FF;
	}
	iThree = iThree << 16;
	iSecond = iSecond <<8;
	*iData = iThree|iSecond|iFirst;
	if(flag != 0)
	{
		(*iData) = ((*iData) +1)*(-1);
	}
}
int mlsConvertBigEndianToInt(unsigned char*pArrBigEndian,int iLength)
{
	int iNumber = 0;
	if(iLength > 4)
		return 0;
	for(int i = 0; i < iLength;i++)
	{
		*((unsigned char *)&iNumber + i)=pArrBigEndian[iLength - 1  - i];
	}
	return iNumber;
}
int elementOfNumber(int iNumber)
{
	
	int iTmp = 1;
	int iCount = 0;
	if(iNumber < 0)
	{
		iCount = 1;
		iNumber = -iNumber;
	}
	while(1)
	{
		if(iNumber < iTmp)
			break;
		iTmp = iTmp *10;
		iCount++;
	}
	return iCount;
}
void subPositiveBigNumber(UINT8 *pFirstNum,int iFirstNumLen,UINT8 *pSecondNum,int iSecondNumLen,UINT8 *pResult,int iResLen)
{
	int iComFlag = compareTwoNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen);
	UINT8 *pBig = NULL,*pSmall = NULL;
	int iBigLen = 0,iSmallLen = 0;
	int i = 0;
	memset(pResult,'0',iResLen);
	if(iComFlag == 1)
	{
		pBig = pFirstNum;
		iBigLen = iFirstNumLen;
		pSmall = pSecondNum;
		iSmallLen = iSecondNumLen;
	}else if(iComFlag == -1)
	{
		pBig = pSecondNum;
		iBigLen = iSecondNumLen;
		pSmall = pFirstNum;
		iSmallLen = iFirstNumLen;
	}
	int iPlus = 0;
	if(iComFlag != 0)
	{
		for(i = 0;i < iSmallLen ;i++)
		{
			if(pBig[iBigLen - i - 1] < (pSmall[iSmallLen - i - 1]+ iPlus))
			{
				pResult[iResLen - i - 1] = (pBig[iBigLen - i - 1] + 10  - pSmall[iSmallLen - i - 1] - iPlus) + 0x30;
				iPlus = 1;
			}
			else
			{
				pResult[iResLen - i - 1] = (pBig[iBigLen - i - 1]  - pSmall[iSmallLen - i - 1] - iPlus) + 0x30;
				iPlus = 0;
			}
		}
		if(i < iBigLen)
		{
			//pResult[iResLen - i - 1] = pBig[iBigLen - i - 1] - iPlus;
			for(;i < iBigLen;i++)
			{
				if(pBig[iBigLen - i - 1] - iPlus - 0x30 < 0)
				{
					pResult[iResLen - i - 1] = pBig[iBigLen - i - 1]  + 10 - iPlus;
					iPlus = 1;
				}else{
					pResult[iResLen - i - 1] = pBig[iBigLen - i - 1] - iPlus;
					iPlus = 0;
				}
			}
		}
	}
}
void sumPositiveBigNumber(UINT8 *pFirstNum,int iFirstNumLen,UINT8 *pSecondNum,int iSecondNumLen,UINT8 *pResult,int iResLen)
{
	int iComFlag = compareTwoNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen);
	UINT8 *pBig = NULL,*pSmall = NULL;
	int iBigLen = 0,iSmallLen = 0;
	int i = 0;
	memset(pResult,'0',iResLen);
	if(iComFlag == 1)
	{
		pBig = pFirstNum;
		iBigLen = iFirstNumLen;
		pSmall = pSecondNum;
		iSmallLen = iSecondNumLen;
	}else
	{
		pBig = pSecondNum;
		iBigLen = iSecondNumLen;
		pSmall = pFirstNum;
		iSmallLen = iFirstNumLen;
	}
	int iPlus = 0;
		for(i = 0;i < iSmallLen ;i++)
		{
			if(pBig[iBigLen - i - 1] + pSmall[iSmallLen - i - 1]+ iPlus - 0x30 - 0x30 >= 10)
			{
				pResult[iResLen - i - 1] = pBig[iBigLen - i - 1] + pSmall[iSmallLen - i - 1] + iPlus - 10 - 0x30;
				iPlus = 1;
			}
			else
			{
				pResult[iResLen - i - 1] = pBig[iBigLen - i - 1]  + pSmall[iSmallLen - i - 1] +iPlus - 0x30;
				iPlus = 0;
			}
		}
		if(i < iBigLen)
		{
			for(;i < iBigLen;i++)
			{
				if(pBig[iBigLen - i - 1] + iPlus - 0x30 >= 10)
				{
					pResult[iResLen - i - 1] = pBig[iBigLen - i - 1] + iPlus - 10;
					iPlus = 1;
				}else{
					pResult[iResLen - i - 1] = pBig[iBigLen - i - 1] + iPlus;
					iPlus = 0;
				}
			}
		}
		if(i < iResLen)
		{
			pResult[iResLen - i - 1] = iPlus + 0x30;
		}

}
void sumBigNumber(UINT8 *pFirstNum,int iFirstNumLen,UINT8 *pSecondNum,int iSecondNumLen,UINT8 *pResult,int iResLen)
{
	int NegFirstFlag = checkNegativeNum(pFirstNum,iFirstNumLen);
	int NegSecondFlag = checkNegativeNum(pSecondNum,iSecondNumLen);

	removeNegativeTypeAndFirstNumZero(pFirstNum,iFirstNumLen);
	removeNegativeTypeAndFirstNumZero(pSecondNum,iSecondNumLen);

	if((NegFirstFlag == 0 && NegSecondFlag == 0)||(NegFirstFlag == 1 && NegSecondFlag == 1))
	{
		sumPositiveBigNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen,pResult,iResLen);
		if(NegFirstFlag == 1)
			setNegativeNum(pResult,iResLen);
	}else{
		subPositiveBigNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen,pResult,iResLen);
		if(NegFirstFlag == 1 && compareTwoNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen) == 1)
			setNegativeNum(pResult,iResLen);
		if(NegSecondFlag == 1 && compareTwoNumber(pFirstNum,iFirstNumLen,pSecondNum,iSecondNumLen) == -1)
			setNegativeNum(pResult,iResLen);
	}
}
void sumArrayCharAndInt(UINT8 *pFirstNum,int iFirstLen,int iSecondNum)
{
	int iNumberLen = elementOfNumber(iSecondNum);
	UINT8 arSecondNum[100];
	itoa(iSecondNum,(char *)arSecondNum,10);
	UINT8 arResult[10];
	sumBigNumber(pFirstNum,iFirstLen,arSecondNum,iNumberLen,arResult,10);
	memcpy(pFirstNum,arResult +(10 - iFirstLen),iFirstLen);
}
BOOL CSCCheckPurseExpiry( unsigned char* arrCreationDate,unsigned char* arrExpiryDate,int currentDay)
{
	BOOL retVal = TRUE;
	unsigned short creationDate,expiryDate;
	creationDate = (unsigned short)((arrCreationDate[0]<<8)|arrCreationDate[1]);
	expiryDate = (unsigned short)((arrExpiryDate[0]<<8)|arrExpiryDate[1]);
	if ((currentDay<=creationDate)||(currentDay>=expiryDate)) retVal = FALSE;
	return retVal;
}

BOOL CSCCheckPurseStatus(unsigned char PurseStatusByte,unsigned char Statuses)
{
	BOOL retVal = TRUE;

	if ((PurseStatusByte&Statuses)==0) retVal = FALSE;

	return retVal;
}
BOOL CSCCheckFund(unsigned char* arrPurseBalance, int TransactionAmount)
{
	BOOL retVal = TRUE;
	int PurseBalance;
	PurseBalance = (int)((arrPurseBalance[0]<<16)|
						 (arrPurseBalance[1]<<8)|
						  arrPurseBalance[2]);
	if (PurseBalance<TransactionAmount) retVal = FALSE;

	return retVal;
}
BOOL CSCCheckFundAutoLoad(unsigned char* arrPurseBalance,unsigned char* arrAutoloadAmount, int PurseDeduction)
{
	BOOL retVal = TRUE;
	int PurseBalance,AutoloadAmount;
	PurseBalance = (int)((arrPurseBalance[0]<<16)|
						 (arrPurseBalance[1]<<8)|
						  arrPurseBalance[2]);
	AutoloadAmount = (int)((arrAutoloadAmount[0]<<16)|
		    			   (arrAutoloadAmount[1]<<8)|
							arrAutoloadAmount[2]);

	if ((PurseBalance+AutoloadAmount)<PurseDeduction) retVal = FALSE;

	return retVal;
}


SC_CEPAS_t CSCCheckCardIssuer(unsigned char* CAN)
{
	SC_CEPAS_t retVal = unknowSC;
	unsigned short IssuerID;
	IssuerID = (unsigned short)((CAN[0]<<8) | CAN[1]);

	if ((0x1000<=IssuerID)&&(IssuerID<=0x1010)) return EzLinkSC;
	if ((0x8000<=IssuerID)&&(IssuerID<=0x8010)) return ConcessionSC;
	//TODO check Net SC

	return retVal;
}
BOOL CSCCheckCANDigit(unsigned char* CAN)
{
	BOOL retVal = TRUE;
	unsigned char weightate[15] = {2,9,8,7,6,5,4,3,2,9,8,7,6,5,4};
	unsigned char i;
	unsigned short checkDigit=0;
	for (i = 0;i<7;i++)
	{
		checkDigit += (CAN[i]>>4)*weightate[i*2] + (CAN[i]&0x0F)*weightate[i*2+1] ;
	}

	checkDigit += (CAN[7]>>4)*weightate[14];
	checkDigit = checkDigit%11;
	if(checkDigit != (CAN[7]&0x0F)) retVal = FALSE;

	return retVal;
}
BOOL CSCCheckBlackList(UInt8* arrIssurespecificData)
{
	BOOL retVal = TRUE;
	UInt8 badDebtCounter;
	//TODO
	//search in blacklist
	//read the blacklist reason code
	//get bad debt counter from EZL&TL issuer specific data
	badDebtCounter = (arrIssurespecificData[1])&0x03;
	//check bad Debt counter

	return retVal;
}
BOOL CSCCheckAutoloadCategory(UInt8* arrIssurespecificData)
{
	BOOL retVal = TRUE;
	UInt8 AutoloadCategory;

	//get Auto load Cattegory from EZL&TL Issuer specific data
	AutoloadCategory = (arrIssurespecificData[2])&0x7F;

	if((AutoloadCategory<0)||(3<AutoloadCategory))
	{
		retVal = FALSE;
	}
	return retVal;
}
BOOL CSCCheckRefundStatus(UInt8* arrIssurespecificData)
{
	BOOL retVal = TRUE;
	UInt8 CardRefundStatus;

	//get Card refund status from EZL&TL issuer specific data
	CardRefundStatus  = (arrIssurespecificData[4]>>6)&0x03;

	if (CardRefundStatus != 0)
	{
		retVal = FALSE;
	}

	return retVal;
}
/*
0 : None
1 : Primary school
2 : Secondary School
3 : Tertiary Students
4 : Senior Citizens
5 : National Servicemen
*/
BOOL CSCCheckCardProfileType(UInt8* arrIssurespecificData, UInt8 ProfileType)
{
	BOOL retVal = TRUE;
	UInt8 CardUserCategory;

	//get Card refund status from EZL&TL issuer specific data
	CardUserCategory = (arrIssurespecificData[3]>>4)&0x0F;

	if (CardUserCategory != ProfileType){
		retVal = FALSE;
	}

	return retVal;
}

int CalculateDateTime(SYSTEMTIME Time,int iBeginYear)
{
	unsigned char DaysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	int days = 0;
	int daytime;

	for (i = iBeginYear; i < Time.wYear;i++)
	{
		if (((i%100 !=0)&&(i%4 == 0))||(i%400==0)) days +=366;
		else days += 365;
	}

	for (i = 1; i< Time.wMonth;i++)
	{
		days += DaysInMonth[i-1];
	}
	if ( (((Time.wYear%100 != 0)&&(Time.wYear%4==0))||(Time.wYear%400==0)) && (Time.wMonth>2))
	{
		days++;
	}

	days += Time.wDay; 

	days--; //count from 0h:1s 1/1/1995

	daytime = days*86400 + Time.wHour*3600 + Time.wMinute*60 + Time.wSecond;

	return daytime;
}
int CalculateDate(SYSTEMTIME Time,int iBeginYear)
{
	unsigned char DaysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	int days = 0;
	for (i = iBeginYear; i < Time.wYear;i++)
	{
		if (((i%100 !=0)&&(i%4 == 0))||(i%400==0)) days +=366;
		else days += 365;
	}

	for (i = 1; i< Time.wMonth;i++)
	{
		days += DaysInMonth[i-1];
	}
	if ( (((Time.wYear%100 != 0)&&(Time.wYear%4==0))||(Time.wYear%400==0)) && (Time.wMonth>2))
	{
		days++;
	}

	days += Time.wDay; 

	days--; //count from 0h:1s 1/1/1995

	

	return days;
}
/**************************************************************************/

#if defined(MLS_TEST)
unsigned short mlsCepasDebit(int handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char ucDebitOption,
							 unsigned char arrTransactionUserData[8],unsigned char arrDebitKey[16],
							 unsigned char arrSignedKey[16],KeyIndex DebitKey,KeyIndex SigningKey,int iAmount)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrDebitCryptogram[24];
	unsigned char arrDebitReceiveData[24];
	unsigned char arrDebitSessionKey[16];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	FILE *hFile;
	int i = 0;
	
	//--------------------------------------------------------------------------------------
	memcpy(abRamdomPICC,arrTerminalRan,8);
	memcpy(abRamdomPICC+8,arrCardRand,8);
	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrDiversifiedDebitKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abRamdomPICC,arrDebitSessionKey,16);
	//-------------------------------------------------------------------------------------------
	//--------calculate Debit Cryptogram
	memset(arrDebitCryptogram,0,24);
	memcpy(arrDebitCryptogram,arrTRP,4); // receive TRP

	//memcpy(arrDebitCryptogram+4,arrCRC,2); // receive CRC
	memcpy(arrDebitCryptogram+4,arrDebitRecordCRC,2);

	arrDebitCryptogram[6] = arrSkfSKn[0]; //receive SKf
	arrDebitCryptogram[7] = arrSkfSKn[1]; //receive SKn
	//transaction header;
	memcpy(arrDebitCryptogram+8,arrTransactionHeader,8);

	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrDebitSessionKey,16);

	mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitCryptogram,16);
	hFile = fopen("logfile.txt","a");

	if(memcmp(arrDebitCryptogramTest,arrDebitCryptogram,16)==0)
	{
		if(hFile != NULL)
			fprintf(hFile,"Create Cryptogram success\n");
	}else
		if(hFile !=NULL)
			fprintf(hFile,"Create Cryptogram error\n");
	//--------------------------------------------------------------------------------------
	//-------Debit receipt cryptogram test vector
	memset(arrDebitCryptogram,0,24);
	memcpy(arrDebitCryptogram,arrPurseBalance,3);
	memcpy(arrDebitCryptogram+3,arrSignedCertificate,5);
	memcpy(arrDebitCryptogram+8,arrSignedCertificate,8);
	memcpy(arrDebitCryptogram+16,arrCounterData,8);
	
	//set iv to be counter data
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitReceiveData,24);
	if(memcmp(arrDebitReceiptCryptogram,arrDebitReceiveData,24)==0)
	{
		if(hFile != NULL)
			fprintf(hFile,"Create Debit receipt cryptogram success\n");
	}else
	{
		if(hFile !=NULL)
			fprintf(hFile,"Create Debit receipt cryptogram error\n");
	}
}
unsigned short mlsCepasCredit(int handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char arrTransactionUserData[8],
							  unsigned char arrCreditKey1[16],unsigned char arrCreditKey2[16],unsigned char arrSignedKey[16],
							  KeyIndex CreditKey1,KeyIndex CreditKey2,KeyIndex SigningKey,int iAmount)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrCreditCryptogram[24];
	unsigned char arrCreditCommandData[37];
	unsigned char arrCreditReceiveData[24];
	unsigned char arrCreditSessionKey[16];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	FILE *hFile;
	int i = 0;
	//-------compute Encrypted Credit Paramater Block
	memcpy(arrTemp,arrCreditRecordCRC,2);
	arrTemp[2] = arrSkfSKn[0]; //receive SKf
	arrTemp[3] = arrSkfSKn[1]; //receive SKn
	memcpy(arrTemp+4,arrTransactionHeardCredit,4);
	//convertIntToArrayUnsignChar(iAmount,arrTemp+5,3);
	mlsCryptoSetContext(&ctx,&arrDiversifiedCreditKey2[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCreditCryptogram+4,8);

	//--------------------------------------------------------------------------------------
	memcpy(abRamdomPICC,arrCardRand,8);
	memcpy(abRamdomPICC+8,arrTerminalRan,8);
	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrDiversifiedCreditKey1[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abRamdomPICC,arrCreditSessionKey,16);
	
	//--------calculate Credit Cryptogram
	memcpy(arrCreditCryptogram,arrTRP,4); // receive TRP
	memcpy(arrCreditCryptogram+12,arrTransactionHeardCredit+4,4);

	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrCreditSessionKey,16);
	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditCryptogram,16);
	hFile = fopen("logfile.txt","a");
	if(memcmp(arrCreditCryptogramTest,arrCreditCryptogram,16)==0)
	{
		if(hFile != NULL)
			fprintf(hFile,"Create Cryptogram success\n");
	}else
		if(hFile !=NULL)
			fprintf(hFile,"Create Cryptogram error\n");
	//--------------------------------------------------------------------------------------

	//-------Debit receipt cryptogram test vector
	memset(arrCreditCryptogram,0,24);
	memcpy(arrCreditCryptogram,arrPurseBalanceCredit,3);
	memcpy(arrCreditCryptogram+3,arrSignedCertificateCredit,5);
	memcpy(arrCreditCryptogram+8,arrSignedCertificateCredit,8);
	memcpy(arrCreditCryptogram+16,arrCounterData,8);

	//set iv to be counter data
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditReceiveData,24);
	if(memcmp(arrCreditReceiptCryptogram,arrCreditReceiveData,24)==0)
	{
		if(hFile != NULL)
			fprintf(hFile,"Create Debit receipt cryptogram success\n");
	}else
	{
		if(hFile !=NULL)
			fprintf(hFile,"Create Debit receipt cryptogram error\n");
	}
}
unsigned short mlsCepasReadPurseAuthen(int handle,unsigned char ucPF,unsigned char arrKey[16],
											   unsigned char ucKeyFile,unsigned char ucKeyNumber,short Le,
											   unsigned char *pbResponseBuffer,short sBufferLength,short *sResponseBufferLength)
{
	unsigned short usStatusCode;
	unsigned char abReadPurseData[16];
	unsigned char abRamdomPICC[16];
	unsigned char arrBuffer[256];
	unsigned char arrReadPurseSessionKey[16];
	short sLength;
	unsigned char arrTemp[16];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	FILE *hFile;
	int i = 0;
	//--------------------------------------------------------------------------------------
	//---create read purse session key
	memcpy(abRamdomPICC,arrTerminalRan,8);
	memcpy(abRamdomPICC+8,arrCardRand,8);
	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrDiversifiedDebitKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abRamdomPICC,arrReadPurseSessionKey,16);
	
	//-- create read purse encrypted data
	memcpy(abReadPurseData,arrSignedCertificateCredit,8);
	memcpy(abReadPurseData+8,arrCounterData,8);
	
	memcpy(ctx.Key,arrReadPurseSessionKey,16);
	mlsCrypto_Encrypt(&ctx,abReadPurseData,abReadPurseData,16);
	hFile = fopen("logfile.txt","a");
	if(memcmp(arrEncryptedData,abReadPurseData,16)==0)
	{
		if(hFile != NULL)
			fprintf(hFile,"Read purse response: encrypted data success\n");
	}else
	{
		if(hFile !=NULL)
			fprintf(hFile,"Read purse response:encrypted data error\n");
	}
}
#endif
#ifndef MLS_TEST
unsigned short mlsCepasDebit(mlsCard_str *handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char ucDebitOption,
							 unsigned char arrTransactionUserData[8],unsigned char arrDebitKey[16],
							 unsigned char arrSignedKey[16],KeyIndex DebitKey,KeyIndex SigningKey,int iAmount)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrDebitCryptogram[24];
	unsigned char arrDebitCommandData[37];
	unsigned char arrDebitReceiveData[24];
	unsigned char arrDebitSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];
	short iDebitReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;

	//get challenge
	usStatusCode = mlsGetChallenge(handle,abRamdomPICC+8,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
		return MLSCEPAS_ERROR_GETCHALLENGE;
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Record CRC
	memset(arrDebitCryptogram,0,24);
	memcpy(arrDebitCryptogram,ucTRP,4); 
	arrDebitCryptogram[4] = ucDebitOption; 
	arrDebitCryptogram[5] = ucPf;
	arrDebitCryptogram[6] = SigningKey.ucKeyFileID;
	arrDebitCryptogram[7] = SigningKey.ucKeyNo;
	
	//transaction header;
	arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrDebitCryptogram+9,arrTemp,3);
	
	memset(arrDateTime,0,4);
	UInt32 unixTime = mlsCurrentUnixTime();
	unixTime = SWAP_32(unixTime);
	memcpy(arrDebitCryptogram+12, &unixTime,4);

	//transaction user data
	memcpy(arrDebitCryptogram+16,arrTransactionUserData,8);
	mlsComputeCrc(arrDebitCryptogram,24,arrCRC);

	//create Debit session key
	for(i = 0 ;i < 8;i++)
	{
		abRamdomPICC[i] = (unsigned char)mlsRandom();
	}
	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrDebitKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abRamdomPICC,arrDebitSessionKey,16);
	//-------------------------------------------------------------------------------------------
	//--------calculate Debit Cryptogram
	memset(arrDebitCryptogram,0,24);
	memcpy(arrDebitCryptogram,ucTRP,4); // receive TRP
	
	memcpy(arrDebitCryptogram+4,arrCRC,2); // receive CRC
	arrDebitCryptogram[6] = SigningKey.ucKeyFileID; //receive SKf
	arrDebitCryptogram[7] = SigningKey.ucKeyNo; //receive SKn
	//transaction header;
	arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrDebitCryptogram+9,arrTemp,3);

	memcpy(arrDebitCryptogram+12,arrDateTime,4);
	
	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrDebitSessionKey,16);
	
	mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitCryptogram,16);
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Command Data
	arrDebitCommandData[0] = ucPf;//Pf
	arrDebitCommandData[1] = DebitKey.ucKeyFileID;//Key file SFI
	arrDebitCommandData[2] = DebitKey.ucKeyNo;//KeyNo
	arrDebitCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	arrDebitCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	memcpy(arrDebitCommandData+5,abRamdomPICC+8,8); //random of PCC
	memcpy(arrDebitCommandData+13,arrDebitCryptogram,16);// Debit Cryptogram
	memcpy(arrDebitCryptogram+29,arrTransactionUserData,8);//Transaction User Data

	usStatusCode = ExecuteCommand(handle,MLS_DEBIT,0x00,ucDebitOption,0x25,arrDebitCommandData,0x18,arrDebitReceiveData,0x18,&iDebitReceiveLength);

	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
		return MLSCEPAS_ERROR_SENDAPDUDEBIT;
	if( iDebitReceiveLength != 0x18 )
		return MLSCEPAS_ERROR_WRONGLENGTH;

	//--------------------------------------------------------------------------------------
	//-----test signed certificate
	//decrypt counter data
	memcpy(ctx.IV,arrDebitReceiveData+8,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+16,arrCounterData,8);
	//decrypt purse balance
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData,arrPurseBalance,8);
	//decrypt certificate
	memcpy(ctx.IV,arrDebitReceiveData,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+8,arrCertificate,8);
	
	//create session key
	memset(arrDebitCryptogram,0,16);
	arrDebitCryptogram[0] = ucDebitOption; // debit option
	memcpy(arrDebitCryptogram+1,arrPurseBalance,3); // purse balance
	memcpy(arrDebitCryptogram+4,ucTRP,4);
	memcpy(arrDebitCryptogram+8,arrCounterData,8);
	memcpy(ctx.Key,arrSignedKey,16);
	mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitSessionKey,16);
	// get Transaction Header
	memset(arrTemp,0,8);
	arrTemp[0] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
	memcpy(arrTemp+4,arrDateTime,4);
	/*convertIntToArrayUnsignChar(iAmount,arrTemp+1,3);
	convertIntToArrayUnsignChar(iDateTime,arrTemp+4,4);*/
	mlsCryptoSetContext(&ctx,arrDebitSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
	if(memcmp(arrCertificate,arrCertificateofPCC,0)== 0)
		return MLSCEPAS_SUCCESS;
	return MLSCEPAS_ERROR_COMPARECERTIFICATE;

}


/*************************************************************************************/
/******************* Debit with SAM **************************************************/

unsigned short mlsCepasDebitSAM(
								mlsCard_str *handleSC,
								mlsCard_str* handleSAM,
								unsigned char &ucPf,
								unsigned char* ucTRP,
								unsigned char ucDebitOption,
								unsigned char* arrTransactionUserData,
								KeyIndex &DebitKey,
								KeyIndex &SigningKey,
								int iAmount,
								int *PurseBalance)
{
	unsigned short usStatusCode;
	SAM_CEPAS_t SAMType;
	SC_CEPAS_t SCType;
	unsigned char abRandomTermPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrDebitCryptogram[24];
	unsigned char arrDebitCommandData[37];
	unsigned char arrDebitReceiveData[24];
	unsigned char arrDebitSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];

	unsigned char arrDataIn[255];
	unsigned char arrDataOut[255];
	unsigned char arrTransHeader[8];
	unsigned char abLtaSAMMCKey[16];
	unsigned char abTID[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//TODO need to set

	
	//for NetSAM
	unsigned char abNetSAMTokenSAM[8],abNetSAMSRn[8],abNetSAMAuthData[8],abNetSAMMC[16];
	
	short iDebitReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	SYSTEMTIME currentTime;
	int TransDateTime;
	BOOL checkCSC;
	int randnr;
	mlsReadPurseRecord PurseData;
	unsigned char arrSignatureSessionKey[16];
	int lenSAMResp; 
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;
	KeyIndex ReadPurseKey;
	
	/* check SC type to set SAM type */
	//TODO need to verify purse file ID of concession card
	usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,ucPf,0x5f,&PurseData);
	if (usStatusCode != MLSCEPAS_SUCCESS) return MLSCEPAS_ERROR_READPURSENOAUTH;
	SCType = CSCCheckCardIssuer(PurseData.arrCAN);
	switch (SCType)
	{
	case EzLinkSC:
		SAMType = ezSAM;
		break;
	case ConcessionSC:
		SAMType = LtaSAM;
		break;
	case NetSC:
		SAMType = netSAM;
		break;
	default:
		return MLSCEPAS_ERROR_UNKNOWNSC;
	}

	/* get and calculate the current local time */
	//GetSystemTime(&currentTime);
	GetLocalTime(&currentTime);
	TransDateTime = CalculateDateTime(currentTime,1995);
	
	
	switch (SAMType){
		case ezSAM:
			//read CAN and CSN of CEPAS card
			usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,ucPf,0x5f,&PurseData);
			if (usStatusCode != MLSCEPAS_SUCCESS) return MLSCEPAS_ERROR_READPURSENOAUTH;

			//Check CSC validation
			checkCSC = CSCCheckPurseExpiry(PurseData.arrPurseCreationDate,PurseData.arrPurseExpiryDate,TransDateTime/86400);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSEEXPIRY;

			checkCSC = CSCCheckPurseStatus(PurseData.ucPurseStatus,PURSE_ENABLE_STATUS);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSESTATUS;

			checkCSC = CSCCheckFund(PurseData.arrPurseBalance,iAmount);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_FUND;

			/*SCType = CSCCheckCardIssuer(PurseData.arrCAN);
			if (SCType == EzLinkSC) return MLSCEPAS_ERROR_CHECK_CARDISSUER;*/

			checkCSC = CSCCheckCANDigit(PurseData.arrCAN);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CAN;
			
			//CSCCheckBlackList(PurseData.arrCAN);
			
			checkCSC = CSCCheckAutoloadCategory(PurseData.arrIssurespecificData);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_AUTOLOADCATEGORY;
			
			checkCSC = CSCCheckRefundStatus(PurseData.arrIssurespecificData);
			if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_REFUNDSTATUS;
			
			//CSCCheckCardProfileType(PurseData.arrIssurespecificData,CardProfileTypes);
			//if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CARDPROFILETYPE;

			/* calculate Debit Record CRC */
			memcpy(arrDebitCryptogram,ucTRP,4); 
			arrDebitCryptogram[4] = ucDebitOption; 
			arrDebitCryptogram[5] = ucPf;
			arrDebitCryptogram[6] = SigningKey.ucKeyFileID;
			arrDebitCryptogram[7] = SigningKey.ucKeyNo;

			//transaction header;
			arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
			iAmount = -iAmount;
			mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
			//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
			memcpy(arrDebitCryptogram+9,arrTemp,3);

			mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);

			//iDateTime = mlsGgetDateTime();
			//convertIntToArrayUnsignChar(iDateTime,arrTemp,4);
			memcpy(arrDebitCryptogram+12,arrDateTime,4);
			//transaction user data
			memcpy(arrDebitCryptogram+16,arrTransactionUserData,8);
			mlsComputeCrc(arrDebitCryptogram,24,arrCRC);

			/* create Debit session key */
			//get challenge
			usStatusCode = mlsGetChallenge(handleSC,abRandomTermPICC+8,8);
			if(usStatusCode != MLSCEPAS_SUCCESS)
			{
				return MLSCEPAS_ERROR_GETCHALLENGE;
			}
			randnr = (unsigned char)mlsRandom();
			abRandomTermPICC[0] = (unsigned char)((randnr<<24)>>24);
			abRandomTermPICC[1] = (unsigned char)((randnr<<16)>>24);
			abRandomTermPICC[2] = (unsigned char)((randnr<<8)>>24);
			abRandomTermPICC[3] = (unsigned char)((randnr)>>24);

			randnr = (unsigned char)mlsRandom();
			abRandomTermPICC[4] = (unsigned char)((randnr<<24)>>24);
			abRandomTermPICC[5] = (unsigned char)((randnr<<16)>>24);
			abRandomTermPICC[6] = (unsigned char)((randnr<<8)>>24);
			abRandomTermPICC[7] = (unsigned char)((randnr)>>24);
			
			//select application
			usStatusCode = mlsEzSAM_SelectApplication(handleSAM);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_SELECTAPPLICATION;
			//enable function
			usStatusCode = mlsEzSAM_EnableApplet(handleSAM,OfflineDebit);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_ENABLEAPPLET;
			//debit session key function
			usStatusCode = mlsEzSAM_DebitSessionKey(handleSAM,abRandomTermPICC,
					PurseData.arrCAN,PurseData.arrCSN,arrDebitSessionKey);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_DEBITSESSIONKEY;

			memset(IV,0,8);
			mlsCryptoSetContext(&ctx,&arrDebitSessionKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);

			/* calculate Debit Cryptogram */

			memcpy(arrDebitCryptogram,ucTRP,4); // receive TRP

			memcpy(arrDebitCryptogram+4,arrCRC,2); // receive CRC
			arrDebitCryptogram[6] = SigningKey.ucKeyFileID; //receive SKf
			arrDebitCryptogram[7] = SigningKey.ucKeyNo; //receive SKn
			//transaction header;
			arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
			mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
			//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
			memcpy(arrDebitCryptogram+9,arrTemp,3);
			
			memcpy(arrDebitCryptogram+12,arrDateTime,4);

			//set ctx.key from arrDebitSessionKey
			memcpy(ctx.Key,arrDebitSessionKey,16);

			mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitCryptogram,16);

			
			break;
		case LtaSAM:
			/* read TID and Lta SAM marriage key from file */
			FILE *pLtaSAMFile;
			pLtaSAMFile = fopen(".\\mlsLtaSAM_MK.txt","r"); 
			for (i=0;i<8;i++)
			{
				fscanf (pLtaSAMFile, "%x", &abTID[i]);
			}
			for (i=0;i<16;i++)
			{
				fscanf (pLtaSAMFile, "%x", &abLtaSAMMCKey[i]);
			}
			fclose(pLtaSAMFile);

			/* SAM mutual authenticate process */
			if (mlsLtaSAMMutualAuthFlag == FALSE)
			{
				usStatusCode = mlsLtaSAM_MutualAuthenticateProcess(handleSAM,abLtaSAMMCKey,abTID,FALSE);
				if (usStatusCode != OPERATION_OK)
				{
					mlsDebugLogSrcInfo("Exit error %x\n",MLSLtaSAM_ERROR_MUTUALAUTHPROCESS);
					return MLSLtaSAM_ERROR_MUTUALAUTHPROCESS;
				}
				mlsLtaSAMMutualAuthFlag = TRUE;
			}
			
			/* Get terminal random number */
			usStatusCode = mlsLtaSAM_GetTerminalRandomNumber(handleSAM,arrDataOut);	
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSLtaSAM_ERROR_GETTERMINALRANDOMNUMBER);
				return MLSLtaSAM_ERROR_GETTERMINALRANDOMNUMBER;
			}
			ucPf = arrDataOut[48];
			ReadPurseKey.ucKeyFileID = arrDataOut[49];
			ReadPurseKey.ucKeyNo = arrDataOut[50];
			memcpy(abRandomTermPICC,&arrDataOut[51],8);//get terminal random number

			/* Get CSC challenge */
			usStatusCode = mlsGetChallenge(handleSC,&abRandomTermPICC[8],8);
			if(usStatusCode != MLSCEPAS_SUCCESS)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
				return MLSCEPAS_ERROR_GETCHALLENGE;
			}

			/* read purse with mutual authenticate from CSC */
			usStatusCode = mlsCepasReadPurseAuthen(handleSC,handleSAM,LtaSAM,
												   abRandomTermPICC,ucPf,ReadPurseKey,
												   81+32,&abRandomTermPICC[8],&PurseData);
			
			if (usStatusCode != MLSCEPAS_SUCCESS)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_READPURSEAUTH);
				return MLSCEPAS_ERROR_READPURSEAUTH;
			}			

			/* Request for authenticate */
			usStatusCode = mlsLtaSAM_RequestForAuthentication(handleSAM,&abRandomTermPICC[8],&PurseData,arrDataOut);
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSLtaSAM_ERROR_REQUESTAUTHENTICATE);
				return MLSLtaSAM_ERROR_REQUESTAUTHENTICATE;
			}
			SigningKey.ucKeyFileID = arrDataOut[48+8+8];
			SigningKey.ucKeyNo = arrDataOut[48+8+8+1];
			
			// calculate Debit Record CRC
			memcpy(arrDebitCryptogram,ucTRP,4); 
			arrDebitCryptogram[4] = ucDebitOption; 
			arrDebitCryptogram[5] = ucPf;
			arrDebitCryptogram[6] = SigningKey.ucKeyFileID;
			arrDebitCryptogram[7] = SigningKey.ucKeyNo;
			//transaction header;
			arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
			iAmount = -iAmount;
			mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
			//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
			memcpy(arrDebitCryptogram+9,arrTemp,3);
			mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);
			memcpy(arrDebitCryptogram+12,arrDateTime,4);
			memcpy(arrTransHeader,&arrDebitCryptogram[8],8);
			//transaction user data
			memcpy(arrDebitCryptogram+16,arrTransactionUserData,8);
			mlsComputeCrc(arrDebitCryptogram,24,arrCRC);
			
			/* Request for debit */
			//make array data in APDU Command
			for (i = 0; i<48 ; i++)
			{
				arrDataIn[i] = 0x00;
			}
			memcpy(&arrDataIn[48],PurseData.arrCAN,8);
			memcpy(&arrDataIn[48+8],PurseData.arrCSN,8);
			memcpy(&arrDataIn[48+8+8],&abRandomTermPICC[8],8);//card random number
			memcpy(&arrDataIn[48+8+8+8],ucTRP,4);
			memcpy(&arrDataIn[48+8+8+8+4],arrCRC,2);
			arrDataIn[48+8+8+8+4+2] = ucDebitOption;
			arrDataIn[48+8+8+8+4+2+1] = 0x00;
			memcpy(&arrDataIn[48+8+8+8+4+2+1+1],arrTransHeader,8);
			usStatusCode = mlsLtaSAM_RequestForDebit(handleSAM,arrDataIn,arrDataOut);
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSLtaSAM_ERROR_REQUESTDEBIT);
				return MLSLtaSAM_ERROR_REQUESTDEBIT;
			}
			DebitKey.ucKeyFileID = arrDataOut[48];
			DebitKey.ucKeyNo = arrDataOut[49];
			memcpy(arrDebitCryptogram,&arrDataOut[50],16);

			break;
		case netSAM:
			unsigned char KeyNo,KSI,ControlByte;

			/* initialize NetSAM */
			usStatusCode = mlsNetSAM_Initialization(handleSAM);
			if(usStatusCode != MLSCEPAS_SUCCESS)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSNetSAM_ERROR_INITIALIZATION);
				return MLSNetSAM_ERROR_INITIALIZATION;
			}
			
			//mls@dev 03 add to get key present, need to test 
			/* read purse no authenticate */
			usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,0x03,0x5f,&PurseData);
			if (usStatusCode != MLSCEPAS_SUCCESS)
			{
				return MLSCEPAS_ERROR_READPURSENOAUTH;
			}
			KSI = *((unsigned char *)&PurseData+72);//KSI - 72th byte of purse data
			for (i=0;i<ucmlsNetSAMNoKeyBlock;i++)
			{
				if (abmlsNetSAMKeySetIndex[i] = KSI)
				{
					break;
				}
			}
			if ((abmlsNetSAMArrayKeyData[i][2] & 0xC0)==0x40) //4th key is prsent
			{
				ucmlsNetSAMKeyIndex =  0x04;
			}
			else if ((abmlsNetSAMArrayKeyData[i][2] & 0xC0)==0x80) //5th key is prsent
			{
				ucmlsNetSAMKeyIndex =  0x05;
			}

			/* read purse file with authenticate */
			ReadPurseKey.ucKeyFileID = 0x0E;
			switch (ucmlsNetSAMKeyIndex)
			{
			case 0x04:
				ReadPurseKey.ucKeyNo = 0x01;
				break;
			case 0x05:
				ReadPurseKey.ucKeyNo = 0x02;
				break;
			default:
				break;
			}
			//generate terminal random number
			randnr = (unsigned char)mlsRandom();
			abRandomTermPICC[0] = (unsigned char)((randnr<<24)>>24);
			abRandomTermPICC[1] = (unsigned char)((randnr<<16)>>24);
			abRandomTermPICC[2] = (unsigned char)((randnr<<8)>>24);
			abRandomTermPICC[3] = (unsigned char)((randnr)>>24);

			randnr = (unsigned char)mlsRandom();
			abRandomTermPICC[4] = (unsigned char)((randnr<<24)>>24);
			abRandomTermPICC[5] = (unsigned char)((randnr<<16)>>24);
			abRandomTermPICC[6] = (unsigned char)((randnr<<8)>>24);
			abRandomTermPICC[7] = (unsigned char)((randnr)>>24);
			// Get CSC challenge
			usStatusCode = mlsGetChallenge(handleSC,&abRandomTermPICC[8],8);
			if(usStatusCode != MLSCEPAS_SUCCESS)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
				return MLSCEPAS_ERROR_GETCHALLENGE;
			}

			usStatusCode = mlsCepasReadPurseAuthen(handleSC,handleSAM,netSAM,
												abRandomTermPICC,0x03,ReadPurseKey,
												0x5F,&abRandomTermPICC[8],&PurseData);
			if (usStatusCode != MLSCEPAS_SUCCESS)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_READPURSEAUTH);
				return MLSCEPAS_ERROR_READPURSEAUTH;
			}
			/* check validation */
			checkCSC = CSCCheckPurseExpiry(PurseData.arrPurseCreationDate,PurseData.arrPurseExpiryDate,TransDateTime/86400);
			if (checkCSC == FALSE)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_PURSEEXPIRY);
				return MLSCEPAS_ERROR_CHECK_PURSEEXPIRY;
			}
			checkCSC = CSCCheckPurseStatus(PurseData.ucPurseStatus,PURSE_ENABLE_STATUS);
			if (checkCSC == FALSE)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_PURSESTATUS);
				return MLSCEPAS_ERROR_CHECK_PURSESTATUS;
			}
			ControlByte = *((unsigned char*)&PurseData+65);
			if ((ControlByte & 0x02)!=0)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSNetSAM_ERROR_DEBIT_CONTROLBYTE);
				return MLSNetSAM_ERROR_DEBIT_CONTROLBYTE;
			}
			/* calculate debit cryptogram */
			
			//format debit crypto data
			memcpy(&arrDataIn[0],PurseData.arrCAN,8);
			memcpy(&arrDataIn[8],PurseData.arrCSN,8);
			memcpy(&arrDataIn[16],abRandomTermPICC,16);
			arrDataIn[32] = ucDebitOption;
			arrDataIn[33] = 0x03;
			
			SigningKey.ucKeyFileID = 0x14;
			SigningKey.ucKeyNo = 0x03;
						
			arrDataIn[34] = SigningKey.ucKeyFileID;
			arrDataIn[35] = SigningKey.ucKeyNo;

			//transaction header;
			arrTransHeader[0] = MLS_DEBIT_TYPE;//TODO check transaction type for Net SAM
			iAmount = -iAmount;
			mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
			memcpy(&arrTransHeader[1],arrTemp,3);
			mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);
			memcpy(&arrTransHeader[4],arrDateTime,4);
			
			memcpy(&arrDataIn[36],arrTransHeader,8);
			memcpy(&arrDataIn[44],arrTransactionUserData,8);

			switch (ucmlsNetSAMKeyIndex)
			{
			case 0x04:
				KeyNo = 0x03;
				break;
			case 0x05:
				KeyNo = 0x04;
				break;
			default:
				break;
			}
			
			KSI = *((unsigned char *)&PurseData+72);//KSI - 72th byte of purse data

			usStatusCode = mlsNetSAM_GenerateDebitCryptogram2(handleSAM,
															KeyNo,
															KSI,
															arrDataIn,
															arrDataOut);
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSNetSAM_ERROR_DEBIT_GDC2);
				return MLSNetSAM_ERROR_DEBIT_GDC2;
			}
			
			memcpy(arrDebitCryptogram,arrDataOut,16);
			memcpy(abNetSAMTokenSAM,&arrDataOut[16],8);
			memcpy(abNetSAMSRn,&arrDataOut[24],8);
			memcpy(ucTRP,&arrDataOut[32],4);
			
			
			//TODO check read NetSAM marriage code from file [16-0]
			// read NetSAM marriage code from file 
			FILE *pNetSAMFile;
			pNetSAMFile = fopen(".\\mlsNetSAM_MC.txt","r"); 
			for (i=0;i<16;i++)
			{
				fscanf (pNetSAMFile, "%x", &abNetSAMMC[i]);
			}
			fclose(pNetSAMFile);

			// check derive
			for(i=0;i<8;i++)
			{
				abNetSAMAuthData[i] = abRandomTermPICC[i]^abNetSAMMC[i+8]^abNetSAMMC[i];
			}
			if (memcmp(abNetSAMAuthData,abNetSAMTokenSAM,8) != 0)
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSNetSAM_ERROR_DEBIT_CMPTOKEN);
				return MLSNetSAM_ERROR_DEBIT_CMPTOKEN;
			}
			ucPf = 0x03;//Pf
			DebitKey.ucKeyFileID = 0x0E;//Key file SFI
			switch (ucmlsNetSAMKeyIndex)
			{
			case 0x04:
				DebitKey.ucKeyNo  = 0x01;
				break;
			case 0x05:
				DebitKey.ucKeyNo = 0x02;
				break;
			default:
				break;
			}
			SigningKey.ucKeyFileID = 0x14;//Signing Key file
			SigningKey.ucKeyNo = 0x03;//KeyNo of Signing Key
			break;
		default:
			break;
	}
	
	/* make Debit Command Data */
	arrDebitCommandData[0] = ucPf;//Pf
	arrDebitCommandData[1] = DebitKey.ucKeyFileID;//Key file SFI
	arrDebitCommandData[2] = DebitKey.ucKeyNo;//KeyNo
	arrDebitCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	arrDebitCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	memcpy(arrDebitCommandData+5,abRandomTermPICC,8); //random of TERMINAL
	memcpy(arrDebitCommandData+13,arrDebitCryptogram,16);// Debit Cryptogram
	memcpy(arrDebitCommandData+29,arrTransactionUserData,8);//Transaction User Data

	/* Send Debit command to CSC */

	usStatusCode = ExecuteCommand(
		handleSC, //card handle
		MLS_DEBIT, //INS
		0x00, //P1
		ucDebitOption, //P2
		0x25, //Lc
		arrDebitCommandData,//data buff
		0x18, //Le
		arrDebitReceiveData,//response buff
		sizeof(arrDebitReceiveData),//size of response buff
		&iDebitReceiveLength); //number of byte return to response buff
	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
		return MLSCEPAS_ERROR_SENDAPDUDEBIT;
	if( iDebitReceiveLength != 0x18 )
		return MLSCEPAS_ERROR_WRONGLENGTH;

	/* test signed certificate */
	switch (SAMType){
		case ezSAM:
			//decrypt counter data
			memcpy(ctx.IV,arrDebitReceiveData+8,8);
			mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+16,arrCounterData,8);
			//decrypt purse balance
			memcpy(ctx.IV,arrCounterData,8);
			mlsCrypto_Decrypt(&ctx,arrDebitReceiveData,arrPurseBalance,8);
			//decrypt certificate
			memcpy(ctx.IV,arrDebitReceiveData,8);
			mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+8,arrCertificate,8);
			abRandomTermPICC[0]= ucDebitOption;
			memcpy(abRandomTermPICC+1,arrPurseBalance,3);
			memcpy(abRandomTermPICC+4,ucTRP,4);
			memcpy(abRandomTermPICC+8,arrCounterData,8);

			usStatusCode = mlsEzSAM_SignatureSessionKey(
				handleSAM,
				abRandomTermPICC,
				PurseData.arrCAN,
				PurseData.arrCSN,
				arrSignatureSessionKey);
			if (usStatusCode == OPERATION_ERROR)
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSEzSAM_ERROR_SIGNATURESESSIONKEY);
				return MLSEzSAM_ERROR_SIGNATURESESSIONKEY;
			}
			memcpy(ctx.Key,arrSignatureSessionKey,16);
			// Form the new Transaction Header
			memset(arrTemp,0,8);
			arrTemp[0] = MLS_DEBIT_TYPE;
			mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
			memcpy(arrTemp+4,arrDateTime,4);
			/*convertIntToArrayUnsignChar(iAmount,arrTemp+1,3);
			convertIntToArrayUnsignChar(iDateTime,arrTemp+4,4);*/
			mlsCryptoSetContext(&ctx,arrSignatureSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
			mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
			if(memcmp(arrCertificate,arrCertificateofPCC,8)!= 0)//mls@dev03 change count:0->8
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSCEPAS_ERROR_COMPARECERTIFICATE);
				return MLSCEPAS_ERROR_COMPARECERTIFICATE;
			}
			//Set purse balance value
			*(((unsigned char *)PurseBalance)+3)=0;	
			*(((unsigned char *)PurseBalance)+2)=arrPurseBalance[0];
			*(((unsigned char *)PurseBalance)+1)=arrPurseBalance[1];
			*((unsigned char *)PurseBalance)=arrPurseBalance[2];

			break;
		case LtaSAM:
			/* request for sign certificated */
			usStatusCode = mlsLtaSAM_RequestForSignCertification(handleSAM,
																arrDebitReceiveData,
																arrDataOut);
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSLtaSAM_ERROR_REQUESTSIGNCERTIFICATE);
				return MLSLtaSAM_ERROR_REQUESTSIGNCERTIFICATE;
			}
			//Set purse balance value
			*(((unsigned char *)PurseBalance)+3)=0;	
			*(((unsigned char *)PurseBalance)+2)=arrDataOut[0];
			*(((unsigned char *)PurseBalance)+1)=arrDataOut[1];
			*((unsigned char *)PurseBalance)=arrDataOut[2];
			break;
		case netSAM:
			//derive
			unsigned char abTokenReader[8];

			for(i=0;i<8;i++)
			{
				abTokenReader[i] = abNetSAMSRn[i]^abNetSAMMC[i+8]^abNetSAMMC[i];
			}
			usStatusCode = mlsNetSAM_VerifyDebitReceiptCryptogram2( handleSAM,
																	arrDebitReceiveData,
																	abTokenReader,
																	arrDataOut);
			
			if ((usStatusCode != OPERATION_OK)&&(usStatusCode != MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION))
			{
				mlsDebugLogSrcInfo("Exit error %x\n", MLSNetSAM_ERROR_DEBIT_VDRC2);
				return MLSNetSAM_ERROR_DEBIT_VDRC2;
			}
			
			if (usStatusCode == MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION)
			{
				usStatusCode = mlsNetSAM_ExceptionHandling(handleSC,handleSAM,iAmount,&PurseData,arrDataOut);
				if (usStatusCode != OPERATION_OK)
				{
					mlsDebugLogSrcInfo("Exit error %x\n", MLSNetSAM_ERROR_DEBIT_EXCEPTION);
					return MLSNetSAM_ERROR_DEBIT_EXCEPTION;
				}
			}
			
			//Set balance
			//Set purse balance value
			*(((unsigned char *)PurseBalance)+3)=0;	
			*(((unsigned char *)PurseBalance)+2)=arrDataOut[0];
			*(((unsigned char *)PurseBalance)+1)=arrDataOut[1];
			*((unsigned char *)PurseBalance)=arrDataOut[2];

			break;
		default:
			break;
	}
	
	mlsDebugLog("Exit CEPAS Debit with OK status %d\n",MLSCEPAS_SUCCESS);
	return MLSCEPAS_SUCCESS; 
	
	
}
#ifndef MLS_TESTSETTLEMENT
unsigned short mlsCepasDebitSAMAndCreTransRec(mlsCard_str *handleSC,mlsCard_str *handleSAM,SAM_CEPAS_t SAM,unsigned char ucPf,unsigned char* ucTRP,unsigned char ucDebitOption,
											  unsigned char* arrTransactionUserData,KeyIndex DebitKey,KeyIndex SigningKey,int iAmount,int *PurseBalance,
											  UINT8 IssuerData,MDTransactionRecord *pMDTransRecord)
{
	unsigned short usStatusCode;
	unsigned char abRandomTermPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrDebitCryptogram[24];
	unsigned char arrDebitCommandData[37];
	unsigned char arrDebitReceiveData[24];
	unsigned char arrDebitSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];
	short iDebitReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	SYSTEMTIME currentTime;
	int TransDateTime;
	BOOL checkCSC;
	int randnr;
	mlsReadPurseRecord PurseData;
	unsigned char arrSignatureSessionKey[16];
	int lenSAMResp; 
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;
	//
	UINT8 arrTransHeader[8];

	mlsDebugLogSrcInfo("Enter \n");

	//get challenge
	usStatusCode = mlsGetChallenge(handleSAM,&abRandomTermPICC[8],8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
		return MLSCEPAS_ERROR_GETCHALLENGE;
	}

	randnr = (unsigned char)mlsRandom();
	abRandomTermPICC[0] = (unsigned char)((randnr<<24)>>24);
	abRandomTermPICC[1] = (unsigned char)((randnr<<16)>>24);
	abRandomTermPICC[2] = (unsigned char)((randnr<<8)>>24);
	abRandomTermPICC[3] = (unsigned char)((randnr)>>24);

	randnr = (unsigned char)mlsRandom();
	abRandomTermPICC[4] = (unsigned char)((randnr<<24)>>24);
	abRandomTermPICC[5] = (unsigned char)((randnr<<16)>>24);
	abRandomTermPICC[6] = (unsigned char)((randnr<<8)>>24);
	abRandomTermPICC[7] = (unsigned char)((randnr)>>24);


	//read CAN and CSN of CEPAS card
	usStatusCode = mlsCepasReadPurseAuthen(handleSC,handleSAM,SAM,abRandomTermPICC,ucPf,DebitKey,0x71, &abRandomTermPICC[8], &PurseData);

	if (usStatusCode != MLSCEPAS_SUCCESS)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_READPURSEAUTH);
		return MLSCEPAS_ERROR_READPURSEAUTH;
	}

	//get the current time
	//GetSystemTime(&currentTime);
	GetLocalTime(&currentTime);
	TransDateTime = CalculateDateTime(currentTime,1995);

	//Check CSC validation
	checkCSC = CSCCheckPurseExpiry(PurseData.arrPurseCreationDate,PurseData.arrPurseExpiryDate,TransDateTime/86400);
	if (checkCSC == FALSE) 
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_PURSEEXPIRY);
		return MLSCEPAS_ERROR_CHECK_PURSEEXPIRY;
	}


	checkCSC = CSCCheckPurseStatus(PurseData.ucPurseStatus,PURSE_ENABLE_STATUS);
	if (checkCSC == FALSE)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_PURSESTATUS);
		return MLSCEPAS_ERROR_CHECK_PURSESTATUS;
	}

	checkCSC = CSCCheckFund(PurseData.arrPurseBalance,iAmount);


	if (checkCSC == FALSE) 
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_FUND);
		return MLSCEPAS_ERROR_CHECK_FUND;
	}


	checkCSC = CSCCheckCardIssuer(PurseData.arrCAN);

	if (checkCSC == FALSE) 
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_CARDISSUER);
		return MLSCEPAS_ERROR_CHECK_CARDISSUER;
	}

	checkCSC = CSCCheckCANDigit(PurseData.arrCAN);

	if (checkCSC == FALSE)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_CHECK_CAN);

		return MLSCEPAS_ERROR_CHECK_CAN;
	}

	//CSCCheckBlackList(PurseData.arrCAN);
	//CSCCheckAutoloadCategory(PurseData.arrIssurespecificData);
	//CSCCheckRefundStatus(PurseData.arrIssurespecificData);
	//CSCCheckCardProfileType(PurseData.arrIssurespecificData,CardProfileTypes);

	//get challenge
	usStatusCode = mlsGetChallenge(handleSC,abRandomTermPICC+8,8);

	if(usStatusCode != MLSCEPAS_SUCCESS)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
		return MLSCEPAS_ERROR_GETCHALLENGE;
	}
	//calculate Debit Record CRC
	memcpy(arrDebitCryptogram,ucTRP,4); 
	arrDebitCryptogram[4] = ucDebitOption; 
	arrDebitCryptogram[5] = ucPf;
	arrDebitCryptogram[6] = SigningKey.ucKeyFileID;
	arrDebitCryptogram[7] = SigningKey.ucKeyNo;

	//transaction header;
	arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	iAmount = -iAmount;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrDebitCryptogram+9,arrTemp,3);

	mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);
	
	/*iDateTime = mlsGgetDateTime();
	convertIntToArrayUnsignChar(iDateTime,arrTemp,4);*/
	memcpy(arrDebitCryptogram+12,arrDateTime,4);
	//------copy data of Transaction Header
	memcpy(arrTransHeader,arrDebitCryptogram+8,8);
	//transaction user data
	memcpy(arrDebitCryptogram+16,arrTransactionUserData,8);
	mlsComputeCrc(arrDebitCryptogram,24,arrCRC);

	//create Debit session key
	randnr = (unsigned char)mlsRandom();
	abRandomTermPICC[0] = (unsigned char)((randnr<<24)>>24);
	abRandomTermPICC[1] = (unsigned char)((randnr<<16)>>24);
	abRandomTermPICC[2] = (unsigned char)((randnr<<8)>>24);
	abRandomTermPICC[3] = (unsigned char)((randnr)>>24);

	randnr = (unsigned char)mlsRandom();
	abRandomTermPICC[4] = (unsigned char)((randnr<<24)>>24);
	abRandomTermPICC[5] = (unsigned char)((randnr<<16)>>24);
	abRandomTermPICC[6] = (unsigned char)((randnr<<8)>>24);
	abRandomTermPICC[7] = (unsigned char)((randnr)>>24);

	switch (SAM){
		case ezSAM:
			//select application
			usStatusCode = mlsEzSAM_SelectApplication(handleSAM);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_SELECTAPPLICATION);
				return MLSEzSAM_ERROR_SELECTAPPLICATION;
			}
			//enable function
			usStatusCode = mlsEzSAM_EnableApplet(handleSAM,OfflineDebit);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_ENABLEAPPLET);
				return MLSEzSAM_ERROR_ENABLEAPPLET;
			}

			//debit session key function
			usStatusCode = mlsEzSAM_DebitSessionKey(handleSAM,abRandomTermPICC,
				PurseData.arrCAN,PurseData.arrCSN,arrDebitSessionKey);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_DEBITSESSIONKEY);
				return MLSEzSAM_ERROR_DEBITSESSIONKEY;
			}
			break;
		case netSAM:
			break;
		default:
			break;
	}

	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrDebitSessionKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);

	//calculate Debit Cryptogram

	memcpy(arrDebitCryptogram,ucTRP,4); // receive TRP

	memcpy(arrDebitCryptogram+4,arrCRC,2); // receive CRC
	arrDebitCryptogram[6] = SigningKey.ucKeyFileID; //receive SKf
	arrDebitCryptogram[7] = SigningKey.ucKeyNo; //receive SKn
	//transaction header;
	arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrDebitCryptogram+9,arrTemp,3);

	memcpy(arrDebitCryptogram+12,arrDateTime,4);

	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrDebitSessionKey,16);

	mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitCryptogram,16);

	//calculate Debit Command Data
	arrDebitCommandData[0] = ucPf;//Pf
	arrDebitCommandData[1] = DebitKey.ucKeyFileID;//Key file SFI
	arrDebitCommandData[2] = DebitKey.ucKeyNo;//KeyNo
	arrDebitCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	arrDebitCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	memcpy(arrDebitCommandData+5,abRandomTermPICC,8); //random of TERMINAL
	memcpy(arrDebitCommandData+13,arrDebitCryptogram,16);// Debit Cryptogram
	memcpy(arrDebitCommandData+29,arrTransactionUserData,8);//Transaction User Data

	usStatusCode = ExecuteCommand(
		handleSC, //card handle
		MLS_DEBIT, //INS
		0x00, //P1
		ucDebitOption, //P2
		0x25, //Lc
		arrDebitCommandData,//data buff
		0x18, //Le
		arrDebitReceiveData,//response buff
		sizeof(arrDebitReceiveData),//size of response buff
		&iDebitReceiveLength); //number of byte return to response buff

	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",OPERATION_ERROR);
		return OPERATION_ERROR;
	}

	if( iDebitReceiveLength != 0x18 )
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_WRONGLENGTH);
		return MLSCEPAS_ERROR_WRONGLENGTH;
	}

	//test signed certificate
	//decrypt counter data
	memcpy(ctx.IV,arrDebitReceiveData+8,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+16,arrCounterData,8);
	//decrypt purse balance
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData,arrPurseBalance,8);

	*(((unsigned char *)PurseBalance)+3)=0;	
	*(((unsigned char *)PurseBalance)+2)=arrPurseBalance[0];
	*(((unsigned char *)PurseBalance)+1)=arrPurseBalance[1];
	*((unsigned char *)PurseBalance)=arrPurseBalance[2];

	//decrypt certificate
	memcpy(ctx.IV,arrDebitReceiveData,8);
	mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+8,arrCertificate,8);

	switch (SAM){
		case ezSAM:
			abRandomTermPICC[0]= ucDebitOption;
			memcpy(abRandomTermPICC+1,arrPurseBalance,3);
			memcpy(abRandomTermPICC+4,ucTRP,4);
			memcpy(abRandomTermPICC+8,arrCounterData,8);

			usStatusCode = mlsEzSAM_SignatureSessionKey(
				handleSAM,
				abRandomTermPICC,
				PurseData.arrCAN,
				PurseData.arrCSN,
				arrSignatureSessionKey);
			if (usStatusCode == OPERATION_ERROR)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_SIGNATURESESSIONKEY);
				return MLSEzSAM_ERROR_SIGNATURESESSIONKEY;
			}
			break;
		case netSAM:
			break;
		default:
			break;
	}

	memcpy(ctx.Key,arrSignatureSessionKey,16);

	// Form the new Transaction Header
	memset(arrTemp,0,8);
	arrTemp[0] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
	memcpy(arrTemp+4,arrDateTime,4);
	/*convertIntToArrayUnsignChar(iAmount,arrTemp+1,3);
	convertIntToArrayUnsignChar(iDateTime,arrTemp+4,4);*/
	mlsCryptoSetContext(&ctx,arrSignatureSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
	if(memcmp(arrCertificate,arrCertificateofPCC,0)== 0)
	{
		mlsCepasCreateTransRecord(PurseData,arrTransHeader,ucTRP,arrPurseBalance,arrCounterData,arrCertificate,currentTime,IssuerData,pMDTransRecord);

		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_SUCCESS);
		return MLSCEPAS_SUCCESS;
	}

	mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_COMPARECERTIFICATE);
	return MLSCEPAS_ERROR_COMPARECERTIFICATE;
}

#endif
#ifdef MLS_TESTSETTLEMENT
unsigned short mlsCepasDebitSAMAndCreTransRec(mlsCard_str *handleSC,mlsCard_str *handleSAM,SAM_CEPAS_t SAM,unsigned char ucPf,unsigned char* ucTRP,unsigned char ucDebitOption,
											  unsigned char* arrTransactionUserData,KeyIndex DebitKey,KeyIndex SigningKey,int iAmount,int *PurseBalance,
											  UINT8 IssuerData,MDTransactionRecord *pMDTransRecord)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrDebitCryptogram[24];
	unsigned char arrDebitCommandData[37];
	unsigned char arrDebitReceiveData[24];
	unsigned char arrDebitSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];
	short iDebitReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	SYSTEMTIME currentTime;
	int TransDateTime;
	BOOL checkCSC;
	int randnr;
	mlsReadPurseRecord PurseData;
	unsigned char arrSignatureSessionKey[16];
	int lenSAMResp; 
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;
	//
	UINT8 arrTransHeader[8];
	//read CAN and CSN of CEPAS card
	usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,ucPf,0x5f,&PurseData);
	if (usStatusCode != MLSCEPAS_SUCCESS) return MLSCEPAS_ERROR_READPURSENOAUTH;

	//get the current time
	//GetSystemTime(&currentTime);
	GetLocalTime(&currentTime);
	TransDateTime = CalculateDateTime(currentTime,1995);

	//Check CSC validation
	checkCSC = CSCCheckPurseExpiry(PurseData.arrPurseCreationDate,PurseData.arrPurseExpiryDate,TransDateTime/86400);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSEEXPIRY;

	checkCSC = CSCCheckPurseStatus(PurseData.ucPurseStatus,PURSE_ENABLE_STATUS);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSESTATUS;

	checkCSC = CSCCheckFund(PurseData.arrPurseBalance,iAmount);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_FUND;

	checkCSC = CSCCheckCardIssuer(PurseData.arrCAN);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CARDISSUER;

	checkCSC = CSCCheckCANDigit(PurseData.arrCAN);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CAN;

	//get challenge
	usStatusCode = mlsGetChallenge(handleSC,abRamdomPICC+8,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
		return MLSCEPAS_ERROR_GETCHALLENGE;

	//calculate Debit Record CRC
	memcpy(arrDebitCryptogram,ucTRP,4); 
	arrDebitCryptogram[4] = ucDebitOption; 
	arrDebitCryptogram[5] = ucPf;
	arrDebitCryptogram[6] = SigningKey.ucKeyFileID;
	arrDebitCryptogram[7] = SigningKey.ucKeyNo;

	//transaction header;
	arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	iAmount = -iAmount;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrDebitCryptogram+9,arrTemp,3);

	mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);

	/*iDateTime = mlsGgetDateTime();
	convertIntToArrayUnsignChar(iDateTime,arrTemp,4);*/
	memcpy(arrDebitCryptogram+12,arrDateTime,4);
	//------copy data of Transaction Header
	memcpy(arrTransHeader,arrDebitCryptogram+8,8);
	//transaction user data
	//memcpy(arrDebitCryptogram+16,arrTransactionUserData,8);
	//mlsComputeCrc(arrDebitCryptogram,24,arrCRC);

	//create Debit session key
	/*randnr = (unsigned char)mlsRandom();
	abRamdomPICC[0] = (unsigned char)((randnr<<24)>>24);
	abRamdomPICC[1] = (unsigned char)((randnr<<16)>>24);
	abRamdomPICC[2] = (unsigned char)((randnr<<8)>>24);
	abRamdomPICC[3] = (unsigned char)((randnr)>>24);

	randnr = (unsigned char)mlsRandom();
	abRamdomPICC[4] = (unsigned char)((randnr<<24)>>24);
	abRamdomPICC[5] = (unsigned char)((randnr<<16)>>24);
	abRamdomPICC[6] = (unsigned char)((randnr<<8)>>24);
	abRamdomPICC[7] = (unsigned char)((randnr)>>24);*/

	//memset(IV,0,8);
	//mlsCryptoSetContext(&ctx,&arrDebitSessionKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);

	//calculate Debit Cryptogram

	//memcpy(arrDebitCryptogram,ucTRP,4); // receive TRP

	//memcpy(arrDebitCryptogram+4,arrCRC,2); // receive CRC
	//arrDebitCryptogram[6] = SigningKey.ucKeyFileID; //receive SKf
	//arrDebitCryptogram[7] = SigningKey.ucKeyNo; //receive SKn
	//transaction header;
	//arrDebitCryptogram[8] = MLS_DEBIT_TYPE;
	//mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	//memcpy(arrDebitCryptogram+9,arrTemp,3);

	//memcpy(arrDebitCryptogram+12,arrDateTime,4);

	//set ctx.key from arrDebitSessionKey
	//memcpy(ctx.Key,arrDebitSessionKey,16);

	//mlsCrypto_Encrypt(&ctx,arrDebitCryptogram,arrDebitCryptogram,16);

	//calculate Debit Command Data
	//arrDebitCommandData[0] = ucPf;//Pf
	//arrDebitCommandData[1] = DebitKey.ucKeyFileID;//Key file SFI
	//arrDebitCommandData[2] = DebitKey.ucKeyNo;//KeyNo
	//arrDebitCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	//arrDebitCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	//memcpy(arrDebitCommandData+5,abRamdomPICC,8); //random of TERMINAL
	//memcpy(arrDebitCommandData+13,arrDebitCryptogram,16);// Debit Cryptogram
	//memcpy(arrDebitCommandData+29,arrTransactionUserData,8);//Transaction User Data

	//usStatusCode = ExecuteCommand(
	//	handleSC, //card handle
	//	MLS_DEBIT, //INS
	//	0x00, //P1
	//	ucDebitOption, //P2
	//	0x25, //Lc
	//	arrDebitCommandData,//data buff
	//	0x18, //Le
	//	arrDebitReceiveData,//response buff
	//	sizeof(arrDebitReceiveData),//size of response buff
	//	&iDebitReceiveLength); //number of byte return to response buff

	/*if(usStatusCode != 0x9000)
		return usStatusCode;
	if( iDebitReceiveLength != 0x18 )
		return sgw7816_WRONG_LENGTH;*/

	//test signed certificate
	//decrypt counter data
	//memcpy(ctx.IV,arrDebitReceiveData+8,8);
	//mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+16,arrCounterData,8);
	memset(arrCounterData,'4',8);
	//decrypt purse balance
	//memcpy(ctx.IV,arrCounterData,8);
	//mlsCrypto_Decrypt(&ctx,arrDebitReceiveData,arrPurseBalance,8);
	memset(arrPurseBalance,'8',3);
	/**(((unsigned char *)PurseBalance)+3)=0;	
	*(((unsigned char *)PurseBalance)+2)=arrPurseBalance[0];
	*(((unsigned char *)PurseBalance)+1)=arrPurseBalance[1];
	*((unsigned char *)PurseBalance)=arrPurseBalance[2];*/

	//decrypt certificate
	//memcpy(ctx.IV,arrDebitReceiveData,8);
	//mlsCrypto_Decrypt(&ctx,arrDebitReceiveData+8,arrCertificate,8);
	memset(arrCertificate,'6',8);
	//memcpy(ctx.Key,arrSignatureSessionKey,16);

	// Form the new Transaction Header
	//memset(arrTemp,0,8);
	//arrTemp[0] = MLS_DEBIT_TYPE;
	//mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
	//memcpy(arrTemp+4,arrDateTime,4);
	//mlsCryptoSetContext(&ctx,arrSignatureSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	//mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
	//if(memcmp(arrCertificate,arrCertificateofPCC,0)== 0)
	//{
		mlsCepasCreateTransRecord(PurseData,arrTransHeader,ucTRP,arrPurseBalance,arrCounterData,arrCertificate,currentTime,IssuerData,pMDTransRecord);
		return MLSCEPAS_SUCCESS;
	//}
	//return sgw7816_CERTIFICATE_ERROR;
}

#endif
int calRecordNumber(CepasMDBatchFile *pCepasMDBatchFile)
{
	int iNumber = 0;
	MDTransactionRecord *pTemp = pCepasMDBatchFile->pTransactionDetail;

	while(pTemp != NULL)
	{
		pTemp = pTemp->pNext;
		iNumber++;
	}
	return iNumber;
}
int calLenOfMDBatchFile(CepasMDBatchFile *pCepasMDBatchFile)
{
	int iLen = sizeof(CepasMDBatchFile) - sizeof(pCepasMDBatchFile->pLastTransactionDetail) - sizeof(pCepasMDBatchFile->pTransactionDetail);
	MDTransactionRecord *pTemp = pCepasMDBatchFile->pTransactionDetail;

	while(pTemp != NULL)
	{
		iLen += sizeof(MDTransactionRecord) - sizeof(pTemp->pNext);
		pTemp = pTemp->pNext;
	}
	return iLen;
}
void freeMemoryOfCepasMDBatchFile(CepasMDBatchFile *pCepasMDBatchFile)
{

	MDTransactionRecord *pTemp = pCepasMDBatchFile->pTransactionDetail;
	free(pCepasMDBatchFile);
	MDTransactionRecord *pNextTemp;
	while(pTemp != NULL)
	{
		pNextTemp = pTemp->pNext;
		free(pTemp);
		pTemp = pNextTemp;
	}
}
int checkFileSizeOrRecordNumber(CepasMDBatchFile *pCepasMDBatchFile)
{
	if((calRecordNumber(pCepasMDBatchFile) + 1) > RECNUMBERMAX || (calLenOfMDBatchFile(pCepasMDBatchFile) +sizeof(MDTransactionRecord) - 4)> SIZE_FILE )
		return 1;
	return 0;
}


UINT16 calCRC_16(CepasMDBatchFile *pCepasMDBatchFile)
{
	INT16 crc16;
	int iLen = calLenOfMDBatchFile(pCepasMDBatchFile);
	UINT8 *pData = (UINT8 *)malloc(iLen - 2);
	int iMDBatchHeaderSize = sizeof(CepasMDBatchFile) - sizeof(pCepasMDBatchFile->pLastTransactionDetail) - sizeof(pCepasMDBatchFile->pTransactionDetail);

	memcpy(pData,(UINT8*)pCepasMDBatchFile,iMDBatchHeaderSize - 33 - 2);
	memcpy(pData + iMDBatchHeaderSize - 33 - 2,((UINT8*)pCepasMDBatchFile) + iMDBatchHeaderSize - 33,33);
	//pData[iMDBatchHeaderSize - 3] = 'D';

	iLen = iMDBatchHeaderSize - 2;

	MDTransactionRecord *pTemp = pCepasMDBatchFile->pTransactionDetail;
	int iMDTranRecordLen = 0;
	if(pTemp != NULL)
		iMDTranRecordLen = sizeof(MDTransactionRecord) - sizeof(pTemp->pNext);
	while(pTemp != NULL)
	{
		memcpy(pData+iLen,pTemp,iMDTranRecordLen);
		iLen = iLen + iMDTranRecordLen;
		pTemp = pTemp->pNext;
	}

	crc16 = LIB_Crc16(pData,iLen);
	free(pData);
	return crc16;
}
void mlsCepasCreateTransRecord(mlsReadPurseRecord PurseData,UInt8 *pTransHeader,UInt8 *pTRP,UInt8*pPurseBalance,UInt8 *pCounterData,
							   UInt8 *pSigCer,SYSTEMTIME currentTime,UInt8 IssuerID,MDTransactionRecord *pMDTransRecord)
{
	//Record Indicator
	pMDTransRecord->dataRecordIndicator = 'D';
	//--set data of MDStransRecord
	//MessageType(indicated in theCEPAS Specification)
	//NOTE:don't know data of MessageType,so set default data
	memcpy(pMDTransRecord->messageType,MESSAGETYPE,2);
	//Card Make
	pMDTransRecord->cardMake = 0x01;
	//Format Version(Set to 1)
	pMDTransRecord->formatVersion = 0x01;
	//CAN
	memcpy(pMDTransRecord->CAN,PurseData.arrCAN,8);
	//Bad Debt Counter
	//NOTE:don't know data of Bad Debt Counter,so set default data
	UINT8 badDebitCounter = PurseData.arrIssurespecificData[1];
	badDebitCounter = (badDebitCounter >>6) & 0x03;
	pMDTransRecord->badDeptCounter = badDebitCounter;
	//RFU
	pMDTransRecord->RFU = 0x00;
	//Transaction Header 
	memcpy((UInt8 *)(&pMDTransRecord->transactionHeader),pTransHeader,8);
	//Terminal Reference Parameter(TRP)
	memcpy((UInt8 *)(&pMDTransRecord->terminalReferenceParam),pTRP,4);
	//Purse Balance
	memcpy(pMDTransRecord->purseBalance,pPurseBalance,3);
	//Counter Data
	memcpy((UInt8 *)(&pMDTransRecord->counterData),pCounterData,8);
	//Signed Certificate
	memcpy(pMDTransRecord->signedCertificate,pSigCer,8);
	//Last Credit Transaction Header
	memcpy((UInt8 *)(&pMDTransRecord->lastCreditTransactionHeader),PurseData.arrLastCreditTransactionHeader,8);
	//Last Credit Transaction TRP
	memcpy((UInt8 *)(&pMDTransRecord->lastCreditTransactionTRP),PurseData.arrLastCreditTransactionTRP,4);
	//Last Purse Status
	pMDTransRecord->lastPurseStatus = PurseData.ucPurseStatus;
	//Last Transaction Header
	memcpy((UInt8 *)(&pMDTransRecord->lastTransactionHeader),PurseData.arrLastTransactionRecord,8);
	//Last Transaction TRP
	memcpy((UInt8 *)(&pMDTransRecord->lastTransactionTRP),PurseData.arrLastTransactionTRP,4);
	//Last Purse Balance
	memcpy(pMDTransRecord->lastPurseBalance,PurseData.arrPurseBalance,3);
	//Last Counter Data
	//NOTE: need to read purse with authenticate
	memcpy((UInt8 *)(&pMDTransRecord->lastCounterData),PurseData.arrCounterData,8);
	//Signed Certificate of Last Txn
	memcpy(pMDTransRecord->lastTxnSignedCertificate,PurseData.arrLastTransactionSignedCertificate,8);
	//Last Debit Txn Debit Option
	pMDTransRecord->lastDebitOption = PurseData.ucLastTransactionDebitOptionsByte;
	//Bussiness Date
	pMDTransRecord->businessDate = CalculateDate(currentTime,1999);
	//Auto-Load Amount

	int iAmount;
	int iLastPurseBalance;
	mlsConvertToIntKeepNegative(&iAmount,pTransHeader,1);
	iAmount = -iAmount;
	mlsConvertToIntKeepNegative(&iLastPurseBalance,PurseData.arrPurseBalance,0);
	if(iLastPurseBalance >= iAmount)
	{
		memset(pMDTransRecord->autoLoadAmount,0,3);
	}
	else
	{
		memcpy(pMDTransRecord->autoLoadAmount,PurseData.arrAutoLoadAmount,3);
	}

	//Issuer Id(0x03 - Transit Link, 0x04 - EzLink,0x02 - NETS)
	pMDTransRecord->issuerID = IssuerID;
	//Issuer Specific Data
	memcpy(pMDTransRecord->issuerSpecificData,PurseData.arrIssurespecificData,32);
	//UTSN
	//NOTE::don't know data of UTSN,so set default data
	memcpy(pMDTransRecord->UTSN,UTSN,8);

	//pNext pointer
	pMDTransRecord->pNext = NULL;


}
void mlsCepasCreateMDBatchFile(SYSTEMTIME currentTime,CepasMDBatchFile *pCepasMBBatchFile)
{
	//Record Indicator (Value = "H")
	pCepasMBBatchFile->headerRecordIndicator = 'H';
	//Format Version
	pCepasMBBatchFile->formatVersion = 0x01;
	//Record_DateTime
	UInt8 arrRecordDateTime[14];
	convertNumbertoArrChar(currentTime.wYear,arrRecordDateTime,4);
	convertNumbertoArrChar(currentTime.wMonth,arrRecordDateTime+4,2);
	convertNumbertoArrChar(currentTime.wDay,arrRecordDateTime+6,2);
	convertNumbertoArrChar(currentTime.wHour,arrRecordDateTime+8,2);
	convertNumbertoArrChar(currentTime.wMinute,arrRecordDateTime+10,2);
	convertNumbertoArrChar(currentTime.wSecond,arrRecordDateTime+12,2);
	memcpy(pCepasMBBatchFile->recordDatetime,arrRecordDateTime,14);
	//Source(Value PSAMT)
	memcpy(pCepasMBBatchFile->source,SOURCE,5);
	//Destination (value "QBFTP")
	memcpy(pCepasMBBatchFile->destination,DESTINATION,5);
	//Merchant Chain ID (Format '99999999',Prefixed with '0')
	memcpy(pCepasMBBatchFile->marchantChainID,MERCHANTCHAINID,8);
	//Location ID (Format '99999999', prefixed with '0')
	memcpy(pCepasMBBatchFile->locationID,LOCATIONID,8);
	//Device Type (Value '0080')
	memcpy(pCepasMBBatchFile->deviceType,DEVICETYPE,4);
	//Device ID
	memcpy(pCepasMBBatchFile->deviceID,DEVICEID,8);
	//Merchant POS ID (Format '9999999999', prefixed with '0')
	memcpy(pCepasMBBatchFile->merchantPOSID,MERCHANTCHAINID,10);
	//Number of records in Batch (Total number of TRANSACTION DATA records in the file)
	memcpy(pCepasMBBatchFile->numberOfRecord,NUMBEROFRECORDS,4);
	//Batch Debit Amount (Total debit amount for all debit TRANSACTION DATA records in the file in the format 9999999999)
	memcpy(pCepasMBBatchFile->debitAmount,BATCHDEBITAMOUNT,10);
	//RFU (Value '0000000000')
	memcpy(pCepasMBBatchFile->RFU,RFU,10);
	//Batch Auto Load Amount (Total auto-load amount for all debit with Auto-Load TRANSACTION DATA records in the file in unit
	//							of 1 cent)
	memcpy(pCepasMBBatchFile->autoLoadAmount,BATCHAUTOLOADAMOUNT,10);
	//CRC-16 Checksum (LTA CRC-16 of the whole file starting from record indicator except itself)
	memcpy(pCepasMBBatchFile->crc16, CRC,2);

	//padding
	memset(pCepasMBBatchFile->padding,0,33);

	pCepasMBBatchFile->pTransactionDetail = NULL;
	pCepasMBBatchFile->pLastTransactionDetail = NULL;
}

void mlsAddRecordtoMBBatchFile(CepasMDBatchFile *pCepasMBBatchFile,MDTransactionRecord *pMDTransRecord)
{
	//if this is the first record
	if(pCepasMBBatchFile->pTransactionDetail == NULL)
	{
		pCepasMBBatchFile->pTransactionDetail = pMDTransRecord;
		pCepasMBBatchFile->pLastTransactionDetail = pMDTransRecord;
	}
	else
	{
		pCepasMBBatchFile->pLastTransactionDetail->pNext = pMDTransRecord;
		pCepasMBBatchFile->pLastTransactionDetail = pMDTransRecord;
	}
	//increase number of record
	sumArrayCharAndInt(pCepasMBBatchFile->numberOfRecord,4,1);
	//increase debit amount
	int iAmount =0;
	mlsConvertToIntKeepNegative(&iAmount,pMDTransRecord->transactionHeader.transactionAmount,0);
	iAmount = -iAmount;
	sumArrayCharAndInt(pCepasMBBatchFile->debitAmount,10,iAmount);
	//increase auto-load amount
	iAmount = mlsConvertBigEndianToInt(pMDTransRecord->autoLoadAmount,3);
	sumArrayCharAndInt(pCepasMBBatchFile->autoLoadAmount,10,iAmount);
	// CRC-16 :LTA CRC-16 of the whole file starting from record indicator except itself
	UINT16 crc16 = calCRC_16(pCepasMBBatchFile);
	memcpy(pCepasMBBatchFile->crc16,(UINT8*)&crc16,2);
}

int mlsCreateMDBatchFile(char * pPathFile,CepasMDBatchFile *pCepasMDBatchFile)
{
	FILE *fp = fopen(pPathFile,"wb");
	if(fp == NULL)
	{
		return 0;
	}
	int iSize = sizeof(CepasMDBatchFile) - sizeof(pCepasMDBatchFile->pLastTransactionDetail) - sizeof(pCepasMDBatchFile->pTransactionDetail);
	int iTem = fwrite(pCepasMDBatchFile,iSize,1,fp);
	fclose(fp);
	if(iTem ==0)
		return 0;
	return 1;
	
}
int mlsWriteAllMDBatchFile(char * pPathFile,CepasMDBatchFile *pCepasMBBatchFile)
{
	FILE *fp ;
	
	mlsDebugLogSrcInfo("Enter\n");
	
	fp = fopen(pPathFile,"wb");
	int result = 1;
	if(fp == NULL)
	{
		return 0;
	}

	int iSize = sizeof(CepasMDBatchFile) - sizeof(pCepasMBBatchFile->pLastTransactionDetail) - sizeof(pCepasMBBatchFile->pTransactionDetail);
	if(fwrite(pCepasMBBatchFile,iSize,1,fp)==0)
	{
		result = 0;
	}
	MDTransactionRecord *pTemp = pCepasMBBatchFile->pTransactionDetail;
	
	while(pTemp != NULL  && result ==1)
	{
		iSize = sizeof(MDTransactionRecord) - sizeof(pTemp->pNext);
		if(fwrite(pTemp,iSize,1,fp)==0)
		{
			result = 0;
		}
		pTemp = pTemp->pNext;
	}
	fclose(fp);

	mlsDebugLogSrcInfo("Exit with status = %x\n",result);

	return result;
}
int mlsWriteLastMBTransRecord(char *pPathFile,MDTransactionRecord *pTransacRecord)
{
	FILE *fp = fopen(pPathFile,"ab+");
	if(fp == NULL)
	{
		return 0;
	}
	int iSize = sizeof(MDTransactionRecord) - sizeof(pTransacRecord->pNext);
	int iTem = fwrite(pTransacRecord,iSize,1,fp);
	fclose(fp);
	if(iTem ==0)
		return 0;
	return 1;

}
int mlsUpdateHeaderMDBatchFile(char *pPathFile,CepasMDBatchFile *pCepasMDBatchFile)
{
	FILE *fp = fopen(pPathFile,"wb+");
	if(fp == NULL)
	{
		return 0;
	}
	int iSize = sizeof(CepasMDBatchFile) - sizeof(pCepasMDBatchFile->pLastTransactionDetail) - sizeof(pCepasMDBatchFile->pTransactionDetail);
	rewind(fp);
	int iTem = fwrite(pCepasMDBatchFile,iSize,1,fp);
	fclose(fp);
	if(iTem ==0)
		return 0;
	return 1;
}


unsigned short mlsCepasCredit(mlsCard_str *handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char arrTransactionUserData[8],
							  unsigned char arrCreditKey1[16],unsigned char arrCreditKey2[16],unsigned char arrSignedKey[16],
							  KeyIndex CreditKey1,KeyIndex CreditKey2,KeyIndex SigningKey,int iAmount)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrCreditCryptogram[24];
	unsigned char arrCreditCommandData[37];
	unsigned char arrCreditReceiveData[24];
	unsigned char arrCreditSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];
	short iCreditReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;
	//get challenge
	usStatusCode = mlsGetChallenge(handle,abRamdomPICC,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
		return MLSCEPAS_ERROR_GETCHALLENGE;
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Record CRC
	memset(arrCreditCryptogram,0,24);
	memcpy(arrCreditCryptogram,ucTRP,4); 
	arrCreditCryptogram[4] = ucPf;
	arrCreditCryptogram[5] = SigningKey.ucKeyFileID;
	arrCreditCryptogram[6] = SigningKey.ucKeyNo;

	//transaction header;
	arrCreditCryptogram[7] = MLS_DEBIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrCreditCryptogram+8,arrTemp,3);

	memset(arrTemp,0,4);
	UInt32 unixTime = mlsCurrentUnixTime();
	unixTime = SWAP_32(unixTime);
	memcpy(arrCreditCryptogram+11, &unixTime,4);

	//transaction user data
	memcpy(arrCreditCryptogram+15,arrTransactionUserData,8);
	mlsComputeCrc(arrCreditCryptogram,23,arrCRC);

	//create Debit session key
	for(i = 0 ;i < 8;i++)
	{
		abRamdomPICC[8+i] = (unsigned char)mlsRandom();
	}

	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrCreditKey1[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abRamdomPICC,arrCreditSessionKey,16);


	//-------------------------------------------------------------------------------------------
	//--------Credit parameter block
	memcpy(arrTemp,arrCRC,2);
	arrTemp[2] = SigningKey.ucKeyFileID; //receive SKf
	arrTemp[3] = SigningKey.ucKeyNo; //receive SKn
	arrTemp[4] = MLS_CREDIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp+5,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp+5,3);
	mlsCryptoSetContext(&ctx,&arrCreditKey2[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCreditCryptogram+4,8);
	//--------calculate Credit Cryptogram
	memcpy(arrCreditCryptogram,ucTRP,4); // receive TRP

	/*memset(arrTemp,0,4);
	convertIntToArrayUnsignChar(iDateTime,arrTemp,4);*/
	memcpy(arrCreditCryptogram+12,arrDateTime,4);

	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrCreditSessionKey,16);

	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditCryptogram,16);
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Command Data
	arrCreditCommandData[0] = ucPf;//Pf
	arrCreditCommandData[1] = CreditKey1.ucKeyFileID;//Key file SFI
	arrCreditCommandData[2] = CreditKey1.ucKeyNo;//KeyNo
	arrCreditCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	arrCreditCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	memcpy(arrCreditCommandData+5,abRamdomPICC+8,8); //random of PCC
	memcpy(arrCreditCommandData+13,arrCreditCryptogram,16);// Debit Cryptogram
	memcpy(arrCreditCryptogram+29,arrTransactionUserData,8);//Transaction User Data

	usStatusCode = ExecuteCommand(handle,MLS_CREDIT,CreditKey2.ucKeyFileID,CreditKey2.ucKeyNo,0x25,arrCreditCommandData,0x18,arrCreditReceiveData,0x18,&iCreditReceiveLength);

	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
		return MLSCEPAS_ERROR_SENDAPDUCREDIT;
	if( iCreditReceiveLength != 0x18 )
		return MLSCEPAS_ERROR_WRONGLENGTH;

	//--------------------------------------------------------------------------------------
	//-----test signed certificate
	//decrypt counter data
	memcpy(ctx.IV,arrCreditReceiveData+8,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData+16,arrCounterData,8);
	//decrypt purse balance
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData,arrPurseBalance,8);
	//decrypt certificate
	memcpy(ctx.IV,arrCreditReceiveData,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData+8,arrCertificate,8);

	//create session key
	memset(arrCreditCryptogram,0,16);
	arrCreditCryptogram[0] = 0; // debit option
	memcpy(arrCreditCryptogram+1,arrPurseBalance,3); // purse balance
	memcpy(arrCreditCryptogram+4,ucTRP,4);
	memcpy(arrCreditCryptogram+8,arrCounterData,8);
	memcpy(ctx.Key,arrSignedKey,16);
	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditSessionKey,16);
	// get Transaction Header
	memset(arrTemp,0,8);
	arrTemp[0] = MLS_CREDIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp+1,3);
	memcpy(arrTemp+4,arrDateTime,4);
	/*convertIntToArrayUnsignChar(iDateTime,arrTemp+4,4);*/
	mlsCryptoSetContext(&ctx,arrCreditSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
	if(memcmp(arrCertificate,arrCertificateofPCC,0)== 0)
		return MLSCEPAS_SUCCESS;
	return MLSCEPAS_ERROR_COMPARECERTIFICATE;
}

/*************************************************************************************/
/******************* Credit with SAM **************************************************/
unsigned short mlsCepasCreditSAM(mlsCard_str *handleSC,mlsCard_str* handleSAM,SAM_CEPAS_t SAM,unsigned char ucPf,unsigned char* ucTRP,
								 unsigned char* arrTransactionUserData, KeyIndex CreditKey1,KeyIndex CreditKey2,
								 KeyIndex SigningKey,int iAmount,int *PurseBalance)
{
	unsigned short usStatusCode;
	unsigned char abRamdomPICC[16];
	unsigned char arrTemp[8];
	unsigned char arrCreditCryptogram[24];
	unsigned char arrCreditCommandData[37];
	unsigned char arrCreditReceiveData[24];
	unsigned char arrCreditSessionKey[16];
	unsigned char arrCounterData[8];
	unsigned char arrPurseBalance[8];
	unsigned char arrCertificate[8];
	unsigned char arrCertificateofPCC[8];
	unsigned char arrCRC[2];
	short iCreditReceiveLength;
	unsigned char arrDateTime[4];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;
	SYSTEMTIME currentTime;
	int TransDateTime;
	mlsReadPurseRecord PurseData;
	BOOL checkCSC;
	unsigned char arrSignatureSessionKey[16];

	//read CAN and CSN of CEPAS card
	usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,ucPf,0x5f,&PurseData);
	if (usStatusCode != MLSCEPAS_SUCCESS) return MLSCEPAS_ERROR_READPURSENOAUTH;

	//get the current time
	GetSystemTime(&currentTime);
	TransDateTime = CalculateDateTime(currentTime,1995);
	
/////////////Check CSC validation
	checkCSC = CSCCheckPurseExpiry(PurseData.arrPurseCreationDate,PurseData.arrPurseExpiryDate,TransDateTime/86400);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSEEXPIRY;
	
	checkCSC = CSCCheckPurseStatus(PurseData.ucPurseStatus,PURSE_ENABLE_STATUS);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_PURSESTATUS;
	
	checkCSC = CSCCheckFund(PurseData.arrPurseBalance,iAmount);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_FUND;
	
	checkCSC = CSCCheckCardIssuer(PurseData.arrCAN);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CARDISSUER;
	
	checkCSC = CSCCheckCANDigit(PurseData.arrCAN);
	if (checkCSC == FALSE) return MLSCEPAS_ERROR_CHECK_CAN;
	//CSCCheckBlackList(unsigned char* CAN)
	//CSCCheckAutoloadCategory(unsigned char AutoloadCategory)
	//CSCCheckRefundStatus(unsigned char CardRefundStatus)
	//CSCCheckCardProfileType(unsigned char UserCategory, unsigned char CardProfileTypes)

	//get challenge
	usStatusCode = mlsGetChallenge(handleSC,abRamdomPICC,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
		return MLSCEPAS_ERROR_GETCHALLENGE;
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Record CRC
	memset(arrCreditCryptogram,0,24);
	memcpy(arrCreditCryptogram,ucTRP,4); 
	arrCreditCryptogram[4] = ucPf;
	arrCreditCryptogram[5] = SigningKey.ucKeyFileID;
	arrCreditCryptogram[6] = SigningKey.ucKeyNo;

	//transaction header;
	arrCreditCryptogram[7] = MLS_CREDIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp,3);
	memcpy(arrCreditCryptogram+8,arrTemp,3);

	memset(arrTemp,0,4);
	mlsConvertIntoBigEndianFomat(TransDateTime,arrDateTime,4);
	//mlsGetDateTime(arrDateTime);
	/*iDateTime = getDateTime();
	convertIntToArrayUnsignChar(iDateTime,arrTemp,4);*/
	memcpy(arrCreditCryptogram+11,arrDateTime,4);
	//transaction user data
	memcpy(arrCreditCryptogram+15,arrTransactionUserData,8);
	mlsComputeCrc(arrCreditCryptogram,23,arrCRC);

	//create Debit session key
	for(i = 0 ;i < 8;i++)
	{
		abRamdomPICC[8+i] = (unsigned char)mlsRandom();
	}

	switch (SAM){
		case ezSAM:
			//read CAN and CSN of CEPAS card
			usStatusCode = mlsCepasReadPurseNoAuthen(handleSC,ucPf,0x5f,&PurseData);
			if (usStatusCode == MLSCEPAS_ERROR_READPURSENOAUTH) return MLSCEPAS_ERROR_READPURSENOAUTH;
			//select application
			usStatusCode = mlsEzSAM_SelectApplication(handleSAM);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_SELECTAPPLICATION;
			//enable function
			usStatusCode = mlsEzSAM_EnableApplet(handleSAM,OfflineDebit);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_ENABLEAPPLET;
			//debit session key function
			usStatusCode = mlsEzSAM_CreditSessionKey(handleSAM,abRamdomPICC,
				PurseData.arrCAN,PurseData.arrCSN,arrCreditSessionKey);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_CREDITSESSIONKEY;
			break;
		case netSAM:
			break;
		default:
			break;
	}

	memset(IV,0,8);
	mlsCryptoSetContext(&ctx,&arrCreditSessionKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	
	

	//-------------------------------------------------------------------------------------------
	//--------Credit parameter block
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp+5,3);

	switch (SAM){
		case ezSAM:
			usStatusCode = mlsEzSAM_EncryptedCreditParameterBlock(handleSAM,arrCRC,SigningKey.ucKeyFileID,
														SigningKey.ucKeyNo,MLS_CREDIT_TYPE,arrTemp,
														PurseData.arrCAN,PurseData.arrCSN,
														arrCreditCryptogram+4);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_ENCRYPTCREDITBLOCK;
			break;
		case netSAM:
			break;
		default:
			break;
	}

	//--------calculate Credit Cryptogram
	memcpy(arrCreditCryptogram,ucTRP,4); // receive TRP

	/*memset(arrTemp,0,4);
	convertIntToArrayUnsignChar(iDateTime,arrTemp,4);*/
	memcpy(arrCreditCryptogram+12,arrDateTime,4);

	//set ctx.key from arrDebitSessionKey
	memcpy(ctx.Key,arrCreditSessionKey,16);

	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditCryptogram,16);
	//--------------------------------------------------------------------------------------
	//-------calculate Debit Command Data
	arrCreditCommandData[0] = ucPf;//Pf
	arrCreditCommandData[1] = CreditKey1.ucKeyFileID;//Key file SFI
	arrCreditCommandData[2] = CreditKey1.ucKeyNo;//KeyNo
	arrCreditCommandData[3] = SigningKey.ucKeyFileID;//Signing Key file
	arrCreditCommandData[4] = SigningKey.ucKeyNo;//KeyNo of Signing Key
	memcpy(arrCreditCommandData+5,abRamdomPICC+8,8); //random of PCC
	memcpy(arrCreditCommandData+13,arrCreditCryptogram,16);// Debit Cryptogram
	memcpy(arrCreditCryptogram+29,arrTransactionUserData,8);//Transaction User Data

	usStatusCode = ExecuteCommand(handleSC,MLS_CREDIT,CreditKey2.ucKeyFileID,CreditKey2.ucKeyNo,0x25,arrCreditCommandData,0x18,arrCreditReceiveData,0x18,&iCreditReceiveLength);

	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
		return MLSCEPAS_ERROR_SENDAPDUCREDIT;
	if( iCreditReceiveLength != 0x18 )
		return MLSCEPAS_ERROR_WRONGLENGTH;

	//--------------------------------------------------------------------------------------
	//-----test signed certificate
	//decrypt counter data
	memcpy(ctx.IV,arrCreditReceiveData+8,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData+16,arrCounterData,8);
	//decrypt purse balance
	memcpy(ctx.IV,arrCounterData,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData,arrPurseBalance,8);
	//decrypt certificate
	memcpy(ctx.IV,arrCreditReceiveData,8);
	mlsCrypto_Decrypt(&ctx,arrCreditReceiveData+8,arrCertificate,8);

	//create signing session key


	memset(arrCreditCryptogram,0,16);
	arrCreditCryptogram[0] = 0; // debit option
	memcpy(arrCreditCryptogram+1,arrPurseBalance,3); // purse balance
	//get purse balance
	mlsConvertThreeByteToInt(PurseBalance,arrPurseBalance,0);
	memcpy(arrCreditCryptogram+4,ucTRP,4);
	memcpy(arrCreditCryptogram+8,arrCounterData,8);

	/////////////////////////////////////////////////////////////////
	/////////// get the signature session key from EzSAM//////////////
	/////////// need to test here ////////////////////////////////////

	switch (SAM){
		case ezSAM:
			usStatusCode = mlsEzSAM_SignatureSessionKey(handleSAM,abRamdomPICC,PurseData.arrCAN,
				PurseData.arrCSN,arrSignatureSessionKey);
			if (usStatusCode == OPERATION_ERROR) return MLSEzSAM_ERROR_SIGNATURESESSIONKEY;
			break;
		case netSAM:
			break;
		default:
			break;
	}

	memcpy(ctx.Key,arrSignatureSessionKey,16);
	mlsCrypto_Encrypt(&ctx,arrCreditCryptogram,arrCreditSessionKey,16);
	// get Transaction Header
	memset(arrTemp,0,8);
	arrTemp[0] = MLS_CREDIT_TYPE;
	mlsConvertIntoBigEndianFomat(iAmount,arrTemp+1,3);
	//convertIntToArrayUnsignChar(iAmount,arrTemp+1,3);
	memcpy(arrTemp+4,arrDateTime,4);
	/*convertIntToArrayUnsignChar(iDateTime,arrTemp+4,4);*/
	mlsCryptoSetContext(&ctx,arrCreditSessionKey,IV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,arrTemp,arrCertificateofPCC,8);
	if(memcmp(arrCertificate,arrCertificateofPCC,0)== 0)
		return MLSCEPAS_SUCCESS;
	return MLSCEPAS_ERROR_COMPARECERTIFICATE;
}

/* mls@dev03 100224 change ReadPurseAuthen prototype to add LtaSAM */
unsigned short mlsCepasReadPurseAuthen(mlsCard_str *handle,
									   mlsCard_str *handleSAM,
									   SAM_CEPAS_t SAM,
									   unsigned char *pTerminalRandomNumber,
									   unsigned char ucPF,
									   KeyIndex readPurKey,
									   short Le, 
									   unsigned char *pCSCChallenge,
									   mlsReadPurseRecord *pReadPurseRecord)
{
	unsigned short usStatusCode;
	unsigned char abReadPurseData[10];
	unsigned char arrRandom[16];
	unsigned char arrBuffer[256];
	short sLength;
	unsigned char arrSessionKey[16];
	unsigned char arrSigCerAndCounterData[16];
	unsigned char IV[8];
	mlsCRYPTO_CONTEXT ctx;
	int i = 0;

	mlsDebugLogSrcInfo("Enter\n");

	//get challenge
	/*usStatusCode = mlsGetChallenge(handle,arrRandom,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
		return MLSCEPAS_ERROR_GETCHALLENGE;
	}*/
	memcpy(arrRandom,pCSCChallenge,8);

	//get key file SFI and Key number
	abReadPurseData[0] = readPurKey.ucKeyFileID;
	abReadPurseData[1] = readPurKey.ucKeyNo;

	//create random number
	/*for(i = 0 ;i < 8;i++)
	{
		abReadPurseData[2+i] = (unsigned char)mlsRandom();
	}*/
	memcpy(&abReadPurseData[2],pTerminalRandomNumber,8);

	//--------------------------------------------------------------------------------------
	usStatusCode = ExecuteCommand(handle,MLS_READPURSEDATA,ucPF,0x00,0x0A,abReadPurseData,Le,arrBuffer,sLength,&sLength);

	mlsDebugLogSrcInfo("READPURSE APDU returns %x\n",usStatusCode);

	if(usStatusCode != MLSCEPAS_SUCCESS_SENDADPU)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_SENDAPDUREADPURSE);
		return MLSCEPAS_ERROR_SENDAPDUREADPURSE;
	}

	convertArrToReadPurseRecord(pReadPurseRecord,arrBuffer,sLength);
	
	switch (SAM){
		case ezSAM:
			//create read purse session key and save in arrTemp
			memcpy(arrRandom + 8,pTerminalRandomNumber,8);
			//select application
			usStatusCode = mlsEzSAM_SelectApplication(handleSAM);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_SELECTAPPLICATION);
				return MLSEzSAM_ERROR_SELECTAPPLICATION;
			}
			//enable function
			usStatusCode = mlsEzSAM_EnableApplet(handleSAM,OfflineDebit);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_ENABLEAPPLET);
				return MLSEzSAM_ERROR_ENABLEAPPLET;
			}
			//debit session key function
			usStatusCode = mlsEzSAM_DebitSessionKey(handleSAM,arrRandom,
				pReadPurseRecord->arrCAN,pReadPurseRecord->arrCSN,arrSessionKey);
			if (usStatusCode == OPERATION_ERROR) 
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSEzSAM_ERROR_DEBITSESSIONKEY);
				return MLSEzSAM_ERROR_DEBITSESSIONKEY;
			}
			//decrypt to read purse data to be Encrypted.
			memcpy(ctx.Key,arrSessionKey,16);
			mlsCrypto_Decrypt(&ctx,arrBuffer + sLength -2 -16,arrSigCerAndCounterData,16);
			memcpy(arrBuffer+sLength -2-16,arrSigCerAndCounterData,16);
			//receive transaction signed certificate
			memcpy(pReadPurseRecord->arrLastTransactionSignedCertificate,arrSigCerAndCounterData,8);
			memcpy(pReadPurseRecord->arrCounterData,&arrSigCerAndCounterData[8],8);
			//convertArrToReadPurseRecord(pReadPurseRecord,arrBuffer,sLength);
			
			break;
		case LtaSAM:
			// TODO use to check decrypt transaction signed certificate and counter data if needed
			break;
		case netSAM:
			unsigned char abDataEncrypt[18];
			unsigned char abDataOut[16];
			unsigned char KeyNo,KSI;
			memcpy(abDataEncrypt,pReadPurseRecord->arrLastTransactionSignedCertificate,8);
			memcpy(&abDataEncrypt[8],pReadPurseRecord->arrCounterData,8);
			memcpy(&abDataEncrypt[16],pReadPurseRecord->arrCounterData,2);	

			switch (ucmlsNetSAMKeyIndex)
			{
			case 0x04:
				KeyNo = 0x03;
				break;
			case 0x05:
				KeyNo = 0x04;
				break;
			default:
				break;
			}
			KSI = *((unsigned char *)&pReadPurseRecord + 71);
			usStatusCode = mlsNetSAM_VerifyReadPurseCryptogram2(handleSAM,
																KeyNo,
																KSI,
																pReadPurseRecord->arrCAN,
																pReadPurseRecord->arrCSN,
																pTerminalRandomNumber,
																pCSCChallenge,
																abDataEncrypt,
																abDataOut);
			if (usStatusCode != OPERATION_OK)
			{
				mlsDebugLogSrcInfo("Exit error %x\n",MLSNetSAM_ERROR_READAUTH);
				return MLSNetSAM_ERROR_READAUTH;
			}
			//receive transaction signed certificate
			memcpy(pReadPurseRecord->arrLastTransactionSignedCertificate,abDataOut,8);
			memcpy(pReadPurseRecord->arrCounterData,&abDataOut[8],8);
			break;
		default:
			break;
	}
	mlsDebugLog("Exit OK status %d\n",MLSCEPAS_SUCCESS);
	return MLSCEPAS_SUCCESS;
}
#endif


unsigned short mlsCepasReadPurseNoAuthen(mlsCard_str * handle,unsigned char ucPF,short sReadPurseRecordLength,
												 mlsReadPurseRecord *pReadPurseRecord)
{
	unsigned short status;
	unsigned char pbResponseBuffer[0xff];
	short sResponseBufferLength;
	status = ExecuteCommand(handle,MLS_READPURSEDATA,ucPF,0,0x00,0,sReadPurseRecordLength,pbResponseBuffer,0xff,&sResponseBufferLength);
	if(status == MLSCEPAS_SUCCESS_SENDADPU)
	{
		convertArrToReadPurseRecord(pReadPurseRecord,pbResponseBuffer,sResponseBufferLength);
		return MLSCEPAS_SUCCESS;
	}
	return MLSCEPAS_ERROR_READPURSENOAUTH;
}

unsigned short mlsCepasReadTransactionLog(mlsCard_str *handle,unsigned char ucPF,unsigned char sRecordOffset,
										  unsigned char sRecordLength,unsigned char *pbResponseBuffer,short *sResponseBufferLength)
{
	unsigned short status = ExecuteCommand(handle,MLS_READPURSEDATA,ucPF,0,0x01,&sRecordOffset,sRecordLength,pbResponseBuffer,
		sRecordLength,sResponseBufferLength);
	if(status == MLSCEPAS_SUCCESS_SENDADPU)
		return MLSCEPAS_SUCCESS;
	return MLSCEPAS_ERROR_READTRANLOG;
}

unsigned short mlsGetChallenge(mlsCard_str *handle,unsigned char * abChallenge,unsigned char length)
{
	unsigned short sStatusCode;
	unsigned char apduCmd[5] = {0x00,0x84,0x00,0x00,0x08};
	unsigned char abBuffer[256];
	int lenResp;
	unsigned short sw1sw2;
	unsigned short retVal;

	mlsDebugLogSrcInfo("Enter \n");
	
	sStatusCode = sendAPDU(handle,apduCmd,5,abBuffer,&lenResp);
	
	if (sStatusCode != 0) return MLSCEPAS_ERROR_GETCHALLENGE;

	if( (lenResp-2) != length )
	{
		mlsDebugLogSrcInfo("Exit status = %x\n",MLSCEPAS_ERROR_WRONGLENGTH);
		return MLSCEPAS_ERROR_WRONGLENGTH;
	}
	memcpy(abChallenge,abBuffer,length);
	
	sw1sw2 = ((abBuffer[lenResp - 2] << 8)| ( abBuffer[ lenResp - 1 ] ));

	if(sw1sw2  == MLSCEPAS_SUCCESS_SENDADPU)
	{
		retVal = MLSCEPAS_SUCCESS;
	}
	else
	{
		retVal = MLSCEPAS_ERROR_GETCHALLENGE;
	}
	
	mlsDebugLogSrcInfo("Exit status = %x\n",retVal);

	return retVal;
}

unsigned short ExecuteCommand(mlsCard_str *handle, unsigned char bINS, unsigned char bP1, unsigned char bP2, 
							  unsigned char wLc, unsigned char * pbData, unsigned char wLe, 
							  unsigned char * pbResponse, int iResponseBufferSize,
							  short * pwResponseLength)
{
	unsigned char abBuffer[256];
	int iBufferLength = 0;
	unsigned short usProtocolStatus;
	unsigned short ucCardStatus;
	memset(abBuffer,0,sizeof(abBuffer));
	inputData(&abBuffer[0],0x90,&iBufferLength); //CLA
	inputData(&abBuffer[0],bINS,&iBufferLength); //INS
	inputData(&abBuffer[0],bP1,&iBufferLength);
	inputData(&abBuffer[0],bP2,&iBufferLength);
	
	//if there is command data which be sent to the PICC
	inputData(&abBuffer[0],wLc,&iBufferLength);// data length
	if(wLc != 0)
	{
		inputArrayData(&abBuffer[0],&iBufferLength,pbData,wLc);
	}

	if(pwResponseLength != NULL)
	{
		if(wLe > 0)
			inputData(&abBuffer[0],wLe,&iBufferLength);
	}
	
	usProtocolStatus = sendAPDU(handle,&abBuffer[0],iBufferLength,&abBuffer[0],&iBufferLength);
	if(usProtocolStatus != 0)
	{
		return usProtocolStatus;
	}

	if( iBufferLength < 2 )//if respond buffer length < 2: error
		return sgwDFCARDOBJ_LENGTH_ERROR;

	if(iBufferLength >= 2) // get to the card status
	{
		ucCardStatus = (unsigned short)((abBuffer[iBufferLength - 2] << 8)| ( abBuffer[ iBufferLength - 1 ] ));
		iBufferLength -= 2;
	}

	if(pwResponseLength !=NULL)
	{
		//	deliver the response length ( excluding card status word ).
		*pwResponseLength = iBufferLength;
	}

	//	If we have been passed a pointer to a buffer where to we can copy the response, we...
	if( ( pbResponse != NULL ) && ( iResponseBufferSize >= (int)iBufferLength ) )
	{
		//  ...deliver the received response in pbResponse.
		memcpy( pbResponse, abBuffer, iBufferLength ) ;
	}

	return ucCardStatus;
}


void convertArrToReadPurseRecord(mlsReadPurseRecord *pReadPurseRecord,unsigned char *pData,unsigned short usDataLength)
{
	int i = 0;
	//get Cerpas version
	if(usDataLength > 0)
		pReadPurseRecord->ucCEPASVersion = pData[0];
	//get Purse Status
	if(usDataLength > 1)
		pReadPurseRecord->ucPurseStatus = pData[1];
	//get Purse Balance
	for(i = 0;i < 3 && i+2 < usDataLength;i++)
	{
		pReadPurseRecord->arrPurseBalance[i] = pData[2+i];
	}
	//get AutoLoad Amount
	for(i = 0;i < 3 && i+5 < usDataLength;i++)
	{
		pReadPurseRecord->arrAutoLoadAmount[i] = pData[5+i];
	}
	//get CAN
	for(i = 0;i < 8 && i+8 < usDataLength;i++)
	{
		pReadPurseRecord->arrCAN[i] = pData[8+i];
	}
	//get CSN
	for(i = 0;i < 8 && i+16 < usDataLength;i++)
	{
		pReadPurseRecord->arrCSN[i] = pData[16+i];
	}
	//get Purse expiry data
	for(i = 0;i < 2 && i+24 < usDataLength;i++)
	{
		pReadPurseRecord->arrPurseExpiryDate[i] = pData[24+i];
	}
	//get Purse creation data
	for(i = 0;i < 2 && i+26 < usDataLength;i++)
	{
		pReadPurseRecord->arrPurseCreationDate[i] = pData[26+i];
	}
	//get Last credit transaction TRP
	for(i = 0;i < 4 && i+28 < usDataLength;i++)
	{
		pReadPurseRecord->arrLastCreditTransactionTRP[i] = pData[28+i];
	}
	//get Last credittransaction header
	for(i = 0;i < 8 && i+32 < usDataLength;i++)
	{
		pReadPurseRecord->arrLastCreditTransactionHeader[i] = pData[28+i];
	}
	//get No. of record in transaction logfile
	if(usDataLength > 40)
		pReadPurseRecord->ucNoOfRecord = pData[40];
	//get Issuer-specific data length
	if(usDataLength > 41)
		pReadPurseRecord->ucIssureSpecificDataLength = pData[41];
	//get Last transaction TRP
	for(i = 0;i < 4 && i+42 < usDataLength;i++)
	{
		pReadPurseRecord->arrLastTransactionTRP[i] = pData[42+i];
	}
	//get Last transaction record
	for(i = 0;i < 16 && i+46 < usDataLength;i++)
	{
		pReadPurseRecord->arrLastTransactionRecord[i] = pData[46+i];
	}
	//get Issuer-specific data
	if(usDataLength > 41)//if have Issuer-specific data length
	{
		for(i = 0;i < pReadPurseRecord->ucIssureSpecificDataLength && i+62 < usDataLength;i++)
		{
			pReadPurseRecord->arrIssurespecificData[i] = pData[62+i];
		}
		//get Last transaction debit options byte
		if(usDataLength > 62+pReadPurseRecord->ucIssureSpecificDataLength)
			pReadPurseRecord->ucLastTransactionDebitOptionsByte = pData[61+pReadPurseRecord->ucIssureSpecificDataLength];
		//get Last transaction signed certificate
		for(i = 0;i < 8 && i+63 +pReadPurseRecord->ucIssureSpecificDataLength < usDataLength;i++)
		{
			pReadPurseRecord->arrLastTransactionSignedCertificate[i] = pData[i+63 +pReadPurseRecord->ucIssureSpecificDataLength];
		}
		//get Counter data
		for(i = 0;i < 8 && i+71 +pReadPurseRecord->ucIssureSpecificDataLength < usDataLength;i++)
		{
			pReadPurseRecord->arrCounterData[i] = pData[i+71 +pReadPurseRecord->ucIssureSpecificDataLength];
		}
		//get ISO/IEC 14443-3 CRC_B
		for(i = 0;i < 2 && i+79 +pReadPurseRecord->ucIssureSpecificDataLength < usDataLength;i++)
		{
			pReadPurseRecord->arrCRC_B[i] = pData[i+79 +pReadPurseRecord->ucIssureSpecificDataLength];
		}
	}

}
void convertIntToArrUnsignChar(int iData,unsigned char *output,int lengthArrByte)
{
	int tmpResult;
	int i;
	for (i = 0; i < lengthArrByte; i++)
	{
		tmpResult = iData >> 8 * i;
		output[i] = (unsigned char)tmpResult;
	}
}
int mlsCardTransmit(
						  mlsCard_str* card,
						  unsigned char* inBuffSend, 
						  int inBuffSendLen, 
						  unsigned char* outBuffRec, 
						  int *inoutBuffRecLen)
{
	LONG SCard_Status;
	*inoutBuffRecLen = 255;
	SCard_Status = SCardTransmit(
		card->cardhandle,
		card->iorequest,
		inBuffSend,
		inBuffSendLen,
		NULL,
		outBuffRec,
		(LPDWORD)inoutBuffRecLen);
	if (SCard_Status != SCARD_S_SUCCESS)
	{
		return 0;
	}else
	{
		return SCARD_S_SUCCESS;
	}
}
unsigned short sendAPDU(mlsCard_str *handle,unsigned char * ucInput,int iInputLength, unsigned char *ucOutPut,int *iOutPutLength)
{
	int status  = mlsCardTransmit(handle,ucInput,iInputLength,ucOutPut,iOutPutLength);
	return status;
}

void inputData(unsigned char * arrData,unsigned char data,int *count)
{
	arrData[*count] = data;
	*count = *count+1;
}
void inputArrayData(unsigned char *arrData,int *iDataLength,unsigned char *input,int inputlength)
{
	int i;
	for(i = 0 ; i < inputlength;i++)
	{
		arrData[*iDataLength+i] = input[i];
	}
	*iDataLength += inputlength;
}
char* getStringError(unsigned short usErrorId)
{
	switch(usErrorId)
	{
	case MLSCEPAS_SUCCESS:
		return "MLSCEPAS_SUCCESS";
	case MLSCEPAS_SUCCESS_SENDADPU:
		return "MLSCEPAS_SUCCESS_SENDADPU";
	case MLSCEPAS_ERROR_CHECK_BACKLIST:
		return "MLSCEPAS_ERROR_CHECK_BACKLIST";
	case MLSCEPAS_ERROR_CHECK_PURSEEXPIRY:
		return "MLSCEPAS_ERROR_CHECK_PURSEEXPIRY";
	case MLSCEPAS_ERROR_CHECK_PURSESTATUS:
		return "MLSCEPAS_ERROR_CHECK_PURSESTATUS";
	case MLSCEPAS_ERROR_CHECK_FUND:
		return "MLSCEPAS_ERROR_CHECK_FUND";
	case MLSCEPAS_ERROR_CHECK_FUNDAUTOLOAD:
		return "MLSCEPAS_ERROR_CHECK_FUNDAUTOLOAD";
	case MLSCEPAS_ERROR_CHECK_AUTOLOADCATEGORY:
		return "MLSCEPAS_ERROR_CHECK_AUTOLOADCATEGORY";
	case MLSCEPAS_ERROR_CHECK_REFUNDSTATUS:
		return "MLSCEPAS_ERROR_CHECK_REFUNDSTATUS";
	case MLSCEPAS_ERROR_CHECK_CARDPROFILETYPE:
		return "MLSCEPAS_ERROR_CHECK_CARDPROFILETYPE";
	case MLSCEPAS_ERROR_CHECK_CARDISSUER:
		return "MLSCEPAS_ERROR_CHECK_CARDISSUER";
	case MLSCEPAS_ERROR_CHECK_CAN:
		return "MLSCEPAS_ERROR_CHECK_CAN";
	case MLSCEPAS_ERROR_READPURSEAUTH:
		return "MLSCEPAS_ERROR_READPURSEAUTH";
	case MLSCEPAS_ERROR_READPURSENOAUTH:
		return "MLSCEPAS_ERROR_READPURSENOAUTH";
	case MLSCEPAS_ERROR_GETCHALLENGE:
		return "MLSCEPAS_ERROR_GETCHALLENGE";
	case MLSCEPAS_ERROR_READTRANLOG:
		return "MLSCEPAS_ERROR_READTRANLOG";
	case MLSCEPAS_ERROR_SENDAPDUDEBIT:
		return "MLSCEPAS_ERROR_SENDAPDUDEBIT";
	case MLSCEPAS_ERROR_SENDAPDUCREDIT:
		return "MLSCEPAS_ERROR_SENDAPDUCREDIT";
	case MLSCEPAS_ERROR_SENDAPDUGETCHALLENGE:
		return "MLSCEPAS_ERROR_SENDAPDUGETCHALLENGE";
	case MLSCEPAS_ERROR_SENDAPDUREADPURSE:
		return "MLSCEPAS_ERROR_SENDAPDUREADPURSE";
	case MLSCEPAS_ERROR_WRONGLENGTH:
		return "MLSCEPAS_ERROR_WRONGLENGTH";
	case MLSCEPAS_ERROR_COMPARECERTIFICATE:
		return "MLSCEPAS_ERROR_COMPARECERTIFICATE";
	
	case MLSEzSAM_ERROR_SELECTAPPLICATION:
		return "MLSEzSAM_ERROR_SELECTAPPLICATION";
	case MLSEzSAM_ERROR_ENABLEAPPLET:
		return "MLSEzSAM_ERROR_ENABLEAPPLET";
	case MLSEzSAM_ERROR_DEBITSESSIONKEY:
		return "MLSEzSAM_ERROR_DEBITSESSIONKEY";
	case MLSEzSAM_ERROR_CREDITSESSIONKEY:
		return "MLSEzSAM_ERROR_CREDITSESSIONKEY";
	case MLSEzSAM_ERROR_SIGNATURESESSIONKEY:
		return "MLSEzSAM_ERROR_SIGNATURESESSIONKEY";
	case MLSEzSAM_ERROR_ENCRYPTCREDITBLOCK:
		return "MLSEzSAM_ERROR_ENCRYPTCREDITBLOCK";

	case MLSLtaSAM_ERROR_SELECTAPPLICATION:
		return "MLSLtaSAM_ERROR_SELECTAPPLICATION";		
	case MLSLtaSAM_ERROR_SAMSTATUS:
		return "MLSLtaSAM_ERROR_SAMSTATUS";				
	case MLSLtaSAM_ERROR_INITIALISEUPDATE:
		return "MLSLtaSAM_ERROR_INITIALISEUPDATE";		
	case MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE:
		return "MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE";	
	case MLSLtaSAM_ERROR_REQUESTMARRIAGE:
		return "MLSLtaSAM_ERROR_REQUESTMARRIAGE";			
	case MLSLtaSAM_ERROR_MUTUALAUTHPROCESS:
		return "MLSLtaSAM_ERROR_MUTUALAUTHPROCESS";		
	case MLSLtaSAM_ERROR_GETTERMINALRANDOMNUMBER:
		return "MLSLtaSAM_ERROR_GETTERMINALRANDOMNUMBER";	
	case MLSLtaSAM_ERROR_REQUESTAUTHENTICATE:
		return "MLSLtaSAM_ERROR_REQUESTAUTHENTICATE";		
	case MLSLtaSAM_ERROR_REQUESTDEBIT	:
		return "MLSLtaSAM_ERROR_REQUESTDEBIT";		
	case MLSLtaSAM_ERROR_REQUESTSIGNCERTIFICATE	:
		return "MLSLtaSAM_ERROR_REQUESTSIGNCERTIFICATE";

	case MLSNetSAM_ERROR_INIT_SELECTDF:
		return "MLSNetSAM_ERROR_INIT_SELECTDF";			
	case MLSNetSAM_ERROR_INIT_SELECTEF01:
		return "MLSNetSAM_ERROR_INIT_SELECTEF01";			
	case MLSNetSAM_ERROR_INIT_SELECTEF01_CHKSUM	:
		return "MLSNetSAM_ERROR_INIT_SELECTEF01_CHKSUM";
	case MLSNetSAM_ERROR_INIT_SELECTEF02:
		return "MLSNetSAM_ERROR_INIT_SELECTEF02";			
	case MLSNetSAM_ERROR_INIT_SELECTEF09:
		return "MLSNetSAM_ERROR_INIT_SELECTEF09";			
	case MLSNetSAM_ERROR_INIT_SAMLOCK:
		return "MLSNetSAM_ERROR_INIT_SAMLOCK";			
	case MLSNetSAM_ERROR_INIT_ACTIVATECMD:
		return "MLSNetSAM_ERROR_INIT_ACTIVATECMD";		
	case MLSNetSAM_ERROR_INIT_ACTIVATERESP:
		return "MLSNetSAM_ERROR_INIT_ACTIVATERESP";		
	case MLSNetSAM_ERROR_POSSESSION_VTCCMD:
		return "MLSNetSAM_ERROR_POSSESSION_VTCCMD";		
	case MLSNetSAM_ERROR_POSSESSION_VTCRESP	:
		return "MLSNetSAM_ERROR_POSSESSION_VTCRESP";	
	case MLSNetSAM_ERROR_POSSESSION_VSC	:
		return "MLSNetSAM_ERROR_POSSESSION_VSC";		
	case MLSNetSAM_ERROR_POSSESSIONESTABLISH:
		return "MLSNetSAM_ERROR_POSSESSIONESTABLISH";		
	case MLSNetSAM_ERROR_READAUTH_VRPCCMD:
		return "MLSNetSAM_ERROR_READAUTH_VRPCCMD";		
	case MLSNetSAM_ERROR_READAUTH_VRPCRESP:
		return "MLSNetSAM_ERROR_READAUTH_VRPCRESP";		
	case MLSNetSAM_ERROR_READAUTH:
		return "MLSNetSAM_ERROR_READAUTH";				
	case MLSNetSAM_ERROR_DEBIT_GDC2CMD:
		return "MLSNetSAM_ERROR_DEBIT_GDC2CMD";			
	case MLSNetSAM_ERROR_DEBIT_GDC2RESP:
		return "MLSNetSAM_ERROR_DEBIT_GDC2RESP";			
	case MLSNetSAM_ERROR_DEBIT_GDC2:
		return "MLSNetSAM_ERROR_DEBIT_GDC2";				
	case MLSNetSAM_ERROR_DEBIT_VDRC2CMD	:
		return "MLSNetSAM_ERROR_DEBIT_VDRC2CMD";		
	case MLSNetSAM_ERROR_DEBIT_VDRC2RESP:
		return "MLSNetSAM_ERROR_DEBIT_VDRC2RESP";			
	case MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION:
		return "MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION";	
	case MLSNetSAM_ERROR_DEBIT_VDRC2:
		return "MLSNetSAM_ERROR_DEBIT_VDRC2";
	case MLSNetSAM_ERROR_DEBIT_EXCEPTION:
		return "MLSNetSAM_ERROR_DEBIT_EXCEPTION";
	case MLSNetSAM_ERROR_DEBIT_CMPTOKEN:
		return "MLSNetSAM_ERROR_DEBIT_CMPTOKEN";			
	case MLSNetSAM_ERROR_DEBIT_CONTROLBYTE:
		return "MLSNetSAM_ERROR_DEBIT_CONTROLBYTE";		
	case MLSNetSAM_ERROR_EXCEPTION_CHKCONDITION:
		return "MLSNetSAM_ERROR_EXCEPTION_CHKCONDITION";	
	case MLSNetSAM_ERROR_EXCEPTION_VRPC2CMP:
		return "MLSNetSAM_ERROR_EXCEPTION_VRPC2CMP";		
	case MLSNetSAM_ERROR_EXCEPTION_VRPC2RESP:
		return "MLSNetSAM_ERROR_EXCEPTION_VRPC2RESP"; 	

	default:
		return "ERROR";
	}
}