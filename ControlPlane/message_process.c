#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"routing.c"
#include"subnet.c"


/*  
	字符串切割分析
	src 源字符串的首地址(buf的地址)
	separator 指定的分割字符
	dest 接收子字符串的数组
	num 分割后子字符串的个数
*/
void split(char* src, const char* separator, char** dest, int* num) 
{
	char* pNext;
	int count = 0;

	//如果传入的地址为空或长度为0，直接终止
	if (src == NULL || strlen(src) == 0)  
		return;

	//如未指定分割的字符串，直接终止
	if (separator == NULL || strlen(separator) == 0)  
		return;

	//必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
	pNext = (char*)strtok(src, separator); 
	while (pNext != NULL) {
		*dest++ = pNext;
		++count;

		//必须使用(char *)进行强制类型转换
		pNext = (char*)strtok(NULL, separator);  
	}
	*num = count;
}

/*  
	在receive_message中对信息分析时调用。分析信息作用
	JOIN	  网络初始化中国，节点申请加入网络
	UPDATE    动态节点变化位置
	DELETE    节点退出网络
	SEND 	  源节点有发送信息需求，此时console提供路由转发功能
	TRANSIMIT 代表为转发信息
*/
char* data_analysis(char* recvData) 
{
	char* revbuf[16];  //存放分割后的子字符串
	static char re[MAXDATASIZE];
	int num = 0;
	split(recvData, "/", revbuf, &num);
	int operation = atoi(revbuf[0]);
	struct node n;
	if (operation == JOIN|| operation == UPDATE) 
	{
		strcpy(n.ip, revbuf[1]);
		n.port = atoi(revbuf[2]);
		n.x = atoi(revbuf[3]);
		n.y = atoi(revbuf[4]);
		n.z = atoi(revbuf[5]);
		n.range = atoi(revbuf[6]);
		strcpy(n.description, revbuf[7]);
		if (operation == JOIN) {
			strcpy(re, append_node(n));
		}
		else 
		{
			strcpy(re, update_node(n));
		}
	}
	else if (operation == DELETE) 
	{
		char ip[20];
		int port;
		strcpy(ip, revbuf[1]);
		port = atoi(revbuf[2]);
		strcpy(re, delete_node(ip, port));
	}
	else if (operation == SEND) 
	{
		char sourceip[20];
		int sourceport;
		char destinationip[20];
		int destinationport;
		strcpy(sourceip, revbuf[1]);
		sourceport = atoi(revbuf[2]);
		strcpy(destinationip, revbuf[3]);
		destinationport = atoi(revbuf[4]);

		//计算路径
		char nextData[64];
		printf("Debug--- 开始进入routing\n");
		strcpy(nextData, routing(sourceip, sourceport, destinationip, destinationport));
		printf("Debug--- routing结束\n");
		read_nodesfile();

		//存放分割后的下一跳
		char* nexthop[64]; 
		split(nextData, "/", nexthop, &num);

		//意味着不可达
		if (atoi(nexthop[num - 1]) == 1) 
		{
			sprintf(re, "%s", "Can not reach the destination!");
			insert_received_message(return_index(sourceip), "Can not reach the destination!");
		}
		else 
		{
			int index = 0;
			index = index + sprintf(re + index, "%d/", TRANSMIT);
			index = index + sprintf(re + index, "%s/", nexthop[num - 1]);
			for (int i = num - 2; i >= 0; --i) 
			{
				index = index + sprintf(re + index, "%s/", NODES[atoi(nexthop[i])].ip);
				index = index + sprintf(re + index, "%d/", NODES[atoi(nexthop[i])].port);
			}
		}
	}
	return re;
}