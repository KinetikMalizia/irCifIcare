#include "headers/channel.hpp"

Channel::Channel(void)
{	}

Channel::~Channel(void)
{	}
Channel::Channel(std::string name): channel_name(name), nmembers(0)
{
	this->c_time = std::time();
}

int	Channel::printMembers(void)
{
	std::map<int, User*>::iterator	itr;
	
	std::cout << "Channel: " << this->channel_name << " members\n";
	for(itr=members.begin(); itr!=members.end();itr++)
	{
		std::cout << itr->first << ": " << itr->second->user_nick << std::endl;
	}
	return (0);
}

int	Channel::addMember(User& member)
{
	//send confirmation message
	this->members[member.fd_user] = &member;
	std::cout << member.user_nick << " joined " << this->channel_name << std::endl;
	if(this->nmembers == 0)
		member.user_mode += 'o';
	this->nmembers++;
	return (0);
}

int	Channel::removeMember(User& member)
{
	std::size_t	o_pos = 0;

	//send confirmation message
	this->members.erase(member.fd_user);
	std::cout << member.user_nick << " left " << this->channel_name << std::endl;
	o_pos = member.user_mode.find('o');
	if (o_pos)
		member.user_mode.erase(o_pos, 1);
	this->nmembers--;
	return (this->nmembers);
}