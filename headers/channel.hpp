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
			std::map<int, User*>		oper;//change it with pointer to real user, so if the nick changes it changes here too
			std::string					channel_name;
			std::time_t					c_time;
			int							nmembers;

			int	printMembers();
			int	addMember(User& member);
			int	removeMember(User& member);
			int	isOper(std::string nick);
			User *isMember(std::string nick);
};

//  The commands which may only be used by channel operators are:

//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel

// add a list
