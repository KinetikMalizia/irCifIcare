#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std:: string all_commands[10] = {"INVITE", "KICK", "TOPIC"};

	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK};

	for (int i = 0; i < 10; i++)
	{
		if (all_commands[i].compare(recToken.front()) == 0)
			(this->*action[i])(recToken, fd);
		else
			return (-1);
	}
	return (0);
}

void Server:: INVITE(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	Channel *chan = this->channels[recToken[2]];

	std::cout << this->channels[recToken[1]] << std::endl;
	std::cout << "invite:" << recToken[1] << std::endl;
	if chan->isMember(recToken[1])
	std::cout << "this is invite here with " << current->user_nick << std:: endl;
}

void Server:: KICK(t_svec recToken,int fd)
{
	(void)recToken;
	std::cout << "Calling kick" << fd << std:: endl;
}

void Server:: TOPIC(t_svec recToken,int fd)
{
	(void)recToken;
	std::cout << "Calling topic" << fd << std:: endl;
}

