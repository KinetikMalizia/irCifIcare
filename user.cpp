/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:44:35 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/25 16:41:43 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/user.hpp"

User::User(void)
{	}

User::~User(void)
{	}

User::User(std::vector<std::string> tokens, int fd_user)
{
	std::vector<std::string>::iterator itr;

	this->fd_user = fd_user;
	for (itr = tokens.begin(); itr != tokens.end(); itr++)
	{
		if (itr == tokens.begin() + 1)
			this->user_name = *itr;
		if(itr == tokens.begin() + 2)
			this->user_nick = *itr;
		if(itr == tokens.begin() + 5)
			this->user_name = *itr;
	}
}