#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "message_process.h"

/*随机产生0，1*/
int get_rand_number()
{
    time_t t;

    //初始化随机数发生器 
    srand((unsigned) time(&t));

    //产生随机数
    int rand_num = rand() % 2;

    //让其间隔超过一秒，保证随机性
    sleep(1.0);

    return rand_num;
}

//获取随机时延
double get_rand_delay()
{
    srand((unsigned)time(NULL));
    double delay =rand();
    return delay;
}

//根据数据库状态表中，判断。n为对应的row[0]
int poor_channel_judge()
{
    MYSQL_RES* result; //保存结果集
	MYSQL_ROW row; //存储结果集中的一行
	MYSQL* mysql;
    int state;

	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);

	if (mysql == NULL) 
	{
		printf("poor_channel:  Connection failed. %s\n", mysql_error(mysql));
	}
	else 
	{  
		int res = 0;
		char sql[1024] = "select id, poor_channel from funtion where id = ";

		//LABLE用来区分每个的frame的@0@,可以改成  return_index(hostip)
		strcat(sql, LABEL); 
		res = mysql_query(mysql, sql);
		if (!res) 
		{
			result = mysql_store_result(mysql);
			if (result == NULL) 
			{
				printf("poor_channel: There is no data.\n\n");
			}
			else
			{
				while ((row = mysql_fetch_row(result)) != NULL) 
				{
					state = atoi(row[1]);
				}
			}
		}
	} 
	mysql_close(mysql);
    return state;
}

