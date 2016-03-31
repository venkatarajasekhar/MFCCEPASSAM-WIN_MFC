#if !defined(__MLSSAM_H__)
#define __MLSSAM_H__
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA

#include "CardExchange_I2.h"

#include "mlsCard.h"

//#include "DESFireSAMCommands_I1.h"

#include "phcsDESFire8SAMXDefinitions.h"

#include "phcsDESFire8SAMXCard.hpp"

using namespace mlsMFCDll;
using namespace phcs_RFID;
//using namespace phcs_READER;
//using namespace phcs_PROTOCOL;
//using namespace phcs_MfDesFire8;
using namespace phcs_DESFIRE8SAMX;

namespace mlsMFCDll
{
	class AFX_EXT_CLASS mlsSAM : public mlsCard
	{
	public:
		//default constructor
		mlsSAM();
		
		//Destructor
		~mlsSAM();

		void SAMInit(mlsCard Card);

		// get the SAM version information
		 uint16_t GetVersion( phcsDESFire8SAMX_VERSION_INFO_t* pstVersionInfo );

		// get a key entry
		 uint16_t GetKeyEntry( uint8_t bKeyNo,
			uint8_t *pbNumKeys,
			phcsDESFire8SAMX_KEY_ENTRY_t *pstKeyEntry );

		// disable cryptographic functionality
		 uint16_t DisableCrypto( uint16_t wProMas );

		// get Key Usage Counter entry
		 uint16_t GetKUCEntry( uint8_t bRefNoKUC,
			phcsDESFire8SAMX_KUC_ENTRY_t *pstKUCEntry );

		// perform a host authentication
		 uint16_t AuthenticateHost( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthData,
			uint8_t   bKeyType,
			uint8_t *pbSecretKey,
			bool qKeepInitVector = false,
			bool qCMAC = false );

		// change a key entry
		 uint16_t ChangeKeyEntry( uint8_t bKeyNo,
			uint8_t bProMas,
			phcsDESFire8SAMX_KEY_ENTRY_FULL_t *pstKeyEntry,
			uint8_t *pbUID = 0,
			bool qPlain = false );

		// dump the (current) session key
		 uint16_t DumpSessionKey( bool qEncrypt,
			uint8_t bLogicalChannel,
			phcsDESFire8SAMX_Commands *poSAM,
			uint8_t *pbSessionKey,
			uint8_t *pbSessionKeyLength );

		// change a key usage counter
		 uint16_t ChangeKUCEntry( uint8_t bRefNoKUC,
			uint8_t bProMas,
			phcsDESFire8SAMX_KUC_ENTRY_t *pstKUCEntry,
			uint8_t *pbUID = 0,
			bool qPlain = false );

		// prepare for changing a key in the PICC
		 void PrepareForChangeKeyPICC(
			phcsDESFire8SAMX_CHANGE_KEY_PICC_DATA_t *pstChangeKeyPicc );

		// create cryptogram, which has to be sent to the card
		// in order to change any key of the PICC
		 uint16_t ChangeKeyPICC( uint8_t *pbCryptogram,
			uint8_t *pbCryptLength,
			bool qChangeCardMasterKey = false );

		// disable a key entry
		 uint16_t DisableKeyEntry( uint8_t bKeyNo );

		// create a key string or get a key
		// in order to change any key of a Mifare card
		 uint16_t ChangeKeyMifare( bool qEncrypt,
			phcsDESFire8SAMX_CHANGE_KEY_MIFARE_DATA_t *pstChangeKeyMifare,
			uint8_t *pbKeyString,
			uint8_t *pbKeyStringLength );

		// select the application specified by AID
		 uint16_t SelectApplication( uint8_t *pbDF_AID );

		// load the init vector (used for KEEP IV)
		 uint16_t LoadInitVector( uint8_t *pbInitVector );

		// verifying a MAC
		 uint16_t VerifyMAC( bool qMoreData, 
			uint8_t *pbPlainData, 
			uint32_t eLen,
			uint8_t  bBytesToCheck = 0);

		// generate a MAC (= Message Authentication Code)
		 uint16_t GenerateMAC( bool qMoreData,
			uint8_t *pbPlainData,
			uint32_t eLen,
			phcsDESFire8SAMX_MAC_t *peMAC );

		// decipher data by SAM
		 uint16_t DecipherData( uint8_t *pbData, bool qUseLFI, uint32_t eLen );

		// encipher data by SAM
		 uint16_t EncipherData( bool qMoreData,
			uint8_t *pbInputBuffer,
			uint32_t eInputLen,
			uint8_t *pbOutputBuffer,
			uint32_t *peOutputLen,
			uint8_t bPlainByteCount = 0 );


		// prepare for PICC authentication
		 void PrepareForAuthenticatePICC( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthStruct );

		// authenticate both, the SAM and the PICC, part one
		 uint16_t AuthenticatePICCPartOne( uint8_t *pbEkNoRndB,
			uint8_t bEkNoRndBLength,
			uint8_t *pbDkNoRndARndB,
			uint8_t *pbDkNoRndARndBLength );

		// part two of combined authentication for the PICC and the SAM
		 uint16_t AuthenticatePICCPartTwo( uint8_t *pbEkNoRndA,
			uint8_t bEkNoRndALength );

		// diversify a Mifare key
		 uint16_t DiversifyMifareKey( bool qEncrypt,
			phcsDESFire8SAMX_MIFARE_DATA_t *pstMifareStruct,
			uint8_t *pbDiversifiedKey );

		// authenticate with a Mifare card, part one
		 uint16_t AuthenticateMifarePartOne( phcsDESFire8SAMX_MIFARE_DATA_t *pstMifareStruct,
			uint8_t *pbNumberRB,
			bool qUseDiversification,
			uint8_t bDivBlockNumber,
			uint8_t *pbTokenAB );

		// authenticate with a Mifare card, part two
		 uint16_t AuthenticateMifarePartTwo( uint8_t *pbTokenBA );

		// kill any existing authentication
		 uint16_t KillAuthentication( void );

		// get a random number
		 uint16_t IsoGetChallenge( uint8_t *pbNoRnd,
			uint16_t *pbNoRndLength );

		// perform an ISO host authentication 
		 uint16_t IsoAuthenticateHost( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthData,
			uint8_t bKeyType,
			uint8_t *pbSecretKey,
			bool qKeepInitVector = false,
			bool qCMAC = false );

		// ISO authenticate both, the SAM and the PICC, part one 
		 uint16_t IsoAuthenticatePICCPartOne( uint8_t *pbRPicc1,
			uint8_t bRPicc1Length,
			uint8_t *pbEkNoRPcd1Picc1,
			uint8_t *pbEkNoRPcd1Picc1Length,
			uint8_t *pbRPcd2,
			uint8_t *pbRPcd2Length );

		// part two of combined ISO authentication for the PICC and the SAM
		 uint16_t IsoAuthenticatePICCPartTwo( uint8_t *pbEkNoRPicc2Pcd2,
			uint8_t bEkNoRPicc2Pcd2Length );

		// set SAM into power down mode
		 uint16_t Sleep( void );

		 phcsDESFire8SAMX_Card* GetNXPSAMObj()
		 {
			 return m_poSAMCardObject;
		 }
	private:

		CByteArray m_cbaAPDUCommand;
		CByteArray m_cbaAPDURespond;
		CardExchange* m_poCardExchange;
		PCSC_INFO* stPCSCInfo;
		phcsDESFire8SAMX_Card*  m_poSAMCardObject;
	};
}//namespace
#undef AFX_DATA
#define AFX_DATA
#endif /* __MLSSAM_H__ */