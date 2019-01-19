/*************************************************************************
	> File Name: ThreadSerial.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月26日 星期三 09时22分40秒
 ************************************************************************/

 #include "GobleDefine.h"
void *thread_serial(void *arg);
 int send_serial(pSerialData pSerData,void *sendBuf,size_t len);
