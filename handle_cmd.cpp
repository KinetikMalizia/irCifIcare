#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std::cout << "Handling " << recToken.front() << std::endl;
	std:: string all_commands[10] = {"INVITE", "KICK"};

	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK};

	for (int i = 0; i < 2; i++)
	{
		if (all_commands[i].compare(recToken.front()) == 0)
		{
			(this->*action[i])(recToken, fd);
			return (0);
		}
	}
	return (-1);
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
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		return ;
	}
	Channel*	channel = this->channels[recToken[1]];
	if (!this->isNickUsed(recToken[2]))
	{
		std::cout << "nick doesn't exist [401]\n";
		return ;
	}
	std::string	kicked = recToken[2];
	User		kicker = *(this->users[fd]);
	if (channel->isOper(kicker.user_nick))
	{
		std::string	message = this->base_msg + " KICK " + channel->channel_name + " " + kicked + " :\r\n";
		write(this->translate(recToken[2]), message.c_str(), message.length());
	}
	else
		std::cout << "Tu t'es pris pour qui? " + kicker.user_nick + "\n";
	
}

void Server:: TOPIC(t_svec recToken,int fd)
{
	(void)recToken;
	std::cout << "Calling topic" << fd << std:: endl;
}

