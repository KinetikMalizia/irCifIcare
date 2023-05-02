#include "headers/user.hpp"

User::User(void)
{	}

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

// User *User::isMember(std::string nick)
// {
// 	for (t_svec)
// }
