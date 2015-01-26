#include "Log.h"
#include <fstream>
#include <windows.h>

Log::Log(void)
{
	m_logPath = "log.txt";
}

Log::~Log(void)
{
}

void Log::RecordLog(string loginfo)
{
	SYSTEMTIME  time;//ʱ��ṹ����������ṹ��ϵͳ�ģ�
	GetLocalTime( &time );//���û�ȡϵͳʱ��api.
	
	fLog = fopen("log.txt", "a+");
	fprintf(fLog, "[INFO]%d-%d-%d %d:%d:%d.%d :%s\n", time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, loginfo);
	fclose(fLog);
	
}

void Log::RecordLog(string loginfo, int loglevel)
{
	SYSTEMTIME  time;
	GetLocalTime( &time );
	string level = "";
	switch (loglevel)
	{
	case LOG_LEVEL::LEVEL_INFO:
		level = "[INFO]";
		break;
	case LOG_LEVEL::LEVEL_WARNING:
		level = "[WARNING]";
		break;
	case LOG_LEVEL::LEVEL_EXCEPTION:
		level = "[EXCEPTION]";
		break;
	case LOG_LEVEL::LEVEL_ERROR:
		level = "[ERROR]";
		break;
	default:
		level = "[INFO]";
		break;
	}

	fLog = fopen("log.txt", "a+");
	fprintf(fLog, "%s%d-%d-%d %d:%d:%d.%d :%s\n", time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, loginfo);
	fclose(fLog);
}