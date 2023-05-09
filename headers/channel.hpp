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
			std::time_t				c_time;
			int						nmembers;
			std::string				topic_name;

			int	printMembers();
			int	addMember(User& member);
			int	removeMember(User& member);
			int	channelMessage(User* current, std::string message);
			int	isOper(std::string nick);
			User *isMember(std::string nick);
			void setTopic(std::string topic);
			std::string getTopic();
};

//  The commands which may only be used by channel operators are:

//         MODE    - Change the channel's mode
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         TOPIC   - Change the channel topic in a mode +t channel

// add a list
