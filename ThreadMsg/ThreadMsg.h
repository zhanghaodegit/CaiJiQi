/*************************************************************************
	> File Name: ThreadMsg.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月27日 星期四 12时59分27秒
 ************************************************************************/

#ifndef Thread_Msg_H
#define Thread_Msg_H
typedef struct Msg{
	uint32_t Msg;
	uint64_t WParam;
	uint32_t LParam;
	struct Msg *next;
}*pThreadMsg,ThreadMsg;

int QueueSendMsg(struct list_head *ThreadList,char *sendTo, uint32_t Msg, uint64_t WParam,uint32_t LParam);
int QueueGetMsg(pThreadData lpThis, pThreadMsg Msg);
#endif
