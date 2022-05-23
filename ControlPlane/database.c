#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "file.h"

void insert_ip_port(int id, char* ip, int port) 
{
	char sql[1024];
	MYSQL* mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	if (mysql == NULL) 
	{
		printf("连接失败:%s\n", mysql_error(mysql));
	}
	else 
	{
		//printf("连接成功, 服务器版本: %s, 客户端版本: %s.\n", MYSQL_SERVER_VERSION, mysql_get_client_info());
		char pre_sql[] = "insert into ipnode(id,ip,port) values('%d','%s','%d')";
		sprintf(sql, pre_sql, id, ip, port);
		//printf("%s\n", sql);
		// 执行语句
		res = mysql_query(mysql, sql);
		if (!res)
		{
			printf("Insert ipnode successfully, Inserted %lu rows\n", (unsigned long)mysql_affected_rows(mysql)); //里头的函数返回受表中影响的行数
			
		}
		else
		{
			fprintf(stderr, "Insert error %d: %s\n", mysql_errno(mysql), mysql_error(mysql)); //分别打印出错误代码及详细信息
		}
	}
	mysql_close(mysql);
}

void insert_delay(char* sourceip, char* destinationip, char* packet, double delay) 
{
	char sql[1024];
	MYSQL*  mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	if (mysql == NULL) 
	{
		printf("Connection failed: %s\n", mysql_error(mysql));
	}
	else 
	{
		char pre_sql[] = "insert into delay(sourceip,destinationip,packet,delay) values('%s','%s','%s','%lf')";
		sprintf(sql, pre_sql, sourceip, destinationip, packet, delay);

		// 执行语句
		res = mysql_query(mysql, sql);
		if (!res) 
		{
			printf("Insert delay successfully, Inserted %lu rows, success!\n\n", (unsigned long)mysql_affected_rows(mysql));
		}
		else
		{
			fprintf(stderr, "Insert error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}

//这个是由负责消息发送的。当结点发送消息成功后，显示在目的地，当结点发送不了，就会在源输出错误
void insert_received_message(int id , char recv_message[]) 
{
	char sql[1024];
	MYSQL* mysql;
	int res = 0;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	if (mysql == NULL) {
		printf("Connection failed:%s\n", mysql_error(mysql));
	} 
	else 
	{		
		//更新即可
		char pre_sql[] = "update recv_message set recvMessage = '%s' where id = '%d'";
		sprintf(sql, pre_sql, recv_message, id);

		// 执行语句
		res = mysql_query(mysql, sql);
		if (!res){
			printf("Update recv_message successfully, Inserted %lu rows\n\n", (unsigned long)mysql_affected_rows(mysql));
		}
		else{
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}

//这个是由monitor告知的结点信息发送到各自的前端中，例如结点添加成功等。
void insert_update_received(int id, char recv_message[]) 
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
		char pre_sql[] = "update update_recv set recvMessage = '%s' where id = '%d'";
		sprintf(sql, pre_sql, recv_message, id);

		// 执行语句
		res = mysql_query(mysql, sql);
		if (!res)
		{
			printf("Update update_recv successfully, Inserted %lu rows\n\n", (unsigned long)mysql_affected_rows(mysql));
		}
		else
		{
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}

//delete 某个结点。在delete_node 中调用
void delete_node_from_location(int id) 
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
		//删除location表中对应内容
		char pre_sql[] = "delete from location where id = %d;";
		sprintf(sql, pre_sql, id);
		
		// 执行语句
		res = mysql_query(mysql, sql);
		if (!res)
		{
			printf("Delete node successfully, Inserted %lu rows\n\n", (unsigned long)mysql_affected_rows(mysql));
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}
	}
	mysql_close(mysql);
}