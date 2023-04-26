/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 09:26:23 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/25 14:18:41 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"

#define SERVER_PORT 6667
#define MAXLINE 4096
#define POLL_SIZE 10

int main(int ac, char **av)
{
	Server ourServer("hostname", av[1], av[2]);
	
	// if (ac < 3)
	// 	std::cout << "Please enter a valid port number and password for connection" << std:: endl;
	// else
	// {
	// 	ourServer.check_args(av);
	// }
	(void)ac;
	(void)av;
	int listenfd, connfd;
	int pollfd, on = 1;
	struct sockaddr_in servaddr;
	char buff[MAXLINE], recvline[MAXLINE];
	//loop through each fd to create a socket? one socket for all fds
	
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "SOCKET ERROR\n";
		return 1;
	}
	//allow socket descriptor to be reusable
	pollfd = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (pollfd < 0)
	{
		std::cerr << "SETSOCKOPT ERROR\n";
		return 1;
	}
	// set socket to be nonblocking
	fcntl(listenfd, F_SETFL, O_NONBLOCK);
	
	std::memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		std::cerr << "BIND ERROR\n";
		return 1;
	}

	if (listen(listenfd, POLL_SIZE) == -1) {
		std::cerr << "LISTEN ERROR\n";
		return 1;
	}
	std::cout << "----------------------------------------\nServer started, listening on port " << SERVER_PORT << "\n----------------------------------------" << std::endl;
	// here we will end and return listenfd


	struct sockaddr_in new_addr;
	int timeout;
	struct pollfd fds[POLL_SIZE];
	int nfds = 1; 
	int current_size = 0; 
	int i, j;
	socklen_t addrlen;
	
	//Initialize pollfd structure
	memset(fds, 0, sizeof(fds));
	// Set up the initial listening socket  
	fds[0].fd = listenfd;
	fds[0].events = POLLIN;
	//initialize the timeout to 3 minutes 
	timeout = (3 * 60 * 1000);
	
	//loop waiting for incoming connects or data on connected sockets
	// add poll in the infinite loop ?
	pollfd = poll(fds, nfds, timeout);
	if (pollfd < 0)
	{
		std::cout << "Poll failed" << std::endl;
	}
	current_size = nfds;
	for (i = 0; i < POLL_SIZE; i++)
	{
		//loop through to find the fds that returned POLLIN
		if (fds[i].revents == 0)
			continue ;
		if (fds[i].revents != POLLIN)//POLLIN == data is ready to read
			std::cout << "Error revents =" << fds[i].revents << std::endl;
		if (fds[i].fd == listenfd)
		{
			//listening descriptor is readable
			//accpet all incoming connections that are queued up on the listening socket
			//call accept
			//we need to add the new client socket to the pollfd structure (socket of accept)
			//get out of infinite loop and then read and write
		}
	}
	bool running = true;
	bool start = true;
	while (running)
	{
		connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
		std::cout << "New connection established" << std::endl;

		for (;;)
		{
			std::memset(recvline, 0, MAXLINE);
			int n = read(connfd, recvline, MAXLINE - 1);
			if (n < 0)
			{
				std::cerr << "READ ERROR\n";
				break;
			}
			else if (n == 0)
			{
				std::cout << "Connection closed by client" << std::endl;
				break;
			}
			recvline[n] = '\0';
			std::cout << "Received: " << recvline << std::endl;

			if (std::strncmp(recvline, "PING", 4) == 0) 
			{
				std::cout << "Sending PONG" << std::endl;
				snprintf(buff, sizeof(buff), "PONG local.host1.com\n");
				write(connfd, buff, std::strlen(buff));
			}
			if (std::strncmp(recvline, "MODE jcarlen +i", 10) == 0) 
			{
				snprintf(buff, sizeof(buff), "jcarlen!~jcarlen@freenode-o6d.g28.dc9e5h.IP MODE jcarlen :+wRix\n");
				write(connfd, buff, std::strlen(buff));
				start = false;
			}
			if (std::strncmp((char*)recvline, "QSERV", 5) == 0)
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
			}
		}
		shutdown(connfd, SHUT_RDWR);
		close(connfd);
		std::cout << "Connection closed" << std::endl;
	}
	// loop through current size to close all the fds
	shutdown(listenfd, SHUT_RDWR);
	close(listenfd); 
	return 0;
}