#ifndef __MLSTYPES_H__
#define __MLSTYPES_H__

#ifndef __WINSCARD_H__
#include <winscard.h>
#endif
namespace mlsMFCDll
{


	#define KEY_TYPE_A 0x60
	#define KEY_TYPE_B 0x61

	#define SCARD_ESTABLISTCONTEXT_SCOPE SCARD_SCOPE_SYSTEM  //SCARD_SCOPE_USER
	#define SCARD_DISCONNECT_DISPOSITION SCARD_EJECT_CARD //SCARD_EJECT_CARD
	#define SCARD_CONNECT_SHARE_MODE SCARD_SHARE_SHARED
	#define SCARD_CONNECT_PROTOCOL SCARD_PROTOCOL_T1
	#define SCARD_TRANSMIT_PCI SCARD_PCI_T1

	#define NUMBER_OF_READER 128

	typedef enum {
		SUCCESS,
		WRONG_LENGTH,
		MEMORY_OP_ERROR,
		TRANSMIT_ERROR,
		RESOURCE_MANAGER_ERROR,
		READER_ERR,
		READER_NOT_AVAILABLE,
		READER_NOT_OPENED,
		READER_OPENED,
		CARD_REMOVED,
		CARD_RESPOND_LENGTH_OR_ADDRESS_ERROR,
		CARD_RESPOND_P1_P2_ERROR,
		CARD_RESPOND_MEMORY_ERROR,
		CARD_RESPOND_AUTHENTICATION_ERROR,
		CARD_INVALID_RESPOND,
		OPERATOR_ERROR,
		UNSPECIFIED_ERROR,
		OPERATION_ERROR
	}mlsErrorCode_tp ;

	typedef enum  {
		CREATE=0,
		INCREMENT=1,
		DECREMENT
	}mlsMifareBlockOp_tp;

	typedef enum
	{
		CARD_CONNECT_MODE_SHARE_SHARED,
		CARD_CONNECT_MODE_SHARE_EXCLUSIVE,
		CARD_CONNECT_MODE_SHARE_DIRECT
	}mlsCardConnectMode_t;

	typedef enum 
	{
		CARD_CONNECT_PROTOCOL_T0 = 0x00000001,
		CARD_CONNECT_PROTOCOL_T1 = 0x00000002,
		CARD_CONNECT_PROTOCOL_ZERO=0x00000004
	}mlsCardConnectProtocol_t;

	typedef enum {
		CARD_DETECT_PRESENT=0,
		CARD_DETECT_ABSENT=1,
		CARD_DETECT_UNPOWER,
		CARD_DETECT_INUSE,
		CARD_DETECT_UNCHANGE,
		CARD_DETECT_INVALID,
		CARD_DETECT_ERROR
	}mlsCardDetectionStatus_tp;

	typedef enum
	{
		CARD_TYPE_ULTRALIGHT=0,
		CARD_TYPE_MIFARE1K=1,
		CARD_TYPE_MIFARE4K=2,
		CARD_TYPE_DESFIRE=3,
		CARD_TYPE_SAM=4,
		CARD_TYPE_EZCEPAS = 5,
		CARD_TYPE_UNKNOWN=6,
		CARD_TYPE_NRITEM=CARD_TYPE_UNKNOWN
	}mlsCardType_tp;

}//namespace

/** Character type */
typedef	char			Char;


/** 8 bits (1 byte) signed integer number */
typedef char			Int8;

/** 8 bits (1 byte) unsigned integer number */
typedef unsigned char	UInt8;

/** 8 bits (1 byte) signed integer number */
typedef short			Int16;

/** 8 bits (1 byte) unsigned integer number */
typedef unsigned short	UInt16;

/** 8 bits (1 byte) signed integer number */
typedef long			Int32;

/** 8 bits (1 byte) unsigned integer number */
typedef unsigned long	UInt32;

typedef long long		Int64;

typedef unsigned long long	UInt64;

#ifndef __cplusplus
typedef	UInt8	bool;
#define	true	1
#define	false	0
#endif

/** Boolean */
typedef bool			Bool;

/** True boolean value */
#define True			true

/** False boolean value */
#define False			false

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#define MLS_UPCASE(ch)	(((ch >= 'a') && (ch <= 'z')) ? ch - ('a' - 'A') : ch)

/**
* @brief Error code enumeration
*/
typedef Int32 mlsErrorCode_t;
/** Success - no error */
#define MLS_SUCCESS								0

/* Error code from mls library */
/** General (unspecified) error */
#define MLS_ERROR								-1

/** Not enough resoure (may be memory) to perform operation */
#define MLS_ERR_INSUFFICIENT_RESOURCE			-2

/** Invalid reference */
#define MLS_ERR_INVALID_REFERENCE				-3

/** Timeout */
#define MLS_ERR_TIMEOUT							-4

/** Memory operation */
#define MLS_ERR_MEMORY_OPERATION				-5

/** Error occur while transmitting APDU */
#define MLS_ERR_TRANSMIT						-6

/** Resource manager error */
#define MLS_ERR_RESOURCE_MANAGER				-7

/** Reader error */
#define MLS_ERR_READER							-8

/** Reader is invalid (may be removed) */
#define MLS_ERR_INVALID_READER					-9

/** Card was removed */
#define MLS_ERR_CARD_REMOVED					-10

/** Error occur while operating with WinScard library */
#define MLS_ERR_OPERATION						-11

/* Error code from apdu resp */
#define PART_OF_RETURNED_DATA_MAY_BE_CORRUPTED	-100
#define END_OF_DATA_REACHED_BEFORE_Le_BYTES		-101
#define NO_INFORMATION_IS_GIVEN					-102
#define MEMORY_FAILURE							-103
#define WRONG_LENGTH							-104
#define CLASS_BYTE_IS_NOT_CORRECT				-105
#define COMMAND_INCOMPATIBLE					-106
#define CARD_KEY_NOT_SUPPORTED_or_SECURITY_STATUS_NOT_SATISFIED -107
#define AUTHENTICATION_CAN_NOT_BE_DONE_or_VERIFY_METHOD_BLOCKED_or_READER_KEY_NOT_SUPPORTED -108
#define PLAIN_TRANSMISSION_NOT_SUPPORTED_or_REFERENCE_KEYorDATA_NOT_USEABLE -109
#define SECURED_TRANSMISSION_NOT_SUPPORTED		-110
#define VOLATILE_MEMORY_IS_NOT_AVAILABLE_or_KEY_TYPE_NOT_KNOWN_or_COMMAND_NOT_ALLOWED -111
#define NON_VOLATILE_MEMORY_IS_NOT_AVAILABLE	-112
#define KEY_NUMBER_NOT_VALID					-113
#define KEY_LENGTH_IS_NOT_CORRECT				-114
#define FUNCTION_NOT_SUPPORTED					-115
#define FILE_BLOCK_BYTE_NOT_FOUND				-116
#define REFERENCE_DATA_NOT_FOUND				-117
#define WRONG_PARAMETER_P1_P2					-118
#define PROCESS_ABORTED_BY_THE_PROTECTION_TIMER	-119
#define COUNT_ERROR								-120
#define CARD_INVALID_APDU_COMMAND				-121


#define MLS_ER_DF_OPERATION_OK					MLS_SUCCESS
#define MLS_ER_DF_NO_CHANGES					-201
#define MLS_ER_DF_OUT_OF_EEPROM_ERROR			-202
#define MLS_ER_DF_ILLEGAL_COMMAND_CODE			-203
#define MLS_ER_DF_INTEGRITY_ERROR				-204
#define MLS_ER_DF_NO_SUCH_KEY					-205
#define MLS_ER_DF_LENGTH_ERROR					-206
#define MLS_ER_DF_PERMISSION_DENIED				-207
#define MLS_ER_DF_PARAMETER_ERROR				-208
#define MLS_ER_DF_APPLICATION_NOT_FOUND			-209
#define MLS_ER_DF_APPL_INTEGRITY_ERROR			-210
#define MLS_ER_DF_AUTHENTICATION_CORRECT		-211
#define MLS_ER_DF_AUTHENTICATION_ERROR			-212
#define MLS_ER_DF_ADDITIONAL_FRAME				-213
#define MLS_ER_DF_BOUNDARY_ERROR				-214
#define MLS_ER_DF_CARD_INTEGRITY_ERROR			-215
#define MLS_ER_DF_COMMAND_ABORTED				-216
#define MLS_ER_DF_CARD_DISABLED_ERROR			-217
#define MLS_ER_DF_COUNT_ERROR					-218
#define MLS_ER_DF_DUPLICATE_ERROR				-219
#define MLS_ER_DF_FILE_NOT_FOUND				-220
#define MLS_ER_DF_FILE_INTEGRITY_ERROR			-221
#define MLS_ER_DF_EEPROM_ERROR					-222

#define MLS_ER_DF_NETWORK_ERROR					-300

#define MLS_ERR_APP_DEFINE						-1000

/** Use this definition to avoid unused variable (parameter) warning */
#define UNUSED_PARAM(param)	((void)(param))

/** Convert 16 bits big endian value to little endian and vice versa */
#define SWAP_16(x) ((((x) >> 8)&0xFF) | (((x) << 8)&0xFF00))

/** Convert 24 bits big endian value to little endian and vice versa */
#define SWAP_24(x) ((((x) >> 16)&0xFF) | ((x) & 0x0000FF00) |(((x) << 16) & 0x00FF0000))

/** Convert 32 bits big endian value to little endian and vice versa */
#define SWAP_32(x) ((((x) >> 24)&0xFF) | (((x) >> 8) & 0x0000FF00)\
	|(((x) << 8) & 0x00FF0000) | (((x) << 24)& 0xFF000000))

#define MLS_MAX(a,b)	(((a) > (b)) ? (a) : (b))

#define MLS_FIELD_OFFSET(s, f)	((UInt32)(UInt8*)(&(((s*)0)->f)))

#endif __MLSTYPES_H__