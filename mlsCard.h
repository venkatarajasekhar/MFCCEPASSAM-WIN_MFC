#if !defined(__MLSCARD_H__)
#define __MLSCARD_H__

#include "mlsTypes.h"
//#include "mlsReader.h"
#include "mlsUtil.h"
#include "mlsATR.h"
#include "mlsAPDUCmd.h"
#include "mlsAPDUResp.h"
namespace mlsMFCDll
{
	class  mlsCard
	{
	public:
		//default constructor
		mlsCard();
		mlsCard(IN mlsATR atr,IN mlsCardType_tp CardType);
		
		mlsCard& operator=(const mlsCard &s )  // assignment operator
		{ 
			this->m_ATR = s.m_ATR;
			this->m_CardHandle = s.m_CardHandle;
			this->m_CardType = s.m_CardType;
			return *this; 
		}
		//default destructor
		~mlsCard();

		mlsErrorCode_tp Transmit(IN mlsAPDUCmd& ApduCmd, OUT mlsAPDUResp& ApduRes);
		
		mlsCardType_tp GetCardType();

		void SetCardType(IN mlsCardType_tp CardType);
		
		mlsATR GetATR();
		void SetATR(IN mlsATR ATR);

		SCARDHANDLE GetCardHandle();
		void SetCardHandle(SCARDHANDLE hCard);

		void SetPCIRequest(LPCSCARD_IO_REQUEST pcireq);
		LPCSCARD_IO_REQUEST GetPCIRequest();

	protected:
		mlsATR m_ATR;
		mlsCardType_tp m_CardType;
		SCARDHANDLE m_CardHandle;
		LPCSCARD_IO_REQUEST m_scPciReq;
	};
}//namespace
#endif /* __MLSCARD_H__ */




