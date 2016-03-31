#include "mlsCrypto.h"
#include "des.h"
#include "havege.h"
#if defined(WIN32)

#include <windows.h>
#include <winbase.h>
#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#endif

static havege_state hs;
static bool hsInit = FALSE;

const UINT16 crc16_tab[] = {0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
							0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
							0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
							0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
							0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
							0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
							0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
							0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
							0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
							0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
							0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
							0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
							0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
							0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
							0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
							0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
							0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
							0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
							0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
							0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
							0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
							0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
							0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
							0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
							0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
							0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
							0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
							0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
							0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
							0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
							0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
							0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

UINT16 LIB_Crc16(void *pStart,int iLen)
{
	UINT8 *pS = (UINT8 *)pStart;
	UINT16 crc16;
	UINT16 polinomial = 0xB408;
	while(iLen--)
	{
		crc16 = crc16_tab[(*pS++^(UINT8)crc16)]^(crc16>>8);
	}
	return crc16;
}

/**************************************************************************************************/
void DoCmacPadding( unsigned char *abIntermediateBuffer, int & eBytesInBuffer,unsigned char *m_abCMAC_SuKey1,unsigned char *m_abCMAC_SuKey2 )
{
	BYTE  bCipherBlockSize =8;
	int i;

	// The OMAC is calculated like a normal mac but with a special padding:
	if( eBytesInBuffer % bCipherBlockSize )    // Block incomplete -> padding
	{
		// Padding:
		abIntermediateBuffer[ eBytesInBuffer ++ ] = 0x80;
		while( eBytesInBuffer % bCipherBlockSize )
			abIntermediateBuffer[ eBytesInBuffer ++ ] = 0x00;

		// XOR the last eight bytes with CMAC_SubKey2
		eBytesInBuffer -= bCipherBlockSize;
		for( i = 0; i < bCipherBlockSize; i ++ )
		{
			abIntermediateBuffer[ eBytesInBuffer + i ] ^= m_abCMAC_SuKey2[ i ];
		}
	}
	else                        // Block complete -> no padding
	{
		// XOR the last eight bytes with CMAC_SubKey1
		eBytesInBuffer -= bCipherBlockSize;
		for( i = 0; i < bCipherBlockSize; i ++ )
		{
			abIntermediateBuffer[ eBytesInBuffer + i ] ^= m_abCMAC_SuKey1[ i ];
		}
	}
	eBytesInBuffer += bCipherBlockSize;
}
BYTE DF8Encrypt( unsigned char *pIV,unsigned char * m_abSessionKey,int enCryptoMethod,
				unsigned char *pabCipherText, unsigned char *pabPlainText, int eNumOfBytes )
{
	unsigned char bPadBAdd = 0;  // ... padding bytes added
	unsigned char bBlockLength = 8;
	mlsCRYPTO_CONTEXT ctx;
	// Looks like we need to do padding for the cryptoPP:
	for( int i = ( eNumOfBytes % bBlockLength ); ( i > 0 ) && ( i < bBlockLength );
		pabPlainText[ eNumOfBytes ] = 0x00, eNumOfBytes ++, i ++, bPadBAdd ++ );

		mlsCryptoSetContext(&ctx,m_abSessionKey,pIV,enCryptoMethod,bBlockLength,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,pabPlainText,pabCipherText,eNumOfBytes);

	// Save the current init vector, which is the last cipher block of the cryptogram:
	memcpy( pIV, pabCipherText + eNumOfBytes - bBlockLength, bBlockLength );

	return bPadBAdd;
}
bool GenerateCMAC_SubKeys(unsigned char * m_abSessionKey,int m_CryptoMethod,unsigned char *m_abCMAC_SuKey1,unsigned char *m_abCMAC_SuKey2)
{
	unsigned char bMSB;
	int i;
	unsigned char bBlockLength = 8;
	unsigned char pIV[16];
	memset(pIV,0,16);
	// Generate the padding bytes for O-MAC by enciphering a zero block
	// with the actual session key:
	memset( m_abCMAC_SuKey1, 0, sizeof( m_abCMAC_SuKey1 ) );

	DF8Encrypt( pIV,m_abSessionKey,m_CryptoMethod, m_abCMAC_SuKey1, m_abCMAC_SuKey1, bBlockLength);

	// If the MSBit of the generated cipher == 1 -> K1 = (cipher << 1) ^ Rb ...
	// store MSB:
	bMSB = m_abCMAC_SuKey1[0];

	// Shift the complete cipher for 1 bit ==> K1:
	for( i = 0; i < ( bBlockLength - 1 ); i ++ )
	{
		m_abCMAC_SuKey1[ i ] <<= 1;
		// add the carry over bit:
		m_abCMAC_SuKey1[ i ] += ( ( m_abCMAC_SuKey1[ i + 1 ] & 0x80 ) ? 0x01:0x00 );
	}

	m_abCMAC_SuKey1[ bBlockLength - 1 ] <<= 1;

	if( bMSB & 0x80 )
		// XOR with Rb:
		m_abCMAC_SuKey1[ bBlockLength - 1 ] ^= 0x1B ;

	// store MSB:
	bMSB = m_abCMAC_SuKey1[0];

	// Shift K1 ==> K2:
	for( i = 0; i < ( bBlockLength - 1); i ++ )
	{
		m_abCMAC_SuKey2[i] = m_abCMAC_SuKey1[i] << 1;
		m_abCMAC_SuKey2[ i ] += ( ( m_abCMAC_SuKey1[ i + 1 ] & 0x80 ) ? 0x01:0x00 );
	}
	m_abCMAC_SuKey2[bBlockLength - 1] = m_abCMAC_SuKey1[ bBlockLength - 1] << 1;

	if( bMSB & 0x80 )
		// XOR with Rb:
		m_abCMAC_SuKey2[ bBlockLength - 1 ] ^= 0x1B;

	return false; // success
}
/**************************************************************************************************/
void CalculateCMac( unsigned char *pIV,unsigned char * m_abSessionKey,int m_CryptoMethod,unsigned char *pabMacedText, int eNumOfBytes, unsigned char *pabMac )
{    
	unsigned char * pabIntermediateBuffer;
	int eSizeOfText;
	unsigned char bBlockLength;
	unsigned char m_abCMAC_SuKey1[16];
	unsigned char m_abCMAC_SuKey2[16];
	memset(m_abCMAC_SuKey1,0,16);
	memset(m_abCMAC_SuKey2,0,16);
	// Adapt the crypto mode if the sessionkey is done in CBC_Send_Decrypt:
	bBlockLength = 8;

	// First we enlarge eNumOfBytes to a multiple of the cipher block size for allocating
	// memory of the intermediate buffer. Zero padding will be done by the DF8Encrypt function.
	// If we are ISO-authenticated, we have to do the spetial padding for the O-MAC:
	if(eNumOfBytes % 8)
	{
		eSizeOfText = eNumOfBytes + ( bBlockLength - (eNumOfBytes % bBlockLength) );
	}
	else
	{
		eSizeOfText = eNumOfBytes;
	}

	pabIntermediateBuffer = new unsigned char[eSizeOfText];

	memcpy( pabIntermediateBuffer, pabMacedText, eNumOfBytes );
	GenerateCMAC_SubKeys(m_abSessionKey,m_CryptoMethod,m_abCMAC_SuKey1,m_abCMAC_SuKey2);
	DoCmacPadding( pabIntermediateBuffer, eNumOfBytes,m_abCMAC_SuKey1,m_abCMAC_SuKey2 );

	DF8Encrypt( pIV,m_abSessionKey,m_CryptoMethod, pabIntermediateBuffer, pabIntermediateBuffer, eNumOfBytes );

	// Save the current init vector, which is the last cipher block of the cryptogram:
	memcpy( pIV, pabIntermediateBuffer + eSizeOfText - bBlockLength, bBlockLength );

	if( NULL != pabMac )
	{
		// The mac is the first half of the init vector:
		memcpy( pabMac, pIV,8 );
	}

	delete pabIntermediateBuffer;
	return;
}
//----------------------------------------------------------------------------------------------------
void CalculateRetailMac(unsigned char *pIV,
						unsigned char * m_abFullKey, 
						unsigned char *pabMacText,	int eMacLen,
						unsigned char *pabMacOutput)
{
	unsigned char * pabIntermediateBuffer;
	unsigned char * pabInterBufferOutput;
	int eSizeOfText;
	unsigned char bBlockLength = 8;
	mlsCRYPTO_CONTEXT ctx;
	unsigned char pIVtemp[8];
	memcpy(pIVtemp, pIV,8);

	//size of MaxText after padding
	if(eMacLen % 8)
	{
		eSizeOfText = eMacLen + ( bBlockLength - (eMacLen % bBlockLength) );
	}
	else
	{
		eSizeOfText = eMacLen;
	}
	pabIntermediateBuffer = (unsigned char *)malloc(eSizeOfText);
	pabInterBufferOutput = (unsigned char *)malloc(eSizeOfText);
	memcpy( pabIntermediateBuffer, pabMacText, eMacLen );
	//padding
	if( (eMacLen) % bBlockLength )    // Block incomplete -> padding
	{
		// Padding:
		pabIntermediateBuffer[eMacLen++] = 0x80;
		while( (eMacLen) % bBlockLength )
			pabIntermediateBuffer[eMacLen++] = 0x00;
	}

	if((eSizeOfText /8)>1)
	{
		//encrypt all block except the final block with Single DES
		mlsCryptoSetContext(&ctx,m_abFullKey,pIVtemp,CRYPTO_DES,8,CIPHERMODE_CBC);
		mlsCrypto_Encrypt(&ctx,pabIntermediateBuffer,pabInterBufferOutput,eSizeOfText - 8);
		memcpy(pIVtemp,pabInterBufferOutput+eSizeOfText - 16,8);
	}
	//encrypt the final block with tripleDES
	mlsCryptoSetContext(&ctx,m_abFullKey,pIVtemp,CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
	mlsCrypto_Encrypt(&ctx,pabIntermediateBuffer+(eSizeOfText-8),pabInterBufferOutput,8);
	memcpy(pabMacOutput,pabInterBufferOutput,8);	
	//
	free(pabIntermediateBuffer);
	free(pabInterBufferOutput);
}
/**************************************************************************************************/


int mlsCryptoSetContext(mlsCRYPTO_CONTEXT *ctx ,CRYPTOBYTE *Key,CRYPTOBYTE *IV,int KeyLength,int IVLength,int CryptoMode)
{
	int i;
	for(i = 0 ; i < KeyLength;i++)
		ctx->Key[i] = Key[i];
	for(i = 0;i < IVLength;i++)
		ctx->IV[i] = IV[i];
	ctx->KeyLength = KeyLength;
	ctx->IVLength = IVLength;
	if(KeyLength != CRYPTO_DES && KeyLength != CRYPTO_TRIPLEDES_EDE2 && KeyLength != CRYPTO_TRIPLEDES_EDE3)
		return -1;
	ctx->CryptoProvider = KeyLength;
	if(CryptoMode != CIPHERMODE_CBC && CryptoMode != CIPHERMODE_ECB)
		return -1;
	ctx->CipherMode = CryptoMode;
	return 1;

}

void mlsCrypto_Encrypt(mlsCRYPTO_CONTEXT *ctx,CRYPTOBYTE *PlainText,CRYPTOBYTE *CipherText,int LengthofPlainText)
{
	des_context desctx;
	des3_context des3ctx;
	unsigned char keyTemp[24];
	unsigned char IVTemp[8];
	memset(keyTemp,0,24);
	memset(IVTemp,0,8);
	memcpy(keyTemp,ctx->Key,ctx->KeyLength);
	if(ctx->IVLength != 0)
		memcpy(IVTemp,ctx->IV,8);

	switch(ctx->CryptoProvider)
	{
	case CRYPTO_DES:
		des_setkey_enc(&desctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des_crypt_cbc(&desctx,DES_ENCRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes_crypt_ecb(&desctx,DES_ENCRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	case CRYPTO_TRIPLEDES_EDE2:
		des3_set2key_enc(&des3ctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des3_crypt_cbc(&des3ctx,DES_ENCRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes3_crypt_ecb(&des3ctx,DES_ENCRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	case CRYPTO_TRIPLEDES_EDE3:
		des3_set3key_enc(&des3ctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des3_crypt_cbc(&des3ctx,DES_ENCRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes3_crypt_ecb(&des3ctx,DES_ENCRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	default:
		break;
	}
}

void mlsCrypto_Decrypt(mlsCRYPTO_CONTEXT *ctx,CRYPTOBYTE *PlainText,CRYPTOBYTE *CipherText,int LengthofPlainText)
{
	des_context desctx;
	des3_context des3ctx;
	unsigned char keyTemp[24];
	unsigned char IVTemp[8];
	memset(keyTemp,0,24);
	memset(IVTemp,0,8);
	memcpy(keyTemp,ctx->Key,ctx->KeyLength);
	if(ctx->IVLength != 0)
		memcpy(IVTemp,ctx->IV,8);
	switch(ctx->CryptoProvider)
	{
	case CRYPTO_DES:
		des_setkey_dec(&desctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des_crypt_cbc(&desctx,DES_DECRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes_crypt_ecb(&desctx,DES_DECRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	case CRYPTO_TRIPLEDES_EDE2:
		des3_set2key_dec(&des3ctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des3_crypt_cbc(&des3ctx,DES_DECRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes3_crypt_ecb(&des3ctx,DES_DECRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	case CRYPTO_TRIPLEDES_EDE3:
		des3_set3key_dec(&des3ctx,keyTemp);
		if(ctx->CipherMode == CIPHERMODE_CBC)
			des3_crypt_cbc(&des3ctx,DES_DECRYPT,LengthofPlainText,IVTemp,PlainText,CipherText);
		else
			mlsDes3_crypt_ecb(&des3ctx,DES_DECRYPT,LengthofPlainText,PlainText,CipherText);
		break;
	default:
		break;
	}
}

void mlsDes_crypt_ecb( des_context *ctx, int mode, int length, unsigned char *input, unsigned char *output )
{

	if( mode == DES_ENCRYPT )
	{
		while( length > 0 )
		{
			des_crypt_ecb( ctx, input, output );
			input  += 8;
			output += 8;
			length -= 8;
		}
	}
	else
	{
		while( length > 0 )
		{
			des_crypt_ecb( ctx, input, output );
			input  += 8;
			output += 8;
			length -= 8;
		}
	}
}

void mlsDes3_crypt_ecb( des3_context *ctx,int mode,int length,unsigned char *input,unsigned char *output )
{

	if( mode == DES_ENCRYPT )
	{
		while( length > 0 )
		{
			des3_crypt_ecb( ctx, input, output );
			input  += 8;
			output += 8;
			length -= 8;
		}
	}
	else
	{
		while( length > 0 )
		{
			des3_crypt_ecb( ctx, input, output );
			input  += 8;
			output += 8;
			length -= 8;
		}
	}
}
unsigned char UpdateCrc(unsigned char ch,unsigned short *lpwCrc)
{
	ch = (ch^(unsigned char)((*lpwCrc) & 0x00FF));
	ch = (ch^(ch<<4));
	*lpwCrc = (*lpwCrc >> 8)^((unsigned short)ch << 8)^((unsigned short)ch<<3)^((unsigned short)ch>>4);
	return(*lpwCrc);
}
void mlsComputeCrc(unsigned char *data,int length,unsigned char *result)
{
	unsigned short wCrc = 0xFFFF;
	unsigned char chBlock;
	do{
		chBlock = *data++;
		UpdateCrc(chBlock,&wCrc);
	}while(--length);
	wCrc = ~wCrc;
	result[0] = (unsigned char)(wCrc & 0xFF);
	result[1] = (unsigned char)((wCrc >> 8) & 0xFF);
}

int mlsRandom()
{
	if (!hsInit)
	{
		havege_init( &hs );
		hsInit = TRUE;
	}
	return havege_rand( &hs );
}
void mlsConvertIntoBigEndianFomat(int iData,unsigned char*pArrBigEndian,int iLength)
{
	unsigned char temp;
	int i = 0;
	for(i = 0; i < iLength;i++)
	{
		temp = iData >> 8*(iLength - 1-i);
		pArrBigEndian[i] = temp;
	}
}	
void mlsConvertThreeByteToInt(int *iData,unsigned char*pArrBigEndian,int begin)
{
	
	unsigned char temp;
	int iSecond = pArrBigEndian[begin+1];
	iSecond = iSecond <<8;
	int iThree = pArrBigEndian[begin+0];
	iThree = iThree << 16;
	*iData = (int)( (iThree|iSecond|pArrBigEndian[begin +2])<<8 )>>8 ;
}
void mlsConvertFourByteToInt(int *iData,unsigned char*pArrBigEndian,int begin)
{

	unsigned char temp;
	int iSecond = pArrBigEndian[begin+2];
	iSecond = iSecond <<8;
	int iThree = pArrBigEndian[begin+1];
	iThree = iThree << 16;
	int iFour = pArrBigEndian[begin+0];
	iFour = iFour << 24;
	*iData = iFour|iThree|iSecond|pArrBigEndian[begin +3];
}
void mlsConvertTwoByteToInt(int *iData,unsigned char*pArrBigEndian)
{

	unsigned char temp;
	int iSecond = pArrBigEndian[0];
	iSecond = iSecond <<8;
	*iData = iSecond|pArrBigEndian[1];
}
#if defined (MLS_CRC_B)
void mlsCRC_BTest1()
{
	unsigned char data2[4] = {0x0a,0x12,0x34,0x56};
	unsigned char result[2];
	mlsComputeCrc(&data2[0],4,&result[0]);
}
#endif
#if defined (MLS_CRYPTO_TEST)
int mlsCryptoTest1()
{
		unsigned char newKey[24] = { 0x00,0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01};

		unsigned char plain[16] ={0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,0x00,0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
		unsigned char cipher[16];
		unsigned char IV[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x00};
		unsigned char pabMac[8];
		CalculateCMac( IV,newKey,CRYPTO_TRIPLEDES_EDE2,plain, 15, pabMac );
		mlsCRYPTO_CONTEXT ctx;
		int resultSetContext;
		resultSetContext  = mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_DES,8,CIPHERMODE_CBC);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);
		mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_CBC);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);
		mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE3,8,CIPHERMODE_CBC);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);

		mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE3,8,CIPHERMODE_ECB);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);

		mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_TRIPLEDES_EDE2,8,CIPHERMODE_ECB);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);

		mlsCryptoSetContext(&ctx,&newKey[0],&IV[0],CRYPTO_DES,8,CIPHERMODE_ECB);
		mlsCrypto_Encrypt(&ctx,plain,cipher,16);
		mlsCrypto_Decrypt(&ctx,cipher,plain,16);
		return 0;
}
#endif
