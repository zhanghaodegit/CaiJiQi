/*************************************************************************
	> File Name: main.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月21日 星期五 11时12分27秒
 ************************************************************************/


const char *tableEvent = "CREATE TABLE Event ("
"ERC	INTEGER,"
"id		INTEGER PRIMARY KEY AUTOINCREMENT,"
"timestamp	DATETIME"
");";
const char *tableDevice = "create table device ("
"item	TEXT PRIMARY KEY NOT NULL UNIQUE,"
"value TEXT"
");";
const char *tableResident="CREATE TABLE Residents ("
			"\"Name\" TEXT PRIMARY KEY NOT NULL, "
			"\"Address\" TEXT NOT NULL DEFAULT '00', "
			"\"Acreage\" REAL, "
			"\"Receptor\" TEXT NOT NULL DEFAULT '0000', "
			"\"Arrears\" INTEGER DEFAULT 0, "
			"\"ShowHeatValue\" INTEGER DEFAULT 0, "
			"\"Valve\" INTEGER DEFAULT 2);";
