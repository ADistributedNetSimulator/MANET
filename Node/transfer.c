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
#include <pthread.h>
#include "message_process.c"
#include "share_memory.c"
#include "poor_channel.c"

#define BACKLOG 100

void* send_message(void* arge); 	//函数声明

//线程1处理函数
void *multi_thread_receive_process(void *cli_info)
{
	int recv_ret = -1;

	//先获取传入的客户端的信息，如描述符clifd,ip,port
	client_information client_info = *((client_information *)cli_info);  

	//用来接收消息，缓冲队列 
	char buf[MAXDATASIZE];

	//存放一个拼接的完整信息，不含时延信息
	char message_get[MAXDATASIZE];

	// 用来拼接message_get数组字符串，
	// 并且标识字符个数，方便手动设置结束符
	int index = 0;  

	//标志信息来源哪个节点
	int label; 

	//循环标志位
	int flag = 1;

	//end记录
	int end = 0;

	//接收信息的时间信息
	long int receive_time;

	// 模拟一个假的队列
	// 用来存储源时间戳，每一次都存储，
	// 只在send_message中判断是否使用		
	char STORE_TIME[MAXDATASIZE][TIME_LENGTH];

	//用来存储对应IP，和上面的TIME1:1对应
	char STORE_IP[MAXDATASIZE][TIME_LENGTH];

	//此循环用来获取时延，以及分割出相应的信息字符串		
	while (flag) 
	{
    	// 将接收缓冲区buffer清空
    	bzero(&buf,sizeof(buf));

        // 接收服务器信息
        printf("R2.1------Receiving messages from client: %s .\n", client_info.client_ip);
		
		//接收信息
		recv_ret = recv(client_info.client_fd, buf, MAXDATASIZE, 0);

		if(recv_ret < 1)
		{
			printf("客户端 %s 断开了连接,并退出\n", client_info.client_ip);

			//关闭accept文件描述符client_info.client_fd
			close(client_info.client_fd);    

			//当前服务的客户端数量减一  
			con_client_count--;   

			//退出当前的线程
			pthread_exit(NULL);  
		}

		//此处记录接到信息的接收时间，并转换为微妙
		receive_time = get_timestamp();

		printf("R2.2------The time of message received: %ld\n\n", receive_time);

		//p用来记录终止符#位置
		char* p;

		//5/0/my|172.20.10.7|1633268445479796#	
		//若接受到终止符'#',手动设置结束符
		if ((p = strchr(buf, PACKET_EOF))) 
		{
			buf[p - buf + 1] = '\0';
		}

		printf("R2.3------received_buf :  %s\n\n",buf);

		//存放分割后的子字符串
		char* buf_split1[MAXDATASIZE]; 
		int num= 0;
		split(buf, "@", buf_split1, &num);

		//清空接收缓存区
		//memset(buf , 0 ,sizeof(buf));  

		//记录发送时候打的时间戳
		long int send_time;

		if (num == 1 && strchr(buf_split1[0], PACKET_EOF)) 
		{
			int split_num2;
			//用来存放分割后的时间字符串
			char* buf_split2[MAXDATASIZE]; 

			split(buf_split1[0], "|", buf_split2, &split_num2);

			//时间解析
			char* pEnd;
			send_time = strtol(buf_split2[2], &pEnd, 10);
			insert_delay(buf_split2[1], hostip, buf_split2[0], receive_time - send_time);
			
			// 把时延存起来
			// 用来存储源时间戳
			strcpy(STORE_TIME[end], buf_split2[2]);

			//用来存储对应IP，和上面的TIME1:1对应
			strcpy(STORE_IP[end], buf_split2[1]);
			end++;

			//拼接字符串message_get
			index = index + sprintf(message_get + index, "%s", buf_split2[0]);
			flag = 0;
		}
		else 
		{
			int split_num2;

			//用来存放分割后的时间字符串
			char* buf_split2[MAXDATASIZE]; 

			split(buf_split1[0], "|", buf_split2, &split_num2);

			//时间解析
			char* pEnd;
			send_time = strtol(buf_split2[2], &pEnd, 10);

			//这里插入时延可以省略，因为不是来自一个节点的最后一个信息，没有必要全部打时间戳
			//insert_delay(buf_split2[1], hostip, buf_split2[0], receive_time - send_time);

			strcpy(STORE_TIME[end], buf_split2[2]);
			strcpy(STORE_IP[end], buf_split2[1]);
			end++;

			index = index + sprintf(message_get + index, "%s", buf_split2[0]);

			//比如lable = 0
			label = atoi(buf_split1[1]);
			int i = 1;

			//根据num来判断，本系统num = 1 或者 2
			while (1) 
			{
				if (i + 2 < num) 
				{					
					if (atoi(buf_split1[i + 2]) == label) 
					{
						int split_num2;

						//用来存放分割后的时间字符串
						char* buf_split3[MAXDATASIZE]; 

						split(buf_split1[i + 1], "|", buf_split3, &split_num2);

						//时间解析
						char* pEnd;
						send_time = strtol(buf_split3[2], &pEnd, 10);
						//insert_delay(buf_split3[1], hostip, buf_split3[0], receive_time - send_time);

						strcpy(STORE_TIME[end], buf_split3[2]);
						strcpy(STORE_IP[end], buf_split3[1]);
						end++;
						index = index + sprintf(message_get + index, "%s", buf_split3[0]);
					}
					else 
					{
						break;
					}
					
					//参照下面else if ，此处需要一个flag = 0吗
					i = i + 2;
				}
				else if (i + 1 < num && strchr(buf_split1[i + 1], PACKET_EOF)) 
				{
					int split_num2;

					//用来存放分割后的时间字符串
					char* buf_split3[MAXDATASIZE]; 

					split(buf_split1[i + 1], "|", buf_split3, &split_num2);

					//时间解析
					char* pEnd;
					send_time = strtol(buf_split3[2], &pEnd, 10);
					insert_delay(buf_split3[1], hostip, buf_split3[0], receive_time - send_time);

					strcpy(STORE_TIME[end], buf_split3[2]);
					strcpy(STORE_IP[end], buf_split3[1]);
					end++;
					index = index + sprintf(message_get + index, "%s", buf_split3[0]);

					//标志位置0，跳出循环
					flag = 0;
					i = i + 1;
				}
				else 
				{
					break;
				}
			}
		}
	}

	message_get[index] = '\0';

	printf("R2.4------Get message %s and Start analysis\n\n", message_get);


	//存放路由转发的信息
	char analysis[MAXDATASIZE];
	strcpy(analysis, data_analysis(message_get));

	char* transmit_analysis[MAXDATASIZE]; 
	int num = 0;
	split(analysis, "/", transmit_analysis, &num);

	//调用转发，三种情况：源节点、中间节点、目的节点
	if (atoi(transmit_analysis[0]) == TRANSMIT) 
	{
		//transmit_flag = 1 意味是中间或者目的节点,设为0标识本节点为源节点
		int transmit_flag = 1;

		//当这个包是转发包时候，同时是来自于Monitor的时候，这就说明现在是源结点
		//如果发送节点的ip与服务端ip相等，说明不是中间结点，而是源结点	
		//此处读取信息完全可以改，如果各个节点都是上帝	
		if (!strcmp(client_info.client_ip, CONSOLE_IP)) 
		{
			transmit_flag = 0;
		}
		//此处信息拼接可以简化，将5/0/my/ 中最后一个/去掉
		if (atoi(transmit_analysis[1]) != 0) 
		{
			int index1 = 0;
			char message[MAXDATASIZE];
			index1 = index1 + sprintf(message + index1, "%d/", TRANSMIT);
			index1 = index1 + sprintf(message + index1, "%d/", atoi(transmit_analysis[1])-1);
			for (int i = 4; i < num; ++i) 
			{
				index1 = index1 + sprintf(message + index1, "%s/", transmit_analysis[i]);
			}

			//表示本节点是源节点要发送信息
			if (transmit_flag == 0) 
			{
				char mes[MAXDATASIZE];
				strcpy(mes, read_messagefile("message.txt"));
				index1 = index1 + sprintf(message + index1, "%s/", mes);
			}
			printf("R2.5------Forward message: %s , write shared memory\n", message);
			
			//触发send
			struct MESSAGE s[0];
			strcpy(s->DESTINATION_IP, transmit_analysis[2]);
			s->DESTINATION_PORT = atoi(transmit_analysis[3]);
			strcpy(s->SEND_MESSAGE, message);
			s->TRANSMIT_FLAG = transmit_flag;

			//时间戳传到另一个进程中
			s->STORE_NUM = end;
			for (int i = 0; i < end; ++i) 
			{
				strcpy(s->STORE_IP[i], STORE_IP[i]);
				strcpy(s->STORE_TIME[i], STORE_TIME[i]);
			}
			//启动send
			s->FLAG = 1;
			s->receive_time = receive_time;

			s->TIME_SLOT = FRAME_SIZE / 5;
			//s->TIME_SLOT = FRAME_SIZE / listen_bandwidth();
			write_shared_memory(s);
		}
		else 
		{		
			// 若不是源结点，说明信息发到目的结点，
			// 如果是从源结点来的，是失败的
			if (transmit_flag != 0) 
			{
				//insert_recv_message(transmit_analysis[2]);
				printf("rR2.6------The destination node received the information successfully ！%s\n", transmit_analysis[2]);
			}
		}
	}
	//为后续加功能留接口
	else 
	{
		printf("%s\n", analysis);
	}
	printf("R2.7------finish this function\n");
	//关闭accept文件描述符clifd
	close(client_info.client_fd);
	return NULL;
}


/*函数解释
while(1)
{
	此循环是不停的等待各节点连接

	while(flag)
	{
		此循环是从一个节点不停接收帧，然后将消息拼接

		if（如果这是最后一条信息）
		else
		{
			while(1)
			{
				此循环是判断来自一个节点传输了几个数据帧
				根据num ，是奇数
				if - else if -else
			}
		}
	}
	if(){} --- 判断信息要发往哪，源节点、中间节点、目的节点
}
*/

void receive_message(char* hostip, int hostport) 
{
	//创建套接字
	int sockfd, client_fd;
	int sin_size;

	//接收返回值的变量
	int ret = -1;

	con_client_count = 0;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	
    //建立socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//设定端口复用，否则频繁关闭会经历time wait
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, 
		SO_REUSEADDR, &opt, sizeof(opt))) 
	{
		perror("setsockopt");
		exit(1);
	}

    //AF_INET地址族
	my_addr.sin_family = AF_INET;

    //设定端口号(host -> networks)
	my_addr.sin_port = htons(hostport);

    //32位IPv4地址
	my_addr.sin_addr.s_addr = INADDR_ANY;

    //置前8个字节为0	
	bzero(&(my_addr.sin_zero), 8); 	

	//绑定
	ret = bind(sockfd, (struct sockaddr*) & my_addr, sizeof(struct sockaddr));
	if (ret == -1)
	{
		perror("bind error！");
		exit(1);
	}

    //监听连接，设置队列中最多拥有连接个数为100 +3 
	ret = listen(sockfd, BACKLOG);
	if (ret == -1)
	{
		perror("listen error!");
		exit(1);
	}

	//循环等待各节点连接，实现两功能：写入时延与信息分析
	while (1)
	{
        //记录sockaddr_in结构体所占字节数
		sin_size = sizeof(struct sockaddr_in);
		
		//等待连接
		if ((client_fd = accept(sockfd, (struct sockaddr*) & remote_addr, &sin_size)) == -1) 
		{
			perror("accept error");
		}

		//放在函数内
		// //测试是否链接成功
		// printf("receive_message: recv a message from  %s\n\n", inet_ntoa(remote_addr.sin_addr));

        //获取新客户端的网络信息 
        client_information client_info;
        client_info.client_fd = client_fd;
        strcpy(client_info.client_ip, inet_ntoa(remote_addr.sin_addr));
        client_info.client_port = remote_addr.sin_port;

        con_client_count++;
        printf("R1------Number of users: %d\n", con_client_count);

        /* 为新连接的客户端开辟线程 multi_thread_receive_process，该线程用来循环接收客户端的数据 */
        pthread_t thr_receive = 0;
        pthread_create(&thr_receive, NULL, multi_thread_receive_process, &client_info);

        printf("R3------A thread has been created for the user.\n");
 
        /* 让进程休息0.1秒 */
        usleep(100000);		

	}
    char *s = "Safe exit from the receive process.";
    pthread_exit(s);

    printf("R4------Waiting for child thread to exit.\n");

	//关闭socket文件描述符
	close(sockfd);     
}

//MESSAGE s 用来封装发送信息  
void* send_message(void* arge) 
{
	printf("thread: pid = %d, tid = %lu\n", getpid(), pthread_self());
	struct MESSAGE* m = (struct MESSAGE*) arge;
	printf("S.1------------测试send_message中可以正常读取结构体数组的值-------------1\n");
	// printf("m->FLAG:%d\n",m->FLAG);	
	//printf("m->DESTINATION_IP:%s\n",m->DESTINATION_IP);
	// printf("m->DESTINATION_PORT:%d\n",m->DESTINATION_PORT);
	// printf("m->SEND_MESSAGE:%s\n",m->SEND_MESSAGE);
	// printf("m->TRANSMIT_FLAG:%d\n",m->TRANSMIT_FLAG);
	// printf("m->TIME_SLOT:%f\n",m->TIME_SLOT);
	//printf("-----------------------------------------------------------------\n\n");

	int sockfd;
	struct hostent* host = gethostbyname(m->DESTINATION_IP);

	struct sockaddr_in serv_addr;

	//build socket --
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("send_message: socket create error！\n");
		return NULL;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m->DESTINATION_PORT);
	serv_addr.sin_addr = *((struct in_addr*)host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	int on = 1;
	if ((setsockopt(sockfd, 
		SOL_SOCKET, 
		SO_REUSEADDR,
		&on, sizeof(on))) < 0)
	{
		perror("send_message: setsockopt failed\n");
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr*) & serv_addr, 
        sizeof(struct sockaddr)) == -1) 
	{
		perror("send_message: connect error!\n");
		close(sockfd);
		//return 1;
		return NULL;
	}

	int message_length = strlen(m->SEND_MESSAGE);
	printf("S.2-------message_length:%d----\n",message_length);
	int i = 0;
	char tail_frame[5];
	sprintf(tail_frame, "%c%s%c", 
    	FRAME_SIZE_EOF, LABEL, FRAME_SIZE_EOF);
	int tail_length = strlen(tail_frame);

	//时间戳下标
	int begin = 0;

	//获取本地ip
	char hostip[20] = { 0 };
	get_localip(hostip);

	//判断是否模拟噪声信道，此处在获取时间后进行，会影响计算传输时间的准确性
	//int poor_channel_state = poor_channel_judge();
	int poor_channel_state = 0;
	printf("S.3-------poor_channel_state:%d----\n",poor_channel_state);

	while (message_length != 0) 
    {
		//发送时间戳,精确到微秒级别
		char time_frame[40];

		// 
		// 	若是转发数据，从中按顺序取，
		// 	因信息经过解析之后会变短，然后清空队列
		// 	若不是，说明是源结点，附上源地址和时间戳
        // 
		if (m->TRANSMIT_FLAG)
        {
			printf("S.4-------：This message is forwarding message.\n\n");
			sprintf(time_frame, "|%s|%s",
            m->STORE_IP[begin], m->STORE_TIME[begin]);
			begin++;
		}
		else 
        {
			struct timeval tv;
			gettimeofday(&tv, NULL);
			sprintf(time_frame, "|%s|%ld", 
            hostip,tv.tv_sec * 1000000 + tv.tv_usec);

			//此处大概算出信息处理时延大小，后续可以存数据库，当节点接收信息算时延时，可以读取并细化处理
			long int send_time = tv.tv_sec * 1000000 + tv.tv_usec;
			long int process_delay = send_time - m->receive_time;
			if(process_delay < 1000000){
				printf("send_message m->receive_time : %ld \n\n", m->receive_time);
				printf("send_message ：process_delay : %ld \n\n", process_delay);
			}
		}

		int time_length = strlen(time_frame);
		int spare_length = 
        	FRAME_SIZE - time_length - tail_length;

		printf("S.5----------message_length <> spare_length- ：判断一个帧空间是否可以存储要发的信息\n");
		if (message_length > spare_length) 
        {
			char limitated_datapacket[FRAME_SIZE] = "";

			strncpy(limitated_datapacket, 
            		m->SEND_MESSAGE + spare_length * i, 
            		spare_length);

			strcat(limitated_datapacket, time_frame);
			strcat(limitated_datapacket, tail_frame);
			message_length = message_length - spare_length;
			i++;

			if (poor_channel_state == 1)
			{
				int num = get_rand_number();
				if(num == 1)
				{
					close(sockfd);
					//return 1;
					return NULL;
				}  
				else
				{
					usleep(get_rand_delay());
					if (send(sockfd, limitated_datapacket, strlen(limitated_datapacket), 0) == -1) 
					{
						perror("send_message: send error！\n");
						close(sockfd);
						//return 1;
						return NULL;
					}
				}
			}
			else
			{
				if (send(sockfd, limitated_datapacket, strlen(limitated_datapacket), 0) == -1) 
				{
					perror("send_message: send error！\n");
					close(sockfd);
					return NULL;
				}

				//测试信息是否发送成功
				printf("S.6-------successful send! --- %s\n\n", limitated_datapacket);
			}
		}
		else
        {
			char limitated_datapacket[message_length + time_length + 1];

			//frame size
			strncpy(limitated_datapacket, 
            		m->SEND_MESSAGE + spare_length * i, 
            		message_length); 
			
			//这里需要先\0然后再拼接否则会识别错误形成@符号
			limitated_datapacket[message_length] = '\0';
			strcat(limitated_datapacket, time_frame);
			limitated_datapacket[message_length + time_length] = PACKET_EOF;

			//这里再加上结束符，因为出现乱码
			limitated_datapacket[message_length + time_length + 1] = '\0';		
			message_length = 0;
			if (poor_channel_state == 1)
			{
				int num = get_rand_number();
				if(num == 1)
				{
					close(sockfd);
					return NULL;        
				}  
				else
				{
					usleep(get_rand_delay());					
					if (send(sockfd, limitated_datapacket, strlen(limitated_datapacket), 0) == -1) 
					{
						perror("send error！\n");
						close(sockfd);
						return NULL;
					}
				}
			}
			else
			{
				if (send(sockfd, limitated_datapacket, strlen(limitated_datapacket), 0) == -1) 
				{
					perror("send_message: send error！\n");
					close(sockfd);
					return NULL;
				}
				//测试信息是否发送成功
				printf("S.6------- successful send! --- %s\n\n", limitated_datapacket);

			}
		}
		//设置时间戳,先给注释
		usleep(m->TIME_SLOT); 
	}

	//调用完转发需要清空STORE_TIME；
	begin = 0;
	close(sockfd);
	return NULL;
}
