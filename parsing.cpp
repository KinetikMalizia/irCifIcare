/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:17:38 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/21 15:45:12 by jcarlen          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

void	tokenize(std::string &str, char delim, std::vector<std::string> &out)
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