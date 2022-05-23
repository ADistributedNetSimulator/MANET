#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "database.c"
#include "message_process.h"

//获取当前虚拟机(docker)的ip
void get_localip(char* local_ip_addr)
{
	register int fd, intrface;
	struct ifreq ifr[32];
	struct ifconf ifc;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > 0)
	{
		ifc.ifc_len = sizeof ifr;
		ifc.ifc_buf = (caddr_t)ifr;
		
        //获取所有接口信息
		if (!ioctl(fd, SIOCGIFCONF, (char*)& ifc)) 
		{
			intrface = ifc.ifc_len / sizeof(struct ifreq);
			while (intrface-- > 0)
			{
				//获取location_ip地址
				if (!(ioctl(fd, SIOCGIFADDR, 
                    (char*)& ifr[intrface])))
				{
					//默认为第一个网卡的ip，docker中默认网卡是etho
					if (strcmp("eth0", 
                        ifr[intrface].ifr_name) == 0)
					{
						sprintf(local_ip_addr, "%s", 
                                inet_ntoa(((struct sockaddr_in*)\
                                (&ifr[intrface].ifr_addr))->sin_addr));
						break;
					}
				}
			}
		}
	}
}

long int get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    //将时间精确到微秒
    long int timestamp = 
		tv.tv_sec * 1000000 +  tv.tv_usec;
    return timestamp;
}

/*
	字符串切割分析
	src 源字符串的首地址(buf的地址)
	separator 指定的分割字符
	dest 接收子字符串的数组
	num 分割后子字符串的个数
*/
void split(char* src, const char* separator, char** dest, int* num)
{
	printf("message_process:  split字符串：%s \t 切割符：%s \t", src, separator); 
	char* pNext;
	int count = 0;

    //如果传入的地址为空或长度为0，直接终止
	if (src == NULL || strlen(src) == 0)
	{  
		return;
    }

    //如未指定分割的字符串，直接终止
	if (separator == NULL || strlen(separator) == 0)
	{  
		return;
    }    

    //必须使用(char *)进行强制类型转换
    //(虽然不写有的编译器中不会出现指针错误)
	pNext = (char*)strtok(src, separator); 
	while (pNext != NULL) 
	{
		*dest++ = pNext;
		++count;
		
        //必须使用(char *)进行强制类型转换
		pNext = (char*)strtok(NULL, separator);  
	}
	*num = count;
	printf("message_process:  切割数量：%d\n\n", *num);
	printf("\n");
}

//将要发送的插入结点信息转换为字符串
//make struct to string
char* join_message(char hostip[], int hostport, int x, int y, int z, int range) 
{
	static char re[MAXDATASIZE];
	int index = 0;
	index = index + sprintf(re + index, "%d/", JOIN);
	index = index + sprintf(re + index, "%s/", hostip);
	index = index + sprintf(re + index, "%d/", hostport);
	index = index + sprintf(re + index, "%d/", x);
	index = index + sprintf(re + index, "%d/", y);
	index = index + sprintf(re + index, "%d/", z);
	index = index + sprintf(re + index, "%d/", range);
	index = index + sprintf(re + index, "%s", "AdHocNode!");
	printf("message_process: send the message %s\n\n",re);
	return re;
}

char* update_message(char hostip[], int hostport, int x, int y, int z, int range) 
{
	static char re[MAXDATASIZE];
	int index = 0;
	index = index + sprintf(re + index, "%d/", UPDATE);
	index = index + sprintf(re + index, "%s/", hostip);
	index = index + sprintf(re + index, "%d/", hostport);
	index = index + sprintf(re + index, "%d/", x);
	index = index + sprintf(re + index, "%d/", y);
	index = index + sprintf(re + index, "%d/", z);
	index = index + sprintf(re + index, "%d/", range);
	index = index + sprintf(re + index, "%s", "AdHocNode!");
	printf("message_process: send the message %s\n\n",re);
	return re;
}

char* delete_message(char hostip[], int hostport) 
{
	static char re[MAXDATASIZE];
	int index = 0;
	index = index + sprintf(re + index, "%d/", DELETE);
	index = index + sprintf(re + index, "%s/",hostip);
	index = index + sprintf(re + index, "%d", hostport);
	printf("message_process: send the message %s\n\n",re);
	return re;
}

char* transmit_message(char hostip[], int hostport, char desip[], int desport) 
{
	static char re[MAXDATASIZE];
	int index = 0;
	index = index + sprintf(re + index, "%d/", SEND);
	index = index + sprintf(re + index, "%s/", hostip);
	index = index + sprintf(re + index, "%d/", hostport);
	index = index + sprintf(re + index, "%s/", desip);
	index = index + sprintf(re + index, "%d", desport);
	printf("message_process: send the message %s\n\n",re);
	return re;
}


//node 节点的分析只承担转发的功能
//为后续功能做准备，例如结点接收到了然后移动的功能
char* data_analysis(char* recvData) 
{
	return recvData;
}


