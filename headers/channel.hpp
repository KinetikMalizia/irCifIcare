/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmalizia <fmalizia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:33:31 by fmalizia          #+#    #+#             */
/*   Updated: 2023/04/26 16:05:31 by fmalizia         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"
typedef std::vector<std::string>	t_svec;

class Channel
{
	private:

	public:
			Channel();
			~Channel();
};

//  The commands which may only be used by channel operators are:

//         KICK    - Eject a client from the channel
//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel