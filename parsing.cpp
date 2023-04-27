/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:17:38 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/27 16:00:48 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.h"

// delimiter should also be \n
// can we define delim as multiple potential characters?
// or first message different than rest?
void	tokenize(std::string str, char delim, t_svec &out)
{
	std::stringstream	ss(str);
	std::string			s,	end,	cut;
	std::size_t			back;

	while (std::getline(ss, s, delim))
	{
		back = s.find('\n');
		if (back != std::string::npos)
		{
			// std::cout << "sep: " << s.substr(0,back) << std::endl;
			cut = s.substr(0,back);
			if(cut.back() == '\r')
				cut.pop_back();
			out.push_back(cut);
			s.erase(0,back + 1);
		}
		if (s[0] == ':')
		{
			std::getline(ss, end);
			s.erase(0,1);
			s += ' ';
			s += end;
		}
		if(s.back() == '\r')
			s.pop_back();
		out.push_back(s);
	}
}
