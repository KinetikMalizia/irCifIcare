#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std:: string all_commands[13] = {"INVITE", "KICK", "TOPIC", "PART", "WHO", "USER", "NICK",
										"JOIN", "PING", "PRIVMSG", "MODE", "NOTICE", "PASS"};
	count_args(recToken);
	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK, &Server::TOPIC,
														&Server::PART, &Server::WHO, &Server::USER, &Server::NICK,
														&Server::JOIN, &Server::PING, &Server::PRIVMSG, &Server::MODE,
														&Server::NOTICE, &Server::PASS};
	User commander = *this->users.find(fd)->second;
	if (commander.registered == 0 && recToken.front() != "PASS")
	{
		std::cout << "ignored: " << recToken.front() << std::endl;
		return (-1);
	}
	for (int i = 0; i < 13; i++)
	{
		if (all_commands[i].compare(recToken.front()) == 0)
		{
			(this->*action[i])(recToken, fd);
			return (0);
		}
	}
	return (-1);
}

void Server:: count_args(t_svec recToken)
{
	std::vector<std::string>::iterator	itr;
	this->args = 0;

	for (itr = recToken.begin(); itr != recToken.end(); itr++)
	{
		this->args++;
	}
}
