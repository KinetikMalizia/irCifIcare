/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:24 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/26 16:05:30 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.h"

typedef std::vector<std::string>	t_svec;

class User
{
	public:
			User();
			~User();
			int	setInfo(t_svec tokens, int fd_user);
			

			int			fd_user;
			std::string user_name;
			std::string user_nick;
			std::string user_mode;
			std::string	server;
			std::string hostname;
};