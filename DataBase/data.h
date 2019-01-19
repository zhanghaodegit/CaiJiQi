/*************************************************************************
	> File Name: data.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月15日 星期六 10时45分23秒
 ************************************************************************/

#define		PATH_FILE_MAX_SIZE 128
#define		SQL_EXEC_MAX_SIZE	1024
//int DataInit(char * path1,char *file1, sqlite3 *ppDb1,char *path2, char *file2,sqlite3 *ppDb2);
int DataInit(char * path1,char *file1, sqlite3 *ppDb1,char *initTables[][2],int32_t initCount);
