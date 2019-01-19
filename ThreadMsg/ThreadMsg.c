/*************************************************************************
	> File Name: ThreadMsg.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月27日 星期四 08时54分20秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
//#define		DEBUG_PRINTF	1
#define		DEBUG_ERROR		1
#include <string.h>
#include "debug.h"
#include "list_head.h"
#include "MulThread.h"
#include "ThreadMsg.h"

int QueueSendMsg(struct list_head *ThreadList,char *sendTo, uint32_t Msg, uint64_t WParam,uint32_t LParam)
{
	struct list_head *i;
	int32_t ret = 0;
	pThreadData sendTh;
	pThreadMsg msg;
	if(ThreadList == NULL)
	{
		PERROR("ThreadList is NULL");
		ret = -1;
		return ret;
	}
	list_for_each(i,ThreadList)
	{
		sendTh = list_entry(i,ThreadData,listThread);
	//	sendTh = container_of(i,ThreadData,listThread);
		if(strcmp(sendTh->threadName,sendTo) == 0)
		{
			PRINTF("send thread name:%s",sendTh->threadName);
			pthread_mutex_lock(&sendTh->mutex_lock);
			msg = malloc(sizeof(ThreadMsg));
			if(msg == NULL)
			{
				pthread_mutex_unlock(&sendTh->mutex_lock);
				PERROR("MSG IS NULL");
				ret = -1;
				return ret;
			}
			msg->Msg = Msg;
			msg->WParam = WParam;
			msg->LParam = LParam;
			msg->next = NULL;
			if(sendTh->MessageQueue)
			{
				PRINTF("had Messqeue");
				*sendTh->MessageQueueAdd = msg;
				sendTh->MessageQueueAdd = &msg->next;
			}
			else{
				PRINTF("no Messqeue");
			
				sendTh->MessageQueue = msg;
				sendTh->MessageQueueAdd = &msg->next; 
			}
			pthread_mutex_unlock(&sendTh->mutex_lock);
			sem_post(&sendTh->sem);
			PRINTF("sem_post ok");
			return ret;
		}
	
	}
	ret = -1;
	return ret;
}
int QueueGetMsg(pThreadData lpThis, pThreadMsg Msg)
{
	int ret = 0;
	pThreadMsg msgTmp;
	PRINTF("IN QueueGetMsg");
	sem_wait(&lpThis->sem);
	PRINTF("sem_wait ok");
	pthread_mutex_lock(&lpThis->mutex_lock);
	if(lpThis->MessageQueue)
	{
		msgTmp = lpThis->MessageQueue;
		PRINTF("name:%s1 ok msgTmp :%d, %d %d",lpThis->threadName,msgTmp->Msg,msgTmp->WParam,msgTmp->LParam);
		lpThis->MessageQueue=lpThis->MessageQueue->next;
		PRINTF("getMsg  ok");
	
	}
	else{
		pthread_mutex_unlock(&lpThis->mutex_lock);
		ret = -1;
		return ret;
	}
	memcpy(Msg, msgTmp,sizeof(ThreadMsg));
	PRINTF("memcpy ok");
	free(msgTmp);
	pthread_mutex_unlock(&lpThis->mutex_lock);
	return ret;
}

