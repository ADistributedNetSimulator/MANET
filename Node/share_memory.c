#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "share_memory.h"

void write_shared_memory(struct MESSAGE* S) 
{
	//共享内存
	int shmid;
	void* shm = NULL;

	//定义一个结构体指针指向S
	struct MESSAGE* SEND_MES;

	//创建key值
	key_t key = 1234;

	//共享内存大小
	long int size = (sizeof(struct MESSAGE))*MAX_PTHREAD;

	//创建共享内存,目前size暂时是固定，没找到满足每次创建共享内存size，shmget错误的解决方法
	shmid = shmget(key, size, 0666 | IPC_CREAT);

	//判断共享内存是否创建成功
	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	//将共享内存连接到当前进程的地址空间
	shm = shmat(shmid, 0, 0);
	if (shm == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	//设置共享内存
	SEND_MES = (struct MESSAGE*)shm;

	printf("W.1---------------传入的结构体数组[0]数值-----------\n\n");
	printf("path_num : %d\n ", S -> path_num);
	//printf("TRANSMIT_FLAG : %d\n ", S -> TRANSMIT_FLAG);
	//printf("FLAG : %d\n ", S -> FLAG);
	printf("DESTINATION_IP : %s\n ", S -> DESTINATION_IP);
	//printf("SEND_MESSAGE : %s\n ", S -> SEND_MESSAGE);
	printf("-------------------完毕-------------------\n\n");

	//数据写入内存
	while (1) 
	{
		if(SEND_MES->FLAG != 1)
		{
			if (S->path_num > 0 && S->path_num < MAX_PTHREAD + 1)
			{
				int path_num = S->path_num;
				for(int i = 0; i < path_num; i++)
				{
					SEND_MES->path_num = S->path_num;
					strcpy(SEND_MES->DESTINATION_IP, S->DESTINATION_IP);
					SEND_MES->DESTINATION_PORT = S->DESTINATION_PORT;
					strcpy(SEND_MES->SEND_MESSAGE, S->SEND_MESSAGE);
					SEND_MES->TRANSMIT_FLAG = S->TRANSMIT_FLAG;
					SEND_MES->STORE_NUM = S->STORE_NUM;
					for (int j = 0; j < S->STORE_NUM; ++j) 
					{
						strcpy(SEND_MES->STORE_IP[j], 
						S->STORE_IP[j]);
						
						strcpy(SEND_MES->STORE_TIME[j], 
						S->STORE_TIME[j]);
					}
					SEND_MES->receive_time = S->receive_time;
					SEND_MES->TIME_SLOT = S->TIME_SLOT;
					SEND_MES->FLAG = 1;

					printf("W.2-------多--------数组->共享内存内数据后，共享内存数据-----------3\n\n");
					printf("测试path_num : %d\n ", SEND_MES -> path_num);
					//printf("测试FLAG : %d\n ", SEND_MES -> FLAG);
					printf("测试DESTINATION_IP : %s\n ", SEND_MES -> DESTINATION_IP);
					//printf("测试SEND_MESSAGE : %s\n ", SEND_MES -> SEND_MESSAGE);
					printf("-------------------测试完毕-------------------\n\n");
					
					SEND_MES++;
					S++;				
				}
				break;
			}
			else
			{
				SEND_MES->path_num = S->path_num;
				strcpy(SEND_MES->DESTINATION_IP, S->DESTINATION_IP);
				SEND_MES->DESTINATION_PORT = S->DESTINATION_PORT;
				strcpy(SEND_MES->SEND_MESSAGE, S->SEND_MESSAGE);
				SEND_MES->TRANSMIT_FLAG = S->TRANSMIT_FLAG;
				SEND_MES->STORE_NUM = S->STORE_NUM;
				for (int j = 0; j < S->STORE_NUM; ++j) 
				{
					strcpy(SEND_MES->STORE_IP[j], 
					S->STORE_IP[j]);
					
					strcpy(SEND_MES->STORE_TIME[j], 
					S->STORE_TIME[j]);
				}
				SEND_MES->receive_time = S->receive_time;
				SEND_MES->TIME_SLOT = S->TIME_SLOT;				
				SEND_MES->FLAG = 1;
				printf("W.2--------单-------数组->共享内存内数据后，共享内存数据-----------3\n\n");
				printf("测试path_num : %d\n ", SEND_MES -> path_num);
				//printf("测试FLAG : %d\n ", SEND_MES -> FLAG);
				printf("测试DESTINATION_IP : %s\n ", SEND_MES -> DESTINATION_IP);
				//printf("测试SEND_MESSAGE : %s\n ", SEND_MES -> SEND_MESSAGE);
				printf("-------------------测试完毕-------------------\n\n");
				break;
			}
		}
	}
}

void read_shared_memory(struct MESSAGE* re)
{
	//共享内存
	int shmid;

	//将进程脱离共享内存的变量值，保存shmdt结果
	int ret;

	void* shm = NULL;
	struct MESSAGE* SEND_MES;

	//创建共享内存
	shmid = shmget((key_t)1234, sizeof(struct MESSAGE)*MAX_PTHREAD, 0666 | IPC_CREAT);

	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	if(shm != NULL){
		printf("error\n");
	}

	//将共享内存连接到当前进程的地址空间
	shm = shmat(shmid, 0, 0);
	if (shm == (void*)-1)
	{
		perror("shmat");
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	//设置共享内存
	SEND_MES = (struct MESSAGE*)shm;

	// printf("R.1------------共享内存内数据-----------\n");
	// printf("测试path_num : %d\t%d\t%d\n ",SEND_MES->path_num,(SEND_MES + 1)->path_num,(SEND_MES + 2)->path_num);
	// printf("测试DESTINATION_IP : %s\t%s\t%s\n ",SEND_MES->DESTINATION_IP,(SEND_MES + 1)->DESTINATION_IP,(SEND_MES + 2)->DESTINATION_IP);
	// printf("测试SEND_MESSAGE : %s\t%s\t%s\n ",SEND_MES->SEND_MESSAGE,(SEND_MES + 1)->SEND_MESSAGE,(SEND_MES + 2)->SEND_MESSAGE);
	// printf("测试FLAG : %d\t%d\t%d\n ",SEND_MES->FLAG,(SEND_MES + 1)->FLAG,(SEND_MES + 2)->FLAG);	
	// printf("------------共享内存数据存在，测试完毕-----------\n\n");

	while (1) 
    {
		if (SEND_MES->FLAG == 1) 
        {
			if (SEND_MES->path_num != 0 && SEND_MES->path_num < MAX_PTHREAD + 1)
			{
				for(int i = 0; i < SEND_MES->path_num; i++)
				{
					strcpy(re->DESTINATION_IP, SEND_MES->DESTINATION_IP);
					re->DESTINATION_PORT = SEND_MES->DESTINATION_PORT;
					re->path_num = SEND_MES->path_num;
					strcpy(re->SEND_MESSAGE, SEND_MES->SEND_MESSAGE);
					re->TRANSMIT_FLAG = SEND_MES->TRANSMIT_FLAG;
					re->STORE_NUM = SEND_MES->STORE_NUM;
					for (int j = 0; j < SEND_MES->STORE_NUM; ++j) 
					{
						strcpy(re->STORE_IP[j], SEND_MES->STORE_IP[j]);
						strcpy(re->STORE_TIME[j], SEND_MES->STORE_TIME[j]);
					}
					re->receive_time = SEND_MES->receive_time;
					re->TIME_SLOT = SEND_MES->TIME_SLOT;
					SEND_MES->FLAG = 0;

					//FLAG = 1为可发送标志
					re->FLAG = 1;

					// printf("R.2-------多--------共享内存内->数组, 数组数值-----------3\n\n");
					// printf("测试path_num : %d\n ", re -> path_num);
					// printf("测试FLAG : %d\n ", re -> FLAG);
					// printf("测试DESTINATION_IP : %s\n ", re -> DESTINATION_IP);
					// printf("测试SEND_MESSAGE : %s\n ", re -> SEND_MESSAGE);
					// printf("-------------------测试完毕-------------------\n\n");

					//指针移动，循环最后一次执行，指针会指向下一个未知地址。
					SEND_MES++;
					re++;				
				}
				break;
			}
			else
			{
				re->path_num = SEND_MES->path_num;
				strcpy(re->DESTINATION_IP, SEND_MES->DESTINATION_IP);
				re->DESTINATION_PORT = SEND_MES->DESTINATION_PORT;
				strcpy(re->SEND_MESSAGE, SEND_MES->SEND_MESSAGE);
				re->TRANSMIT_FLAG = SEND_MES->TRANSMIT_FLAG;
				re->STORE_NUM = SEND_MES->STORE_NUM;
				for (int j = 0; j < SEND_MES->STORE_NUM; ++j) 
				{
					strcpy(re->STORE_IP[j], SEND_MES->STORE_IP[j]);
					strcpy(re->STORE_TIME[j], SEND_MES->STORE_TIME[j]);
				}
				re->receive_time = SEND_MES->receive_time;
				re->TIME_SLOT = SEND_MES->TIME_SLOT;
				SEND_MES->FLAG = 0;

				//FLAG = 1为可发送标志
				re->FLAG = 1;

				// printf("R.2-------单--------共享内存内->数组, 数组数值-----------3\n\n");
				// printf("测试path_num : %d\n ", re -> path_num);
				// printf("测试FLAG : %d\n ", re -> FLAG);
				// printf("测试DESTINATION_IP : %s\n ", re -> DESTINATION_IP);
				// printf("测试SEND_MESSAGE : %s\n ", re -> SEND_MESSAGE);
				// printf("-------------------测试完毕-------------------\n\n");				
				break;				
			}
		}
		break;
	}
    // 分离共享内存和当前进程
    if ((ret = shmdt(shm)) < 0) 
	{
        perror("shmdt error.");
        exit(1);
    } 
	else 
	{
        //printf("Delete shared-memory\n");
    }
}