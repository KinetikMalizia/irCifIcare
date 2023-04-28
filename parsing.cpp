/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@students.42lausanne.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:17:38 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/28 09:53:25 by fmalizia         ###   ########.fr       */
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
			if(cut[cut.length()-1] == '\r')
				cut.erase(cut.length()-1, 1);
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
		if(s[s.length()-1] == '\r')
			s.erase(s.length()-1, 1);
		out.push_back(s);
	}
}
