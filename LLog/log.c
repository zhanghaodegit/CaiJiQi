/*************************************************************************
	> File Name: log.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月09日 星期日 09时08分13秒
 ************************************************************************/

#include<stdio.h>
#include <stdint.h>
#include <fcntl.h>
#define	DEBUG_ERROR		1
#define	DEBUG_PRINTF	1
#include "debug.h"
#include "log.h"

#include <time.h>

int LogInit(char *logPath,int32_t logFlag)
{
	int32_t ret= 0;
	char paths[LOG_PATH_LENGTH];
	char logFilePaths[LOG_FILE_PATH_LENGTH];
	int32_t logFd;
	if(logPath == NULL)
	{
		PERROR("logPath is NULL");
		ret = -1;
		return ret;
	}
	if(strlen(logPath)>LOG_PATH_LENGTH  )
	{
		PERROR("logpath name is too long");
		ret = -1;
		return ret;
	}
	if(access(logPath,0) == -1)
	{
		PRINTF("LogPath no exist");
		ret = mkdir(logPath, S_IRWXU|S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
		if(ret == -1)
		{
			PERROR("mkdir logpath err");
			ret = -1;
			return ret;
		}
	}
	else
	{
		PRINTF("LogPath already exist");
	}
	snprintf(logFilePaths,sizeof(logFilePaths),"%s/%ld.txt",logPath,time(0));

	logFd = open(logFilePaths,O_CREAT|O_APPEND|O_RDWR,0777);
	if(logFd == -1)
	{
		PERROR("open logfilepath err");
		ret = -1;
		return ret;
	
	}


	return ret;

}

