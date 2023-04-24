/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:08 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/24 15:44:46 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.h"

class user;

class Server
{
	private:

	public:
		Server();
		void check_args(char **av);
		// int add_client(int listenfd, (struct sockaddr *)NULL, NULL); //accept connections when users connect (poll())
		int add_modes();
		int Nick(std::vector<std::string> token);
		std:: string first_msg();
		void send_msg(std:: string msg, int msg_code); // reply to client --> create a container with all the possible messages and codes
		//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu 
		~Server();
		struct timeval{
		long int tv_sec;
		long int tv_usec;
		};
		//in most cases, std::string is just a placeholder
		std::map<std::string,std::string>	users; // usernickname and pointer on the client
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
