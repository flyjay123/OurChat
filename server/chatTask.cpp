#include "chatTask.h"
using json = nlohmann::json;

void taskThread(int clientFd)
{
	try
	{
		printf("-------------------- new connect --------------------\n");		
		Session session(clientFd);
		json msg;
		while (1)
		{
			msg = session.recvMsg();			
			if (msg.at("cmd") == cmd_logout)
			{
				std::cout << "用户" << std::to_string(clientFd) << "下线" << std::endl;
				if(userMap.size()==1)
				{
					userMap.erase(clientFd);
					close(clientFd);
					return ;
				}
					
				if(userMap.find(clientFd) != userMap.end())
				{
					//获取用户账号
					int account = userMap.at(clientFd);
					Statement query(db,"update user set online=0 where account=?");
					query.bind(1,account);
					query.exec();
            		
					json message;
					message["account"] = std::to_string(account);
					message["cmd"] = "loggout";
					

					std::cout << "用户" << std::to_string(userMap.at(clientFd)) << "下线" << std::endl;
					userMap.erase(clientFd);
				}
					
				close(clientFd);
				return ;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		close(clientFd);
		return ;
	}
}
