// mlsReader.cpp
//

#include "stdafx.h"
#include "mlsReader.h"
#include "mlsException.h"
#include "mlsMifare.h"

using namespace mlsMFCDll;

CONST BYTE CardTypeATR[CARD_TYPE_NRITEM+1][32]=
{
	{0x3B, 0x8F, 0x80, 0x01, 0x80, 0x4F, 0x0C, 0xA0, 0x00, 0x00, 0x03, 0x06, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x68},//ultralight
	{0x3B, 0x8F, 0x80, 0x01, 0x80, 0x4F, 0x0C, 0xA0, 0x00, 0x00, 0x03, 0x06, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x6A},//1k
	{0x3B, 0x8F, 0x80, 0x01, 0x80, 0x4F, 0x0C, 0xA0, 0x00, 0x00, 0x03, 0x06, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x69},//4k
	{0x3B, 0x81, 0x80, 0x01, 0x80, 0x80},//desfire
	{0x3b, 0xde, 0x18, 0xff, 0x81, 0xf1, 0xfe, 0x43, 0x00, 0x3f, 0x07, 0x83, 0x44, 0x45, 0x53, 0x46, 0x69, 0x72, 0x65, 0x38, 0x20, 0x53, 0x41, 0x4d, 0x2d, 0x58, 0x17},//SAM
	{0x3B, 0x88, 0x80, 0x01, 0x1C, 0x2D, 0x94, 0x11, 0xF7, 0x71, 0x85, 0x00, 0xBE},
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

UINT mlsMFCDll::ReaderHKThread(LPVOID pParam)
{
	mlsReader* pReader = (mlsReader*)pParam;

	mlsCardDetectionStatus_tp newCardStatus;

	while (pReader->IsHKRunning())
	{
		newCardStatus = pReader->CardDetect();

		if (newCardStatus == CARD_DETECT_ERROR)
		{
			break; //exit while loop
		}

		if ((newCardStatus == CARD_DETECT_ABSENT) ||
			 (newCardStatus == CARD_DETECT_PRESENT) )
		{
			CEvent * pEv = pReader->GetNotifyEvent();

			if (pEv != NULL)
			{
				SetEvent(*pEv);
			}
		}
	}

	return 0;
}
mlsReader::mlsReader()
{
	m_ulCardDetectTimeoutMs = 500; //500ms
	m_blHKRunning = FALSE;
	m_blOpened=FALSE;
	m_blUsed=FALSE;
	m_blDetected = TRUE;
	m_blReaderPowered = FALSE;
	m_ulProtocol = CARD_CONNECT_PROTOCOL_T1;//protocol T=1
	m_blCardInserted = FALSE;
	m_pEvNotify = NULL;
	m_hscContext = NULL;
	m_hscCardDetectContext = NULL;
	//m_pDesfireCard = NULL;
	//m_pSAMCard = NULL;
	//m_pMifareCard = NULL;
	m_Card.SetCardType(CARD_TYPE_NRITEM);
}

mlsReader::mlsReader(
	CString rdrname,  
	ULONG ulProtocol)
{
	m_ulCardDetectTimeoutMs = 500; //500ms
	m_blHKRunning = FALSE;
	m_blOpened=FALSE;
	m_blUsed=FALSE;
	m_blDetected = TRUE;
	m_blReaderPowered = FALSE;
	m_ulProtocol = ulProtocol;//protocol T=0
	m_blCardInserted = FALSE;
	m_pEvNotify = NULL;
	m_csReaderName = CString(rdrname);
	m_scReaderState.dwCurrentState = SCARD_STATE_UNAWARE;
	m_scReaderState.dwEventState=SCARD_STATE_UNAWARE;	
	memset(m_scReaderState.rgbAtr,0x00,sizeof(m_scReaderState.rgbAtr));
	m_scReaderState.pvUserData = NULL;
	m_scReaderState.szReader = m_csReaderName.GetBuffer();	
	m_scReaderState.cbAtr = 0;
	m_hscContext = NULL;
	m_hscCardDetectContext = NULL;
	//m_pDesfireCard = NULL;
	//m_pSAMCard = NULL;
	//m_pMifareCard = NULL;
	m_Card.SetCardType(CARD_TYPE_NRITEM);
}

mlsReader::mlsReader(
	CONST CHAR* rdrname,  
	ULONG ulProtocol)
{
	m_ulCardDetectTimeoutMs = 500; //500ms
	m_blHKRunning = FALSE;
	m_blOpened=FALSE;
	m_blUsed=FALSE;
	m_blDetected = TRUE;
	m_blReaderPowered = FALSE;
	m_ulProtocol = ulProtocol;//protocol T=0
	m_blCardInserted = FALSE;
	m_pEvNotify = NULL;
	m_csReaderName.SetString(rdrname);
	m_scReaderState.dwCurrentState = SCARD_STATE_UNAWARE;
	m_scReaderState.dwEventState=SCARD_STATE_UNAWARE;	
	memset(m_scReaderState.rgbAtr,0x00,sizeof(m_scReaderState.rgbAtr));
	m_scReaderState.pvUserData = NULL;
	m_scReaderState.szReader = m_csReaderName.GetBuffer();	
	m_scReaderState.cbAtr = 0;
	m_hscContext = NULL;
	m_hscCardDetectContext = NULL;
	m_Card.SetCardType(CARD_TYPE_NRITEM);
	//m_pDesfireCard = NULL;
	//m_pSAMCard = NULL;
	//m_pMifareCard = NULL;
}

mlsReader::~mlsReader()
{
	//if (m_pMifareCard != NULL) delete m_pMifareCard;
	//if (m_pDesfireCard != NULL) delete m_pDesfireCard;
	//if (m_pSAMCard != NULL) delete m_pSAMCard;
}

mlsReader& mlsReader::operator=(const mlsReader &rhs)
{
	if (this!= &rhs)
	{
		this->m_ulCardDetectTimeoutMs = rhs.m_ulCardDetectTimeoutMs;
		this->m_blHKRunning = rhs.m_blHKRunning;
		this->m_blOpened=rhs.m_blOpened;
		this->m_blUsed=rhs.m_blUsed;
		this->m_blDetected = rhs.m_blDetected;
		this->m_blReaderPowered = rhs.m_blReaderPowered;
		this->m_ulProtocol = rhs.m_ulProtocol;
		this->m_blCardInserted = rhs.m_blCardInserted;
		this->m_pEvNotify = rhs.m_pEvNotify;
		this->m_csReaderName= rhs.m_csReaderName;
		this->m_scReaderState.dwCurrentState = rhs.m_scReaderState.dwCurrentState;
		this->m_scReaderState.dwEventState=rhs.m_scReaderState.dwEventState;	
		memcpy(this->m_scReaderState.rgbAtr,rhs.m_scReaderState.rgbAtr,sizeof(m_scReaderState.rgbAtr));
		this->m_scReaderState.pvUserData = rhs.m_scReaderState.pvUserData;
		this->m_scReaderState.szReader = m_csReaderName.GetBuffer();	
		this->m_scReaderState.cbAtr = rhs.m_scReaderState.cbAtr;
		this->m_hscContext = rhs.m_hscContext;
		this->m_hscCardDetectContext = rhs.m_hscCardDetectContext;	
		//m_pDesfireCard = rhs.m_pDesfireCard;
		//m_pMifareCard = rhs.m_pMifareCard;
		//m_pSAMCard = rhs.m_pSAMCard;
	}	
	return *this;
}

mlsErrorCode_tp mlsReader::Open()
{
	LONG scStatus = 0;	
	mlsErrorCode_tp retVal=SUCCESS;
	try
	{
		if (m_blOpened)
		{ 
			return READER_OPENED;
		}


		scStatus = SCardEstablishContext(SCARD_ESTABLISTCONTEXT_SCOPE,
			NULL,
			NULL,
			&m_hscContext);
		
		if (scStatus != SCARD_S_SUCCESS)
		{
			retVal = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}


		scStatus = SCardEstablishContext(SCARD_ESTABLISTCONTEXT_SCOPE,
			NULL,
			NULL,
			&m_hscCardDetectContext);

		if (scStatus != SCARD_S_SUCCESS)
		{
			retVal = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		m_blOpened = TRUE;
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}
	return retVal;
}

mlsErrorCode_tp mlsReader::Close()
{
	LONG scStatus = 0;	
	mlsErrorCode_tp retVal=SUCCESS;
	try
	{
		if (!m_blOpened)
		{
			return READER_NOT_OPENED;
		}


		scStatus =SCardReleaseContext(
			m_hscContext);

		if (scStatus!= SCARD_S_SUCCESS)
		{
			retVal = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		if (m_hscCardDetectContext) 
			scStatus =SCardReleaseContext(m_hscCardDetectContext);

		if (scStatus!= SCARD_S_SUCCESS)
		{
			retVal = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}
		m_hscCardDetectContext = NULL;

		m_blOpened = FALSE;
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}
	return retVal;
}


BOOL mlsReader::connect(
	mlsCardConnectMode_t connectMode,
	ULONG& connectProtocol)
{
	LONG scStatus;
	BOOL blRetVal = FALSE;
	DWORD dwWinscardConnectMode;
	DWORD dwWinscardPreferProtocol;
	DWORD dwActiveProtocol= 0;
	SCARDHANDLE hCard;

	if (!m_blOpened) 
	{
		return blRetVal;
	}

	m_blUsed = TRUE;
	
	switch (connectMode)
	{
	case CARD_CONNECT_MODE_SHARE_SHARED:
		dwWinscardConnectMode = SCARD_SHARE_SHARED;
		break;
	case CARD_CONNECT_MODE_SHARE_EXCLUSIVE:
		dwWinscardConnectMode = SCARD_SHARE_EXCLUSIVE;
		break;
	case CARD_CONNECT_MODE_SHARE_DIRECT:
		dwWinscardConnectMode = SCARD_SHARE_DIRECT;
		break;
	default:
		dwWinscardConnectMode = SCARD_SHARE_SHARED;
		break;
	}
	dwWinscardPreferProtocol = 0;
	if (connectProtocol & CARD_CONNECT_PROTOCOL_ZERO)
	{
		dwWinscardPreferProtocol =0;
	}
	if (connectProtocol  & CARD_CONNECT_PROTOCOL_T0)
	{
		dwWinscardPreferProtocol = SCARD_PROTOCOL_T0;
	}
	if (connectProtocol  & CARD_CONNECT_PROTOCOL_T1)
	{
		dwWinscardPreferProtocol |= SCARD_PROTOCOL_T1;
	}

	try
	{
/*

		if (m_hscContext==NULL)
		{
			scStatus = SCardEstablishContext(SCARD_ESTABLISTCONTEXT_SCOPE,
				NULL,
				NULL,
				&m_hscContext);

			if (scStatus != SCARD_S_SUCCESS)
			{
				mlsException exc = mlsException(scStatus);
				throw exc;
			}
		}

*/
		scStatus = SCardConnect(
			m_hscContext, 
			(LPCSTR)m_csReaderName,
			dwWinscardConnectMode,
			dwWinscardPreferProtocol,
			(LPSCARDHANDLE)&hCard,
			&dwActiveProtocol );

		if ( SCARD_S_SUCCESS != scStatus)
		{
			if(SCARD_W_REMOVED_CARD == scStatus)
			{	
				m_Card.SetCardHandle(NULL);
				m_blCardInserted = FALSE;
				return blRetVal;
			}
			else
			{
				mlsException exc = mlsException(scStatus);
				throw exc;
			}
		}

		m_Card.SetCardHandle(hCard);

		blRetVal = cardstatus(connectProtocol);

		if (blRetVal==TRUE)
		{
			m_Card.SetPCIRequest(m_scPciRequest);
			
			// Desfire Card
			if (m_Card.GetCardType() == CARD_TYPE_DESFIRE)
			{
				/*if (m_pDesfireCard != NULL) delete m_pDesfireCard;
				m_pDesfireCard = new mlsDesfire();
				m_pDesfireCard->DesfireInit(GetCardInfo());*/
			}
			//SAM card
			else if (m_Card.GetCardType() == CARD_TYPE_SAM)
			{
				/*if (m_pSAMCard != NULL) delete m_pSAMCard;
				m_pSAMCard = new mlsSAM();
				m_pSAMCard->SAMInit(GetCardInfo());*/
			}
			// Mifare card
			else if ((m_Card.GetCardType() == CARD_TYPE_ULTRALIGHT)||
				(m_Card.GetCardType() == CARD_TYPE_MIFARE1K)||
				(m_Card.GetCardType() == CARD_TYPE_MIFARE4K))
			{
				/*if (m_pMifareCard != NULL) delete m_pMifareCard;
				m_pMifareCard = new mlsMifare(m_Card);
				m_pMifareCard->SetCardHandle(m_Card.GetCardHandle());
				m_pMifareCard->SetPCIRequest(m_Card.GetPCIRequest());*/
			}
		}

	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}
	return blRetVal;
}

BOOL mlsReader::cardstatus(OUT ULONG& outProtocol)
{
	LONG scStatus;
	DWORD chLen = 200;
	BYTE Attr[32];
	ULONG ulATRLen = 32;
	DWORD dwState, dwProtocol;
	CByteArray baATR;
	BOOL blRetVal=FALSE;

	try
	{
		scStatus= SCardStatus(
			m_Card.GetCardHandle(),
			NULL,
			(LPDWORD)&chLen,
			(LPDWORD)&dwState,
			(LPDWORD)&dwProtocol,
			(LPBYTE)(Attr),
			(LPDWORD)&ulATRLen); 
		if (scStatus == ERROR_INVALID_HANDLE)
		{
			m_Card.SetCardHandle(NULL);
			m_blCardInserted = FALSE;
			return FALSE;
		}

		if ( SCARD_S_SUCCESS != scStatus)
		{
			m_Card.SetCardHandle(NULL);
			m_blCardInserted = FALSE;

			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		if (dwState==SCARD_ABSENT)
		{
			m_blCardInserted = FALSE;
			blRetVal = FALSE;
		}
		else
		{
			m_blCardInserted = TRUE;
			INT32 idx;
			for (idx=0; idx<CARD_TYPE_NRITEM; idx++ )
			{
				if ((idx == CARD_TYPE_DESFIRE)&&(ulATRLen == 6))
				{
					memcpy(Attr, &CardTypeATR[idx][0], ulATRLen);
					break;
				}
				if (memcmp(&CardTypeATR[idx][0],Attr,ulATRLen)==0)
				{
					break;
				}
			}

			m_Card.SetCardType(mlsCardType_tp(idx));

			baATR.SetSize(ulATRLen);

			for(UINT32 loop =0; loop<ulATRLen; loop++)
			{
				baATR.SetAt(loop, Attr[loop]);
			}

			mlsATR objAtr = mlsATR(baATR);

			m_Card.SetATR(objAtr);
			
			blRetVal = TRUE;
		}

		switch (dwProtocol)
		{
		case SCARD_PROTOCOL_T0:
			outProtocol = m_ulProtocol=CARD_CONNECT_PROTOCOL_T0;
			m_scPciRequest = SCARD_PCI_T0;
			break;	
		case SCARD_PROTOCOL_T1:
			outProtocol = m_ulProtocol=CARD_CONNECT_PROTOCOL_T1;
			m_scPciRequest = SCARD_PCI_T1;
			break;
		default:
			outProtocol = m_ulProtocol=CARD_CONNECT_PROTOCOL_ZERO;
			m_scPciRequest = SCARD_PCI_RAW;
			break;
		}
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}

	return blRetVal;
}

mlsErrorCode_tp mlsReader::Connect()
{
	BOOL blTmp;
	blTmp= connect(CARD_CONNECT_MODE_SHARE_SHARED,m_ulProtocol);
	
	if (blTmp == TRUE)
	{
		return SUCCESS;
	}
	else
	{
		return UNSPECIFIED_ERROR;
	}
}

mlsErrorCode_tp mlsReader::Connect(ULONG& ulProtocol)
{
	BOOL blTmp;

	blTmp = connect(CARD_CONNECT_MODE_SHARE_SHARED,ulProtocol);
	
	m_ulProtocol = ulProtocol;

	if (blTmp == TRUE)
	{
		return SUCCESS;
	}
	else
	{
		return UNSPECIFIED_ERROR;
	}
}

mlsErrorCode_tp mlsReader::Disconnect()
{
	mlsErrorCode_tp retVal = SUCCESS;
	LONG SCard_Status;
	try
	{
		if (!m_blOpened) 
		{
			return READER_NOT_OPENED;
		}

		if((m_blUsed == TRUE) && (m_Card.GetCardHandle() !=0))
		{
			if (SCard_Status = SCardDisconnect(
				m_Card.GetCardHandle(), 
				SCARD_DISCONNECT_DISPOSITION)!=SCARD_S_SUCCESS)
			{
				mlsException exc = mlsException(SCard_Status);
				throw exc;
			}
			m_Card.SetCardHandle(NULL);
			m_Card.SetCardType(CARD_TYPE_NRITEM);
			m_Card.SetPCIRequest(NULL);
			
			////delete Desfire card
			//if (m_pDesfireCard!=NULL) 
			//{
			//	delete m_pDesfireCard;
			//	m_pDesfireCard = NULL;
			//}
			//
			////delete SAM card
			//if (m_pSAMCard != NULL)
			//{
			//	delete m_pSAMCard;
			//	m_pSAMCard = NULL;
			//}
			//
			////delete Mifare Classic card
			//if (m_pMifareCard != NULL) 
			//{
			//	delete m_pMifareCard;
			//	m_pMifareCard = NULL;
			//}

			//m_ulProtocol = 0;
		}
		else
		{
			retVal = UNSPECIFIED_ERROR;
		}
/*

		if (m_hscContext)
		{
			SCard_Status =SCardReleaseContext(
				m_hscContext);

			if (SCard_Status!= SCARD_S_SUCCESS)
			{
				mlsException exc = mlsException(SCard_Status);
				throw exc;
			}
			m_hscContext = NULL;
		}

*/
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}
	return retVal;
}


void mlsReader::setCardDetectTimeOutMs(ULONG timeout)
{
	m_ulCardDetectTimeoutMs = timeout;
	return;
}

mlsErrorCode_tp mlsReader::CancelCardDetect()
{
	mlsErrorCode_tp retVal = SUCCESS;
	try
	{
		LONG scStatus = 0;
	
		if (m_hscCardDetectContext == NULL) return SUCCESS; 
		
		scStatus= SCardCancel(
			m_hscCardDetectContext);

		if (scStatus != SCARD_S_SUCCESS)
		{
			retVal = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		if (m_hscCardDetectContext) 
		{
			scStatus = SCardReleaseContext(m_hscCardDetectContext);
			m_hscCardDetectContext=NULL;

			if (scStatus != SCARD_S_SUCCESS)
			{
				retVal = OPERATION_ERROR;
				mlsException exc = mlsException(scStatus);
				throw exc;
			}			
		}

		m_hscCardDetectContext=NULL;
		
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);

	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}

	return retVal;
}

mlsCardDetectionStatus_tp mlsReader::CardDetect()
{
	LONG scStatus = 0;
	mlsCardDetectionStatus_tp retVal = CARD_DETECT_UNCHANGE;

	try
	{
		if (!m_hscCardDetectContext)
		{
			return retVal;
		}

		scStatus= SCardGetStatusChange(
			m_hscCardDetectContext,
			m_ulCardDetectTimeoutMs,
			&m_scReaderState,
			1);	

		if ((scStatus == SCARD_E_TIMEOUT)||(scStatus == SCARD_E_CANCELLED))
		{
			return retVal;
		}

		if (scStatus != SCARD_S_SUCCESS)
		{
			retVal = CARD_DETECT_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		if ( 0 != ( SCARD_STATE_CHANGED & m_scReaderState.dwEventState) )
		{
			//bStop = TRUE;  
			if ( 0 != (SCARD_STATE_EMPTY & m_scReaderState.dwEventState))
			{
				m_blCardInserted = FALSE;
				retVal = CARD_DETECT_ABSENT;
			}
			if (SCARD_STATE_PRESENT == ((SCARD_STATE_UNPOWERED|SCARD_STATE_INUSE|SCARD_STATE_PRESENT) & m_scReaderState.dwEventState) )
			{
				retVal = CARD_DETECT_PRESENT;
				m_blCardInserted = TRUE;
			}

			if ( 0 != (SCARD_STATE_ATRMATCH & m_scReaderState.dwEventState))
			{
				//debug purpose
			}

			if ( 0 != (SCARD_STATE_EXCLUSIVE & m_scReaderState.dwEventState))
			{
				//debug purpose
			}
			if ( 0 != (SCARD_STATE_INUSE & m_scReaderState.dwEventState))
			{
				//debug purpose
			}

			if ( 0 != (SCARD_STATE_MUTE & m_scReaderState.dwEventState))
			{
				//debug purpose
			}
			if ( 0 != (SCARD_STATE_IGNORE & m_scReaderState.dwEventState))
			{
				//debug purpose
			}
			if ( 0 != (SCARD_STATE_UNKNOWN & m_scReaderState.dwEventState))
			{
				//debug purpose
			}

			if ( 0 != (SCARD_STATE_UNAVAILABLE & m_scReaderState.dwEventState))
			{
				//debug purpose
			}
		}

		m_scReaderState.dwCurrentState = m_scReaderState.dwEventState;

	}
	catch(mlsException& exc)
	{
		//AfxMessageBox(exc.what(),MB_OK,0);
		retVal = CARD_DETECT_ERROR;
	}
	catch (...)
	{
		//AfxMessageBox("ERROR",MB_OK,0);		
		retVal = CARD_DETECT_ERROR;
	}

	return retVal;
}

mlsErrorCode_tp mlsReader::StartReaderHouseKeeping()
{	
    m_blHKRunning = TRUE;
	AfxBeginThread(mlsMFCDll::ReaderHKThread,this);
	return SUCCESS;
}

mlsErrorCode_tp mlsReader::StopReaderHouseKeeping()
{
	m_blHKRunning = FALSE;

	WaitForSingleObject(m_pEvNotify,INFINITE);

	if (CancelCardDetect()!=SUCCESS) return OPERATION_ERROR;	

	return SUCCESS;
}

BOOL mlsReader::IsHKRunning()
{
	return m_blHKRunning;
}

mlsCard mlsReader::GetCardInfo()
{	
	return m_Card;
}

BOOL mlsReader::IsCardAvailable()
{
	return m_blCardInserted;
}

CEvent* mlsReader::GetNotifyEvent(void)
{
	return m_pEvNotify;
}

void mlsReader::SetNotifyEvent(CEvent* pev)
{
	m_pEvNotify = pev;
}

CString mlsReader::GetReaderName(void)
{
	return m_csReaderName;
}

void* mlsReader::GetCard()
{
	if ((m_Card.GetCardType() == CARD_TYPE_UNKNOWN)||
		(m_Card.GetCardType() == CARD_TYPE_NRITEM))
	{
		return NULL;
	}
	else if (m_Card.GetCardType() == CARD_TYPE_DESFIRE)
	{
		//return m_pDesfireCard;
	}
	else if (m_Card.GetCardType() == CARD_TYPE_SAM)
	{
		//return m_pSAMCard;
	}
	else if ((m_Card.GetCardType() == CARD_TYPE_ULTRALIGHT)||
		(m_Card.GetCardType() == CARD_TYPE_MIFARE1K)||
		(m_Card.GetCardType() == CARD_TYPE_MIFARE4K))
	{
		//return m_pMifareCard;
	}
	return NULL;
}