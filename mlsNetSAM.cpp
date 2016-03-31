#include "mlsNetSAM.h"
#include "mlsCepas.h"
#include "mlsDebug.h"

unsigned char abmlsNetSAMArrayKeyData[256][4];//array to save key information in Net SAM
unsigned char ucmlsNetSAMKeyIndex;//use to save key present in Net SAM
unsigned char ucmlsNetSAMNoKeyBlock;//number of Key Block in Net SAM
unsigned char abmlsNetSAMKeySetIndex[256];//Key Set Index[] in Net SAM with correct checksum

static unsigned short _mlsNetSAM_GetResponse(mlsCard_str *handleSAM,
											 int Le,
											 unsigned char *pDataOut)
{
	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	/* Get response data from SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x54;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = Le;

	lenSAMCmd = 5;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Error get response from SAM");
		return OPERATION_ERROR;
	}

	memcpy(pDataOut,apduSAMResp,Le);
	return OPERATION_OK;
}
static unsigned short _mlsNetSAM_POSSessionEstablishment(mlsCard_str *handleSAM,
														 unsigned char *pTSN)
{
	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	int randnr;
	unsigned char abTRn[8],abEncryptTRn[8];
	unsigned char abSRn[8],abEncryptSRn[8];

	//generate random number
	randnr = (unsigned char)mlsRandom();
	abTRn[0] = (unsigned char)((randnr<<24)>>24);
	abTRn[1] = (unsigned char)((randnr<<16)>>24);
	abTRn[2] = (unsigned char)((randnr<<8)>>24);
	abTRn[3] = (unsigned char)((randnr)>>24);

	randnr = (unsigned char)mlsRandom();
	abTRn[4] = (unsigned char)((randnr<<24)>>24);
	abTRn[5] = (unsigned char)((randnr<<16)>>24);
	abTRn[6] = (unsigned char)((randnr<<8)>>24);
	abTRn[7] = (unsigned char)((randnr)>>24);
	//TODO Derive
	//KiTAK = OWF3(KmTAK,TSN)
	//TAK=OWF2(KiTAK,TRn)
	//<abEncryptTRn>=3DES(TAK,TRn)
	
	/* Verify Terminal Cryptogram */
	//Send command
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x72;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x0C; 
	memcpy(&apduSAMCmd[5],abTRn,8);
	memcpy(&apduSAMCmd[13],&abEncryptTRn[4],4);

	lenSAMCmd = 17;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0)
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F0C)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_POSSESSION_VTCCMD);
		return MLSNetSAM_ERROR_POSSESSION_VTCCMD;
	}
	//Get response
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x0C,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_POSSESSION_VTCRESP);
		return MLSNetSAM_ERROR_POSSESSION_VTCRESP;
	}
	//return data from SAM : abSRn[3-0],abSRn(8),9000
	//TODO Derive <SRn> = 3DES(TAK,SRn);
	if (memcmp(apduSAMResp,abEncryptSRn,4)!= 0)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_POSSESSION_VSC);
		return MLSNetSAM_ERROR_POSSESSION_VSC;
	}
	
	//TODO Set Time Request?
	return OPERATION_OK;
}
unsigned short mlsNetSAM_Initialization(mlsCard_str *handleSAM)
{
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd,i,j;
	unsigned short Status,usStatusCode;
	unsigned char ChkSum,KeyBlockSize,SAMStatus,PKSI;
	unsigned char abSID[4];
	unsigned char abShortName[6];
	
	unsigned char abChkSum[256];
	unsigned char OperatorID[6];
	unsigned char abTSN[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//TODO set value
	unsigned char abMC[16];
	
	mlsDebugLogSrcInfo("Enter NEC SAM Initialization\n");

	/* select DF */
	apduSAMCmd[0] = 0x00;
	apduSAMCmd[1] = 0xA4;
	apduSAMCmd[2] = 0x01;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x02; 
	apduSAMCmd[5] = 0x01;
	apduSAMCmd[6] = 0x18;

	lenSAMCmd = 7;
	
	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0)
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x611C)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SELECTDF);
		return MLSNetSAM_ERROR_INIT_SELECTDF;
	}
	
	/* Read EF 00 01 */
	apduSAMCmd[0] = 0x00;
	apduSAMCmd[1] = 0xB0;
	apduSAMCmd[2] = 0x81;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x24;

	lenSAMCmd = 5;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SELECTEF01);
		return MLSNetSAM_ERROR_INIT_SELECTEF01;
	}

	//Calculate ChkSum
	ChkSum = 0;
	for (i=0;i<36;i++)
	{
		ChkSum ^= apduSAMResp[i];
	}
	if (ChkSum != 0xFF)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SELECTEF01_CHKSUM);
		return MLSNetSAM_ERROR_INIT_SELECTEF01_CHKSUM;
	}
	
	//Store 
	memcpy(abSID,&apduSAMResp[5],4);
	memcpy(abShortName,&apduSAMResp[23],6);
	ucmlsNetSAMNoKeyBlock = apduSAMResp[29];
	
	//Calculate KeyBlockSize
	KeyBlockSize = ucmlsNetSAMNoKeyBlock * 4;

	/* Read EF 00 02 */
	apduSAMCmd[0] = 0x00;
	apduSAMCmd[1] = 0xB0;
	apduSAMCmd[2] = 0x82;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = KeyBlockSize;

	lenSAMCmd = 5;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SELECTEF02);
		return MLSNetSAM_ERROR_INIT_SELECTEF02;
	}
	//Store and calculate
	j = 0;
	for (i = 0;i<ucmlsNetSAMNoKeyBlock;i++)
	{
		abChkSum[i] = 0;
		abmlsNetSAMKeySetIndex[i] = 0;
	}
	
	for (i = 0;i<ucmlsNetSAMNoKeyBlock;i++)
	{
		abmlsNetSAMArrayKeyData[i][0] = apduSAMResp[j++];//Key Type
		abmlsNetSAMArrayKeyData[i][1] = apduSAMResp[j++];//KSI
		abmlsNetSAMArrayKeyData[i][2] = apduSAMResp[j++];//Key Length
		abmlsNetSAMArrayKeyData[i][3] = apduSAMResp[j++];//Check Sum
		//Calculate check sum
		abChkSum[i] = abmlsNetSAMArrayKeyData[i][0]^abmlsNetSAMArrayKeyData[i][1]^abmlsNetSAMArrayKeyData[i][2]^abmlsNetSAMArrayKeyData[i][3];
		//TODO check the condition KeyType[x][0]?
		if ((abmlsNetSAMArrayKeyData[i][0] != 0x00)&&(abChkSum[i] == 0xFF))
		{
			abmlsNetSAMKeySetIndex[i] = abmlsNetSAMArrayKeyData[i][1];
		}
	}
	
	/* Read EF 00 09 */
	apduSAMCmd[0] = 0x00;
	apduSAMCmd[1] = 0xB0;
	apduSAMCmd[2] = 0x89;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x08;

	lenSAMCmd = 5;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9000)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SELECTEF09);
		return MLSNetSAM_ERROR_INIT_SELECTEF09;
	}
	//Store
	SAMStatus = apduSAMResp[0];
	memcpy(OperatorID,&apduSAMResp[2],6);
	
	if (SAMStatus == 0xFF)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_SAMLOCK);
		return MLSNetSAM_ERROR_INIT_SAMLOCK;
	}
	
	//TODO Generate 16 bytes of TSN
	//abTSN[15-0]

	//establish session
	if (SAMStatus != 0x00)
	{
		usStatusCode = _mlsNetSAM_POSSessionEstablishment(handleSAM,abTSN);
		if (usStatusCode != OPERATION_OK )
		{
			mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_POSSESSIONESTABLISH);
			return MLSNetSAM_ERROR_POSSESSIONESTABLISH;
		}
		mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);
		return OPERATION_OK;
	}
	
	PKSI = 0x59;//TODO check 0x59 or 59 ?
	/* Activate SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x70;
	apduSAMCmd[2] = 0x04;
	apduSAMCmd[3] = PKSI;
	apduSAMCmd[4] = 0x10;
	memcpy(&apduSAMCmd[5],abTSN,16);
	
	lenSAMCmd = 21;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F10)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_ACTIVATECMD);
		return MLSNetSAM_ERROR_INIT_ACTIVATECMD;
	}

	/* Get response activation from SAM */
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x10,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_ACTIVATERESP);
		return MLSNetSAM_ERROR_INIT_ACTIVATERESP;
	}
	
	memcpy(abMC,apduSAMResp,16);
	/* save MC to file */
	FILE *pNetSAMMCFile;
	pNetSAMMCFile = fopen(".\\mlsNetSAM_MC.txt","w"); 
	for (i = 0;i<16;i++)
	{
		fprintf(pNetSAMMCFile,"%x ",abMC[i]);
	}
	fclose(pNetSAMMCFile);

	// no need to establish session
	/* POS session establishment */
	/*
	usStatusCode = _mlsNetSAM_POSSessionEstablishment(handleSAM,abTSN);
	if (usStatusCode != OPERATION_OK )
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_POSSESSIONESTABLISH);
		return MLSNetSAM_ERROR_POSSESSIONESTABLISH;
	}
	*/
	
	mlsDebugLogSrcInfo("Exit OK status = %x\n",OPERATION_OK);

	return OPERATION_OK;
}

unsigned short mlsNetSAM_VerifyReadPurseCryptogram2(mlsCard_str *handleSAM,
													unsigned char KeyNo,
													unsigned char KSI,
													unsigned char *pCAN,
													unsigned char *pCSN,
													unsigned char *pRRn,
													unsigned char *pCRn,
													unsigned char *pEncryptData,
													unsigned char *pDataOut)
{
	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	/* Send Command to SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x80;
	apduSAMCmd[2] = KeyNo;
	apduSAMCmd[3] = KSI;
	apduSAMCmd[4] = 0x32;
	memcpy(&apduSAMCmd[5],pCAN,8);
	memcpy(&apduSAMCmd[13],pCSN,8);
	memcpy(&apduSAMCmd[21],pRRn,8);
	memcpy(&apduSAMCmd[29],pCRn,8);
	memcpy(&apduSAMCmd[37],pEncryptData,18);

	lenSAMCmd = 55;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F10)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_READAUTH_VRPCCMD);
		return MLSNetSAM_ERROR_READAUTH_VRPCCMD;
	}

	/* Get response data from SAM */
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x10,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{
	
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_INIT_ACTIVATERESP);
		return MLSNetSAM_ERROR_INIT_ACTIVATERESP;
	}

	memcpy(pDataOut,apduSAMResp,16);

	mlsDebugLogSrcInfo("Exit OK");
	return OPERATION_OK;
}

unsigned short mlsNetSAM_GenerateDebitCryptogram2(mlsCard_str *handleSAM,
												  unsigned char KeyNo,
												  unsigned char KSI,
												  unsigned char *pDebitCryptoData,
												  unsigned char *pDataOut)
{
	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	/* Send Command to SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x82;
	apduSAMCmd[2] = KeyNo;
	apduSAMCmd[3] = KSI;
	apduSAMCmd[4] = 0x34;
	memcpy(&apduSAMCmd[5],pDebitCryptoData,52);
	
	lenSAMCmd = 57;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F24)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_DEBIT_GDC2CMD);
		return MLSNetSAM_ERROR_DEBIT_GDC2CMD;
	}

	/* Get response data from SAM */
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x24,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{

		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_DEBIT_GDC2RESP);
		return MLSNetSAM_ERROR_DEBIT_GDC2RESP;
	}

	mlsDebugLogSrcInfo("Exit OK");
	memcpy(pDataOut,apduSAMResp,36);
	return OPERATION_OK;
}
/*
 *	verify debit receipt cryptogram
 */
unsigned short mlsNetSAM_VerifyDebitReceiptCryptogram2(mlsCard_str *handleSAM,
													   unsigned char *pDebitReceiptCryptoData,
													   unsigned char *pTOKENReader,
													   unsigned char *pDataOut)
{
	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;
	/* Send Command to SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x84;
	apduSAMCmd[2] = 0x00;
	apduSAMCmd[3] = 0x00;
	apduSAMCmd[4] = 0x20;
	memcpy(&apduSAMCmd[5],pDebitReceiptCryptoData,24);
	memcpy(&apduSAMCmd[29],pTOKENReader,8);
	apduSAMCmd[37] = MLS_DEBIT;//TODO check transaction type of NET SAM

	lenSAMCmd = 38;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F1E)
	{
		if ((usStatusCode == 0x6982)||(usStatusCode == 0x9804)||(usStatusCode == 0x6A80))
		{
			mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_DEBIT_VDRC2CMD);
			return MLSNetSAM_ERROR_DEBIT_VDRC2CMD;
		}
		else 
		{
			mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION);
			return MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION;
		}	
	}

	/* Get response data from SAM */
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x1E,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{

		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_DEBIT_VDRC2RESP);
		return MLSNetSAM_ERROR_DEBIT_VDRC2RESP;
	}

	mlsDebugLogSrcInfo("Exit OK");
	memcpy(pDataOut,apduSAMResp,30);
	return OPERATION_OK;
}

/*
 *	Exception handling
 */
unsigned short mlsNetSAM_ExceptionHandling(mlsCard_str *handleSC,
										   mlsCard_str *handleSAM,
										   int iAmount,
										   mlsReadPurseRecord *pOldPurseRecord,
										   unsigned char *pDataOut)
{
	mlsReadPurseRecord PurseRecord;
	int randnr,iPurseBalance,iOldPurseBalance;
	unsigned char abRandomTerm[8],abRandomPICC[8],abRecoveryData[67];
	unsigned char ucKSI,ucDebitOption;
	KeyIndex ReadPurseKey;

	unsigned short Status,usStatusCode;
	unsigned char apduSAMCmd[256];
	unsigned char apduSAMResp[256];
	int lenSAMResp,lenSAMCmd;

	
	/* read purse with authenticate */
	//generate terminal random number
	randnr = (unsigned char)mlsRandom();
	abRandomTerm[0] = (unsigned char)((randnr<<24)>>24);
	abRandomTerm[1] = (unsigned char)((randnr<<16)>>24);
	abRandomTerm[2] = (unsigned char)((randnr<<8)>>24);
	abRandomTerm[3] = (unsigned char)((randnr)>>24);

	randnr = (unsigned char)mlsRandom();
	abRandomTerm[4] = (unsigned char)((randnr<<24)>>24);
	abRandomTerm[5] = (unsigned char)((randnr<<16)>>24);
	abRandomTerm[6] = (unsigned char)((randnr<<8)>>24);
	abRandomTerm[7] = (unsigned char)((randnr)>>24);
	// Get CSC challenge
	usStatusCode = mlsGetChallenge(handleSC,abRandomPICC,8);
	if(usStatusCode != MLSCEPAS_SUCCESS)
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSCEPAS_ERROR_GETCHALLENGE);
		return MLSCEPAS_ERROR_GETCHALLENGE;
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

	usStatusCode = mlsCepasReadPurseAuthen( handleSC,handleSAM,netSAM,
											abRandomTerm,0x03,ReadPurseKey,
											0x5F,abRandomPICC,&PurseRecord);
	ucKSI = *((unsigned char *)&PurseRecord+71);//72th byte of data
	ucDebitOption = *((unsigned char *)&PurseRecord + 94);//95th byte of data
	/* compare and check condition */
	iPurseBalance = (int)((PurseRecord.arrPurseBalance[0]<<16)|
						 (PurseRecord.arrPurseBalance[1]<<8)|
						  PurseRecord.arrPurseBalance[2]);
	iOldPurseBalance = (int)((pOldPurseRecord->arrPurseBalance[0]<<16)|
							 (pOldPurseRecord->arrPurseBalance[1]<<8)|
							 pOldPurseRecord->arrPurseBalance[2]);
	if ((memcmp(PurseRecord.arrCAN,pOldPurseRecord->arrCAN,8) != 0)||
	    (memcmp(PurseRecord.arrCSN,pOldPurseRecord->arrCSN,8) != 0)||
	    (memcmp(PurseRecord.arrLastCreditTransactionHeader,pOldPurseRecord->arrLastCreditTransactionHeader,8) != 0)||
	    (memcmp(PurseRecord.arrLastTransactionTRP,pOldPurseRecord->arrLastTransactionTRP  ,8) != 0)||
	    (ucDebitOption != (*((unsigned char *)pOldPurseRecord+94)))||
		((iOldPurseBalance-iAmount) != iPurseBalance)) 
		//add check autoload if needed
	{
		mlsDebugLogSrcInfo("Exit error %x\n",MLSNetSAM_ERROR_EXCEPTION_CHKCONDITION);
		return MLSNetSAM_ERROR_EXCEPTION_CHKCONDITION;
	}  
	/* send command to SAM to verify Read purse cryptogram with recoveryData */
	/* Send Command to SAM */
	apduSAMCmd[0] = 0x80;
	apduSAMCmd[1] = 0x80;
	apduSAMCmd[2] = 0x83;//KeyNo
	switch (ucmlsNetSAMKeyIndex)//KSI
	{
	case 0x04:
		apduSAMCmd[3] = 0x83;
		break;
	case 0x05:
		apduSAMCmd[3] = 0x84;
		break;
	default:
		break;
	}
	apduSAMCmd[4] = 0x43;

	memcpy(abRecoveryData,PurseRecord.arrCAN,8);
	memcpy(&abRecoveryData[8],PurseRecord.arrCSN,8);
	memcpy(&abRecoveryData[16],abRandomTerm,8);
	memcpy(&abRecoveryData[24],abRandomPICC,8);
	memcpy(&abRecoveryData[32],((unsigned char *)&PurseRecord+95),18);//PurseRecord[95]:begin encrypt data
	abRecoveryData[50] = MLS_DEBIT;//TODO check transaction type of NET SAM
	abRecoveryData[51] = ucDebitOption;
	memcpy(&abRecoveryData[52],PurseRecord.arrLastCreditTransactionHeader,8);
	memcpy(&abRecoveryData[60],PurseRecord.arrLastTransactionTRP,4);
	memcpy(&abRecoveryData[64],PurseRecord.arrPurseBalance,3);

	memcpy(&apduSAMCmd[5],abRecoveryData,67);

	lenSAMCmd = 72;

	Status = sendAPDU(handleSAM,apduSAMCmd,lenSAMCmd,apduSAMResp,&lenSAMResp);
	if (Status != 0) 
	{ 
		mlsDebugLogSrcInfo("Send APDU error = %x\n", Status);
		return OPERATION_ERROR;
	}
	if(lenSAMResp >= 2) 
	{
		usStatusCode = (unsigned short)((apduSAMResp[lenSAMResp - 2] << 8)| ( apduSAMResp[ lenSAMResp - 1 ] ));
	}

	mlsDebugLogSrcInfo("APDU status = %x\n", usStatusCode);	

	if (usStatusCode != 0x9F13)
	{
		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_EXCEPTION_VRPC2CMP);
		return MLSNetSAM_ERROR_EXCEPTION_VRPC2CMP;
	}

	/* Get response data from SAM */
	usStatusCode = _mlsNetSAM_GetResponse(handleSAM,0x13,apduSAMResp);
	if (usStatusCode != OPERATION_OK)
	{

		mlsDebugLogSrcInfo("Exit Error status = %x\n",MLSNetSAM_ERROR_EXCEPTION_VRPC2RESP);
		return MLSNetSAM_ERROR_EXCEPTION_VRPC2RESP;
	}
	/* end exception */
	mlsDebugLogSrcInfo("Exit OK");
	memcpy(pDataOut,PurseRecord.arrPurseBalance,0x03);//TODO here just return Purse Balance if transaction approved
	return OPERATION_OK;
}