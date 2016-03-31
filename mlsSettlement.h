#ifndef __MLS_SETTLEMENT_H__
#define __MLS_SETTLEMENT_H__

#include "mlsTypes.h"
#include "basetsd.h"
#define ISSUER_ID_NETS			0x02
#define ISSUER_ID_TRANSIT_LINK	0x03
#define ISSUER_ID_EZLINK		0x04
#define SIZE_FILE 13340
#define RECNUMBERMAX 10

/*TODO mls@dev01 to review*/

#pragma pack(1)
typedef struct MDTransactionHeader_st
{
	UInt8	transactionType;
	UInt8	transactionAmount[3];
	UInt8	transactionDateTime[4];
}MDTransactionHeader;

typedef struct TerminalReferencePara_st
{
	UInt32	agentID:16;
	UInt32	modeOfPayment:4;
	UInt32	acquirerID:4;
	UInt32	debitOption:8;
}TerminalReferenceParam;

typedef struct CounterData_st
{
	UInt8	purseTransactionCounter[3];
	UInt8	addValueCounter[3];
	UInt16	modifyPurseCounter;
}CounterData;

typedef struct MDTransactionRecord_st
{
	UInt8				dataRecordIndicator;	//value = 'D'
	UInt8					messageType[2];		//indicated in the CEPAS Spec
	UInt8					cardMake;			//value = 0x01
	UInt8					formatVersion;		//set to 1
	UInt8					CAN[8];				//BCD
	UInt8					badDeptCounter;
	UInt8					RFU;
	MDTransactionHeader		transactionHeader;
	TerminalReferenceParam	terminalReferenceParam;
	UInt8					purseBalance[3];
	CounterData				counterData;
	UInt8					signedCertificate[8];
	MDTransactionHeader		lastCreditTransactionHeader;
	TerminalReferenceParam	lastCreditTransactionTRP;
	UInt8					lastPurseStatus;
	MDTransactionHeader		lastTransactionHeader;
	TerminalReferenceParam	lastTransactionTRP;
	UInt8					lastPurseBalance[3];
	CounterData				lastCounterData;
	UInt8					lastTxnSignedCertificate[8];
	UInt8					lastDebitOption;
	UInt16					businessDate;		//number of day since 1 Jan 1999 where it starts from 0
	UInt8					autoLoadAmount[3];	//2's complement in unit of 1 cent
	UInt8					issuerID;
	UInt8					issuerSpecificData[32];
	UInt8					UTSN[4];
	MDTransactionRecord_st  *pNext;
}MDTransactionRecord;

typedef struct CepasMDBatchFile_st
{
	UInt8				headerRecordIndicator;	//value = 'H'
	UInt8				formatVersion;			//value = 0x01; default version is 0x01
	UInt8				recordDatetime[14];		//format = "YYYYMMDDhhmmss"
	UInt8				source[5];				//value = "PSAMT"
	UInt8				destination[5];			//value = "QBFTP"
	UInt8				marchantChainID[8];		//format = "99999999" prefix with '0'
	UInt8				locationID[8];			//format = "99999999" prefix with '0'
	UInt8				deviceType[4];			//value = "0080"
	UInt8				deviceID[8];			//TID format = "99999999" prefix with '0'
	UInt8				merchantPOSID[10];		//format = "9999999999" prefix with '0'
	UInt8				numberOfRecord[4];		//format = "9999" prefix with '0'
	UInt8				debitAmount[10];		//format = "9999999999" prefix with '0'
	UInt8				RFU[10];
	UInt8				autoLoadAmount[10];		//format = "9999999999" prefix with '0'
	UInt8				crc16[2];				//CRC of whole file except itself
	UInt8				padding[33];			//padding with 33 byte '0' so that  cepasMDBatchFile header enough 133 byte
	MDTransactionRecord	*pTransactionDetail;
	MDTransactionRecord	*pLastTransactionDetail;
}CepasMDBatchFile;

typedef struct CepasMDBatchFileAck_st
{
	UInt8	recordIndicator;
	UInt8	recordDateTime[14];
	UInt8	source[5];
	UInt8	destination[5];
	UInt8	batchFileID[40];
	UInt8	numberOfRecord[4];
	UInt16	transmissionStatus;
	UInt8	crc16[2];
}CepasMDBatchFileAck;
#pragma pack()

#endif
