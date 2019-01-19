/*************************************************************************
	> File Name: MulThread.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月25日 星期二 09时18分05秒
 ************************************************************************/
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include "list_head.h"
#define		DEBUG_PRINTF	1
#define		DEBUG_ERROR		1
#include "debug.h"
#include "MulThread.h"
#include<stdio.h>
int CreateThread(char *Name,void * (*start_rtn)(void*),struct list_head *pThreadList)
{
	int ret = 0;
	int stacksize = THREAD_STACK_SIZE;
	PRINTF("CreateThread...");
	PRINTF("%p ",pThreadList);
	pthread_attr_t attr;
	pThreadData pThread = (ThreadData * )malloc(sizeof(ThreadData));
	if(pThread == NULL)
	{
		PERROR("malloc pthreadData");
		ret = -1;
		return ret;
	}
	pThread->threadName = Name;
	pthread_attr_init(&attr);
	ret = pthread_attr_setstacksize(&attr, stacksize);
	if(ret != 0)
	{
		PERROR("pthread_setstacksize");
	}
	PRINTF("attr_setstack...");
	ret = pthread_create(&pThread->threadTid,&attr,start_rtn,pThread);
	if(ret != 0)
	{
		PERROR("pthread_create...");
	}
	sem_init(&pThread->sem,0,0);
	pthread_mutex_init(&pThread->mutex_lock,NULL);
	PRINTF("mutex_init...");
	list_add(&pThread->listThread , pThreadList);
	pThread->MessageQueue = NULL;
	pThread->MessageQueueAdd = &pThread->MessageQueue;
	return ret;
}
