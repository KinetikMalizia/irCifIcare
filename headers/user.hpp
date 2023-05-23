#pragma once

#include "irc.h"

typedef std::vector<std::string>	t_svec;

class User
{
	public:
			User();
			~User();
			int	setInfo(t_svec tokens);
			

			int			fd_user;
			int			last_ping;
			std::string	user_name;
			std::string	user_nick;
			std::string	user_mode;
			std::string	full_name;
			std::string	server;
			std::string	hostname;
			std::string	buffer;
			int			registered;
};