#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "message_process.h"

//在写入message的时候先清空前面的
void clear_messagefile(char* path) 
{
	int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0x604);
	if (fd == -1)
	{
		printf("open file error\n");
	}
	close(fd);
}

//文件的写入
void write_messagefile(char* path,char message[]) 
{
	FILE* fp = fopen(path, "a");
	if (fp == NULL)
	{
		printf("open file for write error\n");
		exit(0);
	}
	fprintf(fp, "%s", message);
	fclose(fp);
}

//下面函数在收到来自console节点的转发分析信息时，读取message
char* read_messagefile(char* path) 
{
	FILE* fp = NULL;
	fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("open file for read error\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	char* re = (char*)malloc(sizeof(char) * size+1);

	//读文件，每次读一个，共读size次 
	fread(re, size, 1, fp);
	re[size] = '\0';
	fclose(fp);
	return re;
}


