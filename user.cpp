/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@students.42lausanne.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:44:35 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/29 13:22:41 by fmalizia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/user.hpp"

User::User(void)
{	}

User::~User(void)
{	}

int	User::setInfo(t_svec tokens, int fd_user)
{
	t_svec::iterator itr;

	this->fd_user = fd_user;
	for (itr = tokens.begin(); itr != tokens.end(); itr++)
	{
		if (itr == tokens.begin() + 1)
			this->user_name = *itr;
		if(itr == tokens.begin() + 2)
			this->user_nick = *itr;
		if(itr == tokens.begin() + 3)
			this->hostname = *itr;
		if(itr == tokens.begin() + 4)
			this->full_name = *itr;
	}
	return (0);
}