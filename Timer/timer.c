/*************************************************************************
	> File Name: timer.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月07日 星期五 09时24分45秒
 ************************************************************************/

#include<stdio.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#define	DEBUG_PRINTF	1
#define	 DEBUG_ERROR	1
#include "debug.h"
#include <sys/epoll.h>
#include "list_head.h"
#include "timer.h"
	int32_t gTmFd;
	struct epoll_event eV1,*pEV1;
	int32_t efd;
LIST_HEAD(gTimerList);
time_t gLocalTime;
time_t gRunTime;
struct tm* gDateTime;
int InitTimer(void)
{
	struct itimerspec timerTmp;
	int32_t ret = 0;
	timerTmp.it_interval.tv_nsec=0;
	timerTmp.it_interval.tv_sec = 1;
	timerTmp.it_value.tv_nsec = 0;
	timerTmp.it_value.tv_sec = 1;

	gTmFd = timerfd_create(CLOCK_REALTIME,0);
	if(gTmFd < 0)
	{
		PERROR("timerfd_create");
		ret = -1;
		return ret;
	}
	timerfd_settime(gTmFd, 0, &timerTmp, NULL);
	return ret;	
}

int WaitTimer(void)
{
	uint64_t  exp;
	ssize_t s;
	static  sCount = 0;
	s = read(gTmFd, &exp,sizeof(uint64_t));
	if(s != sizeof(uint64_t))
		PERROR("read sizeof");

	//printf("s=%d, exp : %ld\n",s, exp);
	//printf("Count : %d\n",sCount ++);
}
int EpollTimerInit(void)
{
	
	struct itimerspec timerTmp;
	int32_t ret = 0;
	timerTmp.it_interval.tv_nsec=0;
	timerTmp.it_interval.tv_sec = 1;
	timerTmp.it_value.tv_nsec = 0;
	timerTmp.it_value.tv_sec = 1;
	PRINTF("in epollTimerInit");

	gTmFd = timerfd_create(CLOCK_REALTIME,0);

	if(gTmFd < 0)
	{
		PERROR("timerfd_create");
		ret = -1;
		return ret;
	}

	timerfd_settime(gTmFd, 0, &timerTmp, NULL);

	efd = epoll_create1(0);
	if(efd < 0)
	{
		PERROR("epoll_create");
		ret = -2;
		return ret;
	}

	eV1.data.fd = gTmFd;
	eV1.events = EPOLLIN;
	ret = epoll_ctl(efd,EPOLL_CTL_ADD,gTmFd,&eV1);
	if(ret < 0)
	{
		PERROR("epoll_ctl");
		return ret;
	}

	pEV1 = (struct epoll_event *)calloc(1, sizeof(struct epoll_event));
	if(pEV1 == NULL)
	{
		PERROR("malloc epoll_event");
		ret = -3;
		close(efd);
		close(gTmFd);
		return ret;
	}
	PRINTF("epoll  init  ok");
	return 0;
}
int EpollTimerWait(void)
{
		
	uint64_t value;
	static  tCounter = 0;
	epoll_wait(efd,pEV1,1,-1);
	if(pEV1->events & EPOLLIN)
	{
		read(pEV1->data.fd, &value, sizeof(uint64_t));
		PRINTF(" go to time%d",tCounter++);
	}
	else
	{
		PRINTF("no time fd");
	}
}
int GetRtcTime(time_t *t)
{
	int rtc;
	struct tm timeRtc;
	int ret = 0;
	time_t timeTmp;
	if(t == NULL)
	{
		PERROR("Get Rtc null");
	}
	rtc = open("/dev/rtc0",O_RDONLY);
	if(rtc<0)
	{
		PERROR("open rtc");
		ret = -1;
		return ret;
	}
	memset(&timeRtc, 0, sizeof(struct tm));
	ioctl(rtc,RTC_RD_TIME, &timeRtc);
	close(rtc);
	*t = mktime(&timeRtc);
	if((*t) == -1)
	{
		PERROR("mktime");
		ret = -1;
		return ret;
	}
	return ret;
}
int GetLocalTime(time_t *t)
{
	int ret = 0;
	*t = time(0);
	return ret;
}
int GetUtcTime(time_t *t)
{
	int ret = 0;
	*t = time(0);
	return ret;
}
int TimerListProc()
{
	int ret = 0;
	struct list_head *i;
	pTimerPrivate tmPriv;
	list_for_each(i,&gTimerList)
	{
		tmPriv = list_entry(i,TimerPrivate, timerList);	
		tmPriv->timInter ++;
		if(tmPriv->timInter >= tmPriv->timeout)
		{
			tmPriv->timInter = 0;
			if(tmPriv->cb)
			{
				tmPriv->cb(tmPriv->cb_data, gLocalTime);
			}
		}
	}
}
int SetTimer(int timeout,int mode ,int (*cb)(void *,time_t),void *cb_data)
{
	pTimerPrivate tmPriv;
	int ret = 0;
	tmPriv = (pTimerPrivate)malloc(sizeof(TimerPrivate));
	if(tmPriv == NULL)
	{
		ret = -1;
		return ret;
	}
	tmPriv->timeout = timeout;
	tmPriv->mode = mode ;
	tmPriv->timInter = 0;
	tmPriv->cb = cb;
	tmPriv->cb_data = cb_data;
	list_add(&tmPriv->timerList, &gTimerList);
	return ret;
}
int TimerProc(void)
{
	PRINTF("in TimerProc");
	gLocalTime ++;
	gRunTime ++;
	gDateTime = localtime(&gLocalTime);
	TimerListProc();

}
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
