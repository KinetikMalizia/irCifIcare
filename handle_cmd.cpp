#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std::cout << "Handling " << recToken.front() << std::endl;
	std:: string all_commands[10] = {"INVITE", "KICK", "TOPIC", "PART", "WHO"};
	count_args(recToken);
	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK, &Server::TOPIC,
														&Server::PART, &Server::WHO};

	for (int i = 0; i < 5; i++)
	{
		if (all_commands[i].compare(recToken.front()) == 0)
		{
			(this->*action[i])(recToken, fd);
			return (0);
		}
	}
	return (-1);
}

void Server:: count_args(t_svec recToken)
{
	std::vector<std::string>::iterator	itr;
	this->args = 0;

	for (itr = recToken.begin(); itr != recToken.end(); itr++)
	{
		this->args++;
	}
}

void Server:: INVITE(t_svec recToken, int fd)
{
	std::string invited = recToken[1];
	std::string channel = recToken[2];
	User *current = (this->users).find(fd)->second;
	if (channelExists(channel) == 0)
	{
		err_msg(403, fd, channel, "", "", "");
		std:: cout << "Channel does not exist" << std::endl;
		return ;
	}
	Channel *chan = this->channels[channel];
	User *inv_member = chan->isMember(invited);

	for (std::map<int, User*>::iterator	itr = this->users.begin(); itr!=this->users.end(); itr++)
	{
		if (invited.compare(itr->second->user_nick) == 0) //invited user is on the server
		{
			std::cout << "member exists" << std::endl;
			if (inv_member != 0) // reverse the result if testing with others
			{
				std::string	date = ":" + this->hostname + " 443 " + current->user_nick + " " + channel + " " + recToken[1] + " :is already on channel" + "\r\n";
				write(current->fd_user, date.c_str(), date.length());
			}
			else
			{
				// std::string	toSender = ":" + this->hostname + " 341 " + current->user_nick + " " + invited + " :" +channel + "\r\n";
				// write(current->fd_user, toSender.c_str(), toSender.length());
				std::string	toInvited = this->base_msg + " INVITE " + invited + " :" + channel + "\r\n";
				write(this->translate(invited), toInvited.c_str(), toInvited.length());
				rpl_msg(341, fd, current->user_nick, recToken[1], channel, "");
			}
		}
	}
	err_msg(401, fd, current->user_nick, invited, "", "");
}

void Server:: KICK(t_svec recToken,int fd)
{
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel*	channel = this->channels[recToken[1]];
	if (!this->isNickUsed(recToken[2]))
	{
		std::cout << "nick doesn't exist [401]\n";
		err_msg(401,fd,recToken[2],"","","");
		return ;
	}
	std::string	kicked = recToken[2];
	User		kicker = *(this->users[fd]);
	if (channel->isOper(kicker.user_nick))
	{
		std::string	message = this->base_msg + "KICK " + channel->channel_name + " " + kicked + " :\r\n";
		channel->channelMessage(NULL, message);
		channel->removeMember(*(this->users[this->translate(kicked)]));
	}
	else
		std::cout << "Tu t'es pris pour qui? " + kicker.user_nick + "\n";

}

void Server:: TOPIC(t_svec recToken,int fd)
{
	User *current = (this->users).find(fd)->second;
	std::cout << "number of args: " << recToken.size() << std::endl;
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel	chan = *(this->channels.find(recToken[1])->second);
	if (recToken.size() == 5) //there is a channel and topic name
	{
		std::cout << "am i here 5" << std::endl;
		chan.setTopic(recToken[2]);
		std::cout << "channel name: " << chan.channel_name << std::endl;
		std::string topic = chan.getTopic();
		std::cout << "topic name: " << chan.topic_name << std::endl;
		std::string chan_message = this->base_msg + "TOPIC " + chan.channel_name + " :" + topic + "\r\n";
		write(current->fd_user, chan_message.c_str(), chan_message.length());
	}
	else if (recToken.size() == 4)// only 1 argument to set the topic
	{
		if (chan.topic_name.empty())
		{
			std::cout << "am i here 4" << std::endl;
			chan.setTopic(recToken[2]);
			std::cout << "channel name: " << chan.channel_name << std::endl;
			std::string topic = chan.getTopic();
			std::cout << "current topic: " << topic << std::endl;
			std::string chan_message = this->base_msg + "TOPIC " + chan.channel_name + " :" + topic + " \r\n";
			// chan.channelMessage(NULL, chan_message);
			write(current->fd_user, chan_message.c_str(), chan_message.length());
			// std::string rep = this->base_msg + "TOPIC " + chan.channel_name + ": " + topic;
		}
		else
		{
			chan.setTopic(recToken[2]);
			std::string topic = chan.getTopic();
			std::cout << "current topic: " << topic << std::endl;
			std::string chan_message = this->base_msg + "TOPIC " + chan.channel_name + " :" + topic + " \r\n";
			// chan.channelMessage(NULL, chan_message);
			write(current->fd_user, chan_message.c_str(), chan_message.length());
		}
	}
	else if (!(chan.isMember(current->user_nick)))
	{
		std::string topic = chan.getTopic();
		rpl_msg(332, fd, "topic is:  " ,chan.channel_name, topic, "");

	}
}

void Server::PART(t_svec recToken, int fd)
{
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel*	channel = this->channels[recToken[1]];
	//add exception if leaver is not in channel
	User		leaver = *(this->users[fd]);
	std::string	confirm = this->base_msg + "PART :" + channel->channel_name + "\r\n";
	channel->channelMessage(NULL, confirm);
	channel->removeMember(leaver);
}

//WHO is a little it broken
void Server::WHO(t_svec recToken, int fd)
{
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel*	channel = this->channels[recToken[1]];
	std::map<int, User*>::iterator	itr;
	User		asker = *(this->users[fd]);
	std::string	first = this->hostname + " 352 " + asker.user_nick + " " + channel->channel_name + " ~";
	std::string	message;

	for (itr=channel->members.begin(); itr!=channel->members.end(); itr++)
	{
		User curr = *(itr->second);
		message = first + curr.user_name + " " + curr.hostname + " " + this->hostname + " H :0 " + curr.full_name + "\r\n";
		write(fd, message.c_str(), message.length());
	}
}

void Server::NAMES(t_svec recToken, int fd)
{
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel*	channel = this->channels[recToken[1]];
	User		asker = *(this->users[fd]);
	std::map<int, User*>::iterator	itr;
	std::string	start;
	std::string	end;
	start = ":" + this->hostname + " 353 " + asker.user_nick + " = " + channel->channel_name + " :";
	for (itr=channel->members.begin(); itr!=channel->members.end(); itr++)
	{
		User curr = *(itr->second);
		if (channel->isOper(curr.user_nick))
			start += "@" + curr.user_nick + " ";
		else
			start += curr.user_nick + " ";
	}
	start += "\r\n";
	write(fd, start.c_str(), start.length());
	end = ":" + this->hostname + " 366 " + asker.user_nick + " " + channel->channel_name + " :End of /NAMES list \r\n";
	write(fd, end.c_str(), end.length());
}
//.freenode.net 352 KinKangs #Wow ~fmalizia freenode-o6d.g28.dc9e5h.IP .freenode.net KinKangs H :0 Fabio Malizia
//:.freenode.net 366 KinKangs #Wow :End of /NAMES list.
