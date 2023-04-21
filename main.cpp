/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 09:26:23 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 15:45:27 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"

#define SERVER_PORT 6667
#define MAXLINE 4096

int main(int ac, char **av)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE], recvline[MAXLINE];

	(void)ac;
	(void)av;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "SOCKET ERROR\n";
		return 1;
	}

	std::memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		std::cerr << "BIND ERROR\n";
		return 1;
	}

	if (listen(listenfd, 10) == -1) {
		std::cerr << "LISTEN ERROR\n";
		return 1;
	}

	std::cout << "----------------------------------------\nServer started, listening on port " << SERVER_PORT << "\n----------------------------------------" << std::endl;
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
	shutdown(listenfd, SHUT_RDWR);
	close(listenfd);
	return 0;
}