#ifndef __MLS_DATETIME_H__
#define __MLS_DATETIME_H__

#include "mlsTypes.h"

typedef struct mlsDateTime_st
{
	/** Year */
	UInt32		year;

	/** Month */
	UInt32		month;

	/** Day of month */
	UInt32		dayOfMonth;

	/** Day of week */
	UInt32		dayOfWeek;

	/** hour */
	UInt32		hour;

	/** minute */
	UInt32		minute;

	/** second */
	UInt32		second;

	/** mili-second */
	UInt32		miliSecond;
}mlsDateTime_t;

void mlsUnixTimeToGregorianTime(UInt32 unixTime, mlsDateTime_t *pGTime);
Int32 mlsUnixDate(Int32 year, Int32 month, Int32 day);
UInt32 mlsMaxDayOfMonth(UInt32 month, UInt32 year);
void mlsGetDateTime(mlsDateTime_t* dateTime);
UInt32 mlsCurrentUnixDate();
UInt32 mlsCurrentTimeOfDay();
UInt32 mlsCurrentUnixTime();
UInt16 mlsIBMCrc16(const void *buffer, UInt32 bufferLength, UInt16 initValue);

#endif