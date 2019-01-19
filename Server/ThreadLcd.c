/*************************************************************************
  > File Name: ThreadLcd.c
  > Author: wuji
  > Mail: 158836015@qq.com 
  > Created Time: 2018年12月26日 星期三 11时22分25秒
 ************************************************************************/

#include<stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
 #define     DEBUG_PRINTF        1
 #define     DEBUG_ERROR         1
#include "debug.h"
#include "list_head.h"
#include "MulThread.h"
#include "ThreadMsg.h"
#include "PipeLine.h"
#include "GobleDefine.h"
#include <termios.h>
#include <fcntl.h>
#include <string.h>
static struct termios initial_settings, new_settings;
static int peek_character = -1;
void init_keyboard(void);
void close_keyboard(void);
int kbhit(void);
int readch(void); 
typedef void (*pfun)(void);
extern struct list_head ThreadList;
void init_keyboard()
{
	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;// 设置非正规模式，如果程序每次要从终端读取一个字符的话，这是必须的
	new_settings.c_lflag &= ~ECHO;// 关闭回显
	new_settings.c_cc[VMIN] = 1;// 设置非正规模式下的最小字符数
	new_settings.c_cc[VTIME] = 0;// 设置非正规模式下的读延时
	tcsetattr(0, TCSANOW, &new_settings);
}
int set_nonblocking(int fd)
{
    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}
int lcd_printf(char *s,int flag,int start,int len)
{
	int ret = 0;
	char *sStart;
	char *sLiang;
	char *sEnd;
	int startLen = start;
	int endLen = strlen(s) - start - len;
	int i = 0;
	if(s == NULL)
	{
		PRINTF("lcd_printf null");
		ret = -1;
		return ret;
	}
	if(flag == 0)
	{
		printf("%s\n",s);
		return ret;
	}
	sStart = s;
	sLiang = s + start;
	sEnd = s + start + len;
	for(i = 0 ; i < startLen; i ++)
	{
		printf("%c",*(s+i));
	}
	for(i = startLen ; i < startLen + len; i ++)
	{
	
		printf("\033[00;31m%c\033[0m",*(s+i));
	
	}
	for(i = startLen + len; i < startLen + len + endLen; i ++)
	{
		
		printf("%c",*(s+i));
	
	}
	printf("\n");
}
int draw_item(char **item, int size, int select)
{
	int i = 0;
	for(i = 0 ; i < size; i++)
	{
		if(i == select)
		{
			lcd_printf(*(item+i),1,0,strlen(*(item+i)));
		}
		else{
			lcd_printf(*(item + i),0, 0, 0);
		}
	}



}
void basic_info(void)
{
	printf("basic info\n");
}
int parase_line(char *line,char *argv[])
{
	int argc = 0;
	while(1)
	{
	while(*line == ' '||*line =='\t')
	{
		++line;		
	}
	if(*line == '\0'|| '\n'==*line || '\r' == *line)
	{
		if(argc == 0)
		{
			argv[argc] = NULL;
			return argc;
		}
		else	
		{

			*line='\0';
			argv[argc] = NULL;
			return argc;
		}
	}
	argv[argc++] = line;
	while(*line !=' '&&'\t'!=*line&&'\n'!=*line&&'\r'!=*line&&'\0'!=*line)
	{
		++line;
	}
	if(*line=='\0'||*line=='\n'||*line=='\r')
	{
		*line = '\0';
		argv[argc] = NULL;
		return argc;
	}
	*line++='\0';
	}
}
void import_conf(void)
{
	printf("import_conf\n");
	int i = 0;
	char *argv[6];
	if(access("/opt/materia/tmp/import.txt",R_OK) != -1)
	{
		printf("import exist\n");
	}
	else{
		printf("import no exist\n");
	}
	char importMsg[256];
	FILE *importFp=fopen("/opt/materia/tmp/import.txt","r");
	char *pRet;
	int ret = 0;
	if(importFp == NULL)
	{
		PERROR("open import err");
	}
	while((pRet = fgets(importMsg,sizeof(importMsg),importFp)) != NULL )
	{
		PRINTF("GET:%s",importMsg);
		ret = parase_line(importMsg,argv);
		if(ret == 0)
		{
			continue;
		}
		for(i = 0; i<ret;i++)
		{
			PRINTF("argv[%s]",argv[i]);
		}
		if(ret < 5)
		{
			continue;
		}
		if(strcmp(argv[0],"pipeline") == 0)
		{
			pPipeLine ops = calloc(1,sizeof(PipeLine));
			pRet = strncpy(ops->Config.Number,argv[1],PIPE_NUM_MAX_SIZE);
			pRet = strncpy(ops->Config.Address,argv[2],PIPE_ADDR_MAX_SIZE);
			ops->Config.type = atoi(argv[3]);
			ops->Config.Caliber = atoi(argv[4]);
			PRINTF("add pipeline:%s : addr:%s,type:%d,Caliber:%d",ops->Config.Number,ops->Config.Address,ops->Config.type,ops->Config.Caliber);
			QueueSendMsg(&ThreadList,"Data",MSG_UPDATE_PIPELINE_CONFIG,ops,ADD);
		}
	}

}
void del_pip(void)
{
	printf("del_pip\n");
}
void pipe_info(void)
{
	printf("in pipe_info\n");
}
void res_info(void)
{
	printf("in res_info\n");
}
pfun ConfJump[3]=
{
	basic_info,
	import_conf,
	del_pip
};
void conf_info(void)
{
	printf("in conf_info\n");
	int select = 0;
	int c = 0;
	char *confMenu[3]={"Basic Info","Import Conf","Del Pip"};
		while(1)
		{
		c = getchar();
		switch(c)
		{
			case 27:
				break;
			case 66:
				select ++;
				if(select >= 3)
				{
					select = 0;
				}
				break;
			case 10:
				ConfJump[select]();
				break;
			default:
				printf("get %d", c);
				break;
		}
		draw_item(confMenu,3,select);
		sleep(2);
		}

}

pfun MainJump[3]=
{
	pipe_info,
	res_info,
	conf_info
};

void *thread_lcd(void *arg)
{
	pThreadData lpThis= (pThreadData)arg;
	set_nonblocking(0);
	init_keyboard();
	char c;
	ThreadMsg LcdMsg;
	int select = 0;
	char *MainMenu[3] = {"Pipe Info","Res Info","Conf Info"};
	while(1)
	{
//		import_conf();
#if 1 
		while(1)
		{


		c = getchar();
		switch(c)
		{
			case 27:
				break;
			case 66:
				select ++;
				if(select >= 3)
				{
					select = 0;
				}
				break;
			case 10:
				MainJump[select]();
				break;
			default:
				printf("get %d", c);
				break;
		}
		draw_item(MainMenu,3,select);
		sleep(2);
		}
#endif
		//	QueueGetMsg(lpThis,&LcdMsg);
//		printf("LcdMsg.msg:%d,w:%d,l:%d\n",LcdMsg.Msg,LcdMsg.WParam,LcdMsg.LParam);
		sleep(2);
	}
}

