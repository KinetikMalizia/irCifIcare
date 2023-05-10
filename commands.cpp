#include "headers/server.hpp"

void Server:: USER(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	current->setInfo(recToken, fd);
	std::string cont = first_message(fd);
	// std::string cont = ":" + this->hostname + " 001 " + current->user_nick + " :" +
	// 				"Welcome to 2drunk2code server!!! " + current->user_nick + "!~" + current->user_nick + "@" +
	// 				this->hostname + "\r\n";
	//snprintf(buff, sizeof(buff), ":local.host1.com 001 jcarlen :Welcome to the freenode IRC Network jcarlen!~jcarlen@127.0.0.1\r\n");
	//							 ":server name	   001 nickname :  welcome message						   !~nickname@hostname\r\n";
	write(fd, cont.c_str(), cont.length());
}

void Server:: NICK(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	this->gen_base_msg(fd);
	std::cout << "recieved NICK\n";
	std::string	reponse;
	if (!isNickUsed(recToken[1]))
	{
		reponse = this->base_msg + " NICK :" + recToken[1] + "\r\n";
		current->user_nick = recToken[1];
		//:second!~fmalizia@freenode-o6d.g28.dc9e5h.IP NICK :weewoo
	}
	else
	{
		reponse = err_msg(401, fd, recToken[1], "", "", "");
	}
	// snprintf(buff, sizeof(buff), "changing nick\n");
	write(fd, reponse.c_str(), reponse.length());
}

void Server:: JOIN(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	if (!channelExists(recToken[1]))
	{
		this->channels[recToken[1]] = new Channel(recToken[1]);
		std::cout << "New channel created: " << recToken[1] << std::endl;
	}
	//base_msg JOIN :channel
	std::cout << "Join the CHANNEL\n";
	this->channels[recToken[1]]->addMember(*current);
	std::string confirm = this->base_msg + "JOIN :" + recToken[1] + "\r\n";
	this->channels[recToken[1]]->channelMessage(NULL, confirm);
	this->NAMES(recToken, fd);
	std::stringstream ss;
	ss << this->channels[recToken[1]]->c_time;
	std::string	date = ":" + this->hostname + " 329 " + current->user_nick + " " + recToken[1] + " :" + ss.str() + "\r\n";
	write(current->fd_user, date.c_str(), date.length());
	this->channels[recToken[1]]->printMembers();
}

void Server:: PING(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	std::string pong = "PONG " + recToken[1] + "\r\n";
	// snprintf(buff, sizeof(buff), "%s", pong.c_str());
	current->last_ping = time(NULL);
	this->print_users();
	std::cout << "lastping = " << current->last_ping << " " << current->user_nick << std::endl;
	write(fd, pong.c_str(), pong.length());
	check_user_pings();
}

void Server:: PRIVMSG(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	std::cout << "recieved PRIVMSG\n";
	if (recToken[1][0] == '#')
	{
		std::cout << "Do channel message\n";
		if (!this->channelExists(recToken[1]))
			std::cout << "channel [" << recToken[1] << "] doesn't exitst\n";
		else
		{
			Channel	rec = *(this->channels.find(recToken[1])->second);
			std::string message = ":" + current->user_nick + "!~" + current->user_name + "@" + this->hostname + " PRIVMSG " + recToken[1] + " " + recToken[2] + "\r\n";
			rec.channelMessage(current, message);
		}
	}
	else
	{
		int	target_fd = translate(recToken[1]);

		if (target_fd < 0)
			std::cout << "Nick: "<< recToken[1] << "doesn't exist\n";
			//replace with numerical code error message
		else
		{
			std::string message = ":" + current->user_nick + "!~" + current->user_name + "@" + this->hostname + " PRIVMSG " + recToken[1] + " " + recToken[2] + "\r\n";
			write(target_fd, message.c_str(), message.length());
		}
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

// void Server:: NOTICE(t_svec recToken, int fd)
// {

// }
