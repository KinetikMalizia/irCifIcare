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

			std::map<int, User*>		members;
			std::vector<std::string>	oper;
			std::string					channel_name;
			std::time_t					c_time;
			int							nmembers;

			int	printMembers();
			int	addMember(User& member);
			int	removeMember(User& member);
};

//  The commands which may only be used by channel operators are:

//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel