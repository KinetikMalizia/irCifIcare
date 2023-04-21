/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:24 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 11:40:06 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class User
{
	public:
			User();
			~User();

			std::string user_name;
			std::string user_nick;
			std::string user_mode;
			std::string	server;
			std::string hostname;
};