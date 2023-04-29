/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@students.42lausanne.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:25:40 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/29 13:22:43 by fmalizia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <string>
# include <sstream>
# include <cstdlib>
# include "server.hpp"
# include "user.hpp"
# include "channel.hpp"

void		tokenize(std::string str, char sep, std::vector<std::string> &out);

typedef std::vector<std::string>	t_svec;
#endif
