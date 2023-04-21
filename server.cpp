#include "headers/server.hpp"

Server:: Server()
{


}


Server:: ~Server()
{
    
    //disconnect all clients, close sockets
    
}



int server::Nick(std::vector<std::string> token)
{
	for (itr = this->tokens.begin(); itr != this->tokens.end(); itr++)
	{
		if(itr = this->tokens.begin() + 1)
		{
			if(itr = this->tokens.begin() + 2)
			{
				itr = std::map::server.users.find(this->tokens)
				if(itr = td::map::server.users.end())
					this->tokens = this->user.user_nick;
				else
					std::cout << "nick already taken\n";
			}
		}
}