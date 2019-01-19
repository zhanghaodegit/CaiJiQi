/*************************************************************************
	> File Name: data.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月15日 星期六 09时40分10秒
 ************************************************************************/

#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "data.h"
#define	DEBUG_PRINTF	1
#define	DEBUG_ERROR	1
#include "debug.h"
int doCount(void *pvoid, int argc, char **argv,char **azColName)
{
	if(argv[0] != NULL)
	{
		PRINTF("count :%d", atoi(argv[0]));
		*((int *)pvoid) = atoi(argv[0]);
	}
	else{
		*((int *)pvoid) = 0;	
	}
	return 0;
}
int tableExist(sqlite3 *db,char *tableName)
{
	int32_t ret = 0;
	int count = 0;
	char sqlExec[SQL_EXEC_MAX_SIZE];
	snprintf(sqlExec,sizeof(sqlExec),"select count(*) from sqlite_master where name = '%s'",tableName);
	PRINTF("sqlexec:%s",sqlExec);
	sqlite3_exec(db,sqlExec,doCount,&count,NULL);
	if(count > 0)
	{
		ret = 1;
	}
	else
	{
		PRINTF("count :%d", count);
		ret = 0;
	}
	return ret;
}
int DataInit(char * path1,char *file1, sqlite3  *ppDb1,char *initTables[][2] ,int32_t initCount)
{
	int32_t ret = 0;
	int32_t i = 0;
	char pathFile[PATH_FILE_MAX_SIZE];
	char sqlExec[SQL_EXEC_MAX_SIZE];
	char *errMsg = NULL;
	snprintf(pathFile, sizeof(pathFile),"%s/%s",path1, file1);
	PRINTF("pathFile :%s",pathFile);
	ret = sqlite3_open(pathFile,&ppDb1);
	PRINTF("after sqlite3_open");
	if(ret != SQLITE_OK)
	{
		PERROR("create sqlite_open %s\n ",pathFile);
		ret = -1;
		return ret;
	}
	for(i = 0; i < initCount; i++)
	{
		if((ret = tableExist(ppDb1,initTables[i][0])) == 0)
		{
			memset(sqlExec, '0',SQL_EXEC_MAX_SIZE);
			snprintf(sqlExec,sizeof(sqlExec),"%s",initTables[i][1]);
			PRINTF("sqlexec:%s",sqlExec);
			sqlite3_exec(ppDb1, sqlExec,NULL, NULL, &errMsg);
			if(errMsg != NULL)
			{
				PERROR("initTables %s error",initTables[i][0]);
				sqlite3_free(errMsg);
				ret = -1;
				return ret;
			}
		}
		else{
			PRINTF("table %s already exist will continue\n",initTables[i][0]);
		}
	}
	return ret;
}
//int DataInit(char * path1,char *file1, char *create1,char **ppDb1,char *path2, char *file2, char*create2,char **ppDb2)
/*
int DataInit(char * path1,char *file1, sqlite3  *ppDb1,char *path2, char *file2,sqlite3 *ppDb2)
{
	int32_t ret = 0;
	char pathFile[PATH_FILE_MAX_SIZE];
	snprintf(pathFile, sizeof(pathFile),"%s/%s",path1, file1);
	PRINTF("pathFile :%s",pathFile);
	ret = sqlite3_open(pathFile,&ppDb1);
	PRINTF("after sqlite3_open");
	if(ret != SQLITE_OK)
	{
		PERROR("create sqlite_open %s\n ",pathFile);
		ret = -1;
		return ret;
	}
	memset(pathFile,'\0',sizeof(pathFile));
	snprintf(pathFile,sizeof(pathFile),"%s/%s",path2,file2);
	PRINTF("pathFile :%s",pathFile);
	ret = sqlite3_open(pathFile, &ppDb2);
	if(ret != SQLITE_OK)
	{
		PERROR("create sqlite_open %s\n", pathFile);
		ret = -1;
		return ret;
	}

	return ret;
}
*/

