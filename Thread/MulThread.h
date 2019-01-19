/*************************************************************************
	> File Name: MulPthread.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月25日 星期二 09时18分31秒
 ************************************************************************/
#define	 THREAD_STACK_SIZE		502*2014
typedef struct{
	char *threadName;
	sem_t sem;
	pthread_mutex_t mutex_lock;
	pthread_t threadTid;
	struct list_head listThread;
	void *privateData;
	struct Msg *MessageQueue;
	struct Msg **MessageQueueAdd;
}*pThreadData,ThreadData;



int CreateThread(char *Name,void * (*start_rtn)(void*),struct list_head *pThreadList);


