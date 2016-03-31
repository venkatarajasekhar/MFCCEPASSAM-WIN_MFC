#ifndef MLSEZSAM_H
#define MLSEZSAM_H
#include "mlsCepas.h"
#ifdef __cplusplus
extern "C" {
#endif

#define OfflineCredit  0x02
#define OfflineDebit 0x03

unsigned short mlsEzSAM_SelectApplication(mlsCard_str *handleSAM);

unsigned short mlsEzSAM_EnableApplet(mlsCard_str* handleSAM,unsigned char Operator);

unsigned short mlsEzSAM_DebitSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
										unsigned char* arrCAN, unsigned char* arrCSN,
										unsigned char* DebitSessionKey);

unsigned short mlsEzSAM_AutoloadSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
										   unsigned char* arrCAN, unsigned char* arrCSN,
										   unsigned char* AutoloadSessionKey);

unsigned short mlsEzSAM_CreditSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
										 unsigned char* arrCAN, unsigned char* arrCSN,
										 unsigned char* CreditSessionKey);

unsigned short mlsEzSAM_EncryptedCreditParameterBlock(mlsCard_str* handleSAM,unsigned char* arrCreditRecordCRC, 
													  unsigned char SKf,unsigned char SKn,
													  unsigned char TransactionType,unsigned char* arrAmount,
													  unsigned char* arrCAN, unsigned char* arrCSN,
													  unsigned char* EncryptedCreditParameterBlock);

unsigned short mlsEzSAM_SignatureSessionKey(mlsCard_str* handleSAM,unsigned char* arrRandomNumber,
											unsigned char* arrCAN, unsigned char* arrCSN,
											unsigned char* SignatureSessionKey);


#ifdef __cplusplus
}
#endif

#endif /* mlsEzSAM.h */