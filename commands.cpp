#include "headers/server.hpp"

void Server:: USER(t_svec recToken, int fd)
{
	if (recToken.size() < 2)
		return ;
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
	if (recToken.size() < 2)
		return ;
	User *current = (this->users).find(fd)->second;
	std::cout << "recieved NICK\n";
	std::string	reponse;
	std::string	check = ALLOWED;
	for(int r = 0; recToken[1][r]; r++)
	{
		if (check.find(recToken[1][r]) == std::string::npos)
		{
			err_msg(432,fd,recToken[1],"","","");
			return ;
		}
	}
	if (!isNickUsed(recToken[1]))
	{
		reponse = this->base_msg + " NICK :" + recToken[1] + "\r\n";
		current->user_nick = recToken[1];
		write(fd, reponse.c_str(), reponse.length());
		//:second!~fmalizia@freenode-o6d.g28.dc9e5h.IP NICK :weewoo
	}
	else
	{
		err_msg(433, fd, recToken[1], "", "", "");
	}
	// snprintf(buff, sizeof(buff), "changing nick\n");
}

void Server:: JOIN(t_svec recToken, int fd)
{
	if (recToken.size() < 2)
		return ;
	User *current = (this->users).find(fd)->second;
	std::string pass = "";

	if (recToken.size() < 2)
	{
		err_msg(461,fd,"","","","");
		return ;
	}
	if (recToken.size() > 2)
		pass = recToken[2];
	if (!channelExists(recToken[1]))
	{
		try
		{
			this->channels[recToken[1]] = new Channel(recToken[1]);
			std::cout << "New channel created: " << recToken[1] << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what(); // information from length_error printed
			return ;
		}
		this->channels[recToken[1]]->oper.push_back(current);
	}
	Channel chan = *(this->channels.find(recToken[1])->second);
	//check that the mode is not invite only and if it is, is the person on the list?
	std::cout << "current nick" << current->user_nick << std::endl;
	if (chan.mode_map['i'] == 1 && (chan.isInviteList(current->user_nick) == 0))
	{
		std::cout << "channel is invite only and you have not been invited " << std::endl;
		err_msg(473, fd, chan.channel_name,"","","");
		return ;
	}
	else if (chan.mode_map['k'] == 1 && pass != chan.password && !chan.isInviteList(current->user_nick))
	{
		err_msg(475,fd,current->user_nick,chan.channel_name,"","");
		return ;
	}
	// :*.freenode.net 332 aabbccdd #h3llo :this is the start topic
	std::cout << "Join the CHANNEL\n";
	this->channels[recToken[1]]->addMember(*current);
	std::string confirm = this->base_msg + "JOIN :" + recToken[1] + "\r\n";
	this->channels[recToken[1]]->channelMessage(NULL, confirm);
	std::cout << chan.getTopic() << ":is the current topic of " << chan.channel_name << "\n";
	rpl_msg(332, fd, chan.channel_name, chan.getTopic() ,"","");
	this->NAMES(recToken, fd);
	std::stringstream ss;
	ss << this->channels[recToken[1]]->c_time;
	std::string	date = ":" + this->hostname + " 329 " + current->user_nick + " " + recToken[1] + " :" + ss.str() + "\r\n";
	write(current->fd_user, date.c_str(), date.length());
	this->channels[recToken[1]]->printMembers();
}

void Server:: PING(t_svec recToken, int fd)
{
	if (recToken.size() < 2)
		return ;
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
	if (recToken.size() < 2)
		return ;
	User *current = (this->users).find(fd)->second;
	std::cout << "recieved PRIVMSG\n";
	if (recToken[1][0] == '#')
	{
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
		int		target_fd = translate(recToken[1]);

		if (target_fd < 0)
			this->err_msg(401, fd, recToken[1], "", "", "");
		else
		{
			std::string message = ":" + current->user_nick + "!~" + current->user_name + "@" + this->hostname + " PRIVMSG " + recToken[1] + " " + recToken[2] + "\r\n";
			write(target_fd, message.c_str(), message.length());
		}
	}
}

void Server::NAMES(t_svec recToken, int fd)
{
	if (recToken.size() < 2)
		return ;
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

void Server:: NOTICE(t_svec recToken, int fd)
{
	// NOTICE jcarlen :hello
	//:KinKangs!~fmalizia@freenode/user/KinKangs NOTICE KinKangs :hello
	if (recToken.size() < 2)
		return ;
	std::string	message = this->base_msg;

	if (!this->channelExists(recToken[1]))
	{
		if (!this->isNickUsed(recToken[1]))
		{
			this->err_msg(401, fd, recToken[1], "", "", "");
			return ;
		}
		User	*target = this->users.find(translate(recToken[1]))->second;
		std::cout << "inviting " << target->user_nick << std::endl;
		message += "NOTICE " + target->user_nick + " :" + recToken[2] + "\r\n";
		write(target->fd_user, message.c_str(), message.length());
	}
	else
	{
		Channel	chan = *(this->channels[recToken[1]]);
		message +=  "NOTICE " + recToken[1] + " :" + recToken[2] + "\r\n";
		chan.channelMessage(NULL, message);
	}
}
