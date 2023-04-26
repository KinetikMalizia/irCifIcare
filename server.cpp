/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:48:56 by nnemeth           #+#    #+#             */
/*   Updated: 2023/04/26 15:57:500 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/server.hpp"

Server::Server(void)
{
	this->default_mode = "i";
}

Server:: Server(std::string host_name, std:: string portnumber, std:: string password)
{
	//move the creation of the socket etc from main
	(void)host_name;
	(void)portnumber;
	(void)password;
	// 	check_args(av);
}

void Server:: check_args(char **av)
{
	password = av[2];
	port = atoi(av[1]);
}

int Server:: create_socket()
{
	this->pollfd = 1;
	this->on = 1;
	struct sockaddr_in servaddr;
	int listenfd;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "SOCKET ERROR\n";
		return 1;
	}
	//allow socket descriptor to be reusable
	this->pollfd = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&this->on, sizeof(this->on));
	if (this->pollfd < 0)
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
	return (listenfd);
}

int Server:: accept_connection(int listenfd)
{
	int connfd;
	this->listenfd = listenfd;
	
	accept(this->listenfd, (struct sockaddr *)NULL, NULL);
	this->fds[nfds].fd = connfd;
	this->fds[nfds].events = POLLIN;
	std::cout << "New connection established " << this->fds[nfds].fd << std::endl;
	add_client(this->fds[nfds].fd);
	this->users.find(this->fds[nfds].fd)->user_mode = this->default_mode;
	this->print_map();
	nfds++;
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

void remove_from_poll(struct pollfd fds[], int i, int nfds)
{
	fds[i] = fds[nfds - 1];
	nfds--;
}

// int Server::Nick(t_svec token)
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

void	Server::find_cmd(t_svec recToken, int fd)
{
		char buff[MAXLINE];
		User current = (this->users).find(fd);
		
		std::string firstString = recToken.front();
		std::cout << "first string in vec is : "<< firstString << std::endl;
		while(recToken.empty() != 0)
		{
			if(firstString.compare("PING") == 0)
			{
				std::cout << "recieved PING\n";
				snprintf(buff, sizeof(buff), "PONG TO YOU, ONLY YOU\n");
				write(fd, buff, std::strlen(buff));
			}
			if(firstString.compare("NICK") == 0)
			{
				std::cout << "recieved NICK\n";
				snprintf(buff, sizeof(buff), "changing nick\n");
				write(fd, buff, std::strlen(buff));
			}
			if(firstString.compare("MODE " + current.user_nick + " +i") == 0)
			{
				std::cout << "recieved MODE\n";
				snprintf(buff, sizeof(buff), current.user_nick + "!" + current.user_name + "@" + this->hostname + " MODE " + current.user_nick + ":+" + current.user_mode );
				write(fd, buff, std::strlen(buff));
			}
			else
				recToken.erase(recToken.begin());
		}
}

int	Server::FillUserInfo(t_svec tokens, int user_fd)
{
		User *current = (this->users).find(user_fd);
		current.setInfo(tokens);
		return (0);
}