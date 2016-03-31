#if !defined(__MLSUTIL_H__)
#define __MLSUTIL_H__

#ifndef __MLSTYPES_H__
#include "mlsTypes.h"
#endif

namespace mlsMFCDll
{

	class  mlsUtil
	{
	public:
		//default constructor
		mlsUtil();
		~mlsUtil();

		mlsErrorCode_tp StringToByteArray(CString str,CByteArray& byteArr, unsigned int& length);
		CString ByteArrayToHexString(CByteArray& byteArr,unsigned int length);
		
	private:

	};
}//namespace
#endif /* __MLSUTIL_H__ */
