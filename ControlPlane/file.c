#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "message_process.h"


/*
init_nodestxfile主要创建nodes.txt，
当该文件存在则清空内容, 
用于存储所有加入组网的node结点,
还有延迟统计
*/
void init_nodesfile()
{
	int fd = open("nodes.txt", 
    O_RDWR | O_CREAT | O_TRUNC, 
    0x604);

	if (fd == -1)
	{
		printf("open file error\n");
	}
	close(fd);
}

//将NODES结点信息写入文件中
void write_nodesfile() 
{
	FILE* fp = NULL;
	fp = fopen("nodes.txt", "w");
	if (fp == NULL)
	{
		printf("open file for write error\n");
		exit(0);
	}
	fwrite(NODES, sizeof(struct node), MAX_NODES_NUMBER, fp);
	fclose(fp);
}

//将文件信息读入NODES数组中。
void read_nodesfile()
{
	FILE* fp = NULL;
	fp = fopen
	("nodes.txt", "r");
	if (fp == NULL)
	{
		printf("open file for read error\n");
		exit(0);
	}
	fread(NODES, sizeof(struct node), MAX_NODES_NUMBER, fp);
	fclose(fp);
}
