//mlsAPDUResp.h
#if !defined(__MLSAPDURESP_H__)
#define __MLSAPDURESP_H__

#include "mlsTypes.h"
namespace mlsMFCDll
{

	class mlsAPDUResp
	{
	public:
		//default constructor
		mlsAPDUResp();
		~mlsAPDUResp();

		BYTE GetSW1();
		BYTE GetSW2();

		void SetAPDUResp(CByteArray& ArrResp);
		void GetSW(CByteArray& byteArrSW);
		void GetData(CByteArray& byteArrData);
		void GetAll(CByteArray& byteArrAll);
		CString GetAllString();
		DWORD GetLength();

	private:
		CByteArray m_byteArrData;
		BYTE m_sw1;
		BYTE m_sw2;
	};
}//name space
#endif /* __MLSAPDURESP_H__ */