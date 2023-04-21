/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:44:35 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 15:45:34 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/user.hpp"

User::User(void)
{	}

User:~User(void)
{	}

int User::User(std::vector<std::string> tokens, int fd_user)
{
	std::vector<std::string>::iterator itr;

	fd_user = this->user.fd_user;
	for (itr = this->tokens.begin(); itr != this->tokens.end(); itr++)
	{
		if(itr = this->tokens.begin() + 2)
			this->tokens = this->user.user_nick;
		if(itr = this->tokens.begin() + 5)
			this->tokens = this->user.user_name;
	}
}

int server::Nick(std::vector<std::string> token)
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