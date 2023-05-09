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
			std::vector<User*>		oper;
			std::string				channel_name;
			long long				c_time;
			int						nmembers;
			std::map<char,int>		mode_map;

			int	printMembers();
			std::string channel_mode();
			int update_mode(char key, int value);
			int	addMember(User& member);
			int	removeMember(User& member);
			int	channelMessage(User* current, std::string message);
			int	isOper(std::string nick);
			User *isMember(std::string nick);
			int change_mode(User &member, std::string str);
};

//  The commands which may only be used by channel operators are:

//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel

// add a list
