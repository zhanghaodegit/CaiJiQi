/*************************************************************************
	> File Name: debug.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月06日 星期四 09时46分06秒
 ************************************************************************/

#ifndef	DEBUG_H_
#define	DEBUG_H_

#if(DEBUG_PRINTF)
#define	PRINTF(fmt,arg...)		printf("%s: "fmt"\n",__func__,##arg)
#else
#define	PRINTF(fmt,arg...)
#endif

#if(DEBUG_ERROR)
#define	PERROR(fmt,arg...)     printf("error :%s:"fmt"\n",__func__,##arg);\
	perror(NULL)

#endif

#endif



