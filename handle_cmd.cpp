#include "headers/server.hpp"

int Server:: handle_cmds(std:: string command, int fd)
{
	std:: string all_commands[10] = {"INVITE", "KICK", "TOPIC"};

	void (Server:: *action[])(int fd) = {&Server::INVITE, &Server::KICK};

	for (int i = 0; i < 10; i++)
	{
		std:: cout << "the current command is " << command << std:: endl;
		if (all_commands[i].compare(command) == 0)
			(this->*action[i])(fd);
		else
			return (-1);
	}
	return (0);
}

void Server:: INVITE(int fd)
{
	User *current = (this->users).find(fd)->second;
	std::cout << "this is invite here with " << current->user_nick << std:: endl;
}

void Server:: KICK(int fd)
{
	std::cout << "Calling kick" << fd << std:: endl;
}

void Server:: TOPIC(int fd)
{
	std::cout << "Calling topic" << fd << std:: endl;
}

