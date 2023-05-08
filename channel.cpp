#include "headers/channel.hpp"

Channel::Channel(void)
{	}

Channel::~Channel(void)
{	}
Channel::Channel(std::string name): channel_name(name), nmembers(0)
{
	this->c_time = std::time(NULL);
	this->mode_map.insert(std::pair<char,int>('i',1));
	this->mode_map.insert(std::pair<char, int>('t', 1));
	this->mode_map.insert(std::pair<char, int>('k', 0));
	this->mode_map.insert(std::pair<char, int>('l', 0));
	channel_mode();
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
	if (this->isOper(member.user_nick))
	{
		std::vector<User*>::iterator op;
		for (op=this->oper.begin(); op != this->oper.end(); op++)
		{
			std::cout << ".";
			if (*op == &member)
				break;
		}
		std::cout << "\n";
		if (op != this->oper.end())
			this->oper.erase(op);
	}
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

std::string Channel::channel_mode() const
{
	std::string result;

	for (std::map<char, int>::const_iterator it = this->mode_map.begin(); it != this->mode_map.end(); ++it)
	{
		if (it->second == 1)
			result += it->first;
	}
	std::cout << "server mode are : " << result << std::endl;
	return (result);
}

void Channel::update_mode(char key, int value)
{
	if (this->mode_map.find(key) != this->mode_map.end())
		this->mode_map[key] = value;
	else
		this->mode_map.insert(std::pair<char,int>(key,value));
	channel_mode();
}
