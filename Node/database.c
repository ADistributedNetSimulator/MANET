#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "file.c"

//listen_bandwidth用到
#define DEFAULT_BANDWIDTH 5

//返回监听到的带宽
double listen_bandwidth() 
{
	MYSQL_RES* result; //保存结果集
	MYSQL_ROW row; //存储结果集中的一行
	MYSQL*  mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);

	/*
	记录监听次数，若监听次数>MAX_NODES_NUMBER，
	那说明没有赋值带宽，将返回默认值5Mbps.
	*/
	int listen_num = 0;
	while (1) 
	{
		if (mysql == NULL) 
		{
			printf("Connection failed:%s\n", mysql_error(mysql));
		}
		else 
		{
			char sql[1024] = "select * from setting where id = ";
			strcat(sql, LABEL);

			// 查询数据
			res = mysql_query(mysql, sql);
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
					printf("There is no data.\n");

					//XW+
					mysql_close(mysql);
					return DEFAULT_BANDWIDTH;
				}
				else
				{	
					if (listen_num == MAX_NODES_NUMBER) 
					{
						printf("BW:%d\n\n",DEFAULT_BANDWIDTH);
						mysql_close(mysql);
						return DEFAULT_BANDWIDTH;
					}

					/*
					读取结果集中的数据，返回的是下一行，
					因为保存结果集时，当前的游标在第一行之前
					*/
					while ((row = mysql_fetch_row(result)) != NULL) 
					{
						mysql_close(mysql);
						printf("atof(row[1]:%f\n\n",atof(row[1]));
						return atof(row[1]);
					}
				}
				/*
				printf("selected %lu rows\n", 
				(unsigned long)mysql_affected_rows(mysql));
				里头的函数返回受表中影响的行数
				*/
				printf("listen_num:%d\n\n",listen_num);
				listen_num++;
			}
			else 
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "select error %d: %s\n", 
				mysql_errno(mysql), mysql_error(mysql));
			}
		}
	}
}

//插入时延
void insert_delay(char* sourceip, char* destinationip, char* packet, double delay) 
{
	char sql[1024];
	MYSQL* mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);

	if (mysql == NULL) 
	{
		printf("Connection failed:%s\n", mysql_error(mysql));
	}
	else 
	{
		char pre_sql[] = 
			"insert into delay(sourceip,destinationip,packet,delay) values('%s','%s','%s','%lf')";
		sprintf(sql, pre_sql, sourceip, destinationip, packet, delay);

		// 执行语句
		res = mysql_query(mysql, sql);
		//printf("%d\n", res);
		if (!res)
		{
			//printf("Notice: Insert delay success\n\n");
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, 
					"Insert error %d: %s\n\n", 
					mysql_errno(mysql), 
					mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}

// 监听message
void listen_message(char* ip, int* port, int* temp) 
{
	//保存结果集
	MYSQL_RES* result; 

	//存储结果集中的一行
	MYSQL_ROW row; 
	MYSQL* mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	int flag = 1;

	while (flag) 
	{
		//printf("listen_message: 连接mysql\n");
		if (mysql == NULL) 
		{
			printf("Connection failed:%s\n", mysql_error(mysql));
		}
		else 
		{
			char sql[1024] = "select * from message where id = ";
			strcat(sql, LABEL);

			// 查询数据
			res = mysql_query(mysql, sql);
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
					printf("There is no data.\n\n");
				}
				else
				{
					/*
					读取结果集中的数据，
					返回的是下一行，
					因为保存结果集时，
					当前的游标在第一行之前。
					*/
					while ((row = mysql_fetch_row(result)) != NULL) 
					{
						int message_flag = atoi(row[4]);  //message_flag是message表中的flag	
						if (message_flag == (*temp + 1)) 
						{
							printf("listen_message: There is a message.\n\n");
							strcpy(ip, row[1]);
							*port = atoi(row[2]);


							//此处判断row[3]内容；
							char M[MAXDATASIZE];
							strcpy(M,row[3]);

							/*
							message.txt是row[3]采用文件的方式，
							去记录内容，方便转发。
							*/							
							clear_messagefile("message.txt");

							//将内容写入文件中
							write_messagefile("message.txt",row[3]);
							*temp = *temp + 1;
							flag=0;	
						}
						else 
						{
							//printf("listen_message: There is no new message.\n\n");
							//continue;
							flag=0;	
							break;
						}
					}
				}
			}
			else 
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "select error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
			}
		}
	}
	mysql_close(mysql);
}

//将结点接收到的消息，插入进数据库供前端使用
void insert_recv_message(char receive_message[] ) 
{
	char sql[1024];
	MYSQL* mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);

	if (mysql == NULL) 
	{
		printf("Connection failed:%s\n", mysql_error(mysql));
	}
	else 
	{
		//更新即可
		char pre_sql[] = "update recv_message set recvMessage = concat_ws('-',recvMessage,'%s') where id = '%d';";
		sprintf(sql, pre_sql, receive_message, atoi(LABEL));
		//printf("%s\n", sql);

		// 执行语句
		res = mysql_query(mysql, sql);
		//printf("%d\n", res);
		if (!res)
		{
			printf("Insert receive_message success.\n\n");
			/*printf("Inserted %lu rows\n", 
					 (unsigned long)mysql_affected_rows(mysql));
			里头的函数返回受表中影响的行数
			*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, 
					"Insert error %d: %s\n", 
					mysql_errno(mysql), 
					mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}
