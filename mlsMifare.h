#if !defined(__MLSMIFARE_H__)
#define __MLSMIFARE_H__
//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#include "mlsCard.h"
namespace mlsMFCDll
{

	class AFX_EXT_CLASS mlsMifare:public mlsCard
	{
	public:
		//default constructor
		mlsMifare();

		mlsMifare(mlsCard Card);
		//default destructor
		~mlsMifare();

		mlsErrorCode_tp MifareGetUID(OUT CByteArray& byteArrData,OUT int& lengthUID);
		mlsErrorCode_tp MifareReadBinary(IN byte BlockNumber,IN byte ByteToRead,OUT CByteArray& byteArrData);
		mlsErrorCode_tp MifareUpdateBinary(IN byte BlockNumber,IN CByteArray& Buffer,IN byte BufferLength);
		mlsErrorCode_tp MifareValueBlockCreate(IN byte BlockNumber, IN LONG32 Value);
		mlsErrorCode_tp MifareValueBlockRead(IN byte BlockNumber, LONG32& outValue);
		mlsErrorCode_tp MifareValueBlockIncrement(IN byte BlockNumber, IN LONG32 Value);
		mlsErrorCode_tp MifareValueBlockDecrement(IN byte BlockNumber, IN LONG32 Value);
		mlsErrorCode_tp MifareValueBlockOperation(IN mlsMifareBlockOp_tp op,IN byte BlockNumber, IN LONG32 Value);
		mlsErrorCode_tp MifareValueBlockRestore(IN byte SrcBlockNumber,IN byte DesBlockNumber);
		mlsErrorCode_tp MifareLoadKey(IN byte KeyPosition,IN CByteArray& KeyVal);
		mlsErrorCode_tp MifareAuthenticate(IN byte BlockNumber, IN byte KeyType, IN byte KeyPosition);
	private:
		mlsErrorCode_tp APDURespondError(mlsAPDUResp& APDUResp);
		
	};
}//namespace
//#undef AFX_DATA
//#define AFX_DATA
#endif /* __MLSMIFARE_H__ */