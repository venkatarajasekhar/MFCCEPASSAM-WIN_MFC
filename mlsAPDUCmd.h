#if !defined(__MLSAPDUCMD_H__)
#define __MLSAPDUCMD_H__

#include "mlsTypes.h"

namespace mlsMFCDll
{
	class  mlsAPDUCmd
	{
	public:
		//default constructor
		mlsAPDUCmd();
		mlsAPDUCmd(IN BYTE cla, IN BYTE ins, IN BYTE p1, IN BYTE p2, IN BYTE p3, IN CByteArray DataArr, IN BYTE le);
		mlsAPDUCmd(IN CByteArray& byteArrAPDU);
		
		~mlsAPDUCmd();
		

		BYTE GetCLA();
		BYTE GetINS();
		BYTE GetP1();
		BYTE GetP2();
		BYTE GetLc();
		BYTE GetLe();
		void GetData(OUT CByteArray& byteArrData);
		void GetAll(OUT CByteArray& byteArrAll);
		DWORD GetLength();
		void SetApdu(IN CByteArray& byteArrAPDU);


	private:
		BYTE m_Cla;
		BYTE m_Ins;
		BYTE m_P1;
		BYTE m_P2;
		BYTE m_P3;
		CByteArray m_byteArrData;
		BYTE m_Le;
		CByteArray m_byteArrAPDU;
	};
} // namespace
#endif /* __MLSAPDUCMD_H__ */