/*************************************************************************
  > File Name: ThreadData.c
  > Author: wuji
  > Mail: 158836015@qq.com 
  > Created Time: 2018年12月26日 星期三 11时11分58秒
 ************************************************************************/

#include<stdio.h>

#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include <sqlite3.h>
#define     DEBUG_PRINTF        1
#include "debug.h"
#include "PipeLine.h"
#include "list_head.h"
#include "MulThread.h"
#include "ThreadMsg.h"
#include "timer.h"
#include "GobleDefine.h"

#include "sqlInit.h"
#include "device.h"
sqlite3 *configDb;
sqlite3 *dataDb;
extern WorkSpace   gWorkSpace;
//extern sqlite3 *configDb;
extern struct list_head ThreadList;
void *DataFun(void *cb_data,time_t curTime)
{
	static PipeLine pipe;
	pipe.SerConfig.SerNum = 0;
	pipe.SerConfig.Baud = 2400;
	pipe.SerConfig.DataBit = 8;
	pipe.SerConfig.StopBit = 1;
	pipe.SerConfig.Parity = 1;
	pipe.Config.type = 0;
	strcpy(pipe.Config.Address,"55550012345678");
	//	PRINTF("pipe addr:%s",pipe.Config.Address);
		if(curTime % 10 == 0)
			QueueSendMsg(&ThreadList,"Serial",MSG_READ_PIPELINE,&pipe,3);
}
int SavePipeLine(PipeLine *ops)
{
	int ret = 0;
	char sql[256];
	char *errMsg = NULL;
	snprintf(sql,sizeof(sql),"replace into Pipelines (Number,Address,type,Caliber) values('%s','%s','%d','%d');",ops->Config.Number,ops->Config.Address,ops->Config.type,ops->Config.Caliber);
	//printf("sql:%s",sql);
	//why open configDb again???
	ret = sqlite3_open("/opt/materia/tmp/caijiqi/db/config.db",&configDb);
	sqlite3_exec(configDb,sql,NULL,NULL,&errMsg);
	printf("sql:%s",sql);
	if(errMsg)
	{
		PRINTF("......sql_%s\n",errMsg);
		sqlite3_free(errMsg);
	}
	//PRINTF("......sql_%s\n",errMsg);
}
void *thread_data(void *arg)
{
	pThreadData lpThis= (pThreadData)arg;
	ThreadMsg DataMsg;
	int ret = 0;
	ret = SetTimer(1,0,DataFun,NULL);
	PRINTF("thread data ret :%d", ret);
	char *configInitTables[5][2] = {{tableDeviceName,tableDevice}, {tablePipelineName,tablePipeline},{tableResidentName,tableResident},{tableValveName,tableValve},{tableTaskName,tableTask}};
	char *dataInitTables[4][2] = {{tableEventName,tableEvent}, {tableDataPipelineName,tableDataPipeline}, {tableDataResidentName,tableDataResident},{tableExceptionName,tableException}};
	ret = DataInit(gWorkSpace.Config.path,gWorkSpace.Config.config_db,configDb,configInitTables,5);
	if(ret == -1)
	{   
		PRINTF("DataInit");
		return 1;
	}       
	ret = DataInit(gWorkSpace.Config.path,gWorkSpace.Config.data_db,dataDb,dataInitTables,4);
	if(ret == -1)
	{   
		PRINTF("DataInit");
		return 1;
	};
	while(1)
	{
		printf("Data\n");
		QueueGetMsg(lpThis,&DataMsg);
		printf("DataMsg.msg:%d,w:%d,l:%d\n",DataMsg.Msg,DataMsg.WParam,DataMsg.LParam);
		PipeLine *ops = (PipeLine *)DataMsg.WParam;
		PRINTF("data add pipeline:%s : addr:%s,type:%d,Caliber:%d",ops->Config.Number,ops->Config.Address,ops->Config.type,ops->Config.Caliber);
		switch(DataMsg.Msg)
		{
			case MSG_UPDATE_PIPELINE_CONFIG:
				if(DataMsg.LParam == ADD)
				{
					ret = SavePipeLine(ops);
				}
		}
		sleep(1);
	}
}
