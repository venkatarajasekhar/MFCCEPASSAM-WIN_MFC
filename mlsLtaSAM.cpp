#include "mlsLtaSAM.h"
#include "mlsCepas.h"
#include "mlsDebug.h"

/* public variable */
bool mlsLtaSAMMutualAuthFlag = FALSE;
/* private variable */

static unsigned char _abCMAC[8];
static unsigned char _abMarriageKey[16];
static unsigned char _abKeyData[10];
static unsigned char _abSequenceCounter[2];

/*
 *	Select the application used by the LTA SAM
 */
unsigned short mlsLtaSAM_SelectApplication(mlsCard_str *handleSAM)
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
	apduSAMCmd[4] = 0x09; 
	apduSAMCmd[5] = 0x90;
	apduSAMCmd[6] = 0x00;
	apduSAMCmd[7] = 0x05;
	apduSAMCmd[8] = 0x00;
	apduSAMCmd[9] = 0x00;
	apduSAMCmd[10] = 0x95;
	apduSAMCmd[11] = 0x53;
	apduSAMCmd[12] = 0x55;
	apduSAMCmd[13] = 0x59;
	
	Status = sendAPDU(handleSAM,apduSAMCmd,14,apduSAMResp,&lenSAMResp);
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
/*
 *	Provide the status information of the SAM authentication and marriage state
 */
unsigned short mlsLtaSAM_RequestSAMStatus(mlsCard_str *handleSAM, 
										  unsigned char Operation, 
										  unsigned char *pStatusInformation)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x7E;
	apduSAMCmd[2] = Operation;//0x00 Debit Status; 0x01 Credit Status
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x3A; 

	Status = sendAPDU(handleSAM,apduSAMCmd,5,apduSAMResp,&lenSAMResp);
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

	memcpy(pStatusInformation,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

/*
 *	initiates a Secure Channel Sessin and establishes the session keys
 */
unsigned short mlsLtaSAM_InitialiseUpdate(mlsCard_str *handleSAM, 
										  unsigned char *hostChallenge, 
										  unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x50;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x08; 
	memcpy(apduSAMCmd+5, hostChallenge, 8);
	apduSAMCmd[13] = 0x00;

	Status = sendAPDU(handleSAM,apduSAMCmd,14,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

unsigned short mlsLtaSAM_ExternalAuthenticate(mlsCard_str *handleSAM,
											  unsigned char *hostCryptogram,
											  unsigned char *pCMAC
											  )
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;
	unsigned char abIV[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abKey[8];//TODO Set key for CMAC

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x84;
	apduSAMCmd[1] = 0x82;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x10; 
	memcpy(apduSAMCmd+5, hostCryptogram, 8);
	memcpy(apduSAMCmd+13, pCMAC, 8);

	Status = sendAPDU(handleSAM,apduSAMCmd,21,apduSAMResp,&lenSAMResp);
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

/*
 *	Used to marry or obtain the usage of the SAM
 */

unsigned short mlsLtaSAM_RequestForMarriage(mlsCard_str *handleSAM,
											unsigned char *TerminalID,
											unsigned char *MarriageCryptogram)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x84;
	apduSAMCmd[1] = 0x3A;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x10; 
	memcpy(apduSAMCmd+5, TerminalID, 8);
	memcpy(apduSAMCmd+13, MarriageCryptogram, 8);

	Status = sendAPDU(handleSAM,apduSAMCmd,21,apduSAMResp,&lenSAMResp);
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

/*
 *	Used to obtain the audit register of the SAM
 */
unsigned short mlsLtaSAM_RequestForAuditRegister(mlsCard_str *handleSAM,
												 unsigned char *pAuditDataTime,
												 unsigned char AuditReason,
												 unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x30;
	apduSAMCmd[2] = 0x00;//Debit
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x10; 
	memcpy(apduSAMCmd+5, pAuditDataTime,4);
	apduSAMCmd[9] = AuditReason;
	memset(apduSAMCmd+10,0,11);//reseved - set to 00
	apduSAMCmd[21] = 0x5F;

	
	lenSAMCmd = 22;
		
	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

/*
 * Get terminal random number	
 */
unsigned short mlsLtaSAM_GetTerminalRandomNumber(mlsCard_str *handleSAM,
												 unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x84;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x00;
	apduSAMCmd[5] = 0x3B;
	
	Status = sendAPDU(handleSAM,apduSAMCmd,6,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}
										
/*
 * 
 */
unsigned short mlsLtaSAM_RequestForAuthentication(mlsCard_str *handleSAM,
												unsigned char *pCardRandomNumber,
												mlsReadPurseRecord *PurseRecord,
												unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,i;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x32;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 48+8+113;//113 byte of Purse record
	for (i = 0; i<48 ; i++)
	{
		apduSAMCmd[5+i] = 0x00;
	}
	memcpy(&apduSAMCmd[53],pCardRandomNumber,8);
	memcpy(&apduSAMCmd[61],PurseRecord,113);
	apduSAMCmd[113+61] = 0x42;

	Status = sendAPDU(handleSAM,apduSAMCmd,175,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

/*
 *	Request for Debit
 */
unsigned short mlsLtaSAM_RequestForDebit(mlsCard_str *handleSAM, 
										 unsigned char *pDataIn,
										 unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256],apduSAMResp[256],abDataOut[256];
	unsigned char abKDCmac[16],abKDCmacCDA[16],abSKUmacSDD[16],abSKUmac[16];
	int lenSAMResp,i;
	unsigned short Status,usStatusCode;
	unsigned char abIV[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abKey[8];//TODO Set key for CMAC
	mlsCRYPTO_CONTEXT ctx;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x34;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x60;

	memcpy(&apduSAMCmd[5],pDataIn,48+40);

	memcpy(abIV,_abCMAC,8);
	//KDCmac card derivation data
	memcpy(abKDCmacCDA,&_abKeyData[4],6);
	abKDCmacCDA[6] = 0xF0;
	abKDCmacCDA[7] = 0x02;
	memcpy(&abKDCmacCDA[8],&_abKeyData[4],6);
	abKDCmacCDA[14] = 0xF0;
	abKDCmacCDA[15] = 0x02;
	//Calculate KDCmac
	mlsCryptoSetContext(&ctx,_abMarriageKey,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,abKDCmacCDA,abKDCmac,16);
	//SKUenc session derivation data
	abSKUmacSDD[0] = 0x01;
	abSKUmacSDD[1] = 0x01;
	memcpy(&abSKUmacSDD[2],_abSequenceCounter,2);
	for (i = 0;i<12;i++)
	{
		abSKUmacSDD[4+i] = 0x00;
	}
	//Calculate SKUmac
	mlsCryptoSetContext(&ctx,abKDCmac,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abSKUmacSDD,abSKUmac,16);

	CalculateRetailMac(_abCMAC,abSKUmac, apduSAMCmd, 93, _abCMAC);

	// continue to fill apdu command
	memcpy(&apduSAMCmd[93],_abCMAC,8);
	apduSAMCmd[101] = 0x42;

	Status = sendAPDU(handleSAM,apduSAMCmd,102,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

/*
 *	Used to obtain the Receipt Record after a successful Transaction
 */
unsigned short mlsLtaSAM_RequestForSignCertification(mlsCard_str *handleSAM,
													 unsigned char *ReceiptCryptogram,
													 unsigned char *pDataOut)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,i;
	unsigned short Status,usStatusCode;

	mlsDebugLogSrcInfo("Enter\n");

	//select application
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x38;
	apduSAMCmd[2] = 0x00;//Debit
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x48;

	for (i = 0; i<48 ; i++)
	{
		apduSAMCmd[5+i] = 0x00;
	}

	memcpy(&apduSAMCmd[53],ReceiptCryptogram,24);

	apduSAMCmd[77] = 0x18;

	Status = sendAPDU(handleSAM,apduSAMCmd,78,apduSAMResp,&lenSAMResp);
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

	memcpy(pDataOut,apduSAMResp,lenSAMResp-2);

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}
/*
 *	Host and SAM mutual authentication process
 */

unsigned short mlsLtaSAM_MutualAuthenticateProcess(mlsCard_str *handleSAM,
												   unsigned char *abKey,
												   unsigned char *abTerminalID,
												   bool bTransportKeyFlag
												   )
{
#if !defined (MLSLTASAM_MUTUALAUTHENTICATE_TEST)
	unsigned short usStatusCode;
	mlsCRYPTO_CONTEXT ctx;
	int randnr,i;
	unsigned char abHostChallenge[8];
	unsigned char abHostCryptogram[8];
	unsigned char abDataOut[256],abDataIn[256];
	unsigned char abIV[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abMarriagePlain[16],abMarriageCryptogram[16];
	unsigned char abSAMChallenge[6],abSAMCryptogram[8];
	unsigned char abKDCenc[16],abKDCencCDA[16],abSKUencSDD[16],abSKUenc[16];
	unsigned char abKDCmac[16],abKDCmacCDA[16],abSKUmacSDD[16],abSKUmac[16];
	unsigned char abPadding[8] = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abKMC[16];

	memcpy(abKMC,abKey,16);
	
	mlsDebugLogSrcInfo("Enter\n");

	usStatusCode = mlsLtaSAM_SelectApplication(handleSAM);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_SELECTAPPLICATION);
		return MLSLtaSAM_ERROR_SELECTAPPLICATION;
	}
	/* request SAM Status if mutual authenticate by transport key */
	if (bTransportKeyFlag == TRUE)
	{
		usStatusCode = mlsLtaSAM_RequestSAMStatus(  handleSAM, 
													0x00,//0x00: debit status 
													abDataOut);
		if (usStatusCode == OPERATION_ERROR)
		{
			mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_SAMSTATUS);
			return MLSLtaSAM_ERROR_SAMSTATUS;
		}
		if ((abDataOut[2] == 0x01)&&//Marriage Flag is set
			(abDataOut[1] == 0x01))	//Device Authentication State is set
		{
			return OPERATION_OK;
		}

	}
	/* Get host challenge */
	randnr = (unsigned char)mlsRandom();
	abHostChallenge[0] = (unsigned char)((randnr<<24)>>24);
	abHostChallenge[1] = (unsigned char)((randnr<<16)>>24);
	abHostChallenge[2] = (unsigned char)((randnr<<8)>>24);
	abHostChallenge[3] = (unsigned char)((randnr)>>24);
	randnr = (unsigned char)mlsRandom();
	abHostChallenge[4] = (unsigned char)((randnr<<24)>>24);
	abHostChallenge[5] = (unsigned char)((randnr<<16)>>24);
	abHostChallenge[6] = (unsigned char)((randnr<<8)>>24);
	abHostChallenge[7] = (unsigned char)((randnr)>>24);

	/* Initialise Update */
	usStatusCode = mlsLtaSAM_InitialiseUpdate(handleSAM, abHostChallenge,abDataOut);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_INITIALISEUPDATE);
		return MLSLtaSAM_ERROR_INITIALISEUPDATE;
	}
	
	/*Caculate PSAM session keys*/
	memcpy(_abKeyData,&abDataOut[0],10);
	memcpy(_abSequenceCounter,&abDataOut[12],2);
	memcpy(abSAMChallenge,&abDataOut[14],6);
	memcpy(abSAMCryptogram,&abDataOut[20],8);

	//KDCenc Card derivation data
	memcpy(abKDCencCDA,&_abKeyData[4],6);
	abKDCencCDA[6] = 0xF0;
	abKDCencCDA[7] = 0x01;
	memcpy(&abKDCencCDA[8],&_abKeyData[4],6);
	abKDCencCDA[14] = 0x0F;
	abKDCencCDA[15] = 0x01;
	//Calculate KDCenc 
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,abKDCencCDA,abKDCenc,16);
	//SKUenc session derivation data
	abSKUencSDD[0] = 0x01;
	abSKUencSDD[1] = 0x82;
	memcpy(&abSKUencSDD[2],_abSequenceCounter,2);
	for (i = 0;i<12;i++)
	{
		abSKUencSDD[4+i] = 0x00;
	}
	//Calculate SKUenc
	mlsCryptoSetContext(&ctx,abKDCenc,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abSKUencSDD,abSKUenc,16);

	//KDCmac card derivation data
	memcpy(abKDCmacCDA,&_abKeyData[4],6);
	abKDCmacCDA[6] = 0xF0;
	abKDCmacCDA[7] = 0x02;
	memcpy(&abKDCmacCDA[8],&_abKeyData[4],6);
	abKDCmacCDA[14] = 0x0F;
	abKDCmacCDA[15] = 0x02;
	//Calculate KDCmac
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,abKDCmacCDA,abKDCmac,16);
	//SKUenc session derivation data
	abSKUmacSDD[0] = 0x01;
	abSKUmacSDD[1] = 0x01;
	memcpy(&abSKUmacSDD[2],_abSequenceCounter,2);
	for (i = 0;i<12;i++)
	{
		abSKUmacSDD[4+i] = 0x00;
	}
	//Calculate SKUmac

	mlsCryptoSetContext(&ctx,abKDCmac,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abSKUmacSDD,abSKUmac,16);

	//TODO do need to verify SAM cryptogram?
	/* verify SAM cryptogram */
/*	memcpy(abDataIn,abHostChallenge,8);
	memcpy(&abDataIn[8],_abSequenceCounter,2);
	memcpy(&abDataIn[10],abSAMChallenge,6);
	memcpy(&abDataIn[16],abPadding,8);
	//TODO check MAC algorithm
	//CalculateCMac(abIV,abSKUenc,CRYPTO_TRIPLEDES_EDE2,abDataIn , 24, abDataOut);
	//Compare SAM cryptogram from card and host
	if (memcmp(abSAMCryptogram,abDataOut,8)!=0)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_SAMCRYPTOGRAM);
		return MLSLtaSAM_ERROR_SAMCRYPTOGRAM;
	}
*/
	/* calculate Host cryptogram */
	memcpy(abDataIn,_abSequenceCounter,2);
	memcpy(&abDataIn[2],abSAMChallenge,6);
	memcpy(&abDataIn[8],abHostChallenge,8);
	memcpy(&abDataIn[16],abPadding,8);
	mlsCryptoSetContext(&ctx,abSKUenc,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abDataIn,abDataOut,24);
	memcpy(abHostCryptogram,&abDataOut[16],8);//last 8 byte of LSB Result
	
	/* calculate CMAC */
	abDataIn[0] = 0x84;
	abDataIn[1] = 0x82;
	abDataIn[2] = 0x00;
	abDataIn[3] = 0x00;
	abDataIn[4] = 0x10; 
	memcpy(&abDataIn[5],abHostCryptogram, 8);
	memcpy(&abDataIn[13],abPadding,3);
	CalculateRetailMac(abIV, abSKUmac, abDataIn, 16, _abCMAC);

	usStatusCode = mlsLtaSAM_ExternalAuthenticate(handleSAM, abHostCryptogram, _abCMAC);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE);
		return MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE;
	}
	/* Request for Mabiage */
	if (bTransportKeyFlag == TRUE){
		memcpy(&abKMC[0],_abSequenceCounter,2);
		memcpy(&abKMC[2],abSAMChallenge,6);
		memcpy(&abKMC[8],abHostChallenge,8);
	}
	
	memcpy(&abMarriagePlain[0],&abHostChallenge[0],2);
	memcpy(&abMarriagePlain[2],&abSAMChallenge[4],2);
	memcpy(&abMarriagePlain[4],abTerminalID,8);
	memcpy(&abMarriagePlain[12],&abHostChallenge[6],2);
	memcpy(&abMarriagePlain[14],_abSequenceCounter,2);
	
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abMarriagePlain,abDataOut,16);
	memcpy(abMarriageCryptogram,&abDataOut[8],8);

	usStatusCode = mlsLtaSAM_RequestForMarriage(handleSAM,abTerminalID,&abMarriageCryptogram[8]);		
	if (usStatusCode == OPERATION_ERROR)
	{ 
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_REQUESTMARRIAGE);
		return MLSLtaSAM_ERROR_REQUESTMARRIAGE;
	}

	if (bTransportKeyFlag == TRUE)
	{
		memcpy(_abMarriageKey,abKMC,16);
		memcpy(abKey,abKMC,16);
	}

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);
	return OPERATION_OK;
#else
	unsigned short usStatusCode;
	mlsCRYPTO_CONTEXT ctx;
	int randnr,i;
	unsigned char abHostChallenge[8]={0x41,0x95,0xd9,0x1d,0x5e,0x60,0x75,0x86};
	unsigned char abHostCryptogram[8];
	unsigned char abDataOut[256] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
									 0x00,0x00,0x00,0x02,0x00,0x23,0x5d,0xe9,
									 0xb8,0x09,0x52,0xe8,0x1b,0x59,0xa0,0x51,
									 0xd2,0xa7,0xa9,0x59,0x90,0x00};
	unsigned char abDataIn[256];
	unsigned char abTerminalID[8]={0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31};
	unsigned char abIV[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abMarriagePlain[16],abMarriageCryptogram[8];
	unsigned char abSAMChallenge[6] = {0x5d,0xe9,0xb8,0x09,0x52,0xe8};
	unsigned char abSAMCryptogram[8];
	unsigned char abKDCenc[16],abKDCencCDA[16],abSKUencSDD[16],abSKUenc[16];
	unsigned char abKDCmac[16],abKDCmacCDA[16],abSKUmacSDD[16],abSKUmac[16];
	unsigned char abPadding[8] = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char abKMC[16];//TO DO must be set value

	memcpy(abKMC,abKey,16);
	
	mlsDebugLogSrcInfo("Enter\n");

	/*usStatusCode = mlsLtaSAM_SelectApplication(handleSAM);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_SELECTAPPLICATION);
		return MLSLtaSAM_ERROR_SELECTAPPLICATION;
	}*/
	/* Get host challenge */
	/*randnr = (unsigned char)mlsRandom();
	abHostChallenge[0] = (unsigned char)((randnr<<24)>>24);
	abHostChallenge[1] = (unsigned char)((randnr<<16)>>24);
	abHostChallenge[2] = (unsigned char)((randnr<<8)>>24);
	abHostChallenge[3] = (unsigned char)((randnr)>>24);
	randnr = (unsigned char)mlsRandom();
	abHostChallenge[4] = (unsigned char)((randnr<<24)>>24);
	abHostChallenge[5] = (unsigned char)((randnr<<16)>>24);
	abHostChallenge[6] = (unsigned char)((randnr<<8)>>24);
	abHostChallenge[7] = (unsigned char)((randnr)>>24);*/

	/* Initialise Update */
	/*usStatusCode = mlsLtaSAM_InitialiseUpdate(handleSAM, abHostChallenge,abDataOut);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_INITIALISEUPDATE);
		return MLSLtaSAM_ERROR_INITIALISEUPDATE;
	}*/


	
	/*Caculate PSAM session keys*/
	memcpy(_abKeyData,&abDataOut[0],10);
	memcpy(_abSequenceCounter,&abDataOut[12],2);
	memcpy(abSAMChallenge,&abDataOut[14],6);
	memcpy(abSAMCryptogram,&abDataOut[20],8);
	
	//KDCenc Card derivation data
	memcpy(abKDCencCDA,&_abKeyData[4],6);
	abKDCencCDA[6] = 0xF0;
	abKDCencCDA[7] = 0x01;
	memcpy(&abKDCencCDA[8],&_abKeyData[4],6);
	abKDCencCDA[14] = 0x0F;
	abKDCencCDA[15] = 0x01;
	//Calculate KDCenc 
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,abKDCencCDA,abKDCenc,16);
	//SKUenc session derivation data
	abSKUencSDD[0] = 0x01;
	abSKUencSDD[1] = 0x82;
	memcpy(&abSKUencSDD[2],_abSequenceCounter,2);
	for (i = 0;i<12;i++)
	{
		abSKUencSDD[4+i] = 0x00;
	}
	//Calculate SKUenc
	mlsCryptoSetContext(&ctx,abKDCenc,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abSKUencSDD,abSKUenc,16);

	//KDCmac card derivation data
	memcpy(abKDCmacCDA,&_abKeyData[4],6);
	abKDCmacCDA[6] = 0xF0;
	abKDCmacCDA[7] = 0x02;
	memcpy(&abKDCmacCDA[8],&_abKeyData[4],6);
	abKDCmacCDA[14] = 0x0F;
	abKDCmacCDA[15] = 0x02;
	//Calculate KDCmac
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
	mlsCrypto_Encrypt(&ctx,abKDCmacCDA,abKDCmac,16);
	//SKUenc session derivation data
	abSKUmacSDD[0] = 0x01;
	abSKUmacSDD[1] = 0x01;
	memcpy(&abSKUmacSDD[2],_abSequenceCounter,2);
	for (i = 0;i<12;i++)
	{
		abSKUmacSDD[4+i] = 0x00;
	}
	//Calculate SKUmac

	mlsCryptoSetContext(&ctx,abKDCmac,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abSKUmacSDD,abSKUmac,16);

	//TODO do need to verify SAM cryptogram?
	/* verify SAM cryptogram */
/*	memcpy(abDataIn,abHostChallenge,8);
	memcpy(&abDataIn[8],_abSequenceCounter,2);
	memcpy(&abDataIn[10],abSAMChallenge,6);
	memcpy(&abDataIn[16],abPadding,8);
	//TODO check ac(abIV,abSKUenc,CRYPTO_TRIPLEDES_EDE2,abDataIn , 24, abDataOut);
	//Compare SAM cryptogram from card and host
	if (memcmp(abIV,abSAMCryptogram,abDataOut,8)!=0)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_SAMCRYPTOGRAM);
		return MLSLtaSAM_ERROR_SAMCRYPTOGRAM;
	}
*/
	/* calculate Host cryptogram */
	memcpy(abDataIn,_abSequenceCounter,2);
	memcpy(&abDataIn[2],abSAMChallenge,6);
	memcpy(&abDataIn[8],abHostChallenge,8);
	memcpy(&abDataIn[16],abPadding,8);
	//TODO check MAC algorithm
	//CalculateCMac(abIV,abSKUenc,CRYPTO_TRIPLEDES_EDE2,abDataIn , 24, abHostCryptogram);
	mlsCryptoSetContext(&ctx,abSKUenc,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abDataIn,abDataOut,24);
	memcpy(abHostCryptogram,&abDataOut[16],8);//last 8 byte of LSB Result
	
	/* calculate CMAC */
	abDataIn[0] = 0x84;
	abDataIn[1] = 0x82;
	abDataIn[2] = 0x00;
	abDataIn[3] = 0x00;
	abDataIn[4] = 0x10; 
	memcpy(&abDataIn[5],abHostCryptogram, 8);
	memcpy(&abDataIn[13],abPadding,3);
	//TODO check MAC algorithm
	//CalculateCMac(abIV,abSKUenc,CRYPTO_TRIPLEDES_EDE2,abDataIn , 16, _abCMAC);
	CalculateRetailMac(abSKUmac, abDataIn, 16, _abCMAC);
	

	/*usStatusCode = mlsLtaSAM_ExternalAuthenticate(handleSAM, abHostCryptogram, _abCMAC);
	if (usStatusCode == OPERATION_ERROR)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE);
		return MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE;
	}*/
	/* Request for Marriage */
	//memcpy(&_abMarriageKey[0],_abSequenceCounter,2);
	//memcpy(&_abMarriageKey[2],abSAMChallenge,6);
	//memcpy(&_abMarriageKey[8],abHostChallenge,8);

	memcpy(&abMarriagePlain[0],&abHostChallenge[0],2);
	memcpy(&abMarriagePlain[2],&abSAMChallenge[4],2);
	memcpy(&abMarriagePlain[4],abTerminalID,8);
	memcpy(&abMarriagePlain[12],&abHostChallenge[6],2);
	memcpy(&abMarriagePlain[14],_abSequenceCounter,2);
	
	mlsCryptoSetContext(&ctx,abKMC,abIV,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,abMarriagePlain,abDataOut,16);
	memcpy(abMarriageCryptogram,&abDataOut[8],8);

	/*usStatusCode = mlsLtaSAM_RequestForMarriage(handleSAM,abTerminalID,&abMarriageCryptogram[8]);		
	if (usStatusCode == OPERATION_ERROR)
	{ 
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSLtaSAM_ERROR_REQUESTMARRIAGE);
		return MLSLtaSAM_ERROR_REQUESTMARRIAGE;
	}*/

	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);
	return OPERATION_OK;
#endif
}
