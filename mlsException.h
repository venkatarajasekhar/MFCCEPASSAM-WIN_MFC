#if !defined(__MLSEXCEPTION_H__)
#define __MLSEXCEPTION_H__

#include <iostream>
#include <winscard.h>
using namespace std;
namespace mlsMFCDll
{
	struct errCodeMap_st
	{ 
		LONG code; 
		char *msg; 
	};

	typedef struct errCodeMap_st errCodeMap_t;

	class  mlsException : public exception
	{
	protected:

		LONG m_cause;
		char  m_msg[100];
	public:
		mlsException(LONG cause) { m_cause = cause; }
		mlsException() {m_cause = 0xFFFFFFFF;}
		mlsException(char* msg);
		~mlsException(){}
		virtual const char* what() const throw();
	};
}//namespace
#endif /* __MLSAPDUCMD_H__ */