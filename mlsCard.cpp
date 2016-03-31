#include "stdafx.h"
#include "mlsCard.h"
#include "mlsException.h"

using namespace mlsMFCDll;

mlsCard::mlsCard()
{
	m_CardHandle = 0;
	m_CardType = CARD_TYPE_NRITEM;
	m_CardHandle = 0;
	m_scPciReq = SCARD_PCI_T1;
}

mlsCard::mlsCard( IN mlsATR atr,IN mlsCardType_tp CardType)
{
	m_ATR = atr;
	m_CardType = CardType;
	m_scPciReq = SCARD_PCI_T1;
	m_CardHandle = 0;
}

mlsCard::~mlsCard()
{
	m_CardHandle = 0;
}

mlsErrorCode_tp mlsCard::Transmit( IN mlsAPDUCmd& ApduCmd, OUT mlsAPDUResp& ApduRes )
{
	LONG SCard_Status;
	mlsErrorCode_tp retVal = SUCCESS;
	DWORD dwRecvLength = 260;
	CByteArray byteArrResp,byteArrApduCmd;

	try
	{
		byteArrResp.SetSize(256);
		byteArrApduCmd.SetSize(256);

		ApduCmd.GetAll(byteArrApduCmd);

		SCard_Status = SCardTransmit(
			m_CardHandle,
			m_scPciReq,
			byteArrApduCmd.GetData(),
			ApduCmd.GetLength(),
			NULL,
			byteArrResp.GetData(), 
			&dwRecvLength
			);

		byteArrResp.SetSize(dwRecvLength);
		ApduRes.SetAPDUResp(byteArrResp);

		if (SCard_Status != SCARD_S_SUCCESS)
		{
			retVal = TRANSMIT_ERROR;
			mlsException exc = mlsException(SCard_Status);
			throw exc;
		}

	}
	catch(mlsException& exc)
	{
		AfxMessageBox(exc.what(),MB_OK,0);
	}
	catch (...)
	{
		AfxMessageBox("ERROR",MB_OK,0);		
	}

	return retVal;
}

mlsCardType_tp mlsCard::GetCardType()
{
	return m_CardType;
}

void mlsCard::SetCardType( IN mlsCardType_tp CardType )
{
	m_CardType = CardType;
}

mlsATR mlsCard::GetATR()
{
	return m_ATR;
}

void mlsCard::SetATR(IN mlsATR ATR)
{
	m_ATR = ATR;
}

SCARDHANDLE mlsCard::GetCardHandle()
{
	return m_CardHandle;
}

void mlsCard::SetCardHandle( SCARDHANDLE hCard )
{
	m_CardHandle = hCard;
}

void mlsCard::SetPCIRequest(LPCSCARD_IO_REQUEST pcireq)
{
	m_scPciReq = pcireq;
}

LPCSCARD_IO_REQUEST mlsCard::GetPCIRequest()
{
	return m_scPciReq;
}