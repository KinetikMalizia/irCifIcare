#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std::cout << "Handling " << recToken.front() << std::endl;
	std:: string all_commands[10] = {"INVITE", "KICK", "TOPIC", "PART"};
	// count_args(recToken);
	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK, &Server::TOPIC, &Server::PART};

	for (int i = 0; i < 4; i++)
	{
		if (all_commands[i].compare(recToken.front()) == 0)
		{
			(this->*action[i])(recToken, fd);
			return (0);
		}
	}
	return (-1);
}

// void Server:: count_args(t_svec recToken)
// {
// 	std::vector<std::string>::iterator	itr;

// 	for (itr = recToken.begin(); itr != this->oper.end(); itr++)
// 	{
// 		this->args++;
// 			return (1);
// 	}
// 	std::cout << "number of args: " << args << std::endl;
// }

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
	Channel chan("test");
		(void)fd;
	// check the mode of the user
	// if (this->"topic.empty())
	if (!recToken[2].empty() && (recToken[2].at(0) == '#')) //there is a channel and topic name
	{
		std::cout << "am i here" << std::endl;
		if (!this->channelExists(recToken[1]))
		{
			std::cout << "channel doesnt exist\n";
			err_msg(403,fd,recToken[1],"","","");
			return ;
		}
		else
		{
			Channel	chan = *(this->channels.find(recToken[1])->second);
			chan.setTopic(recToken[2]);
			//get the current channel name
			// 12:26 -!- nikki changed the topic of #wow to: tester
			std::cout << "channel name: " << chan.channel_name << std::endl;
			std::cout << "topic name: " << chan.topic_name << std::endl;
			rpl_msg(332, fd, " changed the topic of ", chan.channel_name, chan.getTopic(), "");
		}
	}
	// else if (recToken[2].empty() ) //only one argument and topic is not set
	// {
	// 	&& chan.topic_name.empty()
	// 	//check what freenode does
	// 	chan.setTopic(recToken[1]);
	// 	//how to get the current channel name, that we are in
	// 	//the topic has to be under channel / each channel will have its own topic name
	// 	rpl_msg(332, fd, " changed the topic of " ,chan.channel_name, chan.topic_name, "");
	// }
	else if (recToken[2].empty() && (recToken[1].empty()))// only 1 argument to set the topic
	{
		// std:: cout << "current topic is: " << chan.topic_name <<std::endl;
		if (chan.topic_name.empty())
		{
			std::cout << "am i here" << std::endl;
			chan.setTopic(recToken[1]);
			std::string topic = chan.getTopic();
			std::string chan_message = this->base_msg + "TOPIC :" + chan.channel_name + " " + topic + " :\r\n";
			chan.channelMessage(NULL, chan_message);
			std::stringstream ss;
			ss << this->channels[recToken[1]]->c_time;
			std::string	date = ":" + this->hostname + " 329 " + current->user_nick + " " + recToken[1] + " :" + ss.str() + "\r\n";
			write(current->fd_user, date.c_str(), date.length());
			this->channels[recToken[1]]->printMembers();
			rpl_msg(332, fd, "current topic of ", chan.channel_name, topic, "");
		}
		else
		{
			std::string topic = chan.getTopic();
			std::cout << "current topic: " << topic << std::endl;
			std::string chan_message = this->base_msg + "TOPIC :" + chan.channel_name + " " + topic + " :\r\n";
			chan.channelMessage(NULL, chan_message);
			std::stringstream ss;
			ss << this->channels[recToken[1]]->c_time;
			rpl_msg(332, fd, "channel name: " ,chan.channel_name, topic, "");
		}
	}
		// else if ((recToken[2].empty()) && (!chan.topic_name.empty())) // only one argument and the topic is set
		// {
		// 	// Topic for #wow: tester
		// 	// 12:27 -!- Topic set by nikki [~nikki@freenode-orb.kjv.68iqjs.IP]
        //   	// [Mon May  8 12:26:36 2023]
		// 	std::string topic = chan.getTopic();
		// 	std::cout << "current topic: " << topic << std::endl;
		// 	std::string chan_message = this->base_msg + "TOPIC :" + chan.channel_name + " " + topic + " :\r\n";
		// 	chan.channelMessage(NULL, chan_message);
		// 	std::stringstream ss;
		// 	ss << this->channels[recToken[1]]->c_time;
		// 	rpl_msg(332, fd, "channel name: " ,chan.channel_name, topic, "");
		// }
	// else
	// 	std::cout << "print something" << std::endl;
	// std::cout << this->topic << std::endl;
	// topic no args :No topic set for #wow
	// nikki changed the topic of #wow to: test



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


//[ client : 8000 ] PART #mytest
//[ server : 6667 ] :KinKangs!~fmalizia@freenode-o6d.g28.dc9e5h.IP PART :#mytest
