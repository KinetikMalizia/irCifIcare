#include "headers/server.hpp"

void Server:: MODE(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	std::string cmp = recToken[1];
	if(cmp.compare(current->user_nick) == 0)
	{
		std::cout << "recieved MODE\n";
		std::string cont = current->user_nick + "!" + current->user_name + "@" + current->hostname + " MODE " + current->user_nick + ":+" + current->user_mode + "\r\n";
		write(fd, cont.c_str(), cont.length());
	}
	if (channelExists(recToken[1]))
	{
		int	target_fd = 0;
		if(recToken.size() > 2)
			target_fd = translate(recToken[3]);
		std::string pars = recToken[2];
		if (pars[0] == '+')
		{
			for (int i = 1; i < static_cast<int>(pars.length()); i++)
			{
				if(pars[i] == 'o')
				{
					this->channels[recToken[1]]->add_mode(target_fd, '+', *current);
					recToken.erase(recToken.begin() + 2);
				}
				if(pars[i] == 'k')
				{
					std::cout << "rec3 : " << recToken[3] << std::endl;
					std::cout << "channel is : " << this->channels[recToken[1]]->channel_name << std::endl;
					this->channels[recToken[1]]->password = recToken[3];
					recToken.erase(recToken.begin() + 2);
				}
				if(pars[i] == 'l')
				{
					std::cout << "rec3 : " << recToken[3] << std::endl;
					std::cout << "channel is : " << this->channels[recToken[1]]->channel_name << std::endl;
					this->channels[recToken[1]]->limit = std::stoi(recToken[3]);
					recToken.erase(recToken.begin() + 2);
				}
				if(this->channels[recToken[1]]->update_mode(pars[i], 1, *current) < 0)
					err_msg(472, fd, current->user_nick, std::string(1, pars[i]), "", "");
			}
		}
		if (pars[0] == '-')
		{
			for (int i = 1; i < static_cast<int>(pars.length()); i++)
			{
				if(pars[i] == 'o')
				{
					this->channels[recToken[1]]->add_mode(target_fd, '-', *current);
				}
				else if(this->channels[recToken[1]]->update_mode(pars[i], 0, *current) < 0)
					err_msg(472, fd, current->user_nick, std::string(1, pars[i]), "", "");
			}
		}
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
				err_msg(443, fd, channel, recToken[2], "", "");
				return ;
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

void Server::TOPIC(t_svec recToken, int fd)
{
	User *current = (this->users).find(fd)->second;
	if (!this->channelExists(recToken[1]))
	{
		std::cout << "channel doesnt exist\n";
		err_msg(403,fd,recToken[1],"","","");
		return ;
	}
	Channel	chan = *(this->channels.find(recToken[1])->second);
	if (!(chan.isMember(current->user_nick)))
	{
		//if im not in the channel
		err_msg(442, fd, chan.channel_name, "", "", "");
		return ;
		// call a function outside of the channel
	}
	//check the mode of the channel --> if t in the channel cannot change topic
	if (chan.mode_map['t'] == 1)
	{
		if (chan.isOper(current->user_nick) == 0)
		{
			err_msg(482, fd, chan.channel_name, "", "", "");
			return ;
		}
		//cannot change the topic
	}
	if (recToken.size() == 2)// only 1 argument to set the topic
	{
		std::string topic = chan.getTopic();
		if (topic.empty())
		{
			std::cout << "am i here topic only" << std::endl;
			// chan.setTopic(recToken[2]);
			std::cout << "channel name: " << chan.channel_name << std::endl;
			std::cout << "current topic: " << topic << std::endl;
			std::string chan_message = this->base_msg + "TOPIC " + chan.channel_name + " :" + topic + " \r\n";
			chan.channelMessage(NULL, chan_message);
			// write(current->fd_user, chan_message.c_str(), chan_message.length());
			// std::string rep = this->base_msg + "TOPIC " + chan.channel_name + ": " + topic;
		}
		else
			rpl_msg(332, fd, chan.channel_name,"","","");
	}
	else if (recToken.size() == 3) //there is a channel and topic name
	{
		std::cout << "am i here topic with topic name" << std::endl;
		if (recToken[2].empty())
			chan.setTopic("");
		else
			chan.setTopic(recToken[2]);
		std::cout << "channel name: " << chan.channel_name << std::endl;
		std::string topic = chan.getTopic();
		std::cout << "topic name: " << chan.topic_name << std::endl;
		std::string chan_message = this->base_msg + "TOPIC " + chan.channel_name + " :" + topic + "\r\n";
		chan.channelMessage(NULL, chan_message);
		// write(current->fd_user, chan_message.c_str(), chan_message.length());
	}
}
//  [ client : 8000 ] TOPIC //outside of the channel
//  [ server : 6667 ] :*.freenode.net 461 nikki TOPIC :Not enough parameters.
//  [ server : 6667 ] :*.freenode.net 650 nikki TOPIC :<channel> [:<topic>]

//  [ client : 8000 ] TOPIC #chat // outside of the channel
//  [ server : 6667 ] :*.freenode.net 332 nikki #chat :[https://wiki.fnchat.org] Welcome to #Chat. No Hate speech, keep political chat limited as to not cause arguments. Please stay longer than a minute to get a response. Other rules are on the website.
//  [ server : 6667 ] :*.freenode.net 333 nikki #chat f :1682516342

//  [ client : 8000 ] TOPIC #nikki //outside of the channel, no topic
//  [ server : 6667 ] :*.freenode.net 331 nikki #nikki :No topic is set.

//  [ client : 8000 ] TOPIC nikki //no hashtag
//  [ server : 6667 ] :*.freenode.net 403 nikki nikki :No such channel

//  [ client : 8000 ] TOPIC #nikki // outside of the channel, topic set
//  [ server : 6667 ] :*.freenode.net 332 nikki #nikki :HellO this is My TopIc
//  [ server : 6667 ] :*.freenode.net 333 nikki #nikki Me!~fmalizia@freenode-o6d.g28.dc9e5h.IP :1683709779

//  [ client : 8000 ] TOPIC #nikki :whatever //no access to change the topic
//  [ server : 6667 ] :*.freenode.net 482 nikki #nikki :You do not have access to change the topic on this channel

//  [ client : 8000 ] TOPIC #nikki :newtopic //trying to set the topic when not in the channel, but access given
//  [ server : 6667 ] :*.freenode.net 442 nikki #nikki :You're not on that channel!

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


//.freenode.net 352 KinKangs #Wow ~fmalizia freenode-o6d.g28.dc9e5h.IP .freenode.net KinKangs H :0 Fabio Malizia
//:.freenode.net 366 KinKangs #Wow :End of /NAMES list.
