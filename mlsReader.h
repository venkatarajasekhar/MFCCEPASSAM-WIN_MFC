// mlsReader.h
#if !defined(__MLSREADER_H__)
#define __MLSREADER_H__

#include "mlsCard.h"
#include "mlsTypes.h"
#include "afxmt.h"

namespace mlsMFCDll
{

	class  mlsReader
	{
	public:
		mlsReader();
		
		mlsReader(
			CString rdrname, 
			ULONG ulProtocol);

		mlsReader(
			CONST CHAR* rdrname,  
			ULONG ulProtocol);

		~mlsReader(); 
		
		mlsErrorCode_tp Open();
		
		mlsErrorCode_tp Close();
		
		mlsErrorCode_tp Connect();
		
		mlsErrorCode_tp Connect(ULONG& ulProtocol);
		
		mlsErrorCode_tp Disconnect();
		
		mlsCardDetectionStatus_tp CardDetect();
		
		mlsErrorCode_tp CancelCardDetect();
		
		void setCardDetectTimeOutMs(ULONG timeout);
		
		mlsErrorCode_tp StartReaderHouseKeeping();
		
		mlsErrorCode_tp StopReaderHouseKeeping();
		
		BOOL IsHKRunning();
		
		BOOL IsCardAvailable();
		
		mlsCard GetCardInfo(void);
		
		CString GetReaderName(void);

		void SetNotifyEvent(CEvent* pev);
		
		CEvent* GetNotifyEvent(void);		

		mlsReader& operator=(const mlsReader &rhs);

		void* GetCard();

	private:
		BOOL mlsReader::connect(
			mlsCardConnectMode_t connectMode,
			ULONG& connectProtocol);
		
		BOOL cardstatus(OUT ULONG& outProtocol);
		
		CEvent* m_pEvNotify;
		
		ULONG m_ulCardDetectTimeoutMs;
		
		mlsCard m_Card;
		
		BOOL m_blHKRunning;
		
		CString m_csReaderName ;
		
		BOOL m_blOpened;
		
		BOOL m_blUsed;
		
		BOOL m_blDetected;
		
		BOOL m_blReaderPowered;
		
		ULONG m_ulProtocol;
		
		LPCSCARD_IO_REQUEST m_scPciRequest;
		
		BOOL m_blCardInserted;
		
		SCARDCONTEXT m_hscContext;
		
		SCARDCONTEXT m_hscCardDetectContext;
	
		SCARD_READERSTATE m_scReaderState;

		//mlsMifare* m_pMifareCard;
		
		//mlsDesfire* m_pDesfireCard;

		//mlsSAM* m_pSAMCard;
	};

	UINT ReaderHKThread(LPVOID pParam);
}//namespace
#undef AFX_DATA
#define AFX_DATA
#endif /* __MLSREADER_H__ */
