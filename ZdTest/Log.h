#pragma once

#include <string>
#include <windows.h>
using namespace std;

enum LOG_LEVEL
{
	LEVEL_INFO = 0,
	LEVEL_WARNING = 1,
	LEVEL_EXCEPTION = 2,
	LEVEL_ERROR = 4
}; 

class Log
{
public:
	Log(void)
	{
		m_logPath = "log.txt";
	}

	void SetLogPath(string logpath)
	{
		m_logPath = logpath;
	}

	void RecordLog(string loginfo);
	
	void RecordLog(string loginfo, int loglevel);

private:
	string m_logPath;
	FILE* fLog;
};
