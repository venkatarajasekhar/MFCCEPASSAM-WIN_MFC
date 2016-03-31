#include "stdafx.h"
#include "mlsCard.h"
#include "mlsException.h"
#include "mlsDesfire.h"

using namespace mlsMFCDll;
using namespace phcs_RFID;
using namespace phcs_PROTOCOL;
using namespace phcs_MfDesFire8;

mlsDesfire::mlsDesfire()
{
	m_poISOCardObject = NULL;
	m_poCardExchange = NULL;
	m_cbaAPDUCommand.RemoveAll();
	m_cbaAPDURespond.RemoveAll();
	stPCSCInfo = new PCSC_INFO();	
}

mlsDesfire::~mlsDesfire()
{
	if (stPCSCInfo != NULL) delete stPCSCInfo;
	if (m_poCardExchange != NULL) delete m_poCardExchange;
	if (m_poISOCardObject != NULL) delete m_poISOCardObject;
}


void mlsDesfire::DesfireInit(mlsCard Card)
{
	m_CardHandle = Card.GetCardHandle();
	m_scPciReq = (LPCSCARD_IO_REQUEST)Card.GetPCIRequest();
	
	stPCSCInfo->hCard = m_CardHandle ;
	stPCSCInfo->pioPCI =(LPSCARD_IO_REQUEST) m_scPciReq;
	m_poCardExchange =new CardExchange(stPCSCInfo);
	m_poISOCardObject = new DF8ISOCard(m_poCardExchange);

}

WORD mlsMFCDll::mlsDesfire::GetCardUID( BYTE abUID[10] )
{
	return m_poISOCardObject->GetCardUID(abUID);
}

WORD mlsMFCDll::mlsDesfire::SetConfiguration( CARD_CONFIG FirstParam, const BYTE* abConfigByteBuffer, const BYTE bSizeOfConfigByteBuffer )
{
	return m_poISOCardObject->SetConfiguration(FirstParam, abConfigByteBuffer, bSizeOfConfigByteBuffer );
}

WORD mlsMFCDll::mlsDesfire::SetCardConfigurationByte( const BYTE bConfigByte )
{
	return m_poISOCardObject->SetCardConfigurationByte(bConfigByte );
}

WORD mlsMFCDll::mlsDesfire::SetDefaultKeyAndVersion( const BYTE abDefaultKey[24], const BYTE bDefaultKeyVersion )
{
	return m_poISOCardObject->SetDefaultKeyAndVersion(abDefaultKey,bDefaultKeyVersion );
}

WORD mlsMFCDll::mlsDesfire::SetUserDefinedATS( const BYTE *pbUserATS, const BYTE bUserATSLength )
{
	return m_poISOCardObject->SetUserDefinedATS( pbUserATS, bUserATSLength );
}

WORD mlsMFCDll::mlsDesfire::GetFreeMemory( DWORD * peNumOfBytes )
{
	return m_poISOCardObject->GetFreeMemory( peNumOfBytes );
}

WORD mlsMFCDll::mlsDesfire::UpdateLastRecord( BYTE bFileID, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->UpdateLastRecord( bFileID, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::UpdateLastRecord( BYTE bFileID, BYTE bCommMode, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->UpdateLastRecord( bFileID, bCommMode, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::GetDFNames( BYTE bMaxDFNames, /* max number of DFNames to retreive */ DF_NAME *adfnDfNames, /* pointer to an array of DFNames */ BYTE *pbNumOfDFNames )
{
	return m_poISOCardObject->GetDFNames(bMaxDFNames, adfnDfNames, pbNumOfDFNames );
}

WORD mlsMFCDll::mlsDesfire::GetISOFid( BYTE bMaxFileIDs, WORD *pwFileIDBuffer, BYTE *pbNFileIDsFound )
{
	return m_poISOCardObject->GetISOFid( bMaxFileIDs, pwFileIDBuffer, pbNFileIDsFound );
}

WORD mlsMFCDll::mlsDesfire::CreateApplication( DWORD eNewApplicationID, BYTE bKeySettings1, BYTE bKeySettings2, WORD wISO_Fid, BYTE *pb7816DF_Name, BYTE b7816DF_Name_Size /*= 0 */ )
{
	return m_poISOCardObject->CreateApplication( eNewApplicationID, bKeySettings1, bKeySettings2, wISO_Fid, pb7816DF_Name, b7816DF_Name_Size /*= 0 */ );
}

WORD mlsMFCDll::mlsDesfire::CreateApplication( DWORD eNewApplicationID, BYTE bKeySettings1, BYTE bNumOfKeys, bool qUse2BytesISOFid, CRYPTO_METHOD enCryptoMethod )
{
	return m_poISOCardObject->CreateApplication(eNewApplicationID, bKeySettings1, bNumOfKeys, qUse2BytesISOFid, enCryptoMethod );
}

WORD mlsMFCDll::mlsDesfire::CreateApplication( DWORD eNewApplicationID, BYTE bKeySettings1, BYTE bNumOfKeys, bool qUse2BytesISOFid, CRYPTO_METHOD enCryptoMethod, WORD wISO_Fid, BYTE *pb7816DF_Name, BYTE b7816DF_Name_Size /*= 0 */ )
{
	return m_poISOCardObject->CreateApplication( eNewApplicationID, bKeySettings1, bNumOfKeys, qUse2BytesISOFid, enCryptoMethod, wISO_Fid, pb7816DF_Name, b7816DF_Name_Size /*= 0 */ );
}

WORD mlsMFCDll::mlsDesfire::CreateApplication( DWORD eNewApplicationID, BYTE bKeySettings, BYTE bNApplicationKeys )
{
	return m_poISOCardObject->CreateApplication( eNewApplicationID, bKeySettings, bNApplicationKeys );
}
WORD mlsMFCDll::mlsDesfire::AuthenticateISO( BYTE bKeyNumber, const BYTE *pbAccessKey, CRYPTO_METHOD enCryptoMethod )
{
	return m_poISOCardObject->AuthenticateISO( bKeyNumber, pbAccessKey, enCryptoMethod );
}

WORD mlsMFCDll::mlsDesfire::AuthenticateAES( BYTE bKeyNumber, const BYTE *pbAccessKey )
{
	return m_poISOCardObject->AuthenticateAES( bKeyNumber, pbAccessKey );
}

WORD mlsMFCDll::mlsDesfire::CreateStdDataISOFile( BYTE bFileID, WORD wISOFileID, BYTE bCommMode, WORD wAccessRights, DWORD eFileSize )
{
	return m_poISOCardObject->CreateStdDataISOFile( bFileID, wISOFileID, bCommMode, wAccessRights, eFileSize );
}

WORD mlsMFCDll::mlsDesfire::CreateBackupDataISOFile( BYTE bFileID, WORD wISOFileID, BYTE bCommMode, WORD wAccessRights, DWORD eFileSize )
{
	return m_poISOCardObject->CreateBackupDataISOFile( bFileID, wISOFileID, bCommMode, wAccessRights, eFileSize );
}

WORD mlsMFCDll::mlsDesfire::CreateLinearRecordISOFile( BYTE bFileID, WORD wISOFileID, BYTE bCommMode, WORD wAccessRights, DWORD eRecordSize, DWORD eMaxNRecords )
{
	return m_poISOCardObject->CreateLinearRecordISOFile( bFileID, wISOFileID, bCommMode, wAccessRights, eRecordSize, eMaxNRecords );
}

WORD mlsMFCDll::mlsDesfire::CreateCyclicRecordISOFile( BYTE bFileID, WORD wISOFileID, BYTE bCommMode, WORD wAccessRights, DWORD eRecordSize, DWORD eMaxNRecords )
{
	return m_poISOCardObject->CreateCyclicRecordISOFile( bFileID, wISOFileID, bCommMode, wAccessRights, eRecordSize, eMaxNRecords );
}

WORD mlsMFCDll::mlsDesfire::ChangeKey( BYTE bKeyNumber, BYTE bKeyVersion, const BYTE *pbNewKey, const BYTE *pbPrevKey /*= NULL */ )
{
	return m_poISOCardObject->ChangeKey( bKeyNumber, bKeyVersion, pbNewKey, pbPrevKey /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::ChangeKey( BYTE bKeyNumber, const BYTE *pbNewKey, const BYTE *pbPrevKey /*= NULL */ )
{
	return m_poISOCardObject->ChangeKey( bKeyNumber, pbNewKey, pbPrevKey /*= NULL */ );
}
WORD mlsMFCDll::mlsDesfire::Authenticate( BYTE bKeyNumber, const BYTE *pbAccessKey, CRYPTO_METHOD enCryptoMethod )
{
	return m_poISOCardObject->Authenticate( bKeyNumber, pbAccessKey, enCryptoMethod );
}

WORD mlsMFCDll::mlsDesfire::ChangeKeySettings( BYTE bNewKeySettings )
{
	return m_poISOCardObject->ChangeKeySettings( bNewKeySettings );
}

WORD mlsMFCDll::mlsDesfire::GetKeySettings( BYTE *pbKeySettings, BYTE *pbNKeys )
{
	return m_poISOCardObject->GetKeySettings( pbKeySettings, pbNKeys );
}

WORD mlsMFCDll::mlsDesfire::GetKeyVersion( BYTE bKeyNumber, BYTE *pbKeyVersion )
{
	return m_poISOCardObject->GetKeyVersion( bKeyNumber, pbKeyVersion );
}

WORD mlsMFCDll::mlsDesfire::FormatPICC()
{
	return m_poISOCardObject->FormatPICC();
}

WORD mlsMFCDll::mlsDesfire::DeleteApplication( DWORD eApplicationID )
{
	return m_poISOCardObject->DeleteApplication( eApplicationID );
}

WORD mlsMFCDll::mlsDesfire::GetApplicationIDs( BYTE bMaxApplicationIDs, DWORD *peApplicationIDs, BYTE *pbNApplicationIDs )
{
	return m_poISOCardObject->GetApplicationIDs( bMaxApplicationIDs, peApplicationIDs, pbNApplicationIDs );
}

WORD mlsMFCDll::mlsDesfire::SelectApplication( DWORD eApplicationID )
{
	return m_poISOCardObject->SelectApplication( eApplicationID );
}

WORD mlsMFCDll::mlsDesfire::GetVersion( VERSION_INFO *pstVersionInfo )
{
	return m_poISOCardObject->GetVersion( pstVersionInfo );
}

WORD mlsMFCDll::mlsDesfire::GetFileIDs( BYTE bMaxFileIDs, BYTE *pbFileIDBuffer, BYTE *pbNFileIDsFound )
{
	return m_poISOCardObject->GetFileIDs( bMaxFileIDs, pbFileIDBuffer, pbNFileIDsFound );
}

WORD mlsMFCDll::mlsDesfire::GetFileSettings( BYTE bFileID, BYTE *pbFileType /*= NULL*/, BYTE *pbCommMode /*= NULL*/, WORD *pwAccessRights /*= NULL*/, ADDITIONAL_FILE_SETTINGS *punAdditionalFileSettings /*= NULL */ )
{
	return m_poISOCardObject->GetFileSettings( bFileID, pbFileType /*= NULL*/, pbCommMode /*= NULL*/, pwAccessRights /*= NULL*/, punAdditionalFileSettings /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::ChangeFileSettings( BYTE bFileID, BYTE bNewCommMode, WORD wNewAccessRights )
{
	return m_poISOCardObject->ChangeFileSettings( bFileID, bNewCommMode, wNewAccessRights );
}

WORD mlsMFCDll::mlsDesfire::ChangeFileSettings( BYTE bFileID, BYTE bCommEncrypted, BYTE bNewCommMode, WORD wNewAccessRights )
{
	return m_poISOCardObject->ChangeFileSettings( bFileID, bCommEncrypted, bNewCommMode, wNewAccessRights );
}

WORD mlsMFCDll::mlsDesfire::CreateStdDataFile( BYTE bFileID, BYTE bCommMode, WORD wAccessRights, DWORD eFileSize )
{
	return m_poISOCardObject->CreateStdDataFile( bFileID, bCommMode, wAccessRights, eFileSize );
}

WORD mlsMFCDll::mlsDesfire::CreateBackupDataFile( BYTE bFileID, BYTE bCommMode, WORD wAccessRights, DWORD eFileSize )
{
	return m_poISOCardObject->CreateBackupDataFile( bFileID, bCommMode, wAccessRights, eFileSize );
}

WORD mlsMFCDll::mlsDesfire::CreateValueFile( BYTE bFileID, BYTE bCommMode, WORD wAccessRights, long lLowerLimit, long lUpperLimit, long lInitialValue, BYTE bOptions )
{
	return m_poISOCardObject->CreateValueFile( bFileID, bCommMode, wAccessRights, lLowerLimit, lUpperLimit, lInitialValue, bOptions );
}

WORD mlsMFCDll::mlsDesfire::CreateLinearRecordFile( BYTE bFileID, BYTE bCommMode, WORD wAccessRights, DWORD eRecordSize, DWORD eMaxNRecords )
{
	return m_poISOCardObject->CreateLinearRecordFile( bFileID, bCommMode, wAccessRights, eRecordSize, eMaxNRecords );
}

WORD mlsMFCDll::mlsDesfire::CreateCyclicRecordFile( BYTE bFileID, BYTE bCommMode, WORD wAccessRights, DWORD eRecordSize, DWORD eMaxNRecords )
{
	return m_poISOCardObject->CreateCyclicRecordFile( bFileID, bCommMode, wAccessRights, eRecordSize, eMaxNRecords );
}

WORD mlsMFCDll::mlsDesfire::DeleteFile( BYTE bFileID )
{
	return m_poISOCardObject->DeleteFile( bFileID );
}

WORD mlsMFCDll::mlsDesfire::ReadData( BYTE bFileID, DWORD eFromOffset, DWORD eNBytesToRead, BYTE *pbDataBuffer, DWORD *peNBytesRead /*= NULL */ )
{
	return m_poISOCardObject->ReadData( bFileID, eFromOffset, eNBytesToRead, pbDataBuffer, peNBytesRead /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::ReadData( BYTE bFileID, BYTE bCommMode, DWORD eFromOffset, DWORD eNBytesToRead, BYTE *pbDataBuffer, DWORD *peNBytesRead /*= NULL */ )
{
	return m_poISOCardObject->ReadData( bFileID, bCommMode, eFromOffset, eNBytesToRead, pbDataBuffer, peNBytesRead /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::WriteData( BYTE bFileID, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->WriteData( bFileID, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::WriteData( BYTE bFileID, BYTE bCommMode, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->WriteData( bFileID, bCommMode, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::GetValue( BYTE bFileID, long *plAmount )
{
	return m_poISOCardObject->GetValue( bFileID, plAmount );
}

WORD mlsMFCDll::mlsDesfire::GetValue( BYTE bFileID, BYTE bCommMode, long *plAmount )
{
	return m_poISOCardObject->GetValue( bFileID, bCommMode, plAmount );
}

WORD mlsMFCDll::mlsDesfire::Credit( BYTE bFileID, long lAmount )
{
	return m_poISOCardObject->Credit( bFileID, lAmount );
}

WORD mlsMFCDll::mlsDesfire::Credit( BYTE bFileID, BYTE bCommMode, long lAmount )
{
	return m_poISOCardObject->Credit( bFileID, bCommMode, lAmount );
}

WORD mlsMFCDll::mlsDesfire::Debit( BYTE bFileID, long lAmount )
{
	return m_poISOCardObject->Debit( bFileID, lAmount );
}

WORD mlsMFCDll::mlsDesfire::Debit( BYTE bFileID, BYTE bCommMode, long lAmount )
{
	return m_poISOCardObject->Debit( bFileID, bCommMode, lAmount) ;
}

WORD mlsMFCDll::mlsDesfire::LimitedCredit( BYTE bFileID, long lAmount )
{
	return m_poISOCardObject->LimitedCredit( bFileID, lAmount );
}

WORD mlsMFCDll::mlsDesfire::LimitedCredit( BYTE bFileID, BYTE bCommMode, long lAmount )
{
	return m_poISOCardObject->LimitedCredit( bFileID, bCommMode, lAmount );
}

WORD mlsMFCDll::mlsDesfire::ReadRecords( BYTE bFileID, DWORD eFromRecord, DWORD eNRecordsToRead, DWORD eRecordSize, BYTE *pbDataBuffer, DWORD *peNRecordsRead /*= NULL */ )
{
	return m_poISOCardObject->ReadRecords( bFileID, eFromRecord, eNRecordsToRead, eRecordSize, pbDataBuffer, peNRecordsRead /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::ReadRecords( BYTE bFileID, BYTE bCommMode, DWORD eFromRecord, DWORD eNRecordsToRead, DWORD eRecordSize, BYTE *pbDataBuffer, DWORD *peNRecordsRead /*= NULL */ )
{
	return m_poISOCardObject->ReadRecords( bFileID, bCommMode, eFromRecord, eNRecordsToRead, eRecordSize, pbDataBuffer, peNRecordsRead /*= NULL */ );
}

WORD mlsMFCDll::mlsDesfire::WriteRecord( BYTE bFileID, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->WriteRecord( bFileID, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::WriteRecord( BYTE bFileID, BYTE bCommMode, DWORD eFromOffset, DWORD eNBytesToWrite, const BYTE *pbDataBuffer )
{
	return m_poISOCardObject->WriteRecord( bFileID, bCommMode, eFromOffset, eNBytesToWrite, pbDataBuffer );
}

WORD mlsMFCDll::mlsDesfire::ClearRecordFile( BYTE bFileID )
{
	return m_poISOCardObject->ClearRecordFile( bFileID );
}

WORD mlsMFCDll::mlsDesfire::CommitTransaction( void )
{
	return m_poISOCardObject->CommitTransaction();
}

WORD mlsMFCDll::mlsDesfire::AbortTransaction( void )
{
	return m_poISOCardObject->AbortTransaction();
}

// Changes the CardMasterKey to an AES key of 16 bytes with key version.

WORD mlsMFCDll::mlsDesfire::ChangeCardMasterKey( BYTE bKeyVersion, const BYTE *pbNewKey )
{
	return m_poISOCardObject->ChangeCardMasterKey(bKeyVersion, pbNewKey );
}

// Changes the CardMasterKey to a (3)Des, (3)IsoDes of 16 bytes or 3K3Des key of 24 bytes
// Note: CRM_3DES_DF4 and CRM_3DES_ISO use the same type of key (16 bytes)

WORD mlsMFCDll::mlsDesfire::ChangeCardMasterKey(CRYPTO_METHOD KeyType,const BYTE *pbNewKey )
{
	return m_poISOCardObject->ChangeCardMasterKey(KeyType, pbNewKey );
}

void mlsMFCDll::mlsDesfire::ConnectSAM( mlsSAM* poDESFireSAM /*= NULL*/ )
{
	return m_poISOCardObject->ConnectSAM(poDESFireSAM->GetNXPSAMObj());
}