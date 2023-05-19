#include "headers/server.hpp"

int Server:: handle_cmds(t_svec recToken, int fd)
{
	std:: string all_commands[14] = {"INVITE", "KICK", "TOPIC", "PART", "WHO", "USER", "NICK",
										"JOIN", "PING", "PRIVMSG", "MODE", "NOTICE", "PASS", "QUIT"};
	count_args(recToken);
	void (Server:: *action[])(t_svec recToken, int fd) = {&Server::INVITE, &Server::KICK, &Server::TOPIC,
														&Server::PART, &Server::WHO, &Server::USER, &Server::NICK,
														&Server::JOIN, &Server::PING, &Server::PRIVMSG, &Server::MODE,
													&Server::NOTICE, &Server::PASS, &Server::QUIT};
	if (this->users.find(fd) == this->users.end())
		return (-1);
	User commander = *this->users.find(fd)->second;
	if (commander.registered == 0 && recToken.front() != "PASS" && this->password != "NOPASSWD")
	{
		std::cout << "ignored: " << recToken.front() << std::endl;
		return (-1);
	}
	for (int i = 0; i < 14; i++)
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
		this->args++;
}
