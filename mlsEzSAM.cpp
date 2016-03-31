#include "mlsEzSAM.h"
#include "mlsCepas.h"
#include "mlsDebug.h"


unsigned short mlsEzSAM_SelectApplication(mlsCard_str *handleSAM)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;
	
	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x00;
	apduSAMCmd[1] = 0xA4;
	apduSAMCmd[2] = 0x04;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x07; //mls@dev01 : command length
	apduSAMCmd[5] = 0x88;
	apduSAMCmd[6] = 0x77;
	apduSAMCmd[7] = 0x66;
	apduSAMCmd[8] = 0x55;
	apduSAMCmd[9] = 0x44;
	apduSAMCmd[10] = 0x33;
	apduSAMCmd[11] = 0x22;
	Status = sendAPDU(handleSAM,apduSAMCmd,12,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	
	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	
	
	if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",OPERATION_ERROR);
		return OPERATION_ERROR;
	}


	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;

}
unsigned short mlsEzSAM_EnableApplet(mlsCard_str* handleSAM,unsigned char Operator)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;
	unsigned char arrSPCredit[16] = {0xF9,0xDD,0x34,0x77,0xFA,0xC0,0x98,0x2F,
		0x12,0x34,0x5D,0xDF,0x09,0x23,0xED,0x11};
	unsigned char arrSPDebit[16] = {0xF8,0x92,0x34,0x56,0x4a,0xbd,0x89,0x0b,
		0xcd,0xed,0x09,0xff,0xee,0x33,0x22,0x00};
	
	mlsDebugLogSrcInfo("Enter\n");	

	if ((Operator!=0x02)&&(Operator!=0x03))
	{
		mlsDebugLogSrcInfo("Operator Mode is not 0x2 or 0x3 -> Wrong param\n");	
		return OPERATION_ERROR;
	}

	//enable function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = Operator;//offline Debit
	apduSAMCmd[2] = apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x12;
	apduSAMCmd[5] = apduSAMCmd[6] = 0xFF;
	if (Operator==0x02)
	{
		/*offline credit*/
		memcpy(apduSAMCmd+7,arrSPCredit,16);

	}
	else
	{
		/*offline debit*/
		memcpy(apduSAMCmd+7,arrSPDebit,16);
	}

	Status = sendAPDU(handleSAM,apduSAMCmd,23,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("1st APDU returns %x\n", usStatusCode);	

	if (usStatusCode == 0x6105)
	{
		apduSAMCmd[0] = 0x00;
		apduSAMCmd[1] = 0xC0;//Get Response
		apduSAMCmd[2] = 0x00;
		apduSAMCmd[3] = 0x00;
		apduSAMCmd[4] = (unsigned char)(usStatusCode&0xff);
		Status = sendAPDU(handleSAM,apduSAMCmd,5,apduSAMResp,&lenSAMResp);
		if (Status != 0) return OPERATION_ERROR;
		if(lenSAMResp >= 2) 
		{
			usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
		}

		mlsDebugLogSrcInfo("2nd APDU returns %x\n", usStatusCode);	
	}
	else if (usStatusCode != 0x9000) 
	{
		mlsDebugLogSrcInfo("Exit Error return code = %d\n",OPERATION_ERROR);
		return OPERATION_ERROR;

	}

	mlsDebugLogSrcInfo("Exit OK return code = %d\n",OPERATION_OK);

	return OPERATION_OK;
	
}
unsigned short mlsEzSAM_DebitSessionKey(mlsCard_str *handleSAM,unsigned char* arrRandomNumber,
										unsigned char* arrCAN, unsigned char* arrCSN,
										unsigned char* DebitSessionKey)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter \n");	

	//debit session key function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = 0x11;//debit key
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x20;
	memcpy(apduSAMCmd+5,arrRandomNumber,16);
	memcpy(apduSAMCmd+21,arrCAN,8);
	memcpy(apduSAMCmd+29,arrCSN,8);
	Status = sendAPDU(handleSAM,apduSAMCmd,37,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	
	mlsDebugLogSrcInfo("1st APDU statuscode %x\n",usStatusCode);
	
	if (usStatusCode == 0x6110)
	{
		apduSAMCmd[0] = 0x00;
		apduSAMCmd[1] = 0xC0;//Get Response
		apduSAMCmd[2] = 0x00;
		apduSAMCmd[3] = 0x00;
		apduSAMCmd[4] = (unsigned char)(usStatusCode&0xff);
		Status = sendAPDU(handleSAM,apduSAMCmd,5,apduSAMResp,&lenSAMResp);
		if (Status != 0) return OPERATION_ERROR;
		if(lenSAMResp >= 2) 
		{
			usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
		}
		mlsDebugLogSrcInfo("2nd APDU statuscode %x\n",usStatusCode);
	}
	else if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Exit error returns %x\n",OPERATION_ERROR);	
		return OPERATION_ERROR;
	}
	memcpy(DebitSessionKey,apduSAMResp,16);

	mlsDebugLogSrcInfo("Exit OK %x\n",OPERATION_OK);	

	return OPERATION_OK;
}
unsigned short mlsEzSAM_AutoloadSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
										   unsigned char* arrCAN, unsigned char* arrCSN,
										   unsigned char* AutoloadSessionKey)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	//autoload session key function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = 0x13;//autoload key
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x20;
	memcpy(apduSAMCmd+5,arrRandomNumber,16);
	memcpy(apduSAMCmd+21,arrCAN,8);
	memcpy(apduSAMCmd+29,arrCSN,8);
	Status = sendAPDU(handleSAM,apduSAMCmd,37,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	if (usStatusCode != 0x9000) return OPERATION_ERROR;
	memcpy(AutoloadSessionKey,apduSAMResp,16);
	return OPERATION_OK;
}
unsigned short mlsEzSAM_CreditSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
										 unsigned char* arrCAN, unsigned char* arrCSN,
										 unsigned char* CreditSessionKey)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	//autoload session key function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = 0x31;//credit key
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x20;
	memcpy(apduSAMCmd+5,arrRandomNumber,16);
	memcpy(apduSAMCmd+21,arrCAN,8);
	memcpy(apduSAMCmd+29,arrCSN,8);
	Status = sendAPDU(handleSAM,apduSAMCmd,37,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	if (usStatusCode != 0x9000) return OPERATION_ERROR;
	memcpy(CreditSessionKey,apduSAMResp,16);
	return OPERATION_OK;
}
unsigned short mlsEzSAM_EncryptedCreditParameterBlock(mlsCard_str* handleSAM,unsigned char* arrCreditRecordCRC, 
													  unsigned char SKf,unsigned char SKn,
													  unsigned char TransactionType,unsigned char* arrAmount,
													  unsigned char* arrCAN, unsigned char* arrCSN,
													  unsigned char* EncryptedCreditParameterBlock)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	//autoload session key function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = 0x32;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x18;
	memcpy(apduSAMCmd+5,arrCreditRecordCRC,2);
	apduSAMCmd[7] = SKf;
	apduSAMCmd[8] = SKn;
	apduSAMCmd[9] = TransactionType;
	memcpy(apduSAMCmd+10,arrAmount,3);
	memcpy(apduSAMCmd+13,arrCAN,8);
	memcpy(apduSAMCmd+21,arrCSN,8);
	Status = sendAPDU(handleSAM,apduSAMCmd,29,apduSAMResp,&lenSAMResp);
	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	if (usStatusCode != 0x9000) return OPERATION_ERROR;
	memcpy(EncryptedCreditParameterBlock,apduSAMResp,8);
	return OPERATION_OK;

}
unsigned short mlsEzSAM_SignatureSessionKey(
	mlsCard_str* handleSAM,
	unsigned char* arrRandomNumber,
	unsigned char* arrCAN, 
	unsigned char* arrCSN,
	unsigned char* SignatureSessionKey)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	//autoload session key function
	apduSAMCmd[0] = 0x90;
	apduSAMCmd[1] = 0x15;//signature key
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x20;
	memcpy(apduSAMCmd+5,arrRandomNumber,16);
	memcpy(apduSAMCmd+21,arrCAN,8);
	memcpy(apduSAMCmd+29,arrCSN,8);

	Status = sendAPDU(handleSAM,apduSAMCmd,37,apduSAMResp,&lenSAMResp);

	if (Status != 0) return OPERATION_ERROR;
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}
	if (usStatusCode == 0x6110)
	{
		apduSAMCmd[0] = 0x00;
		apduSAMCmd[1] = 0xC0;//Get Response
		apduSAMCmd[2] = 0x00;
		apduSAMCmd[3] = 0x00;
		apduSAMCmd[4] = (unsigned char)(usStatusCode&0xff);
		Status = sendAPDU(handleSAM,apduSAMCmd,5,apduSAMResp,&lenSAMResp);
		if (Status != 0) return OPERATION_ERROR;
		if(lenSAMResp >= 2) 
		{
			usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
		}
	}
	else if (usStatusCode != 0x9000) 
	{
		return OPERATION_ERROR;
	}
	memcpy(SignatureSessionKey,apduSAMResp,16);
	return OPERATION_OK;
}
