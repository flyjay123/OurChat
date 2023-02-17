#include "chatTask.h"
using json=nlohmann::json;

void *taskThread(void *arg){
    printf("-------------------- new connect --------------------\n");
    // pthread_mutex_lock(&_mxMessage);   //lock the mutex
    // pthread_mutex_unlock(&_mxMessage);  //unlock the mutex
    int clientFd = *((int *)arg);
	char recvMsg[1024*2];
	int recvLen;
    int head_len = sizeof(DeMessageHead);

	free(arg);
    arg = NULL;

    Session session(clientFd);
	
	json msg;
	while (1)
	{
		msg = session.recvMsg();
		if(msg.at("cmd") == "logout")
		{
			close(clientFd);
			return NULL;
		}

    }
    return NULL;
}