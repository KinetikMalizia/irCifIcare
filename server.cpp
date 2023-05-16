#include "headers/server.hpp"

Server::Server(void)
{
	this->default_mode = "i";
}

Server:: Server(std::string hostname, std:: string portnumber, std:: string password)
{
	//move the creation of the socket etc from main
	this->hostname = hostname;
	(void)portnumber;
	(void)password;
	this->default_mode = "i";
	this->password = "NOPASSWD";
	// 	check_args(av);
}

void Server:: check_args(char **av)
{
	password = std::string(av[2]);
	port = std::atoi(av[1]);
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
	welcome_msg();
	// here we will end and return listenfd
	return (listenfd);
}

int Server:: accept_connection(int listenfd)
{
	int connfd = 0;
	this->listenfd = listenfd;
	//listening descriptor is readable
	//accpet all incoming connections that are queued up on the listening socket
	//call accept
	//we need to add the new client socket to the pollfd structure (socket of accept)
	//get out of infinite loop and then read and write

	connfd = accept(this->listenfd, (struct sockaddr *)NULL, NULL);
	this->fds[nfds].fd = connfd;
	this->fds[nfds].events = POLLIN;
	std::cout << "New connection established " << this->fds[nfds].fd << std::endl;
	add_client(this->fds[nfds].fd);
	this->users.find(this->fds[nfds].fd)->second->user_mode = this->default_mode;
	this->print_users();
	nfds++;
	return (0);
}


int Server:: add_client(int client_fd) //accept connections when users connect (poll())
{
	// continuation of connection with accept -- connfd
	// add client address
	// adding select ??
	this->users[client_fd] = new User();
	return(0);
}

void	Server::print_users(void)
{
	std::cout << "KEY & VALUE" << std::endl;
	for (std::map<int, User*>::iterator iter = this->users.begin(); iter != this->users.end(); iter++)
	{
		std::cout<<(*iter).first<<"\t"<<(*iter).second<<"\n";
	}
}

Server::~Server()
{
    // shutdown(this->listenfd, SHUT_RDWR);
	// close(this->listenfd);
    //disconnect all clients, close sockets

}

void Server::remove_from_poll(struct pollfd fds[], int& nfds, int fd)
{
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].fd == fd)
		{
			fds[i] = fds[nfds - 1];
			nfds--;
			break;
		}
	}
}

void Server::check_user_pings()
{
	std::map<int, User*> users = this->users;
	time_t current_time = time(NULL);
	std::map<int, User*>::iterator it = users.begin();
	while (it != users.end())
	{
		int fd = it->first;
		User* user = it->second;
		if (current_time - user->last_ping > 120 && user->last_ping)
		{
			std::cout << "User " << user->user_nick << " timed out" << std::endl;
			close(fd);
			users.erase(it);
			this->users.erase(fd);
			remove_from_poll(this->fds, this->nfds, fd);
		}
		else
		{
			++it;
		}
	}
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
		//char buff[MAXLINE];
		std::string test;
		// User *current = (this->users).find(fd)->second;
		std::string firstString = recToken.front();
		this->gen_base_msg(fd);
		while(recToken.empty() != 1)
		{
			std::cout << "Token: " << recToken.front() << std::endl;
			handle_cmds(recToken, fd);
			recToken.erase(recToken.begin());
			if(!recToken.empty())
				firstString = recToken.front();
		}
}

int	Server::isNickUsed(std::string nick)
{
	nick = toUpper(nick);
	for (std::map<int, User*>::iterator iter = this->users.begin(); iter != this->users.end(); iter++)
	{
		if (toUpper(iter->second->user_nick) == nick)
		{
			std::cout << toUpper(iter->second->user_nick) << " is the same as " << nick << "\n";
			return (1);
		}
	}
	return (0);
}

int	Server::channelExists(std::string name)
{
	for (std::map<std::string, Channel*>::iterator iter = this->channels.begin(); iter != this->channels.end(); iter++)
	{
		if (iter->first == name)
			return (1);
	}
	return (0);
}

int	Server::FillUserInfo(t_svec tokens, int user_fd)
{
		User *current = (this->users).find(user_fd)->second;
		current->setInfo(tokens, user_fd);
		return (0);
}

int	Server::translate(std::string nick)
{
	std::map<int, User*>::iterator itr;

	for(itr=this->users.begin(); itr!=this->users.end(); itr++)
	{
		if(itr->second->user_nick == nick)
			return (itr->first);
	}
	return(-1);
}

