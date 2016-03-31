//11-06-2009
//mlsAPDUCmd.cpp
#include "stdafx.h"
#include "mlsAPDUCmd.h"

using namespace mlsMFCDll;

mlsAPDUCmd::mlsAPDUCmd()
{
	m_Cla = -1;
	m_Ins = -1;
	m_P1 = -1;
	m_P2 = -1;
	m_P3 = -1;
	m_byteArrData.RemoveAll();
	m_Le = -1;
	m_byteArrAPDU.RemoveAll();
}
mlsAPDUCmd::mlsAPDUCmd( IN BYTE cla, IN BYTE ins, IN BYTE p1, IN BYTE p2, IN BYTE p3, IN CByteArray DataArr, IN BYTE le )
{
	m_Cla = cla;
	m_Ins = ins;
	m_P1 = p1;
	m_P2 = p2;
	m_P3 = p3;
	m_byteArrData.Copy(DataArr);
	m_Le = le;

	if (m_Cla != -1) m_byteArrAPDU.Add(m_Cla);
	if (m_Ins != -1) m_byteArrAPDU.Add(m_Ins);
	if (m_P1 != -1) m_byteArrAPDU.Add(m_P1);
	if (m_P2 != -1) m_byteArrAPDU.Add(m_P2);
	if (m_P3 != -1) m_byteArrAPDU.Add(m_P3);
	m_byteArrAPDU.Append(m_byteArrData);
	if (m_Le != -1) m_byteArrAPDU.Add(m_Le);
}

mlsAPDUCmd::mlsAPDUCmd( IN CByteArray& byteArrAPDU)
{
	m_byteArrAPDU.Copy(byteArrAPDU);
}

mlsAPDUCmd::~mlsAPDUCmd()
{
	m_byteArrData.RemoveAll();
	m_byteArrAPDU.RemoveAll();
}

BYTE mlsAPDUCmd::GetCLA()
{
	return m_Cla;
}

BYTE mlsAPDUCmd::GetINS()
{
	return m_Ins;
}

BYTE mlsAPDUCmd::GetP1()
{
	return m_P1;
}

BYTE mlsAPDUCmd::GetP2()
{
	return m_P2;
}

BYTE mlsAPDUCmd::GetLc()
{
	return m_P3;
}

BYTE mlsAPDUCmd::GetLe()
{
	return m_Le;
}

void mlsAPDUCmd::GetData( OUT CByteArray& byteArrData )
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

void mlsAPDUCmd::GetAll( OUT CByteArray& byteArrAll )
{
	CByteArray caTemp;
	caTemp.Copy(m_byteArrAPDU);
	if (byteArrAll.GetSize()<caTemp.GetSize()){
		caTemp.SetSize(byteArrAll.GetSize());
	}
	for (int i=0;i<caTemp.GetSize();i++){
		byteArrAll.SetAt(i,caTemp.GetAt(i));
	}
}

DWORD mlsAPDUCmd::GetLength()
{
	return (int)m_byteArrAPDU.GetSize();
}