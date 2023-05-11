#include "headers/channel.hpp"

Channel::Channel(void)
{
	this->c_time = std::time(NULL);
	this->mode_map.insert(std::pair<char,int>('i',0));
	this->mode_map.insert(std::pair<char, int>('t', 1));
	this->mode_map.insert(std::pair<char, int>('k', 0));
	this->mode_map.insert(std::pair<char, int>('l', 0));
	this->mode_map.insert(std::pair<char, int>('o', 0));
	channel_mode();
	this->c_time = time(NULL);
}

Channel::~Channel(void)
{	}

Channel::Channel(std::string name): channel_name(name), nmembers(0)
{
	this->c_time = std::time(NULL);
	this->mode_map.insert(std::pair<char,int>('i',0));
	this->mode_map.insert(std::pair<char, int>('t', 1));
	this->mode_map.insert(std::pair<char, int>('k', 0));
	this->mode_map.insert(std::pair<char, int>('l', 0));
	this->mode_map.insert(std::pair<char, int>('o', 0));
	channel_mode();
	this->c_time = time(NULL);
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
	this->nmembers++;
	return (0);
}

int	Channel::addInviteList(User& invited)
{
	this->members[invited.fd_user] = &invited;
	this->invite.push_back(&invited);
	this->invited++;
	// for (size_t i = 0; i < invite.size(); i++)
	// {
	// 	std:: cout << invite[i]->user_nick << std::endl;
	// }
	return (0);
}

int	Channel::isInviteList(std::string nick)
{
	for (size_t i = 0; i < invite.size(); i++)
	{
		std:: cout << invite[i]->user_nick << std::endl;
		if (invite[i]->user_nick == nick)
			return (1);
	}
	return (0);
}

int	Channel::removeMember(User& member)
{
	if (this->isOper(member.user_nick))
	{
		std::vector<User*>::iterator found;

		for (found = oper.begin(); found != oper.end(); found++)
		{
			// std::cout << "to find: " << member.user_nick << std::endl;
			// std::cout << (*found)->user_nick << std::endl;
			if ((*found)->user_nick == member.user_nick)
			{
				oper.erase(found);
				std::cout << "found!\n";
				break;
			}
		}
	}
	std::cout << member.user_nick << " left " << this->channel_name << std::endl;
	this->members.erase(member.fd_user);
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

std::string Channel::channel_mode()
{
	std::string result;

	for (std::map<char, int>::iterator it = this->mode_map.begin(); it != this->mode_map.end(); ++it)
	{
		if (it->second == 1)
			result += it->first;
	}
	std::cout << "channel mode are : [" << result << "]" << std::endl;
	return (result);
}

int Channel::update_mode(char key, int value, User &member)
{
	if (this->isOper(member.user_nick))
	{
		if (this->mode_map.find(key) != this->mode_map.end())
			this->mode_map[key] = value;
		else
			return(-1);
	}
	else
	{
		std::cout << "ur not opper kekw" << std::endl;
	}
	channel_mode();
	return(1);
}

int Channel::add_mode(int target_fd, char o,  User &member)
{
	if (this->isOper(member.user_nick))
	{
		if(o == '+')
		{
			this->members.find(target_fd)->second->user_mode += 'o';
			this->oper.push_back(this->members.find(target_fd)->second);
			return 1;
		}
		if(o == '-')
		{
			this->members.find(target_fd)->second->user_mode += ' ';
			return 1;
		}
	}
	return 0;
}

void Channel:: setTopic(std::string topic)
{
	this->topic_name = topic;
}

std::string Channel:: getTopic()
{
	return (this->topic_name);
}
