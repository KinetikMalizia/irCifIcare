#include "headers/channel.hpp"

Channel::Channel(void)
{	}

Channel::~Channel(void)
{	}
Channel::Channel(std::string name): channel_name(name), nmembers(0)
{
	this->c_time = std::time(NULL);
}

int	Channel::printMembers(void)
{
	std::map<int, User*>::iterator	itr;

	std::cout << "Channel: " << this->channel_name << " members\n";
	for(itr=members.begin(); itr!=members.end();itr++)
	{
		std::cout << itr->first << ": " << itr->second->user_nick;
		if(this->isOper(itr->second->user_nick))
			std::cout << (" ***");
		std::cout << std::endl;
	}
	return (0);
}

int	Channel::addMember(User& member)
{
	//send confirmation message
	this->members[member.fd_user] = &member;
	std::cout << member.user_nick << " joined " << this->channel_name << std::endl;
	if(this->nmembers == 0)
	{
		member.user_mode += 'o';
		this->oper.push_back(&member);
	}
	this->nmembers++;
	return (0);
}

int	Channel::removeMember(User& member)
{
	// std::size_t	o_pos = 0;
	std::string confirm = ":" + member.user_nick + "!~" + member.user_name + "@" + member.hostname + " ";
	//send confirmation message
	confirm += "PART :" + this->channel_name + "\r\n";
	this->channelMessage(NULL, confirm);
	if (this->isOper(member.user_nick))
	{
		std::vector<User*>::iterator op;
		for (op=this->oper.begin(); *op != &member; op++)
			std::cout << ".";
		std::cout << "\n";
		this->oper.erase(op);
	}
	// o_pos = member.user_mode.find('o');
	// if (o_pos)
	// 	member.user_mode.erase(o_pos, 1);
	this->members.erase(member.fd_user);
	std::cout << member.user_nick << " left " << this->channel_name << std::endl;
	this->nmembers--;
	return (this->nmembers);
}

int	Channel::channelMessage(User* current, std::string message)
{
	for (std::map<int, User*>::iterator	itr = this->members.begin(); itr!=this->members.end(); itr++)
	{
		if (current != itr->second)
			write(itr->second->fd_user, message.c_str(), message.length());
	}
	return (0);
}

int	Channel::isOper(std::string nick)
{
	std::vector<User*>::iterator	itr;

	for (itr = this->oper.begin(); itr != this->oper.end(); itr++)
	{
		if ((*itr)->user_nick == nick)
			return (1);
	}
	return(0);
}

User	*Channel::isMember(std::string nick)
{
	for (std::map<int, User*>::iterator iter = this->members.begin(); iter != this->members.end(); iter++)
	{
		if (iter->second->user_nick == nick)
			return (iter->second);
	}
	return (0);
}
