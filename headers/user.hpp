/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:24 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/25 16:41:41 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.h"

class User
{
	public:
			User();
			User(std::vector<std::string> tokens, int fd_user);
			~User();
			

			int			fd_user;
			std::string user_name;
			std::string user_nick;
			std::string user_mode;
			std::string	server;
			std::string hostname;
};