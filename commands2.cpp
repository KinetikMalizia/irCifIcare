#include "headers/server.hpp"

void Server:: MODE(t_svec recToken, int fd)
{
	int error = 0;
	if (recToken.size() < 2)
		return ;
	User *current = (this->users).find(fd)->second;
	std::string cmp = recToken[1];

	if (channelExists(recToken[1]))
	{
		int	target_fd = 0;
		std::string pars = "";
		if(recToken.size() > 3)
			target_fd = translate(recToken[3]);
		if(recToken.size() > 2)
			pars = recToken[2];
		if (pars[0] == '+')
		{
			for (int i = 1; i < static_cast<int>(pars.length()); i++)
			{
				if(pars[i] == 'o')
				{
					if(recToken.size() > 3)
					{
						this->channels[recToken[1]]->add_mode(target_fd, '+', *current);
						recToken.erase(recToken.begin() + 2);
					}
					else
					{
						err_msg(696, fd, current->user_nick, this->channels[recToken[1]]->channel_name, "o", "");
						error += 1;
					}
				}
				if(pars[i] == 'k')
				{
					if(recToken.size() > 3)
					{
						this->channels[recToken[1]]->password = recToken[3];
						recToken.erase(recToken.begin() + 2);
					}
					else
					{
						err_msg(696, fd, current->user_nick, this->channels[recToken[1]]->channel_name, "k", "");
						error += 1;
					}
				}
				if(pars[i] == 'l')
				{
					if(recToken.size() > 3)
					{
						this->channels[recToken[1]]->limit = std::atoi(recToken[3].c_str());
						recToken.erase(recToken.begin() + 2);
					}
					else
					{
						err_msg(696, fd, current->user_nick, this->channels[recToken[1]]->channel_name, "l", "");
						error += 1;
					}
				}
				if(this->channels[recToken[1]]->update_mode(pars[i], 1, *current) < 0)
				{
					error += 1;
					err_msg(472, fd, current->user_nick, std::string(1, pars[i]), "", "");
				}
				else if(this->channels[recToken[1]]->update_mode(pars[i], 1, *current) == 3)
				{
					err_msg(482, fd, current->user_nick, this->channels[recToken[1]]->channel_name, "", "");
				}
			}
		}
		if (pars[0] == '-')
		{
			for (int i = 1; i < static_cast<int>(pars.length()); i++)
			{
				if(pars[i] == 'o')
					this->channels[recToken[1]]->add_mode(target_fd, '-', *current);
				else if(this->channels[recToken[1]]->update_mode(pars[i], 0, *current) < 0)
				{
					err_msg(472, fd, std::string(1, pars[i]), "", "", "");
				}
			}
		}
		if(!pars.empty() && pars[1] != 'o' && error > 0)
		{
			std::string chan_name = this->channels[recToken[1]]->channel_name;
			std::string rply = (this->base_msg + "MODE " + chan_name + " :" + this->channels[recToken[1]]->channel_mode() + "\r\n");
			this->channels[recToken[1]]->channelMessage(NULL, rply);
		}
		else if(error == 0)
		{
			std::string chan_name = this->channels[recToken[1]]->channel_name;
			std::string rply = (this->base_msg + "MODE " + chan_name + " :" + this->channels[recToken[1]]->channel_mode() + "\r\n");
			this->channels[recToken[1]]->channelMessage(NULL, rply);
		}
		if (pars.empty())
			rpl_msg(324, fd, this->channels[recToken[1]]->channel_name,  this->channels[recToken[1]]->channel_mode(), "", "");
	}
}

void Server:: INVITE(t_svec recToken, int fd)
{
	if (recToken.size() < 2)
		return ;
	std::string invited = recToken[1];
	std::string channel = recToken[2];
	std::cout << "invited " << invited << std::endl;
	std::cout << "channel " << channel << std::endl;
	User *current = (this->users).find(fd)->second;
	if (channelExists(channel) == 0)
	{
		err_msg(403, fd, channel, "", "", "");
		std:: cout << "Channel does not exist" << std::endl;
		return ;
	}
	if (isNickUsed(invited) == 0)
	{
		err_msg(401, fd, invited, "", "", "");
		return ;
	}
	Channel *chan = this->channels[channel];
	User *invitee = chan->isMember(current->user_nick);
	//  [ client : 8000 ] INVITE Nikki #wow
	//  [ server : 6667 ] :*.freenode.net 401 niks Nikki :No such nick

	//  [ client : 8000 ] INVITE nikki_ #wow
 	// [ server : 6667 ] :niks!~nikki@freenode-k9a.g28.dc9e5h.IP INVITE nikki_ :#wow
 	// [ server : 6667 ] :*.freenode.net 341 niks nikki_ :#wow
	for (std::map<int, User*>::iterator	itr = this->users.begin(); itr!=this->users.end(); itr++)
	{
		if (invited.compare(itr->second->user_nick) == 0) //invited user is on the server
		{
			User *inv = itr->second;
			std::cout << "member exists" << std::endl;
			if (invitee == 0)
			{
				// invitee is not on the channel
				err_msg(442, fd, channel, recToken[2], "", "");
				return ;
			}
			else if (chan->isMember(invited) != 0)
			{
				std::cout << "invited person is already on the channel" << std::endl;
				err_msg(443, fd, channel, invited, "", "");
				return ;
			}
			else
			{

				if (chan->mode_map['i'] == 1)
				{
					if (chan->isOper(current->user_nick) == 0)
					{
						err_msg(482, fd, channel, "", "", "");
						return ;
					}
					//cannot change the topic
				}
				// std::string	toSender = ":" + this->hostname + " 341 " + current->user_nick + " " + invited + " :" +channel + "\r\n";
				// write(current->fd_user, toSender.c_str(), toSender.length());
				std::string	toInvited = this->base_msg + "INVITE " + invited + " :" + channel + "\r\n";
				write(this->translate(invited), toInvited.c_str(), toInvited.length());
				rpl_msg(341, fd, " INVITE", invited, channel, "");
				chan->addInviteList(*inv);
				//on the join, we need to check if the channel is mode i --> if yes and youre not on the invited list you cant join
			}
		}
	}
}

void Server:: KICK(t_svec recToken,int fd)
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

void Server::TOPIC(t_svec recToken, int fd)
{ //outside of the channel asking for topic with channel name does not work
	if (recToken.size() < 2)
		return ;
	User *current = (this->users).find(fd)->second;
	if (recToken.size() == 2)// only 1 argument to set the topic
	{
		if (recToken[1].empty())
		{
			err_msg(461, fd, "TOPIC","","","");
			err_msg(650, fd, "TOPIC","","","");
			return ;
		}
		// we need to check if the user is the channel
		std::cout << "am i here topic only outside of the channel" << std::endl;
		Channel	*chan = this->channels.find(recToken[1])->second;
		std::cout << chan << std::endl;
		if (chan == NULL)
		{
			std::cout << "Channel doesnt exist" << std::endl;
			err_msg(403,fd,recToken[1],"","","");
			return ;
		}
		std::string topic = chan->getTopic();
		if (topic.empty())
		{
			std::cout << "channel name: " << chan->channel_name << std::endl;
			std::cout << "current topic: " << topic << std::endl;
			rpl_msg(331, fd, chan->channel_name, "", "", "");
		}
		else
			rpl_msg(332, fd, chan->channel_name, chan->getTopic() ,"","");
	}
	else if (recToken.size() == 3) //there is a channel and topic name
	{
		Channel	*chan = this->channels.find(recToken[1])->second;
		if (chan == NULL)
		{
			std::cout << "Channel doesnt exist" << std::endl;
			err_msg(403,fd,recToken[1],"","","");
			return ;
		}
		if ((chan->isMember(current->user_nick)) == 0)
		{
			//if im not in the channel
			err_msg(442, fd, chan->channel_name, "", "", "");
			return ;
			// call a function outside of the channel
		}
		//check the mode of the channel --> if t in the channel cannot change topic
		if (chan->mode_map['t'] == 1)
		{
			if (chan->isOper(current->user_nick) == 0)
			{
				err_msg(482, fd, chan->channel_name, "", "", "");
				return ;
			}
			//cannot change the topic
		}
		std::cout << "am i here topic with topic name" << std::endl;
		if (recToken[2].empty())
			chan->setTopic("");
		else
			chan->setTopic(recToken[2]);
		std::cout << "channel name: " << chan->channel_name << std::endl;
		std::string topic = chan->getTopic();
		std::cout << "topic name: " << chan->getTopic() << std::endl;
		std::string chan_message = this->base_msg + "TOPIC " + chan->channel_name + " :" + topic + "\r\n";
		chan->channelMessage(NULL, chan_message);
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
	if (channel->removeMember(leaver) == 0)
	{
		Channel* del =  this->channels[channel->channel_name];
		this->channels.erase(channel->channel_name);
		delete del;
		// this->channels[channel->channel_name] = 0;
	}
}

void Server::PASS(t_svec recToken, int fd)
{
	User *check = this->users.find(fd)->second;
	if (recToken.size() < 2)
		return ;
	std::string	pass = recToken[1];

	if(check->registered == 1)
		err_msg(462, fd,"","","","");
	else if (pass != this->password)
		err_msg(464, fd,"","","","");
	else
	{
		check->registered = 1;
		std::cout << "REGISTERED!\n";
	}
}
//.freenode.net 352 KinKangs #Wow ~fmalizia freenode-o6d.g28.dc9e5h.IP .freenode.net KinKangs H :0 Fabio Malizia
//:.freenode.net 366 KinKangs #Wow :End of /NAMES list.

void Server:: QUIT(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	std::string combine;
	std:: cout << "current Token: " << recToken[1] << std::endl;
	if (recToken.size() < 2)
		return ;
	// for (int i = 1; i < recToken.size(); i++)
	// 	combine += recToken[i];
	std::string quit = "ERROR : Closing link: " + this->base_msg + "[Quit: " + recToken[1] + "]" + "\r\n";
	write(current->fd_user, quit.c_str(), quit.length());
	removeAllChannel(*current);
	remove_from_poll(&this->fds[0], this->nfds, fd);
	this->users.erase(fd);
	delete current;
}
//only close the client that quit not shut down the whole server  / only if it was the last client
//clients can leave with an argument that has to be printed in the message
//  [ client : 8000 ] QUIT :leaving
//  [ server : 6667 ] ERROR :Closing link: (~nikki@185.25.195.185) [Quit: leaving]
//  [ server : 6667 ] ERROR : Closing link: :nikki!~nikki@2drunk2code [Quit: leaving]
