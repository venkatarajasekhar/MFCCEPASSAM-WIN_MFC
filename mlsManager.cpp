#include "stdafx.h"
#include "mlsManager.h"
#include "mlsException.h"

using namespace mlsMFCDll;

mlsManager::mlsManager()
{
	try
	{
		m_nrReader=0;
		m_blHKRunning = FALSE;
		m_ulRdrDetectTimeOutMs = 100;//100ms
		m_pEvNotify = NULL;
		m_scReaderDetectContext = NULL;
		Initialise();
	}
	catch (CException* e)
	{
		char info[300];
		if(e->GetErrorMessage(info,300)==TRUE)  // Get Error and place string into info
		{	
			char disp[500];
			wsprintf(disp,"Error:%s",info);
			AfxMessageBox(disp);
		}
		e->Delete(); // Will delete exception object correctly
	}
	catch (...)
	{
		ASSERT(0);
	}
}

mlsManager::~mlsManager()
{

}

UINT mlsMFCDll::ManagerHKThread( LPVOID pParam )
{
	mlsManager* pManager = (mlsManager*)pParam;
		
	while(pManager->IsHKRunning())
	{
		if(pManager->ReaderDetect())
		{
			//notify to user
			CEvent* pEv=pManager->GetNotifyEvent();

			pManager->Initialise();

			if (pEv!=NULL)
			{				
				SetEvent(*pEv);
			}
		}
	}

	return 0;
}
void mlsManager::SetNotifyEvent(CEvent* pev)
{
	m_pEvNotify = pev;
}
CEvent* mlsManager::GetNotifyEvent(void)
{
	return m_pEvNotify;
}


INT32 mlsManager::ListAllReader(CStringArray& ListReaders)
{
	//CStringArray _caListReaders;
	for(INT32 loop=0; (loop<(m_nrReader))&&(loop<ListReaders.GetSize()); loop++)
	{
		//ListReaders->Add(m_ReadersArr.GetAt(loop).GetReaderName());
		ListReaders.SetAt(loop,m_ReadersArr.GetAt(loop).GetReaderName());
	}
	return m_nrReader;
}

mlsErrorCode_tp mlsManager::Initialise()
{
	LONG scStatus;
	INT32 nRrdCount;
	DWORD dwLength = 0;
	PCHAR pmszAllReadersName = NULL;
	LPTSTR pCurReaderName = NULL;
	SCARDCONTEXT scContext=NULL;
	mlsErrorCode_tp retValue = SUCCESS;

	try
	{
		m_ReadersArr.RemoveAll();

		if (scStatus = SCardEstablishContext(SCARD_ESTABLISTCONTEXT_SCOPE, 
			NULL, 
			NULL, 
			&scContext)!= SCARD_S_SUCCESS)
		{
			retValue = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}
		//get the length of list readers string
		scStatus = SCardListReaders(
			scContext, 
			NULL, 
			NULL, 
			&dwLength);
		if ((scStatus == S_FALSE)||(scStatus==SCARD_E_NO_READERS_AVAILABLE))
		{
			if (scContext) SCardReleaseContext(scContext);
			scContext = NULL;
			retValue = READER_NOT_AVAILABLE;
			return retValue;
		}
		if (scStatus != SCARD_S_SUCCESS)
		{
			if (scContext) SCardReleaseContext(scContext);
			scContext = NULL;
			retValue = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		pmszAllReadersName = (char*)malloc(dwLength);
		
		scStatus = SCardListReaders(
			scContext,
			NULL,
			pmszAllReadersName,
			&dwLength);
		switch (scStatus)
		{
		case SCARD_E_NO_READERS_AVAILABLE:	
			retValue = READER_NOT_AVAILABLE;
			break;
		case SCARD_S_SUCCESS:

			//m_criticalsect.Lock();
			
			pCurReaderName = pmszAllReadersName;
			nRrdCount = -1;
			while ('\0' != *pCurReaderName)
			{
				nRrdCount ++;//Count the number of Reader

				m_ReadersArr.Add(mlsReader(pCurReaderName,CARD_CONNECT_PROTOCOL_T1));
				/*move pointer to next reader name*/
				pCurReaderName = pCurReaderName + strlen(pCurReaderName) + 1;
			}
			m_nrReader = ++nRrdCount;
			break;
		default:
			mlsException exc = mlsException(scStatus);
			throw exc;
			break;
		}

		if (pmszAllReadersName) free(pmszAllReadersName);
		if (scContext) SCardReleaseContext(scContext);
	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
		retValue = OPERATION_ERROR;
	}

	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);
		retValue = UNSPECIFIED_ERROR;
	}

	return retValue;
}
int mlsManager::GetNrReader()
{
	return m_nrReader;
}

void mlsManager::StopHouseKeeping()
{	
	m_blHKRunning = FALSE;
}

mlsReader* mlsManager::GetReaderByIndex( IN int iIdx )
{
	if (iIdx>=m_ReadersArr.GetCount()) return NULL;
	else return &m_ReadersArr.GetAt(iIdx);
}

mlsReader* mlsManager::GetReaderByName( IN CString strReaderName )
{
	INT32 loop;
	

	try
	{
		for (loop=0;loop<m_nrReader;loop++)
		{
			if (m_ReadersArr.GetAt(loop).GetReaderName().MakeUpper() == strReaderName.MakeUpper())
			{
				break;
			}
		}

		/*if no match reader found*/
		if (loop == m_nrReader)
		{
			mlsException exc=mlsException("Reader Name not found");
			throw exc;
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
	if (loop == m_nrReader) return NULL;// reader can not found
	else return &m_ReadersArr.GetAt(loop);
}

void mlsManager::StartHouseKeeping()
{
	try
	{
		m_blHKRunning = TRUE;
		AfxBeginThread(mlsMFCDll::ManagerHKThread,this);
	}
	catch (CException* e)
	{
		char info[300];
		if(e->GetErrorMessage(info,300)==TRUE)  // Get Error and place string into info
		{	
			char disp[500];
			wsprintf(disp,"Error:%s",info);
			AfxMessageBox(disp);
		}
		e->Delete(); // Will delete exception object correctly
	}
	catch(...)
	{
		ASSERT(0);
	}
}

void mlsManager::SetReaderDetectTimeOutMs(IN ULONG timeOut)
{
	m_ulRdrDetectTimeOutMs = timeOut;
}


mlsErrorCode_tp mlsManager::CancelReaderDetect()
{
	mlsErrorCode_tp retValue = SUCCESS;

	try
	{
		LONG scStatus = 0;

		if (m_scReaderDetectContext == NULL) return SUCCESS;

		scStatus= SCardCancel(
			m_scReaderDetectContext);

		if (scStatus != SCARD_S_SUCCESS)
		{
			retValue = OPERATION_ERROR;
			mlsException exc = mlsException(scStatus);
			throw exc;
			
		}

		if (m_scReaderDetectContext) 
		{
			scStatus = SCardReleaseContext(m_scReaderDetectContext);
			m_scReaderDetectContext=NULL;

			mlsException exc = mlsException(scStatus);
			throw exc;				
		}

		m_scReaderDetectContext=NULL;

	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);

	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}
	return retValue;
}

BOOL mlsManager::ReaderDetect()
{
	BOOL blRetVal= FALSE;
	static SCARD_READERSTATE scReaderState={0};
	LONG scStatus;

	try
	{
		if ((scStatus = SCardEstablishContext(
			SCARD_ESTABLISTCONTEXT_SCOPE, 
			NULL, 
			NULL,
			&m_scReaderDetectContext)) != SCARD_S_SUCCESS )	
		{
			m_scReaderDetectContext=NULL;
			mlsException exc = mlsException(scStatus);
			throw exc;
		}

		scReaderState.szReader = TEXT("\\\\?PnP?\\Notification");

		scStatus= SCardGetStatusChange(	m_scReaderDetectContext,
			m_ulRdrDetectTimeOutMs,
			&scReaderState,
			1);	
		if (scStatus==SCARD_E_TIMEOUT)
		{
			blRetVal = FALSE;
		}
		else if ((scStatus != SCARD_S_SUCCESS))
		{
			if (m_scReaderDetectContext) 
			{
				scStatus = SCardReleaseContext(m_scReaderDetectContext);
				m_scReaderDetectContext=NULL;
				mlsException exc = mlsException(scStatus);
				throw exc;				
			}

			mlsException exc = mlsException(scStatus);
			throw exc;
		}
		else //SCARD_S_SUCCESS
		{
			if (0 != ( SCARD_STATE_CHANGED & scReaderState.dwEventState))
			{
				blRetVal = TRUE;
				Initialise();
			}
		}
		scReaderState.dwCurrentState=scReaderState.dwEventState;

		if (m_scReaderDetectContext) 
		{		
			scStatus = SCardReleaseContext(m_scReaderDetectContext);
			if (scStatus!=SCARD_S_SUCCESS)
			{
				mlsException exc = mlsException(scStatus);
				throw exc;				
			}
			m_scReaderDetectContext=NULL;
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


BOOL mlsManager::IsHKRunning()
{
	if (m_blHKRunning) return TRUE;
	else return FALSE;
}
