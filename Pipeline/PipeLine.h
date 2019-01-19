/*************************************************************************
	> File Name: DevConfig.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2019年01月03日 星期四 10时16分11秒
 ************************************************************************/


#ifndef DEV_CONFIG_H
#define		DEV_CONFIG_H

#define		PIPE_ADDR_MAX_SIZE 16
#define		PIPE_NUM_MAX_SIZE 16
#define		ID_MAX_SIZE			32
#define		MANU_MAX_SIZE		64
typedef struct
{

	struct 
	{
		int SerNum;
		int Baud;
		int DataBit;
		int StopBit;
		int Parity;
	
	}SerConfig;
	struct 
	{
		char Address[PIPE_ADDR_MAX_SIZE];
		int type;
		char Number[PIPE_NUM_MAX_SIZE];
		int Caliber;
		char Id[ID_MAX_SIZE];
		char Manu[MANU_MAX_SIZE];
	}Config;
	struct 
	{
		float cumulativeFlow;
		float instanceFlow;
		float waterInTem;
		float waterOutTem;
		float Reading;
	}Immediate;
	struct 
	{
		double Freeze_Reading;
		float Period_Apportion_Heat;
	}Measure;


}PipeLine ,*pPipeLine;


#endif
