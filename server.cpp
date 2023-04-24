/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:48:56 by nnemeth           #+#    #+#             */
/*   Updated: 2023/04/24 17:10:24 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/server.hpp"

Server:: Server()
{
	//move the creation of the socket etc from main


}

Server:: ~Server()
{
    
    //disconnect all clients, close sockets
    
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

// poll 