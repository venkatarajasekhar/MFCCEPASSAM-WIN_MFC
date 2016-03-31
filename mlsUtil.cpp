#include "stdafx.h";
#include "mlsUtil.h"

using namespace mlsMFCDll;


mlsErrorCode_tp mlsUtil::StringToByteArray(CString str,CByteArray& byteArr, unsigned int& length)
{
	length = (byteArr.GetSize()<str.GetLength())?
						byteArr.GetSize():str.GetLength();
	memcpy(byteArr.GetData(),(LPCTSTR)str,length);
	return SUCCESS;
}

CString mlsUtil::ByteArrayToHexString(CByteArray& byteArr,unsigned int length)
{	
	char* cHex = new char();
	CString str,str1 = "";//((LPCTSTR)byteArr.GetData(),(int) byteArr.GetSize());
	for(int i=0;i<length;i++){
		//str += (CString)(_itoa(byteArr.GetAt(i),cHex,16));
		str1.Format(_T(" %02X"),byteArr.GetAt(i));
		str +=str1;
	}
	delete cHex;	
	return str;
}
mlsUtil::mlsUtil()
{

}

mlsUtil::~mlsUtil()
{

}
