#ifndef MLSLTASAM_H
#define MLSLTASAM_H
#include "mlsCepas.h"


#ifdef __cplusplus
extern "C" {
#endif

extern bool mlsLtaSAMMutualAuthFlag;
/************************************************************************/
/* Functions for LTA SAM Commands                                       */
/************************************************************************/
unsigned short mlsLtaSAM_SelectApplication(mlsCard_str *handleSAM);

unsigned short mlsLtaSAM_RequestSAMStatus(mlsCard_str *handleSAM, 
										  unsigned char Operation, 
										  unsigned char *pStatusInformation);

unsigned short mlsLtaSAM_InitialiseUpdate(mlsCard_str *handleSAM,
										  unsigned char *hostChallenge, 
										  unsigned char *pDataOut);

unsigned short mlsLtaSAM_ExternalAuthenticate(mlsCard_str *handleSAM,
											  unsigned char *hostCryptogram,
											  unsigned char *pCMAC
											  );

unsigned short mlsLtaSAM_RequestForMarriage(mlsCard_str *handleSAM,
											unsigned char *TerminalID,
											unsigned char *MarriageCryptogram);

unsigned short mlsLtaSAM_RequestForAuditRegister(mlsCard_str *handleSAM,
												 unsigned char *pAuditDataTime,
												 unsigned char AuditReason,
												 unsigned char *pDataOut);

unsigned short mlsLtaSAM_GetTerminalRandomNumber(mlsCard_str *handleSAM,
												 unsigned char *pDataOut);

unsigned short mlsLtaSAM_RequestForAuthentication(mlsCard_str *handleSAM,
												unsigned char *pCardRandomNumber,
												mlsReadPurseRecord *PurseRecord,
												unsigned char *pDataOut);

unsigned short mlsLtaSAM_RequestForDebit(mlsCard_str *handleSAM, 
										 unsigned char *pDataIn,
										 unsigned char *pDataOut);

unsigned short mlsLtaSAM_RequestForSignCertification(mlsCard_str *handleSAM,
													 unsigned char *ReceiptCryptogram,
													 unsigned char *pDataOut);

/************************************************************************/
/*LTA SAM  Mutual Authenticate Process                                         */
/************************************************************************/
unsigned short mlsLtaSAM_MutualAuthenticateProcess(mlsCard_str *handleSAM,
												   unsigned char *abKey,
												   unsigned char *abTerminalID,
												   bool bTransportKeyFlag
												   );


#ifdef __cplusplus
}
#endif

#endif /* mlsLtaSAM.h */
