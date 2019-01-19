/*************************************************************************
	> File Name: timer.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月07日 星期五 09时57分29秒
 ************************************************************************/

int TimerProc(void);
int SetTimer(int timeout,int mode ,int (*cb)(void*,time_t),void *cb_data);
int InitTimer(void);
typedef struct {

	struct list_head timerList;
	int (*cb)(void *,time_t);
	void * cb_data;
	int timInter;
	int timeout;
	int mode;
}*pTimerPrivate,TimerPrivate;
