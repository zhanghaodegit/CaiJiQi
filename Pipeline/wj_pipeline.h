/*************************************************************************
	> File Name: wj_pipeline.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2019年01月05日 星期六 09时35分47秒
 ************************************************************************/

int wj_pipe_read(pSerialData pSerData, void *ops,int timeout);

typedef struct {
	unsigned char FE[2];
	unsigned char h;
	uint8_t type;
	uint8_t add[7];
	uint8_t ctrl;
	uint8_t len;
	uint16_t di;
	uint8_t ser;
	uint8_t cs;
	uint8_t end;
}HwcPip;
#define PIPE_RECV_TIMEOUT	3
