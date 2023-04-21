/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:08 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 15:45:22 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.h"

class Server
{
	private:

	public:
		Server();
		int add_client(int listenfd, (struct sockaddr *)NULL, NULL); //accept connections when users connect (poll())
		int add_modes();
		int Nick(std::vector<std::string> token);
		void send_msg(std:: string msg, int msg_code); // reply to client --> create a container with all the possible messages and codes
		//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu 
		~Server();
		
		//in most cases, std::string is just a placeholder
		std::map<std::string,std::string>	users; // usernickname and pointer on the client
		std::vector<std::string>			tokens;
		std::map<std::string, std::string>	channels;

};

#endif 