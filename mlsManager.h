#if !defined(__MLSMANAGER_H__)
#define __MLSMANAGER_H__

#include "mlsTypes.h"
#include "mlsReader.h"
#include "afxmt.h"

namespace mlsMFCDll
{

	class  mlsManager
	{
	private:
		int m_nrReader;
		CArray<mlsReader> m_ReadersArr;
		BOOL m_blHKRunning;
		ULONG m_ulRdrDetectTimeOutMs;
		//CCriticalSection m_criticalsect;
		CEvent* m_pEvNotify;
		SCARDCONTEXT m_scReaderDetectContext;
	public:
		//default constructor
		mlsManager();
		//default destructor
		~mlsManager();

		mlsErrorCode_tp Initialise();
		
		INT32 ListAllReader(CStringArray& ListReaders);

		BOOL ReaderDetect();

		mlsErrorCode_tp CancelReaderDetect();

		int GetNrReader();

		mlsReader* GetReaderByName(IN CString strReaderName);

		mlsReader* GetReaderByIndex(IN int iIdx);

		void StartHouseKeeping();

		void StopHouseKeeping();

		BOOL IsHKRunning();

		void SetReaderDetectTimeOutMs(ULONG timeOut);

		void SetNotifyEvent(CEvent* pev);

		CEvent* GetNotifyEvent(void);
	};

	UINT ManagerHKThread(LPVOID pParam);

}//namespace
#endif /* __MLSMANAGER_H__ */
