/*************************************************************************
	> File Name: device.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月07日 星期五 08时51分05秒
 ************************************************************************/
typedef struct 
{
	struct list_head Master;
	struct list_head Client;
	struct list_head Valve;
	struct
	{
		char path[32];
		char config_db[32];
		char data_db[32];
		char log_path[32];
	}Config;

}WorkSpace;

