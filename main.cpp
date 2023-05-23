#include "headers/irc.h"
#include <signal.h>

volatile sig_atomic_t stop;
void inthand(int signum);

int main(int ac, char **av)
{
	Server ourServer("2drunk2code", "TEST", "TEST");

	if (ac < 3)
	{
		std::cout << "Please enter a valid port number and password for connection" << std:: endl;
		return (0);
	}
	ourServer.port = std::atoi(av[1]);
	if (av[2])
		ourServer.password = av[2];

	ourServer.listenfd = ourServer.create_socket();

	// char buff[MAXLINE];
	char recvline[MAXLINE];

	int timeout;
	ourServer.nfds = 1;
	int current_size = 0;
	int i;
	std::string ending;
	User *curr;
	t_svec	recToken;


	signal(SIGINT, inthand);

	memset(ourServer.fds, 0, sizeof(ourServer.fds));
	// Set up the initial listening socket
	ourServer.fds[0].fd = ourServer.listenfd;
	std::cout << "listenfd is: " << ourServer.listenfd <<std::endl;
	ourServer.fds[0].events = POLLIN;
	//initialize the timeout to 3 minutes
	timeout = (3 * 60 * 1000);
	bool running = true;
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
			// if (ourServer.fds[i].revents != POLLIN)//POLLIN == data is ready to read
			// {
			// 	std::cout << "Error revents on fd: " << ourServer.fds[i].fd << std::endl;
			// 	ourServer.removeAllChannel(*ourServer.users.find(ourServer.fds[i].fd)->second);
			// 	ourServer.remove_from_poll(&ourServer.fds[0], ourServer.nfds, ourServer.fds[i].fd);
			// 	close(ourServer.fds[i].fd);
			// 	ourServer.users.erase(ourServer.fds[i].fd);
			// 	delete ourServer.users[ourServer.fds[i].fd];
			// 	current_size -= 1;
			// 	break;
			// }
			if (ourServer.fds[i].fd == ourServer.listenfd)
				ourServer.accept_connection(ourServer.listenfd);
			else
			{
				std::memset(recvline, 0, MAXLINE);
				int n = read(ourServer.fds[i].fd, recvline, MAXLINE - 1);
				if (n <= 0)
				{
					int fd_to_remove = ourServer.fds[i].fd;
					std::cout << "Connection closed by client with fd: " << fd_to_remove << std::endl;
					ourServer.removeAllChannel(*ourServer.users.find(fd_to_remove)->second);
					close(fd_to_remove);
					delete ourServer.users[fd_to_remove];
					ourServer.users.erase(fd_to_remove);
					ourServer.remove_from_poll(&ourServer.fds[0], ourServer.nfds, fd_to_remove);
					current_size -= 1;
					break;
				}
				recvline[n] = '\0';
				std::cout << "Received: " << recvline << std::endl;
				if (ourServer.users.find(ourServer.fds[i].fd) == ourServer.users.end())
					break;
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
						stop = 1;
						break;
					}
					recToken.clear();
				}
			}
		}
	}

	for (i = 1; i < ourServer.nfds; i++)
	{
		ourServer.removeAllChannel(*ourServer.users.find(ourServer.fds[i].fd)->second);
		delete ourServer.users[ourServer.fds[i].fd];
		ourServer.users.erase(ourServer.fds[i].fd);
		shutdown(ourServer.fds[i].fd, SHUT_RDWR);
		close(ourServer.fds[i].fd);
		std::cout << "Connection closed " <<  ourServer.fds[i].fd << std::endl;
	}
	if (!ourServer.channels.empty())
		for (std::map<std::string,Channel*>::iterator chans = ourServer.channels.begin(); chans != ourServer.channels.end(); chans++)
			delete ourServer.channels[chans->second->channel_name];
	std::cout << "SHUTTING DOWN" << std::endl;
	shutdown(ourServer.listenfd, SHUT_RDWR);
	close(ourServer.listenfd);
	return 0;
}

void inthand(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << "\nNOOT NOOT!\n";
		return ;
	}
	
	// signal(SIGINT, inthand);
    // fflush(stdout);
	(void)signum;
}
