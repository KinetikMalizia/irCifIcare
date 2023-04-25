/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 14:25:40 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/25 10:35:06 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "server.hpp"
#include "user.hpp"
#include "channel.hpp"


void	tokenize(std::string &str, char sep, std::vector<std::string> &out);
