#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "message_process.h"


//初始化数据库的所有表项,即初始化
void init_database()
{
	MYSQL* mysql;
	int res = 0;
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "172.17.0.6", "root", "root", "shiyan", 3306, NULL, 0);
	//    connection = mysql_real_connect(mysql,"localhost","root","root","shiyan",3306,NULL,0);

	if (mysql == NULL)
	{
		printf("连接失败:%s\n", mysql_error(mysql));
	}
	else
	{
		printf("连接成功, 服务器版本: %s, 客户端版本: %s.\n", MYSQL_SERVER_VERSION, mysql_get_client_info());


		// 清空location表
		char sql[1024] = "delete from location;";
		res = mysql_query(mysql, sql);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 清空ipnode表
		char sql1[1024] = "delete from ipnode;";
		// 插入数据
		res = mysql_query(mysql, sql1);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 清空message表
		char sql2[1024] = "delete from message;";
		// 插入数据
		res = mysql_query(mysql, sql2);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 清空setting表
		char sql4[1024] = "delete from setting;";
		// 插入数据
		res = mysql_query(mysql, sql4);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 清空delay表
		char sql5[1024] = "delete from delay;";
		// 插入数据
		res = mysql_query(mysql, sql5);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 清空recv_message表
		char sql3[1024] = "delete from recv_message;";
		// 插入数据
		res = mysql_query(mysql, sql3);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 添加
		for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
		{
			char sql6[1024];
			char pre_sql[] = "insert into recv_message(id,recvMessage) values('%d','%s')";
			sprintf(sql6, pre_sql, i,"waiting");
			// 插入数据
			res = mysql_query(mysql, sql6);
			if (!res)
			{
				printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
				/*里头的函数返回受表中影响的行数*/
			}
			else
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
			}
		}

		// 清空update_recv表
		char sql7[1024] = "delete from update_recv;";

		// 插入数据
		res = mysql_query(mysql, sql7);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 添加
		for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
		{
			char sql8[1024];
			char pre_sql[] = "insert into update_recv(id,recvMessage) values('%d','%s')";
			sprintf(sql8, pre_sql, i, "waiting");
			// 插入数据
			res = mysql_query(mysql, sql8);
			if (!res)
			{
				printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
				/*里头的函数返回受表中影响的行数*/
			}
			else
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
			}
		}

		// 清空state表
		char sql9[1024] = "delete from state;";

		// 插入数据
		res = mysql_query(mysql, sql9);
		if (!res)
		{
			printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
			/*里头的函数返回受表中影响的行数*/
		}
		else
		{
			//分别打印出错误代码及详细信息
			fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
		}

		// 初始化状态都是0
		for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
		{
			char sql10[1024];
			char pre_sql[] = "insert into state(id,state) values('%d','%d')";
			sprintf(sql10, pre_sql, i, 0);
			// 插入数据
			res = mysql_query(mysql, sql10);
			if (!res)
			{
				printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
				/*里头的函数返回受表中影响的行数*/
			}
			else
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "Update error %d: %s\n", mysql_errno(mysql), mysql_error(mysql));
			}
		}
	}
	mysql_close(mysql);
}