#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mysql.h>
#include "share_memory.h"
#include "message_process.h"

//用结构体数组保存对应节点的ip、port、state
struct node_network_imformation
{
    //结点ip
	char ip[20];

    //结点端口
	long int port;

	//节点标识，id
	long int node_id;

	//节点状态，使能状态
	long int state;
};

struct node_network_imformation NODE_NETWORK_IMFORMATION[MAX_NODES_NUMBER];

//保存数据库中读取到的要发送的多径信息
char* MULTI_PATH_MESSAGE[MAX_NODES_NUMBER];

//保存多路径ID号，格式2/4/5
char* MULTI_PATH_ID[MAX_NODES_NUMBER];

//此处用数组保存目的节点号(此处可以换地址传递)
char destination_id[MAX_NODES_NUMBER];

//整数型转为字符串型	
char* int_to_char(int num, char *str)
{
	sprintf(str, "%d", num);		
	return str;
}

//根据ip返回节点id
int return_id(char ip[]) 
{
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (!strcmp(NODE_NETWORK_IMFORMATION[i].ip, ip)) 
		{
			return i;
		}
	}
	printf("multi_path: Error: The node is none.\n");
	return -1;
}

//根据id返回该节点的ip与port
char* return_ip_port(int node_id) 
{
	printf("---Debug: Turn node %d id into ip and port ", node_id);
	static char return_ip_port[MAXDATASIZE];
	int index = 0;
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODE_NETWORK_IMFORMATION[i].state == 1)
		{
			if (NODE_NETWORK_IMFORMATION[i].node_id == node_id) 
			{
				index = index + sprintf(return_ip_port + index, "%s/", NODE_NETWORK_IMFORMATION[i].ip);
				index = index + sprintf(return_ip_port + index, "%ld/", NODE_NETWORK_IMFORMATION[i].port);
				printf("%s\n", return_ip_port);
				return return_ip_port;
			}
		}
	}
	//若节点状态为0，或者节点不存在，则报错！
	char notice[] = "Error: The node does not exist or state is off!\n";
	printf("multi_path: %s", notice);
	return notice;
}

//将节点网络信息（节点号、状态、ip、port）写入结构体数组中
void listen_node_network_imfornation()
{
    MYSQL_RES* result;
    MYSQL_ROW row;
    MYSQL* mysql;
    int res = 0;
    mysql = mysql_init(NULL);
    mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
    int flag = 1;

    while (flag)
    {
        if (mysql == NULL)
        {
                printf("multi_path: Connection failed:%s\n", mysql_error(mysql));
        }
        else
        {
            char sql[1024] = "select * from ipnode inner join state where ipnode.id = state.id";
            res = mysql_query(mysql, sql);

            //printf("%d\n", res);
			if (!res) 
			{
				result = mysql_store_result(mysql);
				if (result == NULL) 
				{
					printf("multi_path: There is no data.");
				}
				else
                {
					//printf("multi_path: The network imformation of nodes\n\n");
                    while ((row = mysql_fetch_row(result)) != NULL) 
					{
                        //printf("row[0]:%s\trow[1]:%s\trow[2]:%s\trow[3]:%s\t\n", row[0], row[1], row[2], row[4]);
                        int i;
                        NODE_NETWORK_IMFORMATION[i].node_id = atoi(row[0]);
                        strcpy(NODE_NETWORK_IMFORMATION[i].ip, row[1]);
                        NODE_NETWORK_IMFORMATION[i].port = atoi(row[2]);
                        NODE_NETWORK_IMFORMATION[i].state = atoi(row[4]);
                        // printf("%ld\t%ld\t%s\t%ld\t\n\n", 
                        //        NODE_NETWORK_IMFORMATION[i].node_id, 
                        //        NODE_NETWORK_IMFORMATION[i].state, 
                        //        NODE_NETWORK_IMFORMATION[i].ip,
                        //        NODE_NETWORK_IMFORMATION[i].port);
                        i++;
                    }
                    flag = 0;
                }
            }
			else
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, 
						"select error %d: %s\n", 
						mysql_errno(mysql), 
						mysql_error(mysql));
			}
        }
    }
	mysql_close(mysql);
}

// 监听多路径传输的message，将message保存在数组中，返回多路径的数量
int listen_multi_path_message(char* hostip, int* temp) 
{
	//保存结果集
	MYSQL_RES* result; 

	//存储结果集中的一行
	MYSQL_ROW row; 
	MYSQL* mysql;
	int res = 0;
	int flag_1 = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	int i = 0;
	int flag = 1;

	int Debug_num2 = 0;

	while (flag) 
	{
		Debug_num2++;	
		if (mysql == NULL) 
		{
			printf("multi_path: Connection failed:%s\n", mysql_error(mysql));
		}
		else 
		{
			char sql1[1024] = "select * from multi_path_transfer where message_flag = '1' and source_id = ";
			strcat(sql1, LABEL);
			res = mysql_query(mysql, sql1);

			if (!res) 
			{
				/*
				 获取全部数据集保存到本机，
				 随后的mysql_fetch_row()就再也不会返回错误，
				 当返回NULL的时候表示已经到达数据集的末端。
				*/
				result = mysql_store_result(mysql);

				//保存查询的结果集
				if (result == NULL) 
				{
					printf("multi_path: There is no data.\n");
				}
				else
				{
					//int destination_id;
					/*
					读取结果集中的数据，
					返回的是下一行，
					因为保存结果集时，
					当前的游标在第一行之前。
					*/
					while ((row = mysql_fetch_row(result)) != NULL) 
					{
						//message_flag是message表中的flag
						int message_flag = atoi(row[4]);	
						if (message_flag == (*temp + 1)) 
						{
							//信息写入数组
							strcpy(destination_id, row[1]);
							MULTI_PATH_ID[i] = row[2];
							MULTI_PATH_MESSAGE[i] = row[3];
							i++;
							// *temp = *temp + 1;
							// flag=0;
						}
						else 
						{
							//printf("There is no new message.\n");
							continue;
						}
					}
					// //此处debug，把下面代码放入i++后面
					*temp = *temp + 1;
					flag=0;
				}
			}
			else 
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "select error %d: %s\n", 
				mysql_errno(mysql), mysql_error(mysql));
			}

			//此处写上对message_flag的更新，变成0就行
			char sql2[1024] = "update multi_path_transfer set message_flag = '0' where message_flag = '1' and source_id = ";
			strcat(sql2, LABEL);
			flag_1 = mysql_query(mysql, sql2);			

			//debug:如果更改成功返回 获取message以及update标志位成功
			if(!flag_1)
			{
				printf("multi_path: Get message and update flag successfully.\n\n");
			}
		}

	}
	mysql_close(mysql);
	return i;
}

void multi_path_mes(char* hostip) 
{
	int temp = 0;
	printf("multi_path: Monitor node network information.\n\n");

	//监听节点网络信息
	listen_node_network_imfornation();

	//得到多径数量，后面写入
	int multi_path_amount = listen_multi_path_message(hostip, &temp);
	printf("multi_path: multi_path_amount  %d\n\n", multi_path_amount);

	if(multi_path_amount != 0)
	{
		//创建写入共享内存的结构体数组
		struct MESSAGE s1[multi_path_amount];
		struct MESSAGE* s1_point = NULL;
		s1_point = s1;

		//存放源及目的节点、以及多跳路径节点(去除下一跳节点)的ip与port;
		//char source_ip_port[30];
		char multi_path_ip_port[MAXDATASIZE];
		char destination_ip_port[30];
		strcpy(destination_ip_port, return_ip_port(atoi(destination_id)));

		for(int i = 0; i < multi_path_amount; i++)
		{
			//存放多路径id
			char* multi_path_id[MAX_NODES_NUMBER];
			int num1 = 0;

			split(MULTI_PATH_ID[i], "/", multi_path_id, &num1);

			//保存除去下一跳的多跳ip与port与message
			//char multi_path_ip_port[MAXDATASIZE];

			for(int j = 1; j < num1; j++)
			{
				strcat(multi_path_ip_port, return_ip_port(atoi(multi_path_id[j])));	
			}

			//存多跳数, strcat形参为字符，存放字符型多跳数
			char multi_hop[16] = {0};
		
			//封装多径传输的信息，此处可以将其5改成6,多径传输专属
			char multi_path_transfer_message[MAXDATASIZE];
			strcat(multi_path_transfer_message, "5");
			strcat(multi_path_transfer_message, "/");
			strcat(multi_path_transfer_message, int_to_char(num1, multi_hop));
			strcat(multi_path_transfer_message, "/");
			strcat(multi_path_transfer_message, multi_path_ip_port);
			strcat(multi_path_transfer_message, destination_ip_port);
			strcat(multi_path_transfer_message, MULTI_PATH_MESSAGE[i]);

			printf("multi_path: multi_path_transfer_message：  %s\n\n", multi_path_transfer_message);

			//得到下一跳ip与port
			char* nexthop_ip_port[64];
			int num2 = 0;
			split(return_ip_port(atoi(multi_path_id[0])), "/", nexthop_ip_port, &num2);


			strcpy(s1_point->SEND_MESSAGE, multi_path_transfer_message);
			strcpy(s1_point->DESTINATION_IP, nexthop_ip_port[0]);
			s1_point->DESTINATION_PORT = atoi(nexthop_ip_port[1]);
			
			//这里不是转发，所以也不需要时间戳的赋值
			s1_point->TRANSMIT_FLAG = 0;

			//把多径数量写入
			s1_point->path_num = multi_path_amount;

			//计算时间戳
			s1_point->TIME_SLOT = FRAME_SIZE / listen_bandwidth();

			//指向下一个地址,在这最后就会超数组,加个判断
			if(i + 1 != multi_path_amount)
			{
				s1_point++;
			}

			//置空数组，数组是"地址传递"，否则信息会累加冗余造成错误
			memset(multi_path_ip_port,'\0', sizeof(multi_path_ip_port));
			memset(multi_path_transfer_message,'\0', sizeof(multi_path_transfer_message));
		}
		write_shared_memory(s1);
	}
}

