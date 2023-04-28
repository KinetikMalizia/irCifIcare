/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnemeth <nnemeth@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:37:45 by nnemeth           #+#    #+#             */
/*   Updated: 2023/04/28 13:55:37 by nnemeth          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/server.hpp"
#include "irc.h"

//just to list the potential messages we will need

// REPLIES
RPL_WELCOME (001) 
 "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
RPL_NOTOPIC (331)
 "<client> <channel> :No topic is set"
RPL_TOPIC (332) --> Sent to a client when joining the <channel> to inform them of the current topic of the channel.
 "<client> <channel> :<topic>"
RPL_INVITELIST (336) --> Sent to a client as a reply to the INVITE command when used with no parameter, to indicate a channel the client was invited to.
 "<client> <channel>"
RPL_INVITING (341) --> Sent as a reply to the INVITE command
 "<client> <nick> <channel>"
RPL_INVEXLIST (346) --> sent as a reply to MODE command 
"<client> <channel> <mask>" 



// ERRORS
ERR_UNKNOWNERROR (400)
  "<client> <command>{ <subcommand>} :<info>"
ERR_NOSUCHNICK (401)
  "<client> <nickname> :No such nick/channel"
ERR_NOSUCHCHANNEL (403) 
  "<client> <channel> :No such channel"
ERR_CANNOTSENDTOCHAN (404)
  "<client> <channel> :Cannot send to channel"
ERR_NORECIPIENT (411) - PRIVMSG
  "<client> :No recipient given (<command>)"
ERR_NOTEXTTOSEND (412) - Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send.
  "<client> :No text to send"
ERR_UNKNOWNCOMMAND (421)
  "<client> <command> :Unknown command"
ERR_NONICKNAMEGIVEN (431)
  "<client> :No nickname given"
ERR_ERRONEUSNICKNAME (432) 
  "<client> <nick> :Erroneus nickname"
ERR_NICKNAMEINUSE (433)
  "<client> <nick> :Nickname is already in use"	
ERR_NICKCOLLISION (436) 
  "<client> <nick> :Nickname collision KILL from <user>@<host>"
ERR_USERNOTINCHANNEL (441) 
  "<client> <nick> <channel> :They aren't on that channel"
ERR_NOTONCHANNEL (442)
  "<client> <channel> :You're not on that channel"
ERR_USERONCHANNEL (443)
  "<client> <nick> <channel> :is already on channel"
ERR_NEEDMOREPARAMS (461)
  "<client> <command> :Not enough parameters"
ERR_ALREADYREGISTRED (462) 
  "<client> :You may not reregister"
ERR_PASSWDMISMATCH (464)
  "<client> :Password incorrect"
ERR_UMODEUNKNOWNFLAG (501)
  "<client> :Unknown MODE flag"

//QUIT & ERROR have their own messages

:dan-!d@localhost JOIN #test  
//current->user_nick + "!" + current->user_name + "@" + this->hostname + " MODE " + current->user_nick + ":+" + current->user_mode;
//return (":" + this->_name + " " + num + " " + nickname + " " + message + "\n");

const std::string  ERR_NEEDMOREPARAMS(int error_code)
{
	return (std::string (": " + this->user_nick + "!" + current->user_name + "@" + this->hostname + error_code + ":Not enough parameters" + "\n");
}

const std::string  ERR_ALREADYREGISTRED(int error_code)
{
	return (std::string (": " + this->user_nick + " " + error_code + this->user_nick + ": You may not reregister" + "\n");
}


