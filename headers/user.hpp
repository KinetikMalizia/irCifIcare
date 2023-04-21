/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:24 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 14:44:52 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"

class User
{
	public:
			User();
			~User();

			int			conn_fd;
			std::string user_name;
			std::string user_nick;
			std::string user_mode;
			std::string	server;
			std::string hostname;
};