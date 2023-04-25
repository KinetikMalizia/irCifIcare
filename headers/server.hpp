/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:08 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/25 16:41:33 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.h"

class User;

class Server
{
	private:

	public:
		Server();
		Server(std::string host_name, std:: string portnumber, std:: string password);
		void check_args(char **av);
		void remove_from_poll(int fd);
		// int add_client(int listenfd, struct of poll); //accept connections when users connect (poll())
		int add_modes();
		int	add_client(int client_fd);
		int Nick(std::vector<std::string> token);
		void find_cmd(std::vector<std::string> recToken);
		void print_map();
		std:: string first_msg();
		void send_msg(std:: string msg, int msg_code); // reply to client --> create a container with all the possible messages and codes
		//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu 
		~Server();
		struct timeval{
		long int tv_sec;
		long int tv_usec;
		};
		//in most cases, std::string is just a placeholder
		std::map<int,User*>	users; // usernickname and pointer on the client
		std::vector<std::string>			tokens;
		std::map<std::string, std::string>	channels;
		std::string 						password;
		int									port;
		std::string							user_mode; // set it as launch
		std::string							hostname;

		

};

#endif 


		// The user modes available: 
        //    i - marks a users as invisible;
        //    s - marks a user for receipt of server notices;
        //    w - user receives wallops;
        //    o - operator flag.
		// If a user attempts to make themselves an operator using the "+o"
		// flag, the attempt should be ignored.  

	// select(int nfds, fd_set *readfds, fd_est *writefds, fd_set *exceptfds, struct timeval *timeout);
