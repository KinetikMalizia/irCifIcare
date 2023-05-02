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
	std::string invitee = recToken[1];
	std::string channel = recToken[2];
	(void)fd;
	// User *current = (this->users).find(fd)->second;
	Channel *chan = this->channels[recToken[2]];
	if (channelExists(channel) == 0)
	{
		std:: cout << "Channel does not exist" << std::endl;
		return ;
	}
	User *inv_member = chan->isMember(recToken[1]);

	for (std::map<int, User*>::iterator	itr = this->users.begin(); itr!=this->users.end(); itr++)
	{
		if (invitee.compare(itr->second->user_nick) == 0) //invited user is on the server
		{
			std::cout << "member exists" << std::endl;
			if (inv_member != 0) // reverse the result if testing with others
			{
				std::cout << "member is already part of the channel" << std:: endl;
			}
			else
			{
				chan->addMember(*(itr->second));
				chan->printMembers();
			}
		}
		std::cout << "searching...\n";
	}
	std::cout << "member is not the server" << std::endl;
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

