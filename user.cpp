#include "headers/user.hpp"

User::User(void)
{	
	this->last_ping = 0;
	this->registered = 0;
	this->buffer = "";
	this->user_name = "";
	this->user_mode = "";
	this->user_nick = "";
	this->full_name = "";
	this->server = "";
	this->hostname = "";
}

User::~User(void)
{	}

int	User::setInfo(t_svec tokens, int fd_user)
{
	t_svec::iterator itr;

	this->fd_user = fd_user;
	for (itr = tokens.begin(); itr != tokens.end(); itr++)
	{
		if (itr == tokens.begin() + 1)
			this->user_name = *itr;
		if(itr == tokens.begin() + 2)
			if(this->user_nick.empty())
				this->user_nick = *itr;
		if(itr == tokens.begin() + 3)
			this->hostname = *itr;
		if(itr == tokens.begin() + 4)
			this->full_name = *itr;
	}
	return (0);
}
