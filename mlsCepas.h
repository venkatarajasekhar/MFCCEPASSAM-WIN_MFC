#ifndef MLSCEPASCARD_H
#define MLSCEPASCARD_H
#include "mlsCrypto.h"
#include "mlsCepasTest.h"
#include "winscard.h"
#include "mlsSettlement.h"
#include <stdio.h>

//#define MLS_TESTSETTLEMENT
//#define MLSLTASAM_MUTUALAUTHENTICATE_TEST//define to test mutual authenticate of concession card

//set value of data that don't known of MD transaction
static const UINT8 MESSAGETYPE[2] = {0x01,0x02};
static const UINT8 BADDEBTCOUNTER = 0x01;
static const UINT8 UTSN[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
static const UINT8 SOURCE[5]={'P','S','A','M','T'};
static const UINT8 DESTINATION[5] = {'Q','B','F','T','P'};
static const UINT8 MERCHANTCHAINID[8] = {'9','9','9','9','9','9','9','9'};
static const UINT8 LOCATIONID[8] = {'9','9','9','9','9','9','9','9'};
static const UINT8 DEVICETYPE[4] = {'0','0','8','0'};
static const UINT8 DEVICEID[8] = {'0','0','0','1','0','4','8','5'};
static const UINT8 MERCHANTPOSID[10]={'9','9','9','9','9','9','9','9','9','9'};
static const UINT8 NUMBEROFRECORDS[4] ={'0','0','0','0'};//total number of TRANSACTION DATA records in the file
static const UINT8 BATCHDEBITAMOUNT[10]={'0','0','0','0','0','0','0','0','0','0'};//total debit amount for all debit TRANSACTION DATA records
//in the file in the format 9999999999
static const UINT8 RFU[10]={'0','0','0','0','0','0','0','0','0','0'};
static const UINT8 BATCHAUTOLOADAMOUNT[10]= {'0','0','0','0','0','0','0','0','0','0'};//total auto-load amount for all debit with Auto-Load
//TRANSACTION DATA records in the file in unit of 1 cent
static const UINT8 CRC[2]={'0','0'};
/*TODO mls@dev01 to review <<<*/

//#define MLS_TESTSETTLEMENT

//define error code
#define MLSCEPAS_SUCCESS						0x00
#define MLSCEPAS_SUCCESS_SENDADPU				0x9000

#define MLSCEPAS_ERROR_CHECK_BACKLIST			0x10
#define MLSCEPAS_ERROR_CHECK_PURSEEXPIRY		0x11
#define MLSCEPAS_ERROR_CHECK_PURSESTATUS		0x12
#define MLSCEPAS_ERROR_CHECK_FUND				0x13
#define MLSCEPAS_ERROR_CHECK_FUNDAUTOLOAD		0x14
#define MLSCEPAS_ERROR_CHECK_AUTOLOADCATEGORY	0x15
#define MLSCEPAS_ERROR_CHECK_REFUNDSTATUS		0x16
#define MLSCEPAS_ERROR_CHECK_CARDPROFILETYPE	0x17
#define MLSCEPAS_ERROR_CHECK_CARDISSUER			0x18
#define MLSCEPAS_ERROR_CHECK_CAN				0x19

#define MLSCEPAS_ERROR_READPURSEAUTH			0x21
#define MLSCEPAS_ERROR_READPURSENOAUTH			0x22
#define MLSCEPAS_ERROR_GETCHALLENGE				0x23
#define MLSCEPAS_ERROR_READTRANLOG				0x24
#define MLSCEPAS_ERROR_UNKNOWNSC				0x25

#define MLSCEPAS_ERROR_SENDAPDUDEBIT			0x31
#define MLSCEPAS_ERROR_SENDAPDUCREDIT			0x32
#define MLSCEPAS_ERROR_SENDAPDUGETCHALLENGE		0x33
#define MLSCEPAS_ERROR_SENDAPDUREADPURSE		0x34

#define MLSCEPAS_ERROR_WRONGLENGTH				0x41
#define MLSCEPAS_ERROR_COMPARECERTIFICATE		0x42

#define MLSEzSAM_ERROR_SELECTAPPLICATION		0x51
#define MLSEzSAM_ERROR_ENABLEAPPLET				0x52
#define MLSEzSAM_ERROR_DEBITSESSIONKEY			0x53
#define MLSEzSAM_ERROR_CREDITSESSIONKEY			0x54
#define MLSEzSAM_ERROR_SIGNATURESESSIONKEY		0x55
#define MLSEzSAM_ERROR_ENCRYPTCREDITBLOCK		0x56

#define MLSLtaSAM_ERROR_SELECTAPPLICATION		0x60
#define MLSLtaSAM_ERROR_SAMSTATUS				0x61
#define MLSLtaSAM_ERROR_INITIALISEUPDATE		0x62
#define MLSLtaSAM_ERROR_EXTERNALAUTHENTICATE	0x63
#define MLSLtaSAM_ERROR_REQUESTMARRIAGE			0x64
#define MLSLtaSAM_ERROR_MUTUALAUTHPROCESS		0x65
#define MLSLtaSAM_ERROR_GETTERMINALRANDOMNUMBER	0x66
#define MLSLtaSAM_ERROR_REQUESTAUTHENTICATE		0x67
#define MLSLtaSAM_ERROR_REQUESTDEBIT			0x68
#define MLSLtaSAM_ERROR_REQUESTSIGNCERTIFICATE	0x69

#define MLSNetSAM_ERROR_INIT_SELECTDF			0x700
#define MLSNetSAM_ERROR_INIT_SELECTEF01			0x701
#define MLSNetSAM_ERROR_INIT_SELECTEF01_CHKSUM	0x702
#define MLSNetSAM_ERROR_INIT_SELECTEF02			0x703
#define MLSNetSAM_ERROR_INIT_SELECTEF09			0x704
#define MLSNetSAM_ERROR_INIT_SAMLOCK			0x705
#define MLSNetSAM_ERROR_INIT_ACTIVATECMD		0x706
#define MLSNetSAM_ERROR_INIT_ACTIVATERESP		0x707
#define MLSNetSAM_ERROR_INITIALIZATION			0x708
#define MLSNetSAM_ERROR_POSSESSION_VTCCMD		0x710 //verify Terminal cryptogram command
#define MLSNetSAM_ERROR_POSSESSION_VTCRESP		0x711 //verify Terminal cryptogram response
#define MLSNetSAM_ERROR_POSSESSION_VSC			0x712 //verify SAM cryptogram
#define MLSNetSAM_ERROR_POSSESSIONESTABLISH		0x713 //POS session establishment
#define MLSNetSAM_ERROR_READAUTH_VRPCCMD		0x720 //verify read purse cryptogram command
#define MLSNetSAM_ERROR_READAUTH_VRPCRESP		0x721 //verify read purse cryptogram response		
#define MLSNetSAM_ERROR_READAUTH				0x722 //verify read purse cryptogram response		
#define MLSNetSAM_ERROR_DEBIT_GDC2CMD			0x730 //generate debit cryptogram 2 command
#define MLSNetSAM_ERROR_DEBIT_GDC2RESP			0x731 //generate debit cryptogram 2 response
#define MLSNetSAM_ERROR_DEBIT_GDC2				0x732 //generate debit cryptogram 2 response
#define MLSNetSAM_ERROR_DEBIT_VDRC2CMD			0x733 //verify debit receipt cryptogram 2 command
#define MLSNetSAM_ERROR_DEBIT_VDRC2RESP			0x734 //verify debit receipt cryptogram 2 response
#define MLSNetSAM_ERROR_DEBIT_VDRC2EXCEPTION	0x735 //2nd bit of control byte (65th byte of purse data)
#define MLSNetSAM_ERROR_DEBIT_VDRC2				0x736 //verify debit receipt cryptogram 2 response
#define MLSNetSAM_ERROR_DEBIT_EXCEPTION			0x737 //2nd bit of control byte (65th byte of purse data)
#define MLSNetSAM_ERROR_DEBIT_CMPTOKEN			0x738 //compare AuthData and token SAM
#define MLSNetSAM_ERROR_DEBIT_CONTROLBYTE		0x739 //2nd bit of control byte (65th byte of purse data)
#define MLSNetSAM_ERROR_EXCEPTION_CHKCONDITION	0x740 //exception handling: check condition
#define MLSNetSAM_ERROR_EXCEPTION_VRPC2CMP		0x741 //exception handling: send command verify read purse cryptogram 2
#define MLSNetSAM_ERROR_EXCEPTION_VRPC2RESP 	0x742 //exception handling: send respond

#define MLS_GetChallenge  0x84
#define MLS_DEBIT 0x34
#define MLS_CREDIT 0x36
#define MLS_READPURSEDATA 0x32

#define EZLINK_DEBIT_OPTION_DISABLEEXPIREYCHECKING_MASK	0x80
#define EZLINK_DEBIT_OPTION_AUTOLOADDISABLE 0x40
#define EZLINK_DEBIT_OPTION_DEDUCTPURSE 0x00
#define EZLINK_DEBIT_OPTION_DEDUCTWIHTPARTIALREFUN 0X01
#define EZLINK_DEBIT_OPTION_CUMULATIVEDEBIT 0x02
#define EZLINK_DEBIT_OPTION_PURSEDISABLE 0x03

#define EZLINK_PURSE_FILE_NR	0x03

#define EZLINK_AUTOLOAD_KEY_SFI	0x15
#define EZLINK_AUTOLOAD_KEY_NR	0x01
#define EZLINK_DEBIT_KEY_SFI	0x15
#define EZLINK_DEBIT_KEY_NR		0x02
#define EZLINK_SIGNING_KEY_SFI	0x14
#define EZLINK_SIGNING_KEY_NR	0x03

#define EZLINK_DEBIT_OPTIONTYPE_ERPCONGESTIONBASED 0X07
#define EZLINK_DEBIT_OPTIONTYPE_ERPDISTANCEBASED 0X08
#define EZLINK_DEBIT_OPTIONTYPE_ERPTIMEBASED 0X09
#define EZLINK_DEBIT_OPTIONTYPE_PURSEDISABLE 0X11
#define EZLINK_DEBIT_OPTIONTYPE_RAILPAY 0X30
#define EZLINK_DEBIT_OPTIONTYPE_BUSPAY 0X31
#define EZLINK_DEBIT_OPTIONTYPE_CASHBACK 0X66
#define EZLINK_DEBIT_OPTIONTYPE_ADDVALUE 0X75						//NTA
#define EZLINK_DEBIT_OPTIONTYPE_PARTIALREFUND 0X76
#define EZLINK_DEBIT_OPTIONTYPE_AUTOLOADDISABLE 0X83
#define EZLINK_DEBIT_OPTIONTYPE_PURSEANDAUTOLOADDISABLE 0X84
#define EZLINK_DEBIT_OPTIONTYPE_RAILAUTOLOADDISABLE 0X85
#define EZLINK_DEBIT_OPTIONTYPE_BUSAUTOLOADDISABLE 0X86
#define EZLINK_DEBIT_OPTIONTYPE_PARTIALREFUNDAUTOLOADDISABLE 0X87
#define EZLINK_DEBIT_OPTIONTYPE_NTAPAYMENTANDAUTOLOAD 0XA0			//NTA
#define EZLINK_DEBIT_OPTIONTYPE_MODIFYPURSE 0XF0						//NTA

static const unsigned char MLS_DEBIT_TYPE  =EZLINK_DEBIT_OPTIONTYPE_NTAPAYMENTANDAUTOLOAD;
static const unsigned char MLS_CREDIT_TYPE = EZLINK_DEBIT_OPTIONTYPE_ADDVALUE;

static const unsigned short   sgw_GENERAL_API_ERROR           = 0xFFFF;

static const unsigned short   sgwDFCARDOBJ_LENGTH_ERROR   = 0x201;
static const unsigned char   sgbOPERATION_OK                 = 0x00;
static const unsigned short   sgw7816_OPERATION_OK				= 0x9000;
static const unsigned short   sgw7816_MEMORY_FAILURE				= 0x6581;
static const unsigned short   sgw7816_WRONG_LENGTH				= 0x6700;
static const unsigned short sgw7816_CERTIFICATE_ERROR = 0x9999;
static const unsigned short OPERATION_ERROR = 0x01;
static const unsigned short OPERATION_OK = 0x00;

typedef struct KeyIndex
{
	unsigned char ucKeyFileID;
	unsigned char ucKeyNo;
}KeyIndex;

typedef struct mlsReadPurseRecord
{
	unsigned char ucCEPASVersion;
	unsigned char ucPurseStatus;
	unsigned char arrPurseBalance[3];
	unsigned char arrAutoLoadAmount[3];
	unsigned char arrCAN[8];
	unsigned char arrCSN[8];
	unsigned char arrPurseExpiryDate[2];
	unsigned char arrPurseCreationDate[2];
	unsigned char arrLastCreditTransactionTRP[4];
	unsigned char arrLastCreditTransactionHeader[8];
	unsigned char ucNoOfRecord;
	unsigned char ucIssureSpecificDataLength;
	unsigned char arrLastTransactionTRP[4];
	unsigned char arrLastTransactionRecord[16];
	unsigned char arrIssurespecificData[32];//mls@dev03 change array size 256 ->32
	unsigned char ucLastTransactionDebitOptionsByte;
	unsigned char arrLastTransactionSignedCertificate[8];
	unsigned char arrCounterData[8];
	unsigned char arrCRC_B[2];
}mlsReadPurseRecord;
typedef struct mlsCard_str
{
	SCARDHANDLE cardhandle;
	LPCSCARD_IO_REQUEST iorequest;
}mlsCard_str;
typedef enum
{
	ezSAM,
	LtaSAM,
	netSAM
}SAM_CEPAS_t;

typedef enum
{
	unknowSC,
	EzLinkSC,
	ConcessionSC,
	NetSC
}SC_CEPAS_t;

typedef union TransactionLogRecord
{
	struct TransactionHeader
	{
		unsigned char ucTransactionType;
		unsigned char unTransactionAmount[3];
		unsigned char TransactionDateTime[4];
	};
	unsigned char unTransactionUserData;
}TransactionLogRecord;
unsigned short mlsGetChallenge(mlsCard_str *handle,unsigned char * abChallenge,unsigned char length);
unsigned short mlsCepasDebit(mlsCard_str *handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char ucDebitOption,
							 unsigned char arrTransactionUserData[8],unsigned char arrDebitKey[16],
							 unsigned char arrSignedKey[16],KeyIndex DebitKey,KeyIndex SigningKey);
unsigned short mlsCepasCredit(mlsCard_str *handle,unsigned char ucPf,unsigned char ucTRP[4],unsigned char arrTransactionUserData[8],
						 unsigned char arrCreditKey1[16],unsigned char arrCreditKey2[16],unsigned char arrSignedKey[16],
						 KeyIndex CreditKey1,KeyIndex CreditKey2,KeyIndex SigningKey,int iAmount);
unsigned short mlsCepasReadPurseNoAuthen(mlsCard_str *handle,unsigned char ucPF,short sReadPurseRecordLength,
												 mlsReadPurseRecord *pReadPurseRecord);
unsigned short mlsCepasReadTransactionLog(mlsCard_str *handle,unsigned char ucPF,unsigned char sRecordOffset
										  ,unsigned char sRecordLength, unsigned char *pbResponseBuffer,
										  short *sResponseBufferLength);
unsigned short mlsCepasReadPurseAuthen(mlsCard_str *handle,
									   mlsCard_str *handleSAM,
									   SAM_CEPAS_t SAM,
									   unsigned char *pTerminalRandomNumber,
									   unsigned char ucPF,
									   KeyIndex readPurKey,
									   short Le, 
									   unsigned char *pCSCChallenge,
									   mlsReadPurseRecord *pReadPurseRecord);
unsigned short ExecuteCommand(mlsCard_str *handle, unsigned char bINS, unsigned char bP1, unsigned char bP2, 
							unsigned char wLc, unsigned char * pbData, unsigned char wLe, 
							unsigned char * pbResponse, int iResponseBufferSize,
							short * pwResponseLength);
int mlsCardTransmit(mlsCard_str *card, unsigned char* inBuffSend, int inBuffSendLen, unsigned char* outBuffRec, 
		 					  int *inoutBuffRecLen);
unsigned short sendAPDU(mlsCard_str *handle,unsigned char * ucInput,int iInputLength, unsigned char *ucOutPut,int *iOutPutLength);
void convertArrToReadPurseRecord(mlsReadPurseRecord *pReadPurseRecord,unsigned char *pData,unsigned short usDataLength);
void convertIntToArrUnsignChar(int iData,unsigned char *output,int lengthArrByte);
void inputData(unsigned char * arrData,unsigned char data,int *count);
void inputArrayData(unsigned char *arrData,int *iDataLength,unsigned char *input,int inputlength);
void convertNumbertoArrChar(int iNumber,UINT8 *pBuffer,int Length);
int CalculateDate(SYSTEMTIME Time,int iBeginYear);
int elementOfNumber(int iNumber);
int compareTwoNumber(UINT8 *pFirstNum,int iFirstNumLen,UINT8 *pSecondNum,int iSecondNumLen);
void sumArrayCharAndInt(UINT8 *pFirstNumber,int iFirstLen,int iSecondNum);
void mlsConvertToIntKeepNegative(int *iData,unsigned char*pArrBigEndian,int begin);
int checkFileSizeOrRecordNumber(CepasMDBatchFile *pCepasMDBatchFile);
void freeMemoryOfCepasMDBatchFile(CepasMDBatchFile *pCepasMDBatchFile);
unsigned short mlsCepasCreditSAM(mlsCard_str *handleSC,mlsCard_str *handleSAM,unsigned char ucPf,unsigned char* ucTRP,unsigned char* arrTransactionUserData,
								 KeyIndex CreditKey1,KeyIndex CreditKey2,KeyIndex SigningKey,int iAmount,int *PurseBalance);
unsigned short mlsCepasDebitSAM(mlsCard_str *handleSC,mlsCard_str *handleSAM,unsigned char &ucPf,unsigned char* ucTRP,unsigned char ucDebitOption,
								unsigned char* arrTransactionUserData,KeyIndex &DebitKey,KeyIndex &SigningKey,int iAmount,int *PurseBalance);
unsigned short mlsCepasDebitSAMAndCreTransRec(mlsCard_str *handleSC,mlsCard_str *handleSAM,SAM_CEPAS_t SAM,unsigned char ucPf,unsigned char* ucTRP,unsigned char ucDebitOption,
								unsigned char* arrTransactionUserData,KeyIndex DebitKey,KeyIndex SigningKey,int iAmount,int *PurseBalance,
								UINT8 IssuerData,MDTransactionRecord *pMDTransRecord);
void mlsCepasCreateTransRecord(mlsReadPurseRecord PurseData,UInt8 *pTransHeader,UInt8 *pTRP,UInt8*pPurseBalance,UInt8 *pCounterData,
							   UInt8 *pSigCer,SYSTEMTIME currentTime,UInt8 IssuerID,MDTransactionRecord *pMDTransRecord);
void mlsCepasCreateMDBatchFile(SYSTEMTIME currentTime,CepasMDBatchFile *pCepasMBBatchFile);
void mlsAddRecordtoMBBatchFile(CepasMDBatchFile *pCepasMBBatchFile,MDTransactionRecord *pMDTransRecord);
int mlsWriteAllMDBatchFile(char * pPathFile,CepasMDBatchFile *pCepasMBBatchFile);
int mlsCreateMDBatchFile(char * pPathFile,CepasMDBatchFile *pCepasMDBatchFile);
int mlsWriteLastMBTransRecord(char *pPathFile,MDTransactionRecord *pTransacRecord);
int mlsUpdateHeaderMDBatchFile(char *pPathFile,CepasMDBatchFile *pTransacRecord);

char* getStringError(unsigned short usErrorId);
//int getDateTime();
//int random();

#endif