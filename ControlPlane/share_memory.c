#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "share_memory.h"

void write_shared_memory(struct MESSAGE S) 
{
	//共享内存
	int shmid;
	void* shm = NULL;
	struct MESSAGE* SEND_MES;

	//创建共享内存
	shmid = shmget((key_t)1234, 
					sizeof(struct MESSAGE), 
					0666 | IPC_CREAT);

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

	while (1) 
	{
		if (SEND_MES->FLAG == 0) 
		{
			strcpy(SEND_MES->DESTINATION_IP, 
				   S.DESTINATION_IP);

			SEND_MES->DESTINATION_PORT = S.DESTINATION_PORT;
			strcpy(SEND_MES->SEND_MESSAGE, 
				   S.SEND_MESSAGE);

			SEND_MES->FLAG = S.FLAG;
			break;
		}
	}
}

struct MESSAGE read_shared_memory() 
{
	//共享内存
	int shmid;
	void* shm = NULL;
	struct MESSAGE* SEND_MES;

	//创建共享内存
	shmid = shmget((key_t)1234, 
		sizeof(struct MESSAGE), 
			0666 | IPC_CREAT);

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
	struct MESSAGE re;

	while (1) {
		if (SEND_MES->FLAG == 1) 
		{
			strcpy(re.DESTINATION_IP, 
				   SEND_MES->DESTINATION_IP);
			re.DESTINATION_PORT = SEND_MES->DESTINATION_PORT;
			strcpy(re.SEND_MESSAGE, 
				   SEND_MES->SEND_MESSAGE); 
			SEND_MES->FLAG = 0;
			break;
		}
	}
	return re;
}

