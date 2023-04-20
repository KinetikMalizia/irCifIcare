/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 09:26:23 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/20 10:36:18 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <iostream>
#include <unistd.h> // needed for read() replace later

#define SERVER_PORT 18000
#define MAXLINE 4096
#define SA struct sockaddr



int	main(void)
{
	int	listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	uint8_t	buff[MAXLINE + 1];
	uint8_t	recvline[MAXLINE + 1];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)//create the socket
	{
		std::cout << "SOCKET ERROR\n";
		return (1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//listen to any address -- change to a certain IP later
	servaddr.sin_port = htons(SERVER_PORT);//set the port you listen on
	bzero(&servaddr.sin_zero, 8); //set the unused info to 0
	
	if ((bind(listenfd, (SA  *) &servaddr, sizeof(servaddr))) < 0)//bind the fd to the address
	{
		std::cout << "BIND ERROR\n";
		return (1);
	}
	if ((listen(listenfd, 10)) < 0)//listen to the fd
	{
		std::cout << "LISTEN ERROR\n";
		return (1);
	}

	// server setup done, now infinite loop

	for(; ;)
	{
		//struct sockaddr_in addr;
		// socklen_t addr_len; 
		// char client_address[MAXLINE + 1]
	
		std::cout << "\n----------------------------------------\nWaiting for connection on port " << SERVER_PORT << "\n----------------------------------------" <<std::endl;
		connfd = accept(listenfd, (SA *)NULL, NULL); //accept connection form everywhere, don't store address of connect
		//connfd = accept(listenfd, (SA *), &addr, &addr_len);
		// instead of NULL we will need to add the address of the client.
		// inet_ntop(AF_INET, &addr, client_address, MAXLINE);
		
		memset(recvline, 0, MAXLINE);
		while ((n = read(connfd, recvline, MAXLINE - 1)) > 0) //replace with c++ function later
		{
			std::cout << recvline << std::endl;//print recieved line

			if (recvline[n - 1] == '\n')//find the end of the message
			{
				break;
			}
			memset(recvline, 0, MAXLINE);//reset memory
		}
		if (n < 0)
		{
			std::cout << "READ ERROR\n";
			return (1);
		}

		//snprintf (6:47) here
		snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200OK\r\n\r\nWEBPAGE!");//format string before sending
		
		write(connfd, (char*)buff, strlen((char*)buff));//write back to the user
		close(connfd);
	}
}