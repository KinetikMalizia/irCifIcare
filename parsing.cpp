/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:17:38 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/27 12:30:27 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"

// delimiter should also be \n
// can we define delim as multiple potential characters?
// or first message different than rest?
void	tokenize(std::string str, char delim, t_svec &out)
{
	std::stringstream	ss(str);
	std::string			s;
	std::string			end;
	std::size_t			back;

	while (std::getline(ss, s, delim))
	{
		back = s.find('\n');
		if (back != std::string::npos)
		{
			// std::cout << "sep: " << s.substr(0,back) << std::endl;
			out.push_back(s.substr(0,back));
			s.erase(0,back + 1);
		}
		if (s[0] == ':')
		{
			std::getline(ss, end);
			s.erase(0,1);
			s += ' ';
			s += end;
		}
		out.push_back(s);
	}
}

std::string	lower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c){ return std::tolower(c); });
}
