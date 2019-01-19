/*************************************************************************
	> File Name: Include/sqlInit.h
	> Author: wuji
	> Mail: 158836015@qq.com 
	> Created Time: 2018年12月22日 星期六 09时02分57秒
 ************************************************************************/


const char *tableDevice = "create table Devices ("
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
const char *tablePipeline="CREATE TABLE Pipelines ("
"Number TEXT PRIMARY KEY NOT NULL UNIQUE,"
"Address TEXT NOT NULL DEFAULT '00',"
"Type INTEGER NOT NULL DEFAULT 0,"
"Caliber INTEGER );";
static const char *tableValve = "CREATE TABLE ValveControllers ("
"Number  TEXT NOT NULL UNIQUE,"
"Address TEXT NOT NULL DEFAULT '00',"
"SetValveOpenRatio_Workday TEXT DEFAULT '100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0',"
"SetValveOpenRatio_Weekend TEXT DEFAULT '100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0',"
"CurrentValveOpenRatio REAL DEFAULT 100.0"
");";
static const char *tableTask = "CREATE TABLE \"Tasks\" "
"(\"id\" INTEGER NOT NULL,"
" \"timestamp\" TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
" \"taskType\" INTEGER NOT NULL,"
" \"param\" BLOB);";
static const char *tableEvent = "CREATE TABLE Events ( "
"ERC       INTEGER,"
"id        INTEGER  PRIMARY KEY AUTOINCREMENT,"
"timestamp DATETIME,"
"size      INTEGER,"
"data      BLOB "
");";

static const char *tableException = "CREATE TABLE Exceptions ( "
"item  TEXT PRIMARY KEY NOT NULL UNIQUE,"
"value INTEGER "
");";

static const char *tablePID = "CREATE TABLE \"Pids\" ("
"\"timestamp\" TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
"\"Name\" TEXT NOT NULL, "
"\"Temperature_IN\" REAL, "
"\"Temperature_Out\" REAL, "
"\"CurOpenRate\" REAL, "
"\"SetOpenRate\" REAL); ";


static const char *tableDataResident = "CREATE TABLE \"Residents\" ("
"\"timestamp\" TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
"\"Name\" TEXT NOT NULL, "
"\"Valve_Open_Time\" INTEGER, "
"\"Totle_Valve_Open_Time\" INTEGER, "
"\"Use_Heat\" REAL, "
"\"Set_Temperature\" REAL, "
"\"Room_Temperature\" REAL, "
"\"Totle_Heat_For_Season\" REAL, "
"\"Open_Rate\" INTEGER, "
"\"Temperature_In\" REAL, "
"\"Temperature_Out\" REAL, "
"\"State\" INTEGER)";

static const char *tableDataPipeline = "CREATE TABLE Pipelines ("
"\"timestamp\" TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
"\"Name\" TEXT NOT NULL, "
"\"Temperature_In\" REAL, "
"\"Temperature_Out\" REAL, "
"\"Instantaneous_Flow\" REAL, "
"\"Instantaneous_Heat\" REAL, "
"\"Cumulative_Flow\" REAL, "
"\"Readings\" REAL, "
"\"Period_Apportion_Heat\" REAL, "
"\"Totle_Apportion_Heat_Season\" REAL, "
"\"State\" INTEGER);";

const char *tableDeviceName = "Devices";
const char *tablePipelineName = "Pipelines";
const char *tableResidentName = "Residents";
const char *tableValveName = "ValveControllers";
const char *tableTaskName = "Tasks";
const char *tableEventName = "Events";
const char *tableExceptionName = "Exceptions";
const char *tableDataPipelineName = "Pipelines";
const char *tableDataResidentName = "Residents";
