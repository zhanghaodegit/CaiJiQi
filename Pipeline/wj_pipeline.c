/*************************************************************************
	> File Name: wj_resident.c
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2019年01月05日 星期六 09时08分13秒
 ************************************************************************/
#include<stdio.h>
#include <stdint.h>
#define     DEBUG_PRINTF        1
#define     DEBUG_ERROR         1
#include "debug.h"
#include "GobleDefine.h"
#include "PipeLine.h"
#include "wj_pipeline.h"
#include "ThreadSerial.h"
//extern int send_serial(pSerialData pSerData,void *sendBuf,size_t len); 
//int test(int i, int j);
static void fill_mbus(void *dst, void *src, int len)
{
	unsigned char* dstTmp = dst;
	unsigned char* srcTmp = src;
	int j = 0;
	unsigned char tmp = 0;
	for(j = 0; j < len/2; j++)
	{
		tmp = (*srcTmp)>'9'?(*srcTmp < 'f'?*srcTmp - 'a':*srcTmp - 87):*srcTmp - '0';
		tmp <<= 4;
		srcTmp ++;
		tmp |= (*srcTmp)>'9'?(*srcTmp < 'f'?*srcTmp - 'a':*srcTmp - 87):*srcTmp - '0';
		printf("tmp:%x\n",tmp);
		dstTmp[len/2 - j - 1] = tmp;
		printf("dsttmp:%x\n",dstTmp[len/2 - j - 1]);
		srcTmp ++;
		tmp = 0;
	}

}
unsigned char ChkSum(unsigned char *in, int num)
{
	unsigned char sum = 0;
	if(in == NULL)
	{
		PERROR("chkSum in is null");
	}
	while(num--)
	{
		sum+=*in++;
	}

}
static int recv_serial(pSerialData pSerData, uint8_t *recvBuf, int32_t recMaxSize, int32_t timeOut)
{
	PRINTF("in recv_serial\n");
	int ret = 0;
	uint8_t angData[] = {0x88,0x20,0x78,0x56,0x34,  0x12,0x00,0x55,0x55,0x81,  0x2e,0x1f,0x90,0x01,  0x00,0x21,0x32,0x43,0x03,   0x00,0x34,0x43,0x24,0x03,  0x00,0x78,0x56,0x21,0x02,  0x00,0x21,0x32,0x43,0x03,  0x00,0x34,0x43,0x24,0x03, 0x00,0x23,0x12,  0x00,0x78,0x21, 0x00,0x56,0x34, 0x00,0x35,0x10,0x11,0x01,0x19,0x20, 0x00,0x00,0x01, 0x16};
	if(sizeof(angData) > recMaxSize)
	{
		PRINTF("recv_serial too big data\n");
		ret = -1;
		return ret;
	}
	PRINTF("befoe strncpy");
	printf("sizoef andData is %d\n",sizeof(angData));
	/*
	for(int i = 0; i<59;i++)
	{
		recvBuf[i] = angData[i];
	}
	*/
	//strncpy(recvBuf,angData, 59);
	memcpy(recvBuf,angData,sizeof(angData));
	return ret;
}
int wj_pipe_read(pSerialData pSerData, void *ops,int timeout)                                   
{    
	int ret = 0;
	PRINTF("wj_pipe_read addr ddd");
	PipeLine *pipe = ops;
	PRINTF("ADDR:%s",pipe->Config.Address);
	uint8_t Pack[256];
	HwcPip *send = (HwcPip*)Pack;
	send->FE[0] = 0xfe;
	send->FE[1] = 0xfe;
	send->h = 0x68;
	send->type = 0x20;
	send->ctrl = 0x01;
	send->len = 0x03;
	send->di = 0x901f;
	send->ser = 0x01;
	send->end = 0x16;
	fill_mbus(send->add,pipe->Config.Address,14);
	printf("send1：%x,send2:%x",send->add[0], send->add[1]);
	send->cs = ChkSum(&send->h, sizeof(*send) - 4);
	ret = send_serial(pSerData, (void *)send,sizeof(*send));
	ret = recv_serial(pSerData, Pack,sizeof(Pack),PIPE_RECV_TIMEOUT);
	for(int i = 0; i < 59; i++)
		PRINTF("pACK%d:%x ", i,Pack[i]);
	pipe->Immediate.cumulativeFlow++;
	pipe->Immediate.instanceFlow++;
	pipe->Immediate.waterInTem= 30.0;
	pipe->Immediate.waterOutTem = 20.0;
	pipe->Immediate.Reading++;
	pipe->Measure.Freeze_Reading = pipe->Immediate.Reading;
	return ret;
}
