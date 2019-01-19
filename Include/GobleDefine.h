/*************************************************************************
	> File Name: GobleDefine.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2019年01月04日 星期五 09时24分34秒
 ************************************************************************/

#ifndef GODEF_H
#define		GODEF_H
enum{
	MSG_INIT=100,
	MSG_READ_PIPELINE,
	MSG_UPDATE_PIPELINE_CONFIG,
};
enum{
	ADD = 0,
	DELETE = 1,
};
typedef struct{
    int serialHandle;
    int AlreadyOpen;
}SerialData, *pSerialData;
#endif
