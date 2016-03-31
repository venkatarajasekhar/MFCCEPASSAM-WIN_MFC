#if !defined(__MLSATR_H__)
#define __MLSATR_H__

#include "mlsTypes.h"
namespace mlsMFCDll
{
	class  mlsATR
	{
	public:
		//default constructor
		mlsATR();
		mlsATR(IN CByteArray& atr);
		mlsATR(IN CString strATR);
		mlsATR( const mlsATR &s )  // copy constructor 
		{ this->m_ATR.Copy(s.m_ATR); }
		mlsATR& operator=(const mlsATR &s )  // assignment operator
		{ this->m_ATR.Copy(s.m_ATR);return *this; }


		~mlsATR();

		void GetATR(CByteArray& byteATR);
		CString GetATRString();
		int GetATRLength();


	private:
		CByteArray m_ATR;
	};

} //namespace
#endif /* __MLSATR_H__ */
