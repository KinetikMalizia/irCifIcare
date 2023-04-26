/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:48:56 by nnemeth           #+#    #+#             */
/*   Updated: 2023/04/25 16:41:41 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/server.hpp"

Server::Server(void)
{	}

Server:: Server(std::string host_name, std:: string portnumber, std:: string password)
{
	//move the creation of the socket etc from main
	(void)host_name;
	(void)portnumber;
	(void)password;
}


void Server:: check_args(char **av)
{
	password = av[2];
	port = atoi(av[1]);
}

int Server:: add_client(int client_fd) //accept connections when users connect (poll())
{
	// continuation of connection with accept -- connfd
	// add client address
	// adding select ?? 
	this->users[client_fd] = new User();
	return(0);
}


void	Server::print_map(void)
{
	std::cout << "KEY & VALUE" << std::endl;
	for (std::map<int, User*>::iterator iter = this->users.begin(); iter != this->users.end(); iter++)
	{
		std::cout<<(*iter).first<<"\t"<<(*iter).second<<"\n";
	}
}

Server::~Server()
{
    
    //disconnect all clients, close sockets
    
}

std::string Server:: first_msg()
{
	//enter welcome message here
	return ("Fill This\r\n");
}

void remove_from_poll(int fd)
{
	(void)fd;
}

// int Server::Nick(std::vector<std::string> token)
// {
// 	for (itr = this->tokens.begin(); itr != this->tokens.end(); itr++)
// 	{
// 		if(itr = this->tokens.begin() + 1)
// 		{
// 			if(itr = this->tokens.begin() + 2)
// 			{
// 				itr = std::map::server.users.find(this->tokens)
// 				if(itr = td::map::server.users.end())
// 					this->tokens = this->user.user_nick;
// 				else
// 					std::cout << "nick already taken\n";
// 			}
// 		}
// }


// //getsockname - returns the current address to which the socket sockfd is bound
// //getprotobyname - returns a pointer to a protoent structure for the network protocol specified on the call.
// //gethostbyname - 

void Server::find_cmd(std::vector<std::string> recToken)
{
		std::string firstString = recToken.front();
		if(firstString.compare("PING") == 0)
			std::cout << "recieved PING\n";
		else
			std::cout << "first string in vec is : "<< firstString << std::endl;
}