//#include <stdafx.h>
#include <windows.h>
#include "mlsDateTime.h"

void mlsUnixTimeToGregorianTime(UInt32 unixTime, mlsDateTime_t *pGTime)
{
	Int32 j, g, dg, c, dc, b, db, a, da, y, m, d;

	j = unixTime / 86400 + 2472632;
	g = j / 146097;
	dg = j % 146097;
	c = (dg / 36524 + 1) * 3 / 4;
	dc = dg - c * 36524;
	b = dc / 1461;
	db = dc % 1461;
	a = (db / 365 + 1) * 3 / 4;
	da = db - a * 365;
	y = g * 400 + c * 100 + b * 4 + a;
	m = (da * 5 + 308) / 153 - 2;
	d = da - (m + 4) * 153 / 5 + 122;
	pGTime->year = y - 4800 + (m + 2) / 12;
	pGTime->month = (m + 2) % 12 + 1;
	pGTime->dayOfMonth = d + 1;
	pGTime->dayOfWeek = (j + 2) % 7;

	j = unixTime % 86400;
	pGTime->hour = j / 3600;
	pGTime->minute = (j % 3600) / 60;
	pGTime->second = j % 60;
	pGTime->miliSecond = 0;
}

/* Calculate number of day from Jan 1 1970 */
Int32 mlsUnixDate(Int32 year, Int32 month, Int32 day)
{
	if (year < 1970)
	{
		return 0;
	}
	return (1461 * (year + 4800 + (month - 14)/12))/4
		+ (367 * (month - 2 - 12 * ((month - 14)/12)))/12
		- (3 * ((year + 4900 + (month - 14)/12)/100))/4
		+ day - 2472663;
}

UInt32 mlsMaxDayOfMonth(UInt32 month, UInt32 year)
{
	if ((month == 1) || (month == 3) || (month == 5) || (month == 7)
		|| (month == 8) || (month == 10) || (month == 12))
	{
		return 31;
	}
	if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
	{
		return 30;
	}
	if (month == 2)
	{
		if (((year % 400) == 0) || (((year % 4) == 0) && ((year % 100) != 0)))
		{
			return 29;
		}
		return 28;
	}
	return 0;
}

void mlsGetDateTime(mlsDateTime_t* dateTime)
{
	SYSTEMTIME systemTime;

	GetSystemTime(&systemTime);
	dateTime->year = systemTime.wYear;
	dateTime->month = systemTime.wMonth;
	dateTime->dayOfMonth = systemTime.wDay;
	dateTime->dayOfWeek = systemTime.wDayOfWeek;
	dateTime->hour = systemTime.wHour;
	dateTime->minute =systemTime.wMinute;
	dateTime->second = systemTime.wSecond;
	dateTime->miliSecond = systemTime.wMilliseconds;
}

UInt32 mlsCurrentUnixDate()
{
	mlsDateTime_t	dateTime;

	mlsGetDateTime(&dateTime);
	return mlsUnixDate(dateTime.year, dateTime.month, dateTime.dayOfMonth);
}

UInt32 mlsCurrentTimeOfDay()
{
	mlsDateTime_t	dateTime;

	mlsGetDateTime(&dateTime);
	return dateTime.hour * 3600 + dateTime.minute * 60 + dateTime.second;
}

UInt32 mlsCurrentUnixTime()
{
	mlsDateTime_t	dateTime;

	mlsGetDateTime(&dateTime);
	return mlsUnixDate(dateTime.year, dateTime.month, dateTime.dayOfMonth) * 86400
		+ dateTime.hour * 3600 + dateTime.minute * 60 + dateTime.second;
}

static UInt16 IBMCrcByte(UInt8 data, UInt16 crc)
{
	int j;
	UInt16 	BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
	for (j=0;j<8;j++)
	{
		DATA_0 = (data >> j) & 0x0001;
		BCC_0 = (crc & 0x0001);
		BCC_1 = (crc >> 1) & 0x0001;
		BCC_14 = (crc >> 14) & 0x0001;
		X16 = (BCC_0 ^ DATA_0) & 0x0001; 	// bit15 of BCC after shift
		X15  = (BCC_1 ^ X16) & 0x0001;		// bit0 of BCC after shift
		X2  = (BCC_14 ^ X16) & 0x0001;		// bit13 of BCC after shift
		crc = crc >> 1;
		crc = crc & 0x5FFE;
		crc = crc | (X15);
		crc = crc | (X2 << 13);
		crc = crc | (X16 << 15);
	}
	return crc;
}

/*
* Implements the standard CRC-16:
*   Width 16
*   Poly  0x8005 (x^16 + x^15 + x^2 + 1)
*/
UInt16 mlsIBMCrc16(const void *buffer, UInt32 bufferLength, UInt16 initValue)
{
	UInt8 *buff = (UInt8*)buffer;
	UInt16 crc16 = initValue;
	while (bufferLength--){
		crc16 = IBMCrcByte(*buff++,crc16);
	}
	return crc16;
}