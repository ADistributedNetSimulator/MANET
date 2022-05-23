#include<stdio.h>
#include<math.h>
#include"file.c"

void show_all_nodes();
int return_index(char ip[]);

//每进行一次对结点的操作，都维护一次network里的邻接表等。
void subnet_divided_range() 
{
	int i, j;
	for (i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) 
		{
			for (j = i; j < MAX_NODES_NUMBER; ++j) 
			{
				if (NODES[j].state == 1) 
				{
					if (i == j) 
					{
						REACH_TABLE[i][j] = 0;
						DISTANCE_TABLE[i][j] = 0;
					}
					else 
					{
						int distance = (int)sqrt(
							(NODES[i].x - NODES[j].x) * (NODES[i].x - NODES[j].x) +
							(NODES[i].y - NODES[j].y) * (NODES[i].y - NODES[j].y) +
							(NODES[i].z - NODES[j].z) * (NODES[i].z - NODES[j].z));
						DISTANCE_TABLE[i][j] = distance;
						DISTANCE_TABLE[j][i] = distance;
						if (distance <= NODES[i].range) 
						{
							REACH_TABLE[i][j] = 1;
						}
						else 
						{
							REACH_TABLE[i][j] = 0;
						}
						if (distance <= NODES[j].range) 
						{
							REACH_TABLE[j][i] = 1;
						}
						else 
						{
							REACH_TABLE[j][i] = 0;
						}
					}
				}
			}
		}
		else if (NODES[i].state == 0) 
		{
			int k;
			for (k = 0; k < MAX_NODES_NUMBER; ++k) 
			{
				REACH_TABLE[i][k] = -1;
				REACH_TABLE[k][i] = -1;
			}
		}
	}
	write_nodesfile();
	show_all_nodes();
}

/*
	假设依据油量划分子网
	设定油量大于4则形成一个子网，
	1-4则视为不可达，小于1视节点状态为0
*/
void update_network_oil() 
{
	int i, j;
	for (i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) {
			for (j = i; j < MAX_NODES_NUMBER; ++j) 
			{
				if (NODES[j].state == 1) 
				{
					if (i == j)
					{
						REACH_TABLE[i][j] = 0;
						OIL_TABLE[i][j] = 0;
					}
					else 
					{
						OIL_TABLE[i][j] = NODES[i].oil;
						OIL_TABLE[j][i] = NODES[j].oil;
						if (NODES[i].oil > 4 || NODES[j].oil > 4) 
						{
							REACH_TABLE[i][j] = 1;
							REACH_TABLE[j][i] = 1;
						}
						else if(NODES[i].oil < 1 && NODES[j].oil < 1)
						{
							REACH_TABLE[i][j] = -1;
							REACH_TABLE[j][i] = -1;							
						}
						else
						{
							REACH_TABLE[i][j] = 0;
							REACH_TABLE[j][i] = 0;								
						}
					}
				}
			}
		}
		else if (NODES[i].state == 0) 
		{
			int k;
			for (k = 0; k < MAX_NODES_NUMBER; ++k) 
			{
				REACH_TABLE[i][k] = -1;
				REACH_TABLE[k][i] = -1;
			}
		}
	}
	write_nodesfile();
	show_all_nodes();
}


/*
添加结点到组网中
*/
char* append_node(struct node n) 
{
	int i;
	char* re;
	read_nodesfile();
	for (i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state && !strcmp(NODES[i].ip, n.ip)) 
		{
			re = "The node for this ip already exists!\n";
			return re;
		}
	}
	for (i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 0) 
		{
			//将结点信息写到数据库中
			insert_ip_port(i, n.ip, n.port);
			n.node_id = i;
			n.state = 1;

            //此处将结构体n的值写入NODES数组中
			NODES[i] = n;
			subnet_divided_range();
			re = "Append node successful!\n";
			return re;
		}
	}
	re = "Exceed the maximum number limit!\n";
	return re;
}
/**
 * 删除组网中的结点
 */
char* delete_node(char* ip, int port) 
{
	char* re;
	read_nodesfile();
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) 
		{
			if (!strcmp(NODES[i].ip, ip) && NODES[i].port == port) 
			{
				NODES[i].state = 0;
				subnet_divided_range();
				delete_node_from_location(return_index(ip));
				re = "Delete node successful!\n";
				return re;
			}
		}
	}
	re = "The node for this ip and port does not exist!\n";
	return re;
}

//DEBUG
char* update_node(struct node n) 
{
	char* re;
	read_nodesfile();
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) 
		{
			if (!strcmp(NODES[i].ip, n.ip) && NODES[i].port == n.port) 
			{
				NODES[i].x = n.x;
				NODES[i].y = n.y;
				NODES[i].z = n.z;
				subnet_divided_range();
				re = "Update node successful!\n";
				return re;
			}
		}
	}
	re = "The node for this ip and port does not exist!\n";
	return re;
}


//DEBUG,输出所有在线结点信息
void show_all_nodes() 
{
	sleep(15);
	int i;
	printf("ip\t port\t node_id \t x\t y\t z\t range\t description\t \n");
	for (i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (NODES[i].state == 1) 
		{
			printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t\n\n",
					NODES[i].ip,NODES[i].port,NODES[i].node_id,
					NODES[i].x,NODES[i].y,NODES[i].z,
					NODES[i].range,
					NODES[i].description);
			// printf("ip=%s\t", NODES[i].ip);
			// printf("port=%d\n", NODES[i].port);
			// printf("node_id=%d\n", NODES[i].node_id);
			// printf("x=%d\n", NODES[i].x);
			// printf("y=%d\n", NODES[i].y);
			// printf("z=%d\n", NODES[i].z);
			// printf("range=%d\n", NODES[i].range);
			// printf("description=%d\n", NODES[i].description);
		}
	}
	
	printf("-----REACH_TABLE-----\n");
	int j;
	for (i = 0; i < MAX_NODES_NUMBER; ++i)
	{
		for (j = 0; j < MAX_NODES_NUMBER; ++j) 
		{
			printf("%d ", REACH_TABLE[i][j]);
		}
		printf("\n");
	}
	printf("-----REACH_TABLE-----\n\n");
}


//根据ip返回下标的index值
int return_index(char ip[])
{
	read_nodesfile();
	for (int i = 0; i < MAX_NODES_NUMBER; ++i) 
	{
		if (!strcmp(NODES[i].ip, ip)) 
		{
			return i;
		}
	}
	return -1;
}
