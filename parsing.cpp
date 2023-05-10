#include "headers/irc.h"
#include "headers/server.hpp"

void	tokenize(std::string str, char delim, t_svec &out)
{
	std::stringstream	ss(str);
	std::string			s,	end,	cut;
	std::size_t			back;

	while (std::getline(ss, s, delim))
	{
		back = s.find('\n');
		if (back != std::string::npos && s[0] != ':')
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
	if (!out.empty())
		if (out.back().empty())
			out.pop_back();
}
