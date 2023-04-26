/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:17:38 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/26 17:34:13 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"

// delimiter should also be \n
// can we define delim as multiple potential characters?
// or first message different than rest?
void	tokenize(std::string str, char delim, t_svec &out)
{
	std::stringstream ss(str);
	std::string s;
	std::string	end;
	
	while (std::getline(ss, s, delim))
	{
		if (s[0] == ':')
		{
			std::getline(ss, end);
			s.erase(0,1);
			s += ' ';
			s += end;
		}
		std::cout << s << std::endl;
		out.push_back(s);
	}
}
