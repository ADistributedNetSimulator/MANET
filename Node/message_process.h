#pragma once

//定义server基站节点的ip port ，node port为1200
#define CONSOLE_IP "172.17.0.2"
#define CONSOLE_PORT 3333
#define HOSTPORT 1200
#define MAXCLIENTNUM 10

//定义节点数量
#define MAX_NODES_NUMBER 5
#define NODE_DES_LENGTH 32
#define MAXDATASIZE 4096	

//拿数字定义节点操作
#define JOIN 1   
#define DELETE 2
#define UPDATE 3
#define SEND 4
#define TRANSMIT 5

//时延暂且限定为10ms
#define FRAME_SIZE 50 

//标识本节点号
#define LABEL "0" 

//发送信息时字符串拼接符
#define FRAME_SIZE_EOF '@' 

//字符串结束符
#define PACKET_EOF '#'  

// struct node_network_imformation
// {
//     //结点ip
// 	char ip[20];

//     //结点端口
// 	int port;

//     /*
//     结点区分编号 
//     在服务器那边编排这个参数
//     */
// 	int node_id;

//     /*
//     结点使能状态 
//     在服务器那边编排这个参数
//     */
// 	int state;
// }

struct node
{
    //结点ip
	char ip[20];

    //结点端口
	int port;

    /*
    结点区分编号 
    在服务器那边编排这个参数
    */
	int node_id;

    /*
    结点使能状态 
    在服务器那边编排这个参数
    */
	int state;

    //坐标
	int x;
	int y;
	int z;
    
    /*
    节点覆盖范围，
    根据此进行子网划分
    */
	int range;

    //结点描述
	char description[NODE_DES_LENGTH];
};

//结构体数组用来存放结点信息
struct node NODES[MAX_NODES_NUMBER];

//本机ip
char hostip[20];


/* 套接字信息结构体，用于记录客户端信息 */
typedef struct CLIENTINFORMATION{
    int client_fd;               // 套接字描述符
    char client_ip[20];          // 客户端IP地址
    int client_port;             // 客户端端口号
}client_information;

// /* 用于记录客户端信息的数组 */
// struct CLIENTINFORMATION arr_con_socket[MAXCLIENTNUM];
int con_client_count;                 // 当前客户端数量