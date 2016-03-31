#include "stdafx.h"
#include "mlsATR.h"

using namespace mlsMFCDll;

//
mlsATR::mlsATR()
{
	m_ATR.RemoveAll();
}

mlsATR::mlsATR(IN CByteArray& ATR)
{
	m_ATR.RemoveAll();
	m_ATR.Copy(ATR);
}

mlsATR::mlsATR(IN CString strATR)
{
	m_ATR.RemoveAll();
	m_ATR.SetSize(strATR.GetLength());
	CopyMemory(m_ATR.GetData(),(LPCTSTR)strATR,strATR.GetLength());	
}

mlsATR::~mlsATR()
{
	if (m_ATR.GetCount()!= 0) m_ATR.RemoveAll();
}

void mlsATR::GetATR(CByteArray& byteATR)
{
	CByteArray caTemp;
	caTemp.Copy(m_ATR);
	if (byteATR.GetSize()<byteATR.GetSize()){
		caTemp.SetSize(byteATR.GetSize());
	}
	for (int i=0;i<caTemp.GetSize();i++){
		byteATR.SetAt(i,caTemp.GetAt(i));
	}
}

CString mlsATR::GetATRString()
{
	CString str((LPCTSTR)m_ATR.GetData(),(int)m_ATR.GetSize());
	return str;
}

int mlsATR::GetATRLength()
{
	return (int)m_ATR.GetSize();
}