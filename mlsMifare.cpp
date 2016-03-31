#include "stdafx.h"
#include "mlsMifare.h"
#include "mlsTypes.h"

using namespace mlsMFCDll;


mlsMifare::mlsMifare()
{
	
}


mlsMifare::mlsMifare(mlsCard Card)
{
	SetCardType(Card.GetCardType());
	SetCardHandle(Card.GetCardHandle());
	SetPCIRequest(Card.GetPCIRequest());
	
}

mlsMifare::~mlsMifare()
{

}

mlsErrorCode_tp mlsMifare::APDURespondError(mlsAPDUResp& APDUResp)
{	
	BYTE SW1 = APDUResp.GetSW1();
	BYTE SW2 = APDUResp.GetSW2();
	if ((SW1 == 0x90)&&(SW2 == 00)) return SUCCESS;
	else if ((SW1 == 0x67)&&(SW2 == 00)) return CARD_RESPOND_LENGTH_OR_ADDRESS_ERROR;
	else if ((SW1 == 0x6A)&&(SW2 == 00)) return CARD_RESPOND_P1_P2_ERROR;
	else if ((SW1 == 0x65)&&(SW2 == 81)) return CARD_RESPOND_MEMORY_ERROR;
	else if ((SW1 == 0x63)&&(SW2 == 00)) return CARD_RESPOND_AUTHENTICATION_ERROR;
	else return CARD_INVALID_RESPOND;
}

mlsErrorCode_tp mlsMifare::MifareGetUID( OUT CByteArray& byteArrData,OUT int& lengthUID )
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;
		
	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xCA);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(0x00);//P2
	APDUSend.Add(0x00);//Le
	
	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	if (retVal == SUCCESS)
	{
		APDURes.GetData(byteArrData);
		lengthUID = APDURes.GetLength()-2;
	}
	return retVal;	  
}

mlsErrorCode_tp mlsMifare::MifareReadBinary(IN byte BlockNumber,IN byte ByteToRead,OUT CByteArray& byteArrData )
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xB0);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(ByteToRead);//Le

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	if (retVal == SUCCESS)
	{
		APDURes.GetData(byteArrData);
	}
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareUpdateBinary( IN byte BlockNumber,IN CByteArray& Buffer,IN byte BufferLength )
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xD6);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(BufferLength);//Le
	APDUSend.Append(Buffer);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareValueBlockCreate(IN byte BlockNumber, IN LONG32 Value)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;
	BYTE* valueByte = (BYTE*) &Value;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xD7);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(0x05);//Le
	APDUSend.Add(0x00);
	APDUSend.Add(valueByte[3]);
	APDUSend.Add(valueByte[2]);
	APDUSend.Add(valueByte[1]);
	APDUSend.Add(valueByte[0]);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareValueBlockRead(IN byte BlockNumber, LONG32& outValue)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend,byteArr;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xB1);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(0x04);//Le

		

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	if (retVal == SUCCESS)
	{
		byteArr.SetSize(4);
		APDURes.GetData(byteArr);
		BYTE* convertValue = (BYTE*)&outValue;
		
		memcpy(&convertValue[0],&byteArr.ElementAt(3),1);
		memcpy(&convertValue[1],&byteArr.ElementAt(2),1);
		memcpy(&convertValue[2],&byteArr.ElementAt(1),1);
		memcpy(&convertValue[3],&byteArr.ElementAt(0),1);
	}
	return retVal;
}
mlsErrorCode_tp mlsMifare::MifareValueBlockIncrement(IN byte BlockNumber, IN LONG32 Value)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;
	BYTE* valueByte = (BYTE*)&Value;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xD7);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(0x05);//Le
	APDUSend.Add(0x01);
	APDUSend.Add(valueByte[3]);
	APDUSend.Add(valueByte[2]);
	APDUSend.Add(valueByte[1]);
	APDUSend.Add(valueByte[0]);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareValueBlockDecrement(IN byte BlockNumber, IN LONG32 Value)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;
	BYTE* valueByte = (BYTE*)&Value;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xD7);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(BlockNumber);//P2
	APDUSend.Add(0x05);//Le
	APDUSend.Add(0x02);
	APDUSend.Add(valueByte[3]);
	APDUSend.Add(valueByte[2]);
	APDUSend.Add(valueByte[1]);
	APDUSend.Add(valueByte[0]);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareValueBlockOperation( IN mlsMifareBlockOp_tp op,IN byte BlockNumber, IN LONG32 Value )
{
	mlsErrorCode_tp retVal = SUCCESS;
	if (op == CREATE) retVal = this->MifareValueBlockCreate(BlockNumber,Value);
	else if (op == INCREMENT) retVal = this->MifareValueBlockIncrement(BlockNumber,Value);
	else if (op == DECREMENT) retVal = this->MifareValueBlockDecrement(BlockNumber,Value);
	else retVal = OPERATOR_ERROR;
	return retVal;
}


mlsErrorCode_tp mlsMifare::MifareValueBlockRestore( IN byte SrcBlockNumber,IN byte DesBlockNumber )
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;
	
	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0xD7);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(SrcBlockNumber);//P2
	APDUSend.Add(0x02);//Le
	APDUSend.Add(0x03);
	APDUSend.Add(DesBlockNumber);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);

	return retVal;
}
mlsErrorCode_tp mlsMifare::MifareLoadKey(IN byte KeyPosition,IN CByteArray& KeyVal)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0x82);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(KeyPosition);//P2
	APDUSend.Add(0x06);//Le
	APDUSend.Append(KeyVal);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	
	return retVal;
}

mlsErrorCode_tp mlsMifare::MifareAuthenticate(IN byte BlockNumber, IN byte KeyType ,IN byte KeyPosition)
{
	mlsErrorCode_tp retVal = SUCCESS;
	CByteArray APDUSend;

	APDUSend.Add(0xFF);//CLA
	APDUSend.Add(0x86);//INS
	APDUSend.Add(0x00);//P1
	APDUSend.Add(0x00);//P2
	APDUSend.Add(0x05);
	APDUSend.Add(0x00);
	APDUSend.Add(0x00);
	APDUSend.Add(BlockNumber);
	APDUSend.Add(KeyType);
	APDUSend.Add(KeyPosition);

	mlsAPDUCmd APDUCmd(APDUSend);
	mlsAPDUResp APDURes;
	retVal = this->Transmit(APDUCmd,APDURes);
	if (retVal == SUCCESS) retVal = APDURespondError(APDURes);
	
	return retVal;
}

