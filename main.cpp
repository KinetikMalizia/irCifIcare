/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 09:26:23 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/26 16:05:30 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"


int main(int ac, char **av)
{
	Server ourServer("hostname", NULL, NULL);;
	int listenfd;
	int connfd;
	(void)ac;
	(void)av;

	// if (ac < 3)
	// 	std::cout << "Please enter a valid port number and password for connection" << std:: endl;
	// else
	// {
	listenfd = ourServer.create_socket();
	// }
	// int pollfd, on = 1;
	// struct sockaddr_in servaddr;
	char buff[MAXLINE];
	char recvline[MAXLINE];
	
	int timeout;
	// struct pollfd fds[POLL_SIZE];
	ourServer.nfds = 1; 
	int current_size = 0; 
	int i;
	t_svec	recToken;
	// Server	our_serv;
	//loop through each fd to create a socket? one socket for all fds
	
	// if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	// 	std::cerr << "SOCKET ERROR\n";
	// 	return 1;
	// }
	//allow socket descriptor to be reusable
	// pollfd = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	// if (pollfd < 0)
	// {
	// 	std::cerr << "SETSOCKOPT ERROR\n";
	// 	return 1;
	// }
	// set socket to be nonblocking
	// fcntl(listenfd, F_SETFL, O_NONBLOCK);
	
	// std::memset(&servaddr, 0, sizeof(servaddr));
	// servaddr.sin_family = AF_INET;
	// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// servaddr.sin_port = htons(SERVER_PORT);

	// if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
	// 	std::cerr << "BIND ERROR\n";
	// 	return 1;
	// }

	// if (listen(listenfd, POLL_SIZE) == -1) {
	// 	std::cerr << "LISTEN ERROR\n";
	// 	return 1;
	// }
	// std::cout << "----------------------------------------\nServer started, listening on port " << SERVER_PORT << "\n----------------------------------------" << std::endl;
	// here we will end and return listenfd


	//struct sockaddr_in new_addr;
	//socklen_t addrlen;
	
	//Initialize pollfd structure
	memset(ourServer.fds, 0, sizeof(ourServer.fds));
	// Set up the initial listening socket  
	ourServer.fds[0].fd = listenfd;
	ourServer.fds[0].events = POLLIN;
	//initialize the timeout to 3 minutes 
	timeout = (3 * 60 * 1000);
	bool running = true;
	bool start = true;
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
			{
				//listening descriptor is readable
				//accpet all incoming connections that are queued up on the listening socket
				//call accept
				//we need to add the new client socket to the pollfd structure (socket of accept)
				//get out of infinite loop and then read and write
				connfd = ourServer.accept_connection(listenfd);
				// accept(listenfd, (struct sockaddr *)NULL, NULL);
				// fds[nfds].fd = connfd;
				// fds[nfds].events = POLLIN;
				// nfds++;
				// std::cout << "New connection established " << fds[nfds - 1].fd << std::endl;
			}
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
				//replace recvline with recToken;
				// if (std::strncmp(recvline, "MODE jcarlen +i", 10) == 0) 
				// {
				// 	snprintf(buff, sizeof(buff), "jcarlen!~jcarlen@freenode-o6d.g28.dc9e5h.IP MODE jcarlen :+wRix\n");
				// 	write(connfd, buff, std::strlen(buff));
				// 	start = false;
				// }
				if (std::strncmp((char*)recvline, "QUIT :leaving", 5) == 0)
				{
					std::cout << "Shutting down server" << std::endl;
					running = false;
					break;
				}
				if (start == true)
				{
					std::cout << "Sending welcome message" << std::endl;
					snprintf(buff, sizeof(buff), ":local.host1.com 001 jcarlen :Welcome to the freenode IRC Network jcarlen!~jcarlen@127.0.0.1\n");
					write(connfd, buff, std::strlen(buff));
					start = false;
				}
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
