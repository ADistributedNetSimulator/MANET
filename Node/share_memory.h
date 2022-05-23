#pragma once
#define MAXDATASIZE 4096

//时间戳的长度
#define TIME_LENGTH 17

//因硬件条件限制，最大send并发数设为5
#define MAX_PTHREAD 5 

struct MESSAGE 
{
	//标识是否可读
	int FLAG;

	// //标识是否可发
	// int send_flag;

	//标识发送内容是否为转发包
	int TRANSMIT_FLAG;

	//发送的目的IP
	char DESTINATION_IP[25];

	//发送的目的PORT
	int DESTINATION_PORT;

	//发送内容
	char SEND_MESSAGE[MAXDATASIZE];

	//记录存了多少个时间戳
	int STORE_NUM;

	//记录时间戳
	char STORE_TIME[MAXDATASIZE][TIME_LENGTH];

	//记录时间戳的来源
	char STORE_IP[MAXDATASIZE][TIME_LENGTH];

    //记录当接收来自某个节点的多个帧的时间信息
	long int receive_time;

	//用来保存设定的带宽值
	double TIME_SLOT;

	//用来记录多路径数量
	int path_num;
};
