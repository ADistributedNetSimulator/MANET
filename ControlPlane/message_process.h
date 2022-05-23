#pragma once

#define MAX_NODES_NUMBER 5
#define NODE_DES_LENGTH 32
#define MAXDATASIZE 4096	

#define JOIN 1
#define DELETE 2
#define UPDATE 3
#define SEND 4
#define TRANSMIT 5
#define MULTI_PATH_SEND 6
#define MULTI_PATH_TRANSMIT 7

#define CONSOLE_IP "172.17.0.2"
#define CONSOLE_PORT 3333

//此处换成节点0节点
#define LABEL "C"

//固定monitor 的带宽以及数据包大小
#define FRAME_SIZE 50 

//默认时延
#define TIME_SOLT 0.1 

//距离矩阵的最大值
const int MAXINT = 32767;

#define FRAME_SIZE_EOF '@'
#define PACKET_EOF '#'

struct node
{
	//结点ip
	char ip[20];

	//结点端口
	int port;

	//结点区分编号 在服务器那边编排这个参数
	int node_id;

	//结点使能状态 在服务器那边编排这个参数
	int state;

	//坐标
	int x;
	int y;
	int z;

	//通信半径
	int range;

	int oil;

	//结点描述
	char description[NODE_DES_LENGTH];
};

//结构体数组用来存放结点
struct node NODES[MAX_NODES_NUMBER];

//邻接矩阵,通过距离和覆盖范围来判断可达性。
int REACH_TABLE[MAX_NODES_NUMBER][MAX_NODES_NUMBER];

//距离矩阵，记录点和点之间的距离
int DISTANCE_TABLE[MAX_NODES_NUMBER][MAX_NODES_NUMBER];

//记录运动节点的油量
int OIL_TABLE[MAX_NODES_NUMBER][MAX_NODES_NUMBER];