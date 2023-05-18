#include "headers/irc.h"


int main(int ac, char **av)
{
	Server ourServer("2drunk2code", "TEST", "TEST");
	if (av[1])
		ourServer.password = av[1];
	// int listenfd;
	//int connfd;
	(void)ac;
	(void)av;

	// if (ac < 3)
	// 	std::cout << "Please enter a valid port number and password for connection" << std:: endl;
	// else
	// {
	ourServer.listenfd = ourServer.create_socket();
	// }
	// char buff[MAXLINE];
	char recvline[MAXLINE];

	int timeout;
	ourServer.nfds = 1;
	int current_size = 0;
	int i;
	std::string ending;
	User *curr;
	t_svec	recToken;

	memset(ourServer.fds, 0, sizeof(ourServer.fds));
	// Set up the initial listening socket
	ourServer.fds[0].fd = ourServer.listenfd;
	ourServer.fds[0].events = POLLIN;
	//initialize the timeout to 3 minutes
	timeout = (3 * 60 * 1000);
	bool running = true;
	//bool start = true;
	while (running)
	{
		ourServer.pollfd = poll(ourServer.fds, ourServer.nfds, timeout);
		if (ourServer.pollfd < 0)
		{
			std::cout << "Poll failed" << std::endl;
			running = false;
			break;
		}
		current_size = ourServer.nfds;
		for (i = 0; i < current_size; i++)
		{
			//loop through to find the fds that returned POLLIN
			if (ourServer.fds[i].revents == 0)
				continue ;
			if (ourServer.fds[i].revents != POLLIN)//POLLIN == data is ready to read
			{
				ourServer.remove_from_poll(&ourServer.fds[i], ourServer.nfds, i);
				ourServer.removeAllChannel(*ourServer.users.find(i)->second);
				ourServer.users.erase(i);
				delete ourServer.users[i];
				break;
			}
			if (ourServer.fds[i].fd == ourServer.listenfd)
				ourServer.accept_connection(ourServer.listenfd);
			else
			{
				std::memset(recvline, 0, MAXLINE);
				int n = read(ourServer.fds[i].fd, recvline, MAXLINE - 1);
				if (n < 0)
				{
					std::cout << "READ ERROR!" << std::endl;
					running = false;
					break;
				}
				else if (n == 0)
				{
					std::cout << "Connection closed by client" << std::endl;
					break;
				}
				recvline[n] = '\0';
				std::cout << "Received: " << recvline << std::endl;
				curr = ourServer.users.find(ourServer.fds[i].fd)->second;
				curr->buffer += std::string(recvline);
				ending = lastN(curr->buffer, 1);
				if (ending == "\n")
				{	
					tokenize(curr->buffer, ' ' ,recToken);
					ourServer.find_cmd(recToken, ourServer.fds[i].fd);
					curr->buffer = "";
					if (std::strncmp((char*)recvline, "QSERV\r\n", 5) == 0)
					{
						std::cout << "Shutting down server" << std::endl;
						running = false;
						break;
					}
					recToken.clear();
				}
			}
		}
	}
	for (i = 1; i < ourServer.nfds; i++)
	{
		shutdown(ourServer.fds[i].fd, SHUT_RDWR);
		close(ourServer.fds[i].fd);
		std::cout << "Connection closed " <<  ourServer.fds[i].fd << std::endl;
	}
	//loop waiting for incoming connects or data on connected sockets
	// add poll in the infinite loop ?
	// loop through current size to close all the fds
	shutdown(ourServer.listenfd, SHUT_RDWR);
	close(ourServer.listenfd);
	return 0;
}
