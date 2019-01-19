/*************************************************************************
  > File Name: main.c
  > Author: wuji
  > Mail: 158836015@qq.com 
  > Created Time: 2018年12月06日 星期四 09时33分19秒
 ************************************************************************/

#include<stdio.h>
#include <sys/vfs.h>
#include <stdint.h>
#include <sqlite3.h>
#include <semaphore.h>
#include <pthread.h>
#define		DEBUG_PRINTF		1
#define		DEBUG_ERROR			1
#include "ThreadSerial.h"
#include "ThreadData.h"
#include "ThreadLcd.h"
//#include "sqlInit.h"
#include "list_head.h"
#include "debug.h"
#include "device.h"
#include "timer.h"
#include "MulThread.h"
#include "ThreadMsg.h"
#include <string.h>
#include "log.h"
#include "PipeLine.h"
#include "GobleDefine.h"

extern gTimerList;

WorkSpace	gWorkSpace;
#if 0
sqlite3 *configDb;
sqlite3 *dataDb;
#endif
struct list_head ThreadList;
#if 0
time_t gLocalTime;
time_t gRunTime;
struct tm* gDateTime;
#endif
extern time_t gLocalTime;
#if 0
int LocalTimeInit(void)
{
	int32_t ret = 0;
	time_t utcTime;
//	ret = GetRtcTime(&gLocalTime);
	ret = GetUtcTime(&utcTime);
	if(ret != 0)
	{
		PERROR("GetUtc err");
		return ret;
	}
	gLocalTime  = utcTime;
	PRINTF("get Utc time :%lu",utcTime);
	PRINTF("Local time h:m:d:%s", ctime(&utcTime));
	
	/*
	ret = Time2Date(&gLocalTime,&gDateTime);
	if(ret != 0)
	{
		PERROR("Time2Date err");
		return ret;
	}
	*/
	return ret;

}
#endif
#if 0
int TimerProc(void)
{
	PRINTF("in TimerProc");
	gLocalTime ++;
	gRunTime ++;
	gDateTime = localtime(&gLocalTime);
	TimerListProc();

}
#endif
int SerialFun(void * i)
{

	PipeLine pipe;
	pipe.SerConfig.SerNum = 0;
	pipe.SerConfig.Baud = 2400;
	pipe.SerConfig.DataBit = 8;
	pipe.SerConfig.StopBit = 1;
	pipe.SerConfig.Parity = 1;
	pipe.Config.type = 0;
		QueueSendMsg(&ThreadList,"Serial",MSG_READ_PIPELINE,&pipe,3);
}
int main(int argc, char argv[])
{
	char cmd[128];
	char path[128];
	int32_t ret = 0;
	int32_t i, j;
	LIST_HEAD(listTest2);
	PRINTF("Start DT7...Build ai %s %s\n",__DATE__,__TIME__);
#if 1
	strcpy(gWorkSpace.Config.path, "/opt/materia/tmp/caijiqi/db");
	strcpy(gWorkSpace.Config.config_db,"config.db");
	strcpy(gWorkSpace.Config.data_db,"data.db");
	strcpy(gWorkSpace.Config.log_path,"/opt/materia/tmp/caijiqi/log");
#endif

	PRINTF("gWorkSpace.Config.path:%s\n",gWorkSpace.Config.path);
	PRINTF("gWorkSpace.Config.config_db:%s\n",gWorkSpace.Config.config_db);
	PRINTF("gWorkSpace.Config.data_db:%s\n",gWorkSpace.Config.data_db);
	PRINTF("gWorkSpace.Config.log_path:%s\n",gWorkSpace.Config.log_path);
	if(access(gWorkSpace.Config.path,0) == -1)
	{
		PRINTF("gWorkSpace.Config.path no exist");
		snprintf(cmd, sizeof(cmd),"mkdir -p -r %s",gWorkSpace.Config.path);
		system(cmd);
	}
	else
	{
		PRINTF("gWorkSpace.Config.path already exist");
	}
//	ret = InitTimer();
	ret = EpollTimerInit();
	if(ret < 0)
	{
		return 1;
	}
	ret = LocalTimeInit();
	ret = LogInit(gWorkSpace.Config.log_path,1);

	if(ret < 0)
	{
		return  1;
	}
	PRINTF("before dataInitArrays");
	/*
	   char *p[2][2]={"00","01","10","11"};
	   for(i = 0; i < 2; i++)
	   for(j = 0; j < 2; j++)
	   {
	   PRINTF("%s",p[i][j]);
	   }
	   */
#if 0
	char *configInitTables[5][2] = {{tableDeviceName,tableDevice}, {tablePipelineName,tablePipeline}, {tableResidentName,tableResident},{tableValveName,tableValve},{tableTaskName,tableTask}};
	char *dataInitTables[4][2] = {{tableEventName,tableEvent}, {tableDataPipelineName,tableDataPipeline}, {tableDataResidentName,tableDataResident},{tableExceptionName,tableException}};
	ret = DataInit(gWorkSpace.Config.path,gWorkSpace.Config.config_db,configDb,configInitTables,5);
	if(ret == -1)
	{
		PERROR("DataInit");
		return 1;
	}
	ret = DataInit(gWorkSpace.Config.path,gWorkSpace.Config.data_db,dataDb,dataInitTables,4);
	if(ret == -1)
	{
		PERROR("DataInit");
		return 1;
	}
#endif
	INIT_LIST_HEAD(&gWorkSpace.Master);
	INIT_LIST_HEAD(&gWorkSpace.Client);
	INIT_LIST_HEAD(&gWorkSpace.Valve);
	INIT_LIST_HEAD(&ThreadList);
	CreateThread("Serial",thread_serial,&ThreadList);
	CreateThread("Data",thread_data,&ThreadList);
	CreateThread("Lcd",thread_lcd,&ThreadList);
/*
	PipeLine pipe;
	pipe.SerConfig.SerNum = 0;
	pipe.SerConfig.Baud = 2400;
	pipe.SerConfig.DataBit = 8;
	pipe.SerConfig.StopBit = 1;
	pipe.SerConfig.Parity = 1;
	pipe.Config.type = 0;
*/
	/*
	PipeLine pipe1;
	pipe1.SerConfig.SerNum = 1;
	pipe1.SerConfig.Baud = 4800;
	pipe1.SerConfig.DataBit = 8;
	pipe1.SerConfig.StopBit = 1;
	pipe1.SerConfig.Parity = 1;
	pipe1.Config.type = 0;
	*/
	//PRINTF("pipe addr:%p",&pipe);
/*
	TimerPrivate SerTmPriv;
	SerTmPriv.cb = SerialFun;
	list_add(&SerTmPriv.timerList, &gTimerList);
*/
//	SetTimer(1,0,SerialFun,NULL);
	while(1)
	{
		EpollTimerWait();
		TimerProc();
//		WaitTimer();
	//	QueueSendMsg(&ThreadList,"Serial",MSG_READ_PIPELINE,&pipe,3);
	//	QueueSendMsg(&ThreadList,"Serial",MSG_READ_PIPELINE,&pipe1,3);
	//	QueueSendMsg(&ThreadList,"Data",2,4,6);
	//	QueueSendMsg(&ThreadList,"Lcd",3,6,8);
	}
}






