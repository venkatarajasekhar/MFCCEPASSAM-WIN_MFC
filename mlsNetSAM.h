#ifndef MLSNETSAM_H
#define MLSNETSAM_H
#include "mlsCepas.h"
#ifdef _cplusplus
extern "C"
{
#endif

extern unsigned char abmlsNetSAMArrayKeyData[256][4];//array to save key information in Net SAM
extern unsigned char ucmlsNetSAMKeyIndex;//use to save key present in Net SAM
extern unsigned char ucmlsNetSAMNoKeyBlock;//number of Key Block in Net SAM
extern unsigned char abmlsNetSAMKeySetIndex[256];//Key Set Index[] in Net SAM with correct checksum


unsigned short mlsNetSAM_Initialization(mlsCard_str *handleSAM);

unsigned short mlsNetSAM_VerifyReadPurseCryptogram2(mlsCard_str *handleSAM,
													unsigned char KeyNo,
													unsigned char KSI,
													unsigned char *pCAN,
													unsigned char *pCSN,
													unsigned char *pRRn,
													unsigned char *pCRn,
													unsigned char *pEncryptData,
													unsigned char *pDataOut);

unsigned short mlsNetSAM_GenerateDebitCryptogram2(mlsCard_str *handleSAM,
												unsigned char KeyNo,
												unsigned char KSI,
												unsigned char *pDebitCryptoData,
												unsigned char *pDataOut);

unsigned short mlsNetSAM_VerifyDebitReceiptCryptogram2(mlsCard_str *handleSAM,
													  unsigned char *pDebitReceiptCryptoData,
													  unsigned char *pTOKENReader,
													  unsigned char *pDataOut);

unsigned short mlsNetSAM_ExceptionHandling(mlsCard_str *handleSC,
										   mlsCard_str *handleSAM,
										   int iAmount,
										   mlsReadPurseRecord *pOldPurseRecord,
										   unsigned char *pDataOut);

#ifdef _cplusplus
}
#endif

#endif /* mlsNetSAM.h */