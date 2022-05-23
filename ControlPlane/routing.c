#include <stdio.h>
#include <math.h>
#include "string.h"
#include "database.c"
#include "share_memory.c"
#include "message_process.h"

//Dijkstra算法，返回一个字符串 ，desid/nextnodeid/num+1 ， 按路径结点id倒序拼接
char* Dijkstra(int TABLE[][MAX_NODES_NUMBER], int source_id, int destination_id, int label[]) 
{
	//用来记录路径节点ID
	int pred[MAX_NODES_NUMBER]; 

	//存放发送节点到其余节点的距离 
	int distance[MAX_NODES_NUMBER];  

	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		distance[i] = TABLE[source_id][i];  
		pred[i] = source_id;
	}

	distance[source_id] = 0;

	//label 1是标记已经访问过的结点，从source_node开始
	label[source_id] = 1;

	//定义最短距离变量
	long min_distance;  

	//记录最小距离的下一跳目的节点ID
	int next_hop_id;

	//循环标志位
	int count = 1;
	while (count < MAX_NODES_NUMBER) 
	{
		min_distance = MAXINT + 1;
		for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
		{
			//若未访问过以及可达，不停的让min_dis为最小距离
			if (distance[i] < min_distance && label[i] == 0) 
			{  
				min_distance = distance[i]; 
				next_hop_id = i;  
			}
		}

		//记录
		label[next_hop_id] = 1;  

		//若不可达
		if (min_distance == MAXINT) 
		{	
			pred[next_hop_id] = -1;
		}
		else 
		{
			for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
			{

				//表示未访问过
				if (label[i] == 0) 
				{  
					//比较多跳总距离与直达距离
					if ((min_distance + TABLE[next_hop_id][i]) < distance[i]) 
					{  
						distance[i] = min_distance + TABLE[next_hop_id][i];

						//记录下一跳节点的ID
						pred[i] = next_hop_id;  
					}
				}
			}
		}
		count++;
	}

	//用j初值记录目的节点的ID
	int j = destination_id;  
	//static char re[64];
	char re[MAXDATASIZE];
	int num = 0;

	//拼接一个字符串 ，desid/sourceid/num+1/   index是此字符串含字符数量，可以定义位置
	int index = 0;  
	index = index + sprintf(re + index, "%d/", j);

	do 
	{
		//记录下一跳节点ID 
		j = pred[j];
		if (j == -1) 
		{
			break;
		}
		else 
		{
			index = index + sprintf(re + index, "%d/",j);
			num++;
		}
	} while (j != source_id);

	//此处num+1是指从发送节点到目的节点经过几个路径
	index = index + sprintf(re + index, "%d/",num + 1); 
	
	//返回一个字符串 ，desid/nextnodeid/num+1 ，
	return re;
}

//由起终点ip与port得到路径上的id
char* routing(char* source_ip, int source_port, char* destination_ip, int destination_port) 
{
    //re数组存放路由分析得到的多跳id
	//static char re[MAXDATASIZE];
	char re[MAXDATASIZE];

	printf("Debug--- 开始读nodesfile\n");
	read_nodesfile();
	printf("Debug--- 读nodesfile结束\n");

	//初始化发送节点与接收节点的节点号
	int source_id = -1, destination_id = -1;  

	//判断节点是否存在，若存在则记录发送节点与接收节点的序号
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) {
			if (!strcmp(NODES[i].ip, source_ip) && NODES[i].port == source_port) 
			{
				source_id = i;
			}
			else if (!strcmp(NODES[i].ip, destination_ip) && NODES[i].port == destination_port) {
				destination_id = i;
			}
		}
	}

	if (source_id == -1 || destination_id == -1) 
	{
		strcpy(re, "The source node or destination node does not exist!\n");
	}
	else 
	{
		/*
		距离矩阵
		以下可以优化，调用updata_network，
		返回一个二维数组表即可
		*/
		int TABLE[MAX_NODES_NUMBER][MAX_NODES_NUMBER];
		int i, j;

		//标志已经访问过了，如果是没有添加的结点直接置为1 
		int label[MAX_NODES_NUMBER] = { 0 };  

		for (i = 0; i < MAX_NODES_NUMBER; ++i) 
		{
			if (NODES[i].state == 1) 
			{
				for (j = i; j < MAX_NODES_NUMBER; ++j) 
				{
					if (NODES[j].state == 1) 
					{
						if (i == j) 
						{
							TABLE[i][j] = 0;
						}
						else 
						{
							int distance = (int)sqrt(
								(NODES[i].x - NODES[j].x) * (NODES[i].x - NODES[j].x) +
								(NODES[i].y - NODES[j].y) * (NODES[i].y - NODES[j].y) +
								(NODES[i].z - NODES[j].z) * (NODES[i].z - NODES[j].z));
							if (distance <= NODES[i].range) 
							{
								TABLE[i][j] = distance;
							}
							else 
							{
								TABLE[i][j] = MAXINT;
							}
							if (distance <= NODES[j].range) 
							{
								TABLE[j][i] = distance;
							}
							else 
							{
								TABLE[j][i] = MAXINT;
							}
						}
					}
				}
			}
			else if (NODES[i].state == 0) 
			{
				int k;
				label[i] = 1;
				for (k = 0; k < MAX_NODES_NUMBER; ++k) 
				{
					TABLE[i][k] = -1;
					TABLE[k][i] = -1;
				}
			}
		}
		printf("Debug--- 进入Dijstra\n");
		strcpy(re, Dijkstra(TABLE, source_id, destination_id, label));
		printf("Debug---Dijstra结束，返回re：%s\n",re);
	}
	return re;
}