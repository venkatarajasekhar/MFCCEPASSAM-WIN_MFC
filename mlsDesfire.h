#if !defined(__MLSDESFIRE_H__)
#define __MLSDESFIRE_H__
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA

#include "mlsTypes.h"

#include "mlsCard.h"

#include "CardExchange_I2.h"

#include "phcsMfDesFire8_DF8ISOCard.h"

#include "mlsSAM.h"

using namespace phcs_RFID;
//using namespace phcs_READER;
using namespace phcs_PROTOCOL;
using namespace phcs_MfDesFire8;


namespace mlsMFCDll
{
	class AFX_EXT_CLASS mlsDesfire : public mlsCard
	{
	public:
		//default constructor
		mlsDesfire();
		//mlsDesfire(mlsCard* Card);
		~mlsDesfire();
		void DesfireInit(mlsCard Card);

		//-------------------------------------------------------------------------
		//      New within DesFire8
		//-------------------------------------------------------------------------

		// CMD = 0x51
		WORD GetCardUID( BYTE abUID[10] ) ;

		// CMD = 0x5C
		// General SetCardConfiguration function:
		WORD SetConfiguration( 
			CARD_CONFIG FirstParam,
			const BYTE* abConfigByteBuffer,
			const BYTE bSizeOfConfigByteBuffer ) ;

		// SetCardConfigurationByte: bConfigByte = 0xXX -> random UID
		WORD SetCardConfigurationByte(
			const BYTE bConfigByte ) ;

		WORD SetDefaultKeyAndVersion(
			const BYTE abDefaultKey[24],
			const BYTE bDefaultKeyVersion ) ;

		WORD SetUserDefinedATS(
			const BYTE *pbUserATS,
			const BYTE bUserATSLength ) ;

		// CMD = 0x6E
		WORD GetFreeMemory( DWORD * peNumOfBytes ) ;

		// UpdateLastRecord modifies the last record in a record file (WriteRecord adds a new record)
		WORD UpdateLastRecord(
			BYTE        bFileID,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer ) ;

		WORD UpdateLastRecord(
			BYTE        bFileID,
			BYTE        bCommMode,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer ) ;

		WORD GetDFNames(
			BYTE        bMaxDFNames,            // max number of DFNames to retreive
			DF_NAME     *adfnDfNames,           // pointer to an array of DFNames
			BYTE        *pbNumOfDFNames ) ;  // number of retreived DFNames

		// DF8Command: Retrieves all ISO7814-4 DF IDs stored within an Application.
		// One ISO FID consists of two bytes. One Application can holde up to 32 files.
		WORD GetISOFid(
			BYTE        bMaxFileIDs,
			WORD        *pwFileIDBuffer,
			BYTE        *pbNFileIDsFound ) ;

		// Overloaded versions of CreateApplication(...)
		// DF8Command:
		WORD CreateApplication(
			DWORD       eNewApplicationID,
			BYTE        bKeySettings1,
			BYTE        bKeySettings2,
			WORD        wISO_Fid,
			BYTE        *pb7816DF_Name,
			BYTE        b7816DF_Name_Size = 0 ) ;

		// Overloaded versions of CreateApplication(...)
		// DF8Command:
		WORD CreateApplication(
			DWORD           eNewApplicationID,
			BYTE            bKeySettings1,
			BYTE            bNumOfKeys,
			bool            qUse2BytesISOFid,
			CRYPTO_METHOD   enCryptoMethod ) ;

		// Overloaded versions of CreateApplication(...)
		// DF8Command:
		WORD CreateApplication(
			DWORD           eNewApplicationID,
			BYTE            bKeySettings1,
			BYTE            bNumOfKeys,
			bool            qUse2BytesISOFid,
			CRYPTO_METHOD   enCryptoMethod,
			WORD            wISO_Fid,
			BYTE            *pb7816DF_Name,
			BYTE            b7816DF_Name_Size = 0 ) ;

		// DF8Command:
		WORD AuthenticateISO(
			BYTE            bKeyNumber,
			const BYTE      *pbAccessKey,
			CRYPTO_METHOD   enCryptoMethod ) ;

		// DF8Command:
		WORD AuthenticateAES(
			BYTE        bKeyNumber,
			const BYTE  *pbAccessKey ) ;
		//-------------------------------------------------------------------------
		// Overloaded functions supporting DesFire8 extended functionality:

		WORD CreateStdDataISOFile(
			BYTE        bFileID,
			WORD        wISOFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eFileSize ) ;

		WORD CreateBackupDataISOFile(
			BYTE        bFileID,
			WORD        wISOFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eFileSize ) ;

		WORD CreateLinearRecordISOFile(
			BYTE        bFileID,
			WORD        wISOFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eRecordSize,
			DWORD       eMaxNRecords ) ;

		WORD CreateCyclicRecordISOFile(
			BYTE        bFileID,
			WORD        wISOFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eRecordSize,
			DWORD       eMaxNRecords ) ;

		WORD ChangeKey(
			BYTE        bKeyNumber,
			BYTE        bKeyVersion,
			const BYTE  *pbNewKey,
			const BYTE  *pbPrevKey = NULL ) ;
		// Changes the CardMasterKey to an AES key of 16 bytes with key version.
		WORD ChangeCardMasterKey(
			BYTE            bKeyVersion,
			const BYTE      *pbNewKey );

		// Changes the CardMasterKey to a (3)Des, (3)IsoDes of 16 bytes or 3K3Des key of 24 bytes
		// Note: CRM_3DES_DF4 and CRM_3DES_ISO use the same type of key (16 bytes)
		WORD ChangeCardMasterKey(
			CRYPTO_METHOD   KeyType,
			const BYTE      *pbNewKey );
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		//  Security related commands.

		WORD Authenticate(
			BYTE            bKeyNumber,
			const BYTE      *pbAccessKey,
			CRYPTO_METHOD   enCryptoMethod ) ;

		WORD ChangeKeySettings(
			BYTE        bNewKeySettings ) ;

		WORD GetKeySettings( 
			BYTE        *pbKeySettings,
			BYTE        *pbNKeys ) ;

		WORD ChangeKey(
			BYTE        bKeyNumber,
			const BYTE  *pbNewKey,
			const BYTE  *pbPrevKey = NULL ) ;

		WORD GetKeyVersion(
			BYTE        bKeyNumber,
			BYTE        *pbKeyVersion ) ;

		//  Card level commands.

		WORD FormatPICC( ) ;

		WORD CreateApplication( 
			DWORD       eNewApplicationID,
			BYTE        bKeySettings,
			BYTE        bNApplicationKeys ) ;

		WORD DeleteApplication(
			DWORD       eApplicationID ) ;

		WORD GetApplicationIDs(
			BYTE        bMaxApplicationIDs,
			DWORD       *peApplicationIDs,
			BYTE        *pbNApplicationIDs ) ;

		WORD SelectApplication( 
			DWORD       eApplicationID ) ;

		WORD GetVersion(
			VERSION_INFO
			*pstVersionInfo ) ;

		//  Application level commands:

		WORD GetFileIDs(
			BYTE        bMaxFileIDs,
			BYTE        *pbFileIDBuffer,
			BYTE        *pbNFileIDsFound ) ;

		WORD GetFileSettings(
			BYTE        bFileID,
			BYTE        *pbFileType = NULL,
			BYTE        *pbCommMode = NULL,
			WORD        *pwAccessRights = NULL,
			ADDITIONAL_FILE_SETTINGS
			*punAdditionalFileSettings = NULL ) ;

		WORD ChangeFileSettings( 
			BYTE        bFileID,
			BYTE        bNewCommMode,
			WORD        wNewAccessRights ) ;

		WORD ChangeFileSettings( 
			BYTE        bFileID,
			BYTE        bCommEncrypted,
			BYTE        bNewCommMode,
			WORD        wNewAccessRights ) ;

		WORD CreateStdDataFile( 
			BYTE        bFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eFileSize ) ;

		WORD CreateBackupDataFile( 
			BYTE        bFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eFileSize ) ;

		// Options: bit 0 -> en/disable limited credit
		//          bit 1 -> en/disable get free value
		WORD CreateValueFile( 
			BYTE        bFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			long        lLowerLimit,
			long        lUpperLimit,
			long        lInitialValue,
			BYTE        bOptions ) ;

		WORD CreateLinearRecordFile( 
			BYTE        bFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eRecordSize,
			DWORD       eMaxNRecords ) ;

		WORD CreateCyclicRecordFile( 
			BYTE        bFileID,
			BYTE        bCommMode,
			WORD        wAccessRights,
			DWORD       eRecordSize,
			DWORD       eMaxNRecords ) ;

		WORD DeleteFile(
			BYTE        bFileID ) ;

		//  Data file commands.

		WORD ReadData(
			BYTE        bFileID,
			DWORD       eFromOffset,
			DWORD       eNBytesToRead,
			BYTE        *pbDataBuffer,
			DWORD       *peNBytesRead = NULL ) ;

		WORD ReadData(
			BYTE        bFileID,
			BYTE        bCommMode,
			DWORD       eFromOffset,
			DWORD       eNBytesToRead,
			BYTE        *pbDataBuffer,
			DWORD       *peNBytesRead = NULL ) ;

		WORD WriteData(
			BYTE        bFileID,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer ) ;

		WORD WriteData(
			BYTE        bFileID,
			BYTE        bCommMode,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer ) ;

		//  Value file commands.

		WORD GetValue(          
			BYTE        bFileID,
			long        *plAmount ) ;

		WORD GetValue(          
			BYTE        bFileID,
			BYTE        bCommMode,
			long        *plAmount ) ;

		WORD Credit(
			BYTE        bFileID,
			long        lAmount ) ;

		WORD Credit(
			BYTE        bFileID,
			BYTE        bCommMode,
			long        lAmount ) ;

		WORD Debit(
			BYTE        bFileID,
			long        lAmount ) ;

		WORD Debit(
			BYTE        bFileID,
			BYTE        bCommMode,
			long        lAmount ) ;

		WORD LimitedCredit(
			BYTE        bFileID,
			long        lAmount ) ;

		WORD LimitedCredit(
			BYTE        bFileID,
			BYTE        bCommMode,
			long        lAmount ) ;

		//  Record file commands.

		WORD ReadRecords(
			BYTE        bFileID,
			DWORD       eFromRecord,
			DWORD       eNRecordsToRead,
			DWORD       eRecordSize,
			BYTE        *pbDataBuffer,
			DWORD       *peNRecordsRead = NULL ) ;

		WORD ReadRecords(
			BYTE        bFileID,
			BYTE        bCommMode,
			DWORD       eFromRecord,
			DWORD       eNRecordsToRead,
			DWORD       eRecordSize,
			BYTE        *pbDataBuffer,
			DWORD       *peNRecordsRead = NULL ) ;

		WORD WriteRecord(
			BYTE        bFileID,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer );

		WORD WriteRecord(
			BYTE        bFileID,
			BYTE        bCommMode,
			DWORD       eFromOffset,
			DWORD       eNBytesToWrite,
			const BYTE  *pbDataBuffer );

		WORD ClearRecordFile( BYTE bFileID );

		//  General file level commands.

		WORD CommitTransaction( void );

		WORD AbortTransaction( void );

		void ConnectSAM(mlsSAM *poDESFireSAM = NULL);

	private:

		DF8ISOCard*  m_poISOCardObject;
		CByteArray m_cbaAPDUCommand;
		CByteArray m_cbaAPDURespond;
		CardExchange* m_poCardExchange;
		PCSC_INFO* stPCSCInfo;
	};
}//namespace
#undef AFX_DATA
#define AFX_DATA
#endif /* __MLSDESFIRE_H__ */