#ifndef MLSCRYPTO_H
#define MLSCRYPTO_H
#include "mlsCryptoTypes.h"
#include "des.h"
#include "string.h"
#include "mlsSettlement.h"
//#define MLS_CRYPTO_TEST
//#define MLS_CRC_B




typedef struct
{
	CRYPTOBYTE Key[24];
	int KeyLength;
	CRYPTOBYTE IV[16];
	int IVLength;
	int CipherMode;
	int CryptoProvider;
}mlsCRYPTO_CONTEXT;

#if defined (MLS_CRYPTO_TEST)
int mlsCryptoTest1();
#endif
#if defined (MLS_CRC_B)
void mlsCRC_BTest1();
#endif

UINT16 LIB_Crc16(void *pStart,int iLen);
void CalculateCMac( unsigned char *pIV,unsigned char * m_abSessionKey,int m_CryptoMethod,unsigned char *pabMacedText, int eNumOfBytes, unsigned char *pabMac );
bool GenerateCMAC_SubKeys(unsigned char *pIV,unsigned char * m_abSessionKey,int m_SessionKeyCryptoMethod,
						  unsigned char *m_abCMAC_SuKey1,unsigned char *m_abCMAC_SuKey2);
void CalculateRetailMac(unsigned char *pIV,
						unsigned char * m_abFullKey, 
						unsigned char *pabMacText,	int eMacLen,
						unsigned char *pabMacOutput);


int mlsCryptoSetContext(mlsCRYPTO_CONTEXT *ctx ,CRYPTOBYTE *Key,CRYPTOBYTE *IV,int KeyLength,int IVLength,int CipherMode);

void mlsCrypto_Encrypt(mlsCRYPTO_CONTEXT *ctx,CRYPTOBYTE *PlainText,CRYPTOBYTE *CipherText,int LengthofPlainText);

void mlsCrypto_Decrypt(mlsCRYPTO_CONTEXT *ctx,CRYPTOBYTE *PlainText,CRYPTOBYTE *CipherText,int LengthofPlainText);
void mlsDes_crypt_ecb( des_context *ctx,int mode,int length, unsigned char *input,unsigned char *output );
void mlsDes3_crypt_ecb( des3_context *ctx,int mode,int length,unsigned char *input,unsigned char *output );
unsigned char UpdateCrc(unsigned char ch,unsigned short *lpwCrc);
void mlsComputeCrc(unsigned char *data,int length,unsigned char *result);
int mlsRandom();
void mlsConvertIntoBigEndianFomat(int iData,unsigned char*pArrBigEndian,int iLength);
void mlsConvertThreeByteToInt(int *iData,unsigned char*pArrBigEndian,int begin);
void mlsConvertFourByteToInt(int *iData,unsigned char*pArrBigEndian,int begin);
void mlsConvertTwoByteToInt(int *iData,unsigned char*pArrBigEndian);
#endif /* mlsCrypto.h */
