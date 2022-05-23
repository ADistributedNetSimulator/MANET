#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include "multi_process.c"
#include "manage.c"

int main()
{
	//初始化数据库
	//init_database();

	//进入多进程
	multi_process();
	return 0;
}