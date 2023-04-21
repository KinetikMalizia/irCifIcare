/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:08 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 11:37:16 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>

class Server
{
	private:

	public:
		Server();
		~Server();
		
		//in most cases, std::string is just a placeholder
		std::map<std::string,std::string>	users;
		std::vector<std::string>			tokens;
		std::map<std::string, std::string>	channels;

};

#endif