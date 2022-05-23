#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <sys/time.h>
#include "message_process.c"

//socket连接数，100+3
#define BACKLOG 100

int send_message(char ip[], int port, char message[]);

void listen_message(char* server_ip, int server_port) 
{
	//socket配置
	int sockfd, client_fd;
	int sin_size;

	//用来接收缓冲队列
	char buf[MAXDATASIZE];
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;

	//建立socket --
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//设定端口复用，否则频繁关闭会经历time wait
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & opt, sizeof(opt))) 
	{
		perror("setsockopt");
		exit(1);
	}

	/*
	1. AF_INET地址族
	2. 设定端口号(host -> networks)
	3. 32位IPv4地址
	4. 置前8个字节为0
	*/
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(server_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8); 

	if (bind(sockfd, (struct sockaddr*) & my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind error！");
		exit(1);
	}

	//监听socket连接，设置队列中最多拥有连接个数为100+3
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen error！");
		exit(1);
	}

	while (1)
	{
		//记录sockaddr_in结构体所占字节数
		sin_size = sizeof(struct sockaddr_in);

		//accept()缺省是阻塞函数，阻塞到有连接请求为止 --
		if ((client_fd = accept(sockfd, (struct sockaddr*) & remote_addr, &sin_size)) == -1)
		{
			perror("accept error");
		}

		printf("Recv a message from  %s\n\n", inet_ntoa(remote_addr.sin_addr));

		//存储拼接的信息字符串
		char buf_recv[MAXDATASIZE];

		//标记拼接的字符串结束符位置
		int index = 0;

		//标记信息的来源，节点id
		int label;

		//标志位用于跳出循环
		int flag = 1;

		while (flag) 
		{
			recv(client_fd, buf, MAXDATASIZE, 0);
			char* p;
			if ((p = strchr(buf, PACKET_EOF))) 
			{			
				buf[p - buf + 1] = '\0';	//手动设置结束符
			}
			printf("%s\n", buf);

			//recv time 接收时间精确到微秒
			struct timeval tv;
			gettimeofday(&tv, NULL);
			long int recv_time = tv.tv_sec * 1000000 + tv.tv_usec;

			//存放分割后的子字符串
			char* revbuf[MAXDATASIZE]; 
			int num = 0;
			split(buf, "@", revbuf, &num);

			//记录发送时候打的时间戳
			long int send_time;
			if (num == 1) 
			{
				if (strchr(revbuf[0], PACKET_EOF)) 
				{
					//存放分割后的时间字符串
					char* timebuf[MAXDATASIZE]; 
					int time_num;
					split(revbuf[0], "|", timebuf, &time_num);
					char* pEnd;

					//时间解析
					send_time = strtol(timebuf[2], &pEnd, 10);

					//将延迟写入数据库中
					insert_delay(timebuf[1], server_ip, timebuf[0], recv_time - send_time);

					//字符串拼接
					index = index + sprintf(buf_recv + index, "%s", timebuf[0]);
					flag = 0;
				}
			}
			else 
			{
				//存放分割后的时间字符串
				char* timebuf[MAXDATASIZE]; 
				int time_num;
				split(revbuf[0], "|", timebuf, &time_num);
				char* pEnd;

				//时间解析
				send_time = strtol(timebuf[2], &pEnd, 10);

				//将延迟写入数据库中
				insert_delay(timebuf[1], server_ip, timebuf[0], recv_time - send_time);

				index = index + sprintf(buf_recv + index, "%s", timebuf[0]);
				label = atoi(revbuf[1]);
				int i = 1;
				while (1) 
				{
					if (i + 2 < num) 
					{
						if (atoi(revbuf[i + 2]) == label) 
						{
							int time_num;
							char* timebuf[MAXDATASIZE];
							split(revbuf[i+1], "|", timebuf, &time_num);
							char* pEnd;

							//时间解析
							send_time = strtol(timebuf[2], &pEnd, 10);

							//将延迟写入数据库中
							insert_delay(timebuf[1], server_ip, timebuf[0], recv_time - send_time);

							index = index + sprintf(buf_recv + index, "%s", timebuf[0]);
						}
						else 
						{
							break;
						}
						i = i + 2;
					}
					else if (i + 1 < num) 
					{
						if (strchr(revbuf[i+1], PACKET_EOF)) 
						{
							int time_num;
							char* timebuf[MAXDATASIZE]; 
							split(revbuf[i + 1], "|", timebuf, &time_num);
							char* pEnd;

							//时间解析
							send_time = strtol(timebuf[2], &pEnd, 10);

							//将延迟写入数据库中
							insert_delay(timebuf[1], server_ip, timebuf[0], recv_time - send_time);
							index = index + sprintf(buf_recv + index, "%s", timebuf[0]);

							//标志位跳出循环
							flag = 0;
						}
						i = i + 1;
					}
					else 
					{
						break;
					}
				}
			}
			printf("%s\n\n", buf_recv);
		}

		//手动设置数组结束符
		buf_recv[index] = '\0';
		char analysis[MAXDATASIZE];

		printf("Debug--- 插入时延后开始进入数据分析\n\n");
		strcpy(analysis, data_analysis(buf_recv));

		//存放路由分析结果
		char* transmit_analysis[MAXDATASIZE]; 
		int num = 0;
		split(analysis, "/", transmit_analysis, &num);

		if (atoi(transmit_analysis[0]) == TRANSMIT) 
		{	
			//说明不可达
			if (atoi(transmit_analysis[1]) != 1) 
			{ 	
				char message[MAXDATASIZE];
				int index = 0;
				index = index + sprintf(message + index, "%d/", TRANSMIT);
				index = index + sprintf(message + index, "%d/", atoi(transmit_analysis[1]) - 1);
				for (int i = 4; i < num; ++i) 
				{
					index = index + sprintf(message + index, "%s/", transmit_analysis[i]);
				}
				message[index] = '\0';
				struct MESSAGE s;
				//触发send
				strcpy(s.DESTINATION_IP, transmit_analysis[2]);
				s.DESTINATION_PORT = atoi(transmit_analysis[3]);
				strcpy(s.SEND_MESSAGE, message);

				//启动send
				s.FLAG = 1;
				write_shared_memory(s);
			}
		}
		else 
		{
			int r = return_index(inet_ntoa(remote_addr.sin_addr));
			if (r != -1) 
			{
				insert_update_received(r, analysis);
			}
			printf("%s\n", analysis);
		}
	}
	close(client_fd);
}

int send_message(char ip[], int port, char message[]) 
{
	int sockfd;
	struct hostent* host = gethostbyname(ip);
	struct sockaddr_in serv_addr;

	//build socket --
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket create error！");
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *((struct in_addr*)host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	//设置端口复用
	int on = 1;
	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
	{
		perror("setsockopt failed");
		exit(1);
	}

	//request --
	if (connect(sockfd, (struct sockaddr*) & serv_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("connect error！");
		close(sockfd);
		return 1;
	}
	int length = strlen(message);
	int i = 0;

	//存放尾部帧
	char tail_frame[5];

	sprintf(tail_frame, "%c%s%c", FRAME_SIZE_EOF, LABEL, FRAME_SIZE_EOF);
	int tail_length = strlen(tail_frame);
	while (length != 0) 
	{
		//发送时间戳,精确到微秒级别
		struct timeval tv;
		gettimeofday(&tv, NULL);
		char time_frame[40];
		sprintf(time_frame, "|%s|%ld", CONSOLE_IP, tv.tv_sec * 1000000 + tv.tv_usec);
		int time_length = strlen(time_frame);
		int spare_length = FRAME_SIZE - time_length - tail_length;
		if (length > spare_length) 
		{
			char limitation_size[FRAME_SIZE] = "";
			strncpy(limitation_size, message + spare_length * i, spare_length);
			strcat(limitation_size, time_frame);
			strcat(limitation_size, tail_frame);
			length = length - spare_length;
			i++;
			printf("%s\n", limitation_size);
			if (send(sockfd, limitation_size, strlen(limitation_size), 0) == -1) 
			{
				perror("send error！");
				close(sockfd);
				return 1;
			}
		}
		else 
		{
			char limitation_size[length + time_length + 1];
			strncpy(limitation_size, message + spare_length * i, length);

			//这里需要先\0然后再拼接否则会识别错误形成@符号
			limitation_size[length] = '\0';
			strcat(limitation_size, time_frame);
			limitation_size[length + time_length] = PACKET_EOF;
			length = 0;
			if (send(sockfd, limitation_size, strlen(limitation_size), 0) == -1) 
			{
				perror("send error！");
				close(sockfd);
				return 1;
			}
		}
		usleep(TIME_SOLT); 
	}
	close(sockfd);
	return 0;
}
