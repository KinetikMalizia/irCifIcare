#ifndef IRC_H
# define IRC_H

# define SERVER_PORT 6667
# define POLL_SIZE 10

# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <string>
# include <sstream>
# include <cstdlib>
# include "server.hpp"
# include "user.hpp"
# include "channel.hpp"

void		tokenize(std::string str, char sep, std::vector<std::string> &out);
std::string	lastN(std::string input, int n);
std::string	toUpper(std::string str);

typedef std::vector<std::string>	t_svec;

# define ALLOWED "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_[]{}\\|"
#endif
