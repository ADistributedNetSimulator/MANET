#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include "correspond.c"

void multi_process() 
{
	//初始化nodes.txt文件
	init_nodesfile();

	int son1 = fork();
	if (son1 < 0) 
    {
		printf("Fail to fork!\n");
		exit(1);
	}
	else if (son1 == 0) 
    {
		printf("son1 pid is : %d\n", getpid());
		listen_message(CONSOLE_IP, CONSOLE_PORT);
	}
	else 
    {
		printf("father pid is : %d\n", getpid());

		//利用全局变量去将send函数独立于listen之外。
		//类似单片机触发功能。
		while(1)
        {
			struct MESSAGE re = read_shared_memory();
			if (send_message(re.DESTINATION_IP, re.DESTINATION_PORT, re.SEND_MESSAGE))
            {
				printf("send message error!\n");
			}
		}
	}
}