//#define MLSDEBUG_CEPAS_LOG
#if defined (MLSDEBUG_CEPAS_LOG)
#define MLSDEBUG_LOGFILE_PATH ".\\cepaslog.txt"

#define mlsDebugLogSrcInfo(fmt,...)	\
	{\
		FILE *hLogFile;\
		hLogFile = fopen(MLSDEBUG_LOGFILE_PATH,"a+"); \
		/*fprintf(hLogFile,"%s %d %s ",__FILE__, __LINE__, __FUNCTION__);*/ \
		fprintf(hLogFile,"%s %d:",__FUNCTION__, __LINE__ );\
		fprintf(hLogFile,fmt, __VA_ARGS__); \
		fclose(hLogFile);\
	}

#define mlsDebugLogDateTimeInfo(fmt,...)	\
	{\
	FILE *hLogFile;\
	hLogFile = fopen(MLSDEBUG_LOGFILE_PATH,"a+"); \
	fprintf(hLogFile,"%s %s ",__DATE__, __TIME__); \
	fprintf(hLogFile,fmt, __VA_ARGS__); \
	fclose(hLogFile);\
	}

#define mlsDebugLogFullInfo(fmt,...)	\
	{\
	FILE *hLogFile;\
	hLogFile = fopen(MLSDEBUG_LOGFILE_PATH,"a+"); \
	fprintf(hLogFile,"%s %s %s %d %s ",__DATE__, __TIME__,__FILE__, __LINE__, __FUNCTION__); \
	fprintf(hLogFile,fmt, __VA_ARGS__); \
	fclose(hLogFile);\
	}

#define mlsDebugLog(fmt,...)	\
	{\
	FILE *hLogFile;\
	hLogFile = fopen(MLSDEBUG_LOGFILE_PATH,"a+"); \
	fprintf(hLogFile,fmt, __VA_ARGS__); \
	fclose(hLogFile);\
	}


#else
void mlsDebugDummy(void);

#define mlsDebugLogSrcInfo(fmt,...) mlsDebugDummy()
#define mlsDebugLogDateTimeInfo(fmt,...) mlsDebugDummy()
#define mlsDebugLogFullInfo(fmt,...) mlsDebugDummy()
#define mlsDebugLog(fmt,...) mlsDebugDummy()
#endif