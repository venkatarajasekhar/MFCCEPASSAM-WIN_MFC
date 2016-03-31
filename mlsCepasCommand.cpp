// mlsCepasCommand.cpp : Defines the entry point for the console application.
//
#include "mlsW32Dll.h"
#include "mlsCepas.h"
#define READERNAMECSC TEXT("STYL STY90x PICC Reader 0")
#define READERNAMESAM TEXT("STYL STY90x ICC Reader 0")

void testReadPurseNoAuthentication(int ReaderHandler)
{
	mlsReadPurseRecord readPurseRecord;
	unsigned short status;
	status = mlsCepasReadPurseNoAuthen(ReaderHandler,0x03,0x5f,&readPurseRecord);
	//free(readPurseRecord.pIssurespecificData); 
}
void testReadTransactionLog(int ReaderHandler)
{
	unsigned char pbResponseBuffer[16];
	unsigned short length;
	unsigned short status;
	status = mlsCepasReadTransactionLog(ReaderHandler,0x03,0x00,0x10,pbResponseBuffer,&length);

}

int mlsW32AppReaderAndCardDetectionTest(void)
{
	mlsW32ErrorCode_t errorCode=1; /*return code of mlsW32 APIs*/
	mlsW32CardStatus_t state; /*state of card: present or remove*/
	INT32 nReader=0; /*number of reader detected*/
	INT32 ReaderCSCHandler,ReaderSAMHandler;/*handler of the working reader*/
	INT32 Counter; /*number of card present/remove detection time */
	CHAR cdummy;
	BYTE ATR[32];
	ULONG atrLen;
	mlsW32CardConnectProtocol_t protocol;
	mlsW32CardType_t cardType;

	unsigned char arrTRP[4] = {0,0,0,0};
	unsigned char arrTransUser[8] = {0,0,0,0,0,0,0,0};
	KeyIndex keyIndex,skeyIndex;
	/*1. list all reader, returns Number of reader in nReader*/
	errorCode = mlsW32ListReaders(&nReader);

	/*2. if no reader avalilabe in the system, prompt user to attach a 
	reader*/
	if (errorCode == MLSW32_READER_NOT_AVAILABLE)
	{
		BOOL blReaderAvail = FALSE;
		printf("No PCSC Reader available in the system. please attach a \
			   reader ....\n");
		/*waiting for a reader presented. blocking*/
		errorCode = MLSW32_SUCCESS;
		while ((blReaderAvail == FALSE)&&(errorCode == MLSW32_SUCCESS))
		{
			errorCode = mlsW32DetectReader(0xffffffff,&blReaderAvail);
		}

		if ((errorCode == MLSW32_SUCCESS)&& (blReaderAvail == TRUE))
		{
			printf("At least a reader has been detected, prepare to \
				   listReaders again ...\n");
			/*at least a reader just attached, update internal data now*/
			errorCode = mlsW32ListReaders(&nReader);
		}
	}

	/*******************************************************************/
	/* Connect to SAM card */
	/*get Handler of the SAM reader name, in this case*/
	errorCode = mlsW32GetReaderHandle(READERNAMESAM, &ReaderSAMHandler);

	if (errorCode!=MLSW32_SUCCESS)
	{
		printf("error in mlsW32GetReaderHandle, seem like the %s is not\
			   available in the system\n", READERNAMESAM);
		return 1;
	}

	/*open Reader with handler is ReaderCSCHandler*/
	errorCode = mlsW32ReaderOpen(ReaderSAMHandler);

	if (errorCode!= MLSW32_SUCCESS)
	{
		printf("error when calling mlsW32ReaderOpen");
		return 1;
	}
	/*connect to the ReaderSAMHandler reader*/
	protocol = CARD_CONNECT_PROTOCOL_T0|CARD_CONNECT_PROTOCOL_T1;
	errorCode = mlsW32CardConnect(
		ReaderSAMHandler,
		CARD_CONNECT_MODE_SHARE_SHARED,
		&protocol,
		ATR,
		&atrLen,
		&cardType
		);
	if (errorCode == MLSW32_SUCCESS)
	{
		printf("\tConnect to SAM ok\n");
	}
	while (errorCode == MLSW32_CARD_REMOVED)
	{
		printf("No SAM card in the reader, please insert the SAM card \n");
		/*not card in this slot, waiting for a card present*/
		state = CARD_STATUS_REMOVE;
		while (state == CARD_STATUS_REMOVE)
		{
			errorCode = mlsW32ReaderDetectCard(ReaderSAMHandler,0xffffffff,&state);
		}

		errorCode = mlsW32CardConnect(
			ReaderSAMHandler,
			CARD_CONNECT_MODE_SHARE_SHARED,
			&protocol,
			ATR,
			&atrLen,
			&cardType
			);

		if (errorCode == MLSW32_SUCCESS)
		{
			printf("\tConnect to SAM ok\n");
		}

		if (errorCode == MLSW32_RESOURCE_MANAGER_ERROR)
		{
			return 1;
		}
	}
	/*********************************************************************/
	/*Connect to CSC*/
	/*get Handler of the attention reader name, in this case
	"READERNAMECSC"*/
	errorCode = mlsW32GetReaderHandle(READERNAMECSC, &ReaderCSCHandler);

	if (errorCode!=MLSW32_SUCCESS)
	{
		printf("error in mlsW32GetReaderHandle, seem like the %s is not\
			   available in the system\n", READERNAMECSC);
		return 1;
	}

	/*open Reader with handler is ReaderCSCHandler*/
	errorCode = mlsW32ReaderOpen(ReaderCSCHandler);

	if (errorCode!= MLSW32_SUCCESS)
	{
		printf("error when calling mlsW32ReaderOpen");
		return 1;
	}
	/*connect to the ReaderCSCHandler reader*/
	protocol = CARD_CONNECT_PROTOCOL_T0|CARD_CONNECT_PROTOCOL_T1;
	errorCode = mlsW32CardConnect(
		ReaderCSCHandler,
		CARD_CONNECT_MODE_SHARE_SHARED,
		&protocol,
		ATR,
		&atrLen,
		&cardType
		);
	if (errorCode == MLSW32_SUCCESS)
	{
		printf("\tConnect to CSC ok, card type: ");
		if (cardType == CARD_TYPE_ULTRALIGHT)
		{
			printf("ULTRALIGHT\n");
		}
		else if (cardType == CARD_TYPE_MIFARE1K)
		{
			printf("MIFARE 1K\n");

		}
		else if (cardType == CARD_TYPE_MIFARE4K)
		{
			printf("MIFARE 4K\n");

		}
		else if (cardType == CARD_TYPE_DESFIRE)
		{
			printf("DESFire\n");
		}
		else
		{
			printf("UNKNOWN\n");
		}

	}

	while (errorCode == MLSW32_CARD_REMOVED)
	{
		printf("No card in the reader, please insert a card \n");
		/*not card in this slot, waiting for a card present*/
		state = CARD_STATUS_REMOVE;
		while (state == CARD_STATUS_REMOVE)
		{
			errorCode = mlsW32ReaderDetectCard(ReaderCSCHandler,0xffffffff,&state);
		}

		errorCode = mlsW32CardConnect(
			ReaderCSCHandler,
			CARD_CONNECT_MODE_SHARE_SHARED,
			&protocol,
			ATR,
			&atrLen,
			&cardType
			);

		if (errorCode == MLSW32_SUCCESS)
		{
			printf("\tConnect to CSC ok, card type: ");
			if (cardType == CARD_TYPE_ULTRALIGHT)
			{
				printf("ULTRALIGHT\n");
			}
			else if (cardType == CARD_TYPE_MIFARE1K)
			{
				printf("MIFARE 1K\n");

			}
			else if (cardType == CARD_TYPE_MIFARE4K)
			{
				printf("MIFARE 4K\n");

			}
			else if (cardType == CARD_TYPE_DESFIRE)
			{
				printf("DESFire\n");
			}
			else
			{
				printf("UNKNOWN\n");
			}
		}

		if (errorCode == MLSW32_RESOURCE_MANAGER_ERROR)
		{
			return 1;
		}
	}
	
	keyIndex.ucKeyFileID = 0x00;
	keyIndex.ucKeyNo = 0x00;
	
	skeyIndex.ucKeyFileID = 0x00;
	skeyIndex.ucKeyNo = 0x00;
		
	mlsGetChallenge(ReaderCSCHandler,arrTransUser,8);

	mlsCepasDebitSAM(ReaderCSCHandler,ReaderSAMHandler,ezSAM,0x03,arrTRP,0x00,arrTransUser,keyIndex,skeyIndex,300);
	testReadPurseNoAuthentication(ReaderCSCHandler);
	testReadTransactionLog(ReaderCSCHandler);
}

int main()
{
	int returnValue;
	KeyIndex temp;
	temp.ucKeyFileID = 0;
	temp.ucKeyNo = 0;
	/*//mls@dev03
	//test CEPAS2.0
	mlsCepasDebit(0,0,0,0,0,0,0,temp,temp);
	mlsCepasCredit(0,0,0,0,0,0,0,temp,temp,temp,0);
	mlsCepasReadPurseAuthen(0,0,0,0,0,0,0,0,0,0);
	*/
	returnValue = mlsW32AppReaderAndCardDetectionTest();
	return 0;
}

