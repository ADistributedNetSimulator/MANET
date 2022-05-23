#pragma once
struct MESSAGE 
{	
	int FLAG; //标识是否可读
	char DESTINATION_IP[20];
	int DESTINATION_PORT;
	char SEND_MESSAGE[4096];
};

