#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <pthread.h>
#include "transfer.c"
#include "dynamic_update.c"
#include "multi_path.c"

/*
	grandson1: 多路径传输
	监听多路径传输
	如监听到多个数据，则读入相应数据
	循环写入共享内存

	grandson2: 
	读共享内存
	发送信息

	son1: 位置更新信息发给console
	监听节点状态、带宽
	更新节点状态，如位置/油量
	封装节点更新的信息，
	准备发送给console节点
	写入共享内存

	son2: 
	接收信息,分析处理buf，
	若是转发包
	写入共享内存

	father：路由
	监听结点要发送的消息（如指令信息等）、带宽，
	封装源与目的节点ip与port信息，
	准备发送给console节点
	写入共享内存
*/

void multi_process()
{
	//获取本机ip
	memset(hostip, 0, 20);
	get_localip(hostip);
    
	pid_t son1, son2, grandson1, grandson2;

    //创建子进程son1
	son1 = fork();

    //出错处理
	if (son1 == -1)
    {
		printf("Fail to fork son1!\n");
		exit(1);
	}

	else if (son1 == 0) 
    {
        //创建son1的子进程grandson1
		grandson1 = fork();

        //出错处理
		if (grandson1 < 0) 
        {
			printf("Fail to fork grandson!\n");
			exit(1);
		}


		//多线程创建多个socket
		else if (grandson1 == 0) 
        {
			printf("multi_path_transfer : grandson1 pid is : %d\n\n", getpid());	
			while(1)
			{
				multi_path_mes(hostip); 
				sleep(10);	//每隔10秒更新一次
			}
		} 
		else 
        {
			printf("dynamic_update : son1 pid is : %d\n\n", getpid());
			dynamic_update(hostip);
		}
	}
	else 
    {
		son2 = fork();
		if (son2 < 0) 
        {
			printf("Fail to fork son2!\n");
			exit(1);
		}
		else if (son2 == 0)
		{ 
			//创建son2的子进程grandson2
			grandson2 = fork();

			//出错处理
			if (grandson2 < 0) 
			{
				printf("Fail to fork grandson!\n");
				exit(1);
			}
			else if (grandson2 == 0) 
			{
				printf("grandson2 pid is : %d\n\n", getpid());
				/*
				利用全局变量去将send函数独立于listen之外，
				类似单片机触发功能。
				*/
				while (1)
				{		
					//1. 得到要发送的多路径信息（结构体数组）,最多五个路径
					struct MESSAGE send_mes[MAX_PTHREAD];
					read_shared_memory(send_mes);

					// printf("测试是否将内存数据读出来：send_mes[0].path_num:%d",send_mes[0].path_num);
					// printf("测试是否将内存数据FLAG读出来：send_mes[0].FLAG:%d\n",send_mes[0].FLAG);

					if(send_mes[0].FLAG == 1 && send_mes[0].path_num > 0)
					{
						//2. 先确定线程数量，ret[i]用于接收子线程的返回值
						int pthread_amount = send_mes[0].path_num;
						int *ret[pthread_amount];
						printf("线程数量：%d，获取成功\n",pthread_amount);

						//3. 创建线程pthread_t数组
						pthread_t tid[pthread_amount];

						//4. 循环创建多线程
						for (int i = 0; i < pthread_amount; i++) 
						{
							printf("创建第%d个多线程\n",i);
							int ret = pthread_create(&tid[i], NULL, send_message, (void *)&send_mes[i]);
							if(ret != 0)
							{
								//strerror是将错误号转为字符串
								fprintf(stderr, "pthread_create error: %s\n",strerror(ret));
								exit(1);
							}
						}

						//5. 循环回收子进程
						for (int i = 0; i < pthread_amount; i++) {
							pthread_join(tid[i], (void**)&ret[i]);
							printf("--------%d 's ret = %d\n", i, (int)ret[i]);
						}

						//6. 置空结构体数组
						memset(send_mes,0,sizeof(struct MESSAGE)*MAX_PTHREAD);
					}
					else if(send_mes[0].FLAG == 1 && send_mes[0].path_num == 0)
					{
						//printf("---单径准备进入send_message---\n");
						send_message((void *)&send_mes[0]);

						// 置空结构体数组
						memset(send_mes,0,sizeof(struct MESSAGE)*MAX_PTHREAD);
					}
					else
					{
						memset(send_mes,0,sizeof(struct MESSAGE)*MAX_PTHREAD);
						//printf("----数组还是原样，此时啥也不用发---\n");
					}

					//循环的时间设为0.1秒
					usleep(100000);

				}
			} 
			else 
			{
				printf("receive_message : son2 pid is : %d\n\n", getpid());
				receive_message(hostip, HOSTPORT);
			}
		}
		else 
        {
			//这里是father模块，监听信息指令，如发送移动指令等
			printf("listen_message : father pid is : %d\n\n", getpid());

			//temp记录信息被读几次
			int temp = 0;

			//循环监听结点转发消息的内容，同时监听带宽，并写入共享内存中
			while (1) 
			{
				char ip[20];
				int port;

				//可以每隔5s监听一次
				sleep(5);

				//将从数据库中读取的信息写入到message.txt里
				listen_message(ip, &port,&temp);

				struct MESSAGE *s = (struct MESSAGE*)malloc(sizeof(struct MESSAGE)*1);

				strcpy(s->SEND_MESSAGE, transmit_message(hostip, HOSTPORT, ip, port));

				strcpy(s->DESTINATION_IP, CONSOLE_IP);
				s->DESTINATION_PORT = CONSOLE_PORT;

				//这些都不是转发，所以也不需要时间戳的赋值
				s->TRANSMIT_FLAG = 0;
				
				//计算时间戳
				s->TIME_SLOT = FRAME_SIZE / listen_bandwidth();
				write_shared_memory(s);	
				free(s);
			}
		}
	}
}
