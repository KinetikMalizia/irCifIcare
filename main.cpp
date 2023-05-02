#include "headers/irc.h"


int main(int ac, char **av)
{
	Server ourServer("2drunk2code", "TEST", "TEST");
	int listenfd;
	//int connfd;
	(void)ac;
	(void)av;

	// if (ac < 3)
	// 	std::cout << "Please enter a valid port number and password for connection" << std:: endl;
	// else
	// {
	listenfd = ourServer.create_socket();
	// }
	// char buff[MAXLINE];
	char recvline[MAXLINE];
	
	int timeout;
	ourServer.nfds = 1; 
	int current_size = 0; 
	int i;
	t_svec	recToken;

	memset(ourServer.fds, 0, sizeof(ourServer.fds));
	// Set up the initial listening socket  
	ourServer.fds[0].fd = listenfd;
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
				std::cout << "Error revents =" << ourServer.fds[i].fd << std::endl;
				running = false;
				break;
			}
			if (ourServer.fds[i].fd == listenfd)
				ourServer.accept_connection(listenfd);
			else
			{
				std::memset(recvline, 0, MAXLINE);

				int n = read(ourServer.fds[i].fd, recvline, MAXLINE - 1);
				if (n < 0)
				{
					std::cerr << "READ ERROR!\n";
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
				tokenize(std::string(recvline), ' ' ,recToken);
				ourServer.find_cmd(recToken, ourServer.fds[i].fd);
				if (std::strncmp((char*)recvline, "QSERV\r\n", 5) == 0)
				{
					std::cout << "Shutting down server" << std::endl;
					running = false;
					break;
				}
				// if (start == true)
				// {
				// 	std::cout << "Sending welcome message" << std::endl;
				// 	snprintf(buff, sizeof(buff), ":local.host1.com 001 jcarlen :Welcome to the freenode IRC Network jcarlen!~jcarlen@127.0.0.1\n");
				// 	write(connfd, buff, std::strlen(buff));
				// 	start = false;
				// }
				recToken.clear();
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
	shutdown(listenfd, SHUT_RDWR);
	close(listenfd); 
	return 0;
}
