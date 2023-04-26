/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:44:35 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/26 15:43:16 by jcarlen          ###   ########.ch       */
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
		if(itr == tokens.begin() + 5)
			this->user_name = *itr;
	}
	return (0);
}