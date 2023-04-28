/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@students.42lausanne.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:33:31 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/28 11:20:34 by fmalizia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.h"

typedef std::vector<std::string>	t_svec;

class Channel
{
	private:
	public:
			Channel();
			Channel(std::string name);
			~Channel();

			std::map<int, User*>	members;
			std::string				channel_name;
			int						nmembers;

			int	printMembers();
			int	addMember(User& member);
			int	removeMember(User& member);
};

//  The commands which may only be used by channel operators are:

//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel