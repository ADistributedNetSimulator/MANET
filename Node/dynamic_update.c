#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "message_process.h"
#include "share_memory.h"

/*
此处为位置的动态更新
*/
void location_update(int* posX, int* posY, int* posZ) 
{
    //结点更新的趋势和速度
    int speedpar = 20;
    double trendX = 0.8;
    double trendY = 0.2;
    double trendZ = 0.2;

	//在节点当前位置的基础上加的随机变量
	double radx = 0.0;
	double rady = 0.0;
	double radz = 0.0;
	srand((int)time(NULL));

	/*
	trendX越大，倾向于向负方向移动，
	trendX越小，倾向于向正方向移动。
	RAND_MAX用来产生随机小数
	*/
	radx = rand() / (double)(RAND_MAX)-trendX;
	rady = rand() / (double)(RAND_MAX)-trendY;
	radz = rand() / (double)(RAND_MAX)-trendZ;

	// 下面三个if是为了防止碰壁
	if (*posX <= 0.5 * speedpar) 
	{
		while (radx < 0.0) 
		{
			trendX = 1 - trendX;
			radx = rand() / (double)(RAND_MAX)-trendX;
		}
	}
	if (*posY <= 0.5 * speedpar) 
	{
		while (rady < 0.0) 
		{
			trendY = 1 - trendY;
			rady = rand() / (double)(RAND_MAX)-trendY;
		}
	}
	if (*posZ <= 0.5 * speedpar) 
	{
		while (radz < 0.0) 
		{
			trendZ = 1 - trendZ;
			radz = rand() / (double)(RAND_MAX)-trendZ;
		}
	}

	// 更新当前节点的位置信息
	*posX = *posX + (int)(radx * speedpar);
	*posY = *posY + (int)(rady * speedpar);
	*posZ = *posZ + (int)(radz * speedpar);
}
 
/*
	监听节点状态，接着启动或更新
	当返回值是0的时候，说明是插入
	当返回值是1的时候，说明是更新
	temp来记录是否插入，就是指插入一次
*/
int listen_node_state(int* x, int* y, int* z, int* range, int* temp, int* current_state) 
{
	//保存结果集
	MYSQL_RES* result; 

	//存储结果集中的一行
	MYSQL_ROW row; 
	MYSQL* mysql;
	int res = 0;
	int flag1 = 0;

	/*连接前，用mysql_init初始化MYSQL连接句柄*/
	mysql = mysql_init(NULL);
	mysql = mysql_real_connect(mysql, "210.30.97.150", "root", "root", "NetSimulation", 3306, NULL, 0);
	while (1) 
	{
		if (mysql == NULL) 
		{
			printf("dynamic_update: Connection failed:%s\n", mysql_error(mysql));
		}
		else 
		{
			int state;
			char sql[1024];
			char pre_sql[] = "select state.id, state.state, location.x, location.y, location.z, location.range, location.flag from state inner join location where state.id = '%d' and location.id = '%d'";
			sprintf(sql, pre_sql, atoi(LABEL),  atoi(LABEL));
			res = mysql_query(mysql, sql);
			if (!res) 
			{
				// 查询数据
				result = mysql_store_result(mysql);
				if (result == NULL) 
				{
					printf("dynamic_update: There is no data.");
				}
				else
				{
					while ((row = mysql_fetch_row(result)) != NULL) 
					{
						state = atoi(row[1]);
						*x = atoi(row[2]);
						*y = atoi(row[3]);
						*z = atoi(row[4]);
						*range = atoi(row[5]);
						int location_flag = atoi(row[6]);

						/*
						如果state表中该点的state为1， 
						且location表中有数据
						说明节点已存入，待启动
						*/
						if (*current_state ==0 && state == 1 && 
							location_flag == 1 && *temp == 0) 
						{
							printf("dynamic_update: The node starts working！\n\n");
							*temp = *temp + 1;

							//记录下启动状态
							*current_state = 1;
							mysql_close(mysql);
							return 0;
						}

						/*
						如果state表中该点的状态为 1 
						且location 中有数据做了更新，
						说明需要更新结点状态
						*/
						else if (*current_state == 1 && 
						state == 1 && location_flag >*temp) 
						{
							printf("dynamic_update: Wait for the manual update！\n\n");
							*temp = *temp + (location_flag - *temp);
							mysql_close(mysql);
							return 1;
						}

						/*
							如果当前状态是1且state表中该点状态为1，
							且location 中没有数据做了更新，
							说明需要调用自动更新的函数
						*/
						else if(*current_state == 1)
						{

							//若state == 0，说明前端已将该结点删除
							if (state == 0) 
							{
								*current_state = 0;
								return 2;
							}
							else 
							{
								printf("dynamic_update： Automatic update！\n\n");
								int x1 = *x;
								int y1 = *y;
								int z1 = *z;

								//每十秒动态更新一次
								sleep(60);

								location_update(&x1, &y1, &z1);
								char sql1[1024];
								char pre_sql1[] = "update location set x = '%d',y = '%d',z = '%d' where id = '%d' ";
								sprintf(sql1, pre_sql1, x1, y1, z1, atoi(LABEL));

								res = mysql_query(mysql, sql1);
								if (!res)
								{
									printf("dynamic_update： Update location success\n\n");
								}
								else
								{
									//分别打印出错误代码及详细信息
									fprintf(stderr, "Update error %d: %s\n", 
									mysql_errno(mysql), mysql_error(mysql));
								}
							}		
						}
						else 
						{
							printf("dynamic_update： Wait for the manual update！\n\n");

							//xw+ ,不然while循环卡在这
							sleep(10);
							break;
						}
					}
				}
			}
			else 
			{
				//分别打印出错误代码及详细信息
				fprintf(stderr, "select error %d: %s\n", 
				mysql_errno(mysql), mysql_error(mysql));
			}
			//利用标志位只打印一次，避免重复打印
			if(flag1 == 1){
				printf("state:%d ；current_state:%d\n", state, *current_state);
				printf("Wait for starting! \n\n");
				flag1++;
			}
		}
	}
}

void dynamic_update(char* hostip)
{
	/*
		temp负责监听结点插入更新 
		记录是否插入，一个标志位变量，
		标志节点位置插入了多少次
	*/
	int temp = 0;
	int current_state = 0;
	while (1) 
	{
		sleep(10);
		int x, y, z, range;
		
		/*
			当返回值是0的时候，说明是插入
			当返回值是1的时候，说明是更新；
			当返回值是2的时候，说明前端删除了
		*/
		int re = listen_node_state(
			&x, &y, &z, &range, 
				&temp, &current_state);

		struct MESSAGE s1[0];

		// struct MESSAGE* s1 = new MESSAGE;

		//插入新节点信息,更新节点信息,删除节点信息
		if (re == 0) 
		{
			strcpy(s1->SEND_MESSAGE, join_message(hostip, HOSTPORT, x, y, z, range));
		}
		else if (re == 1) 
		{
			strcpy(s1->SEND_MESSAGE, 
				update_message(hostip, HOSTPORT, 
					x, y, z, range));
		}
		else 
		{
			strcpy(s1->SEND_MESSAGE, delete_message(hostip, HOSTPORT));
		}

		double BW = listen_bandwidth();
		printf("BW:%f\n",BW);
		strcpy(s1->DESTINATION_IP, CONSOLE_IP);
		s1->DESTINATION_PORT = CONSOLE_PORT;
		s1->TRANSMIT_FLAG = 0;
		s1->TIME_SLOT = FRAME_SIZE / BW; 
		s1->path_num = 0;

		/*
			不是转发信息，不需要时间戳的赋值
			.TIME_SLOT返回设定的带宽值。
			默认返回FRAME_SIZE
		*/
		printf("1---------------测试动态更新信息是否正确写入共享内存内-----------1\n\n");
		printf("测试path_num : %d\n ", s1 -> path_num);
		printf("测试TRANSMIT_FLAG : %d\n ", s1->TRANSMIT_FLAG);
		printf("测试FLAG : %d\n ", s1 -> FLAG);
		printf("测试DESTINATION_IP : %s\n ", s1 -> DESTINATION_IP);
		printf("测试TIME_SLOT : %f\n ", s1 -> TIME_SLOT);
		printf("测试SEND_MESSAGE : %s\n ", s1 -> SEND_MESSAGE);
		printf("-------------------测试完毕-------------------\n\n");

		write_shared_memory(s1);
	}
}


/*
可以根据油量来划分子网
此处为简单的油耗变化
void oil_consumption(int* oil)
{
    //运动节点如无人机油耗的速度
    double trend_oil_consumption = 0.01;

    //当油量低于5，则停止该节点运动.
    if(*oil >= 5)
    {
        *oil -= trend_oil_consumption;
    }
	sleep(1000);
}

//此处是根据油量划分子网的部分代码
oil_consumption(&oil);
char sql1[1024];
char pre_sql1[] = "update location set x = '%d',y = '%d',z = '%d', oil = '%d' where id = '%d' ";
sprintf(sql1, pre_sql1, x1, y1, z1, oil1, atoi(LABEL));								
*/


