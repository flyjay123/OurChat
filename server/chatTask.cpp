#include "chatTask.h"
using json = nlohmann::json;

void *taskThread(void *arg)
{
	try
	{
		printf("-------------------- new connect --------------------\n");
		// pthread_mutex_lock(&_mxMessage);   //lock the mutex
		// pthread_mutex_unlock(&_mxMessage);  //unlock the mutex
		
		int clientFd = *((int *)arg);
		free(arg);
		arg = NULL;
		Session session(clientFd);
		json msg;
		while (1)
		{
			msg = session.recvMsg();
			if (msg.at("cmd") == "logout")
			{
				std::cout << "用户" << std::to_string(clientFd) << "下线" << std::endl;
				if(userMap.find(clientFd) != userMap.end())
				{
					std::cout << "用户" << std::to_string(userMap.at(clientFd)) << "下线" << std::endl;
					userMap.erase(clientFd);
				}
					
				close(clientFd);
				
				return NULL;
			}
		}
		return NULL;
	}
	catch (...)
	{
		std::cerr << "Something Wrong" << endl;
		return NULL;
	}
}