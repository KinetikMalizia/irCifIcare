/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:48:56 by nnemeth           #+#    #+#             */
/*   Updated: 2023/04/25 12:23:33 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/server.hpp"

Server:: Server(std::string host_name, std:: string portnumber, std:: string password))
{
	//move the creation of the socket etc from main

}


void Server:: check_args(char **av)
{
	password = av[2];
	port = atoi(av[1]);
}

int Server:: add_client(int listenfd, (struct sockaddr *)NULL, NULL) //accept connections when users connect (poll())
{
	// continuation of connection with accept -- connfd
	// add client address
	// adding select ?? 
}

std:string Server:: first_msg()
{
	//enter welcome message here
}

int Server::Nick(std::vector<std::string> token)
{
	for (itr = this->tokens.begin(); itr != this->tokens.end(); itr++)
	{
		if(itr = this->tokens.begin() + 1)
		{
			if(itr = this->tokens.begin() + 2)
			{
				itr = std::map::server.users.find(this->tokens)
				if(itr = td::map::server.users.end())
					this->tokens = this->user.user_nick;
				else
					std::cout << "nick already taken\n";
			}
		}
}

Server:: ~Server()
{
    
    //disconnect all clients, close sockets
    
}

//getsockname - returns the current address to which the socket sockfd is bound
//getprotobyname - returns a pointer to a protoent structure for the network protocol specified on the call.
//gethostbyname - 