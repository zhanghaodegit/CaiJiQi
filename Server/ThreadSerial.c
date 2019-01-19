/*************************************************************************
  > File Name: ThreadSerial.c
  > Author: wuji
  > Mail: 158836015@qq.com 
  > Created Time: 2018年12月26日 星期三 09时21分20秒
 ************************************************************************/

#include<stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#define     DEBUG_PRINTF        1
#define     DEBUG_ERROR         1
#include "debug.h"
#include "list_head.h"
#include "MulThread.h"
#include "ThreadMsg.h"
#include "PipeLine.h"
#include "GobleDefine.h"
#include "ThreadSerial.h"
#include "wj_pipeline.h"
#include "test.h"
/*
   typedef struct{
   int serialHandle;
   int AlreadyOpen;
   }SerialData, *pSerialData;
   */
/*
   int wj_pipe_read(pSerialData pSerData, void *ops,int timeout)
   {
   PRINTF("wj_pipe_read");
   PipeLine *pipe = ops;
   }
   */
/*
   int tj_pipe_read(pSerialData pSerData, void *ops,int timeout)
   {
   PipeLine *pipe = ops;
   }
   */
typedef struct {
	char *name;
	int (*read)(pSerialData, void *, int);

}DeviceOps;
static DeviceOps	DevicesOne[]=
{
	[0] = {
		.name = "WuJi",
		.read = wj_pipe_read,
	}
};

static DeviceOps	DevicesTwo[]=
{
	[0] = {
		.name = "TaJi",
		.read = wj_pipe_read,
	}
};

typedef struct
{
	int SerNum;
	int Baud;
	int DataBit;
	int StopBit;
	int Parity;

}*pSerialConfig;

int init_serial(pSerialData pSerData, pSerialConfig pSerConf)
{
	int ret = 0;
	//test();
	pSerData->serialHandle = pSerConf->SerNum;
	PRINTF("serialHandle:%d\n",pSerData->serialHandle);
	PRINTF("serial Baud:%d,Even:%d,Stop:%d,Data:%d",pSerConf->Baud,pSerConf->Parity,pSerConf->StopBit,pSerConf->DataBit);
	return ret;

}
int send_serial(pSerialData pSerData,void *sendBuf,size_t len)
{
	int ret = 0;
	while(len--)
	{
		printf("send:%x",*(char *)sendBuf++);
	}
	return ret;
}
void *thread_serial(void *arg)
{
	pThreadData lpThis;
	lpThis = (pThreadData)arg;
	ThreadMsg SerialMsg;
	struct DeviceOps *devOps;
	SerialData SerData;
	PipeLine *pipe;
	DeviceOps *dev;
	int ret = 0;
	while(1)
	{
		sleep(1);
		printf("haha \n");
		QueueGetMsg(lpThis,&SerialMsg);
		printf("SrialMsg.msg:%d,w:%p,l:%d\n",SerialMsg.Msg,SerialMsg.WParam,SerialMsg.LParam);
		switch(SerialMsg.Msg)
		{
			case MSG_INIT:
				break;
			case MSG_READ_PIPELINE:
				PRINTF("msg_read_pipeline");
				pipe = (void *)SerialMsg.WParam;
				if(pipe == NULL)
				{	
					PERROR("pipe is NULL");
					break;
				}
				if(pipe->SerConfig.SerNum == 0 && pipe->Config.type >= 0 && pipe->Config.type <= sizeof(DevicesOne)/sizeof(DeviceOps))
				{
					PRINTF("DeviceOne");
					dev=&DevicesOne[pipe->Config.type];	
				}
				else if(pipe->SerConfig.SerNum == 1 && pipe->Config.type >= 0 && pipe->Config.type <= sizeof(DevicesTwo)/sizeof(DeviceOps))
				{

					dev=&DevicesTwo[pipe->Config.type];	
				}
				if(dev == NULL)
				{
					PERROR("devOps is null");
					ret = -1;
					break;
				}
				ret  = init_serial(&SerData, &pipe->SerConfig);
				dev->read(&SerData,pipe,0);
				PRINTF("Energy:%f GJ\n",pipe->Immediate.Reading);
				PRINTF("Volume Flow:%0.2f \n",pipe->Immediate.instanceFlow);
				PRINTF("Flow Temperature:%0.2f C\n",pipe->Immediate.waterInTem);
				PRINTF("Return Temperature:%0.2f C\n",pipe->Immediate.waterOutTem);
			default:
				break;
		}
	}

}

