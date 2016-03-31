#include "stdafx.h"
#include "mlsCard.h"
#include "mlsException.h"
#include "mlsSam.h"

//using namespace mlsMFCDll;
//using namespace phcs_RFID;
//using namespace phcs_PROTOCOL;
//using namespace phcs_MfDesFire8;

mlsMFCDll::mlsSAM::mlsSAM()
{
	m_poSAMCardObject = NULL;
	m_poCardExchange = NULL;
	m_cbaAPDUCommand.RemoveAll();
	m_cbaAPDURespond.RemoveAll();
	stPCSCInfo = new PCSC_INFO();	
}

mlsMFCDll::mlsSAM::~mlsSAM()
{
	if (stPCSCInfo != NULL) delete stPCSCInfo;
	if (m_poCardExchange != NULL) delete m_poCardExchange;
	if (m_poSAMCardObject != NULL) delete m_poSAMCardObject;
}

void mlsMFCDll::mlsSAM::SAMInit( mlsCard Card )
{
	m_CardHandle = Card.GetCardHandle();
	m_scPciReq = (LPCSCARD_IO_REQUEST)Card.GetPCIRequest();
	stPCSCInfo->hCard = m_CardHandle ;
	stPCSCInfo->pioPCI =(LPSCARD_IO_REQUEST) m_scPciReq;
	m_poCardExchange = new CardExchange(stPCSCInfo);
	m_poSAMCardObject = new phcsDESFire8SAMX_Card(m_poCardExchange);
}

// get the SAM version information
uint16_t mlsMFCDll::mlsSAM::GetVersion( phcsDESFire8SAMX_VERSION_INFO_t* pstVersionInfo )
{
	return m_poSAMCardObject->GetVersion(pstVersionInfo);
}

// get a key entry
uint16_t mlsMFCDll::mlsSAM::GetKeyEntry( uint8_t bKeyNo,
					 uint8_t *pbNumKeys,
					 phcsDESFire8SAMX_KEY_ENTRY_t *pstKeyEntry )
{
	return m_poSAMCardObject->GetKeyEntry(  bKeyNo,
											pbNumKeys,
											pstKeyEntry );
}

// disable cryptographic functionality
uint16_t mlsMFCDll::mlsSAM::DisableCrypto( uint16_t wProMas )
{
	return m_poSAMCardObject->DisableCrypto( wProMas );
}

// get Key Usage Counter entry
uint16_t mlsMFCDll::mlsSAM::GetKUCEntry( uint8_t bRefNoKUC,
					 phcsDESFire8SAMX_KUC_ENTRY_t *pstKUCEntry )
{
	return m_poSAMCardObject->GetKUCEntry(  bRefNoKUC,
											pstKUCEntry );
}

// perform a host authentication
uint16_t mlsMFCDll::mlsSAM::AuthenticateHost( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthData,
						  uint8_t   bKeyType,
						  uint8_t *pbSecretKey,
						  bool qKeepInitVector,
						  bool qCMAC)
{
	return m_poSAMCardObject->AuthenticateHost( pstAuthData,
												bKeyType,
												pbSecretKey,
												qKeepInitVector,
												qCMAC);
}

// change a key entry
uint16_t mlsMFCDll::mlsSAM::ChangeKeyEntry( uint8_t bKeyNo,
						uint8_t bProMas,
						phcsDESFire8SAMX_KEY_ENTRY_FULL_t *pstKeyEntry,
						uint8_t *pbUID,
						bool qPlain)
{
	return m_poSAMCardObject->ChangeKeyEntry(   bKeyNo,
												bProMas,
												pstKeyEntry,
												pbUID,
												qPlain);
}

// dump the (current) session key
uint16_t mlsMFCDll::mlsSAM::DumpSessionKey( bool qEncrypt,
						uint8_t bLogicalChannel,
						phcsDESFire8SAMX_Commands *poSAM,
						uint8_t *pbSessionKey,
						uint8_t *pbSessionKeyLength )
{
	return m_poSAMCardObject->DumpSessionKey(   qEncrypt,
												bLogicalChannel,
												poSAM,
												pbSessionKey,
												pbSessionKeyLength );
}

// change a key usage counter
uint16_t mlsMFCDll::mlsSAM::ChangeKUCEntry( uint8_t bRefNoKUC,
						uint8_t bProMas,
						phcsDESFire8SAMX_KUC_ENTRY_t *pstKUCEntry,
						uint8_t *pbUID,
						bool qPlain )
{
	return m_poSAMCardObject->ChangeKUCEntry(    bRefNoKUC,
												 bProMas,
												 pstKUCEntry,
												 pbUID,
												 qPlain );
}

// prepare for changing a key in the PICC
void mlsMFCDll::mlsSAM::PrepareForChangeKeyPICC(
							 phcsDESFire8SAMX_CHANGE_KEY_PICC_DATA_t *pstChangeKeyPicc )
{
	return m_poSAMCardObject->PrepareForChangeKeyPICC(pstChangeKeyPicc );
}

// create cryptogram, which has to be sent to the card
// in order to change any key of the PICC
uint16_t mlsMFCDll::mlsSAM::ChangeKeyPICC( uint8_t *pbCryptogram,
					   uint8_t *pbCryptLength,
					   bool qChangeCardMasterKey )
{
	return m_poSAMCardObject->ChangeKeyPICC(pbCryptogram,
											pbCryptLength,
											qChangeCardMasterKey );
}

// disable a key entry
uint16_t mlsMFCDll::mlsSAM::DisableKeyEntry( uint8_t bKeyNo )
{
	return m_poSAMCardObject->DisableKeyEntry(bKeyNo );
}

// create a key string or get a key
// in order to change any key of a Mifare card
uint16_t mlsMFCDll::mlsSAM::ChangeKeyMifare( bool qEncrypt,
						 phcsDESFire8SAMX_CHANGE_KEY_MIFARE_DATA_t *pstChangeKeyMifare,
						 uint8_t *pbKeyString,
						 uint8_t *pbKeyStringLength )
{
	return m_poSAMCardObject->ChangeKeyMifare(	qEncrypt,
												pstChangeKeyMifare,
												pbKeyString,
												pbKeyStringLength );
}

// select the application specified by AID
uint16_t mlsMFCDll::mlsSAM::SelectApplication( uint8_t *pbDF_AID )
{
	return m_poSAMCardObject->SelectApplication(pbDF_AID );
}

// load the init vector (used for KEEP IV)
uint16_t mlsMFCDll::mlsSAM::LoadInitVector( uint8_t *pbInitVector )
{
	return m_poSAMCardObject->LoadInitVector(pbInitVector );
}
// verifying a MAC
uint16_t mlsMFCDll::mlsSAM::VerifyMAC( bool qMoreData, 
				   uint8_t *pbPlainData, 
				   uint32_t eLen,
				   uint8_t  bBytesToCheck)
{
	return m_poSAMCardObject->VerifyMAC(qMoreData, 
										pbPlainData, 
										eLen,
										bBytesToCheck);
}

// generate a MAC (= Message Authentication Code)
uint16_t mlsMFCDll::mlsSAM::GenerateMAC( bool qMoreData,
					 uint8_t *pbPlainData,
					 uint32_t eLen,
					 phcsDESFire8SAMX_MAC_t *peMAC )
{
	return m_poSAMCardObject->GenerateMAC(qMoreData,
										  pbPlainData,
										  eLen,
										  peMAC );
}

// decipher data by SAM
uint16_t mlsMFCDll::mlsSAM::DecipherData( uint8_t *pbData, bool qUseLFI, uint32_t eLen )
{
	return m_poSAMCardObject->DecipherData( pbData,qUseLFI,eLen );
}

// encipher data by SAM
uint16_t mlsMFCDll::mlsSAM::EncipherData( bool qMoreData,
					  uint8_t *pbInputBuffer,
					  uint32_t eInputLen,
					  uint8_t *pbOutputBuffer,
					  uint32_t *peOutputLen,
					  uint8_t bPlainByteCount)
{
	return m_poSAMCardObject->EncipherData( qMoreData,
											pbInputBuffer,
											eInputLen,
											pbOutputBuffer,
											peOutputLen,
											bPlainByteCount);
}


// prepare for PICC authentication
void mlsMFCDll::mlsSAM::PrepareForAuthenticatePICC( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthStruct )
{
	return m_poSAMCardObject->PrepareForAuthenticatePICC(pstAuthStruct );
}

// authenticate both, the SAM and the PICC, part one
uint16_t mlsMFCDll::mlsSAM::AuthenticatePICCPartOne( uint8_t *pbEkNoRndB,
								 uint8_t bEkNoRndBLength,
								 uint8_t *pbDkNoRndARndB,
								 uint8_t *pbDkNoRndARndBLength )
{
	return m_poSAMCardObject->AuthenticatePICCPartOne(  pbEkNoRndB,
														bEkNoRndBLength,
														pbDkNoRndARndB,
														pbDkNoRndARndBLength );
}

// part two of combined authentication for the PICC and the SAM
uint16_t mlsMFCDll::mlsSAM::AuthenticatePICCPartTwo( uint8_t *pbEkNoRndA,
								 uint8_t bEkNoRndALength )
{
	return m_poSAMCardObject->AuthenticatePICCPartTwo(pbEkNoRndA,bEkNoRndALength );
}

// diversify a Mifare key
uint16_t mlsMFCDll::mlsSAM::DiversifyMifareKey( bool qEncrypt,
							phcsDESFire8SAMX_MIFARE_DATA_t *pstMifareStruct,
							uint8_t *pbDiversifiedKey )
{
	return m_poSAMCardObject->DiversifyMifareKey(qEncrypt,
												 pstMifareStruct,
												 pbDiversifiedKey );
}

// authenticate with a Mifare card, part one
uint16_t mlsMFCDll::mlsSAM::AuthenticateMifarePartOne( phcsDESFire8SAMX_MIFARE_DATA_t *pstMifareStruct,
								   uint8_t *pbNumberRB,
								   bool qUseDiversification,
								   uint8_t bDivBlockNumber,
								   uint8_t *pbTokenAB )
{
	return m_poSAMCardObject->AuthenticateMifarePartOne(pstMifareStruct,
														pbNumberRB,
														qUseDiversification,
														bDivBlockNumber,
														pbTokenAB );
}

// authenticate with a Mifare card, part two
uint16_t mlsMFCDll::mlsSAM::AuthenticateMifarePartTwo( uint8_t *pbTokenBA )
{
	return m_poSAMCardObject->AuthenticateMifarePartTwo( pbTokenBA );
}

// kill any existing authentication
uint16_t mlsMFCDll::mlsSAM::KillAuthentication( void )
{
	return m_poSAMCardObject->KillAuthentication();
}

// get a random number
uint16_t mlsMFCDll::mlsSAM::IsoGetChallenge( uint8_t *pbNoRnd,
						 uint16_t *pbNoRndLength )
{
	return m_poSAMCardObject->IsoGetChallenge(pbNoRnd,pbNoRndLength );
}

// perform an ISO host authentication 
uint16_t mlsMFCDll::mlsSAM::IsoAuthenticateHost( phcsDESFire8SAMX_AUTH_DATA_t *pstAuthData,
							 uint8_t bKeyType,
							 uint8_t *pbSecretKey,
							 bool qKeepInitVector ,
							 bool qCMAC )
{
	return m_poSAMCardObject->IsoAuthenticateHost(  pstAuthData,
													bKeyType,
													pbSecretKey,
													qKeepInitVector ,
													qCMAC );
}

// ISO authenticate both, the SAM and the PICC, part one 
uint16_t mlsMFCDll::mlsSAM::IsoAuthenticatePICCPartOne( uint8_t *pbRPicc1,
									uint8_t bRPicc1Length,
									uint8_t *pbEkNoRPcd1Picc1,
									uint8_t *pbEkNoRPcd1Picc1Length,
									uint8_t *pbRPcd2,
									uint8_t *pbRPcd2Length )
{
	return m_poSAMCardObject->IsoAuthenticatePICCPartOne(   pbRPicc1,
															bRPicc1Length,
															pbEkNoRPcd1Picc1,
															pbEkNoRPcd1Picc1Length,
															pbRPcd2,
															pbRPcd2Length );
}

// part two of combined ISO authentication for the PICC and the SAM
uint16_t mlsMFCDll::mlsSAM::IsoAuthenticatePICCPartTwo( uint8_t *pbEkNoRPicc2Pcd2,
									uint8_t bEkNoRPicc2Pcd2Length )
{
	return m_poSAMCardObject->IsoAuthenticatePICCPartTwo( pbEkNoRPicc2Pcd2,
														  bEkNoRPicc2Pcd2Length );
}

// set SAM into power down mode
uint16_t mlsMFCDll::mlsSAM::Sleep( void )
{
	return m_poSAMCardObject->Sleep();
}
