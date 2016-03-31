//mlsAPDUResp.cpp
#include "stdafx.h"
#include "mlsAPDUResp.h"

using namespace mlsMFCDll;

mlsAPDUResp::mlsAPDUResp()
{
	m_byteArrData.RemoveAll();
	m_sw1 = 0;
	m_sw2 = 0;
}

mlsAPDUResp::~mlsAPDUResp()
{
	m_byteArrData.RemoveAll();
}
BYTE mlsAPDUResp::GetSW1()
{
	return m_sw1;
}

BYTE mlsAPDUResp::GetSW2()
{
	return m_sw2;
}

void mlsAPDUResp::GetSW( CByteArray& byteArrSW )
{	
	CByteArray caTemp;
	caTemp.Add(m_sw1);
	caTemp.Add(m_sw2);
	if (byteArrSW.GetSize()<caTemp.GetSize()){
		caTemp.SetSize(byteArrSW.GetSize());
	}
	for (int i=0;i<caTemp.GetSize();i++){
		byteArrSW.SetAt(i,caTemp.GetAt(i));
	}
}

void mlsAPDUResp::GetData( CByteArray& byteArrData )
{
	CByteArray caTemp;
	caTemp.Copy(m_byteArrData);
	if (byteArrData.GetSize()<caTemp.GetSize()){
		caTemp.SetSize(byteArrData.GetSize());
	}
	for (int i=0;i<caTemp.GetSize();i++){
		byteArrData.SetAt(i,caTemp.GetAt(i));
	}
}

void mlsAPDUResp::GetAll( CByteArray& byteArrAll )
{
	CByteArray caTemp;
	caTemp.Copy(m_byteArrData);
	caTemp.Add(m_sw1);
	caTemp.Add(m_sw2);
	if (byteArrAll.GetSize()<caTemp.GetSize()){
		caTemp.SetSize(byteArrAll.GetSize());
	}
	for (int i=0;i<caTemp.GetSize();i++){
		byteArrAll.SetAt(i,caTemp.GetAt(i));
	}	
}

void mlsAPDUResp::SetAPDUResp( CByteArray& ArrResp )
{
	int n = (int)ArrResp.GetSize();
	if (n<2) return;
	//Get data respond
	m_byteArrData.SetSize(n-2);
	memcpy(m_byteArrData.GetData(),ArrResp.GetData(),n-2);
	m_sw1 = ArrResp.GetAt(n-2);
	m_sw2 = ArrResp.GetAt(n-1);
}

DWORD mlsAPDUResp::GetLength()
{
	return m_byteArrData.GetSize()+2;
}

