#include "headers/server.hpp"
#include "headers/irc.h"

std::string Server::err_msg(int errorCode, int fd, std::string parameter1="", std::string parameter2="", std::string parameter3="", std::string info="")
{
	//User *current = (this->users).find(fd)->second;
	// (void)fd;
	std::stringstream ss;
	ss << errorCode;
	(void)info;
	std::string errorMessage = ss.str();

	switch(errorCode)
	{
		case 400:
			errorMessage += " UNKNOWNERROR " + parameter1 + parameter2 + " :Unknown error";
			break;
		case 401:
			errorMessage += " NOSUCHNICK " + parameter1 + " :No such nick/channel";
			break;
		case 403:
			errorMessage += " NOSUCHCHANNEL " + parameter1 + " :No such channel";
			break;
		case 404:
			errorMessage += " CANNOTSENDTOCHAN " + parameter1 + " :Cannot send to channel";
			break;
		case 411:
			errorMessage += " NORECIPIENT :No recipient given (" + parameter1 + ")";
			break;
		case 412:
			errorMessage += " NOTEXTTOSEND :No text to send";
			break;
		case 421:
			errorMessage += " UNKNOWNCOMMAND " + parameter1 + " :Unknown command";
			break;
		case 431:
			errorMessage += " NONICKNAMEGIVE :No nickname given";
			break;
		case 432:
			errorMessage += " ERRONEUSNICKNAME " + parameter1 + " :Erroneous nickname";
			break;
		case 433:
			errorMessage += " NICKNAMEINUSE " + parameter1 + " :Nickname is already in use";
			break;
		case 436:
			errorMessage += " NICKCOLLISION " + parameter1 + " :Nickname collision KILL from " + parameter2 + "@" + parameter3;
			break;
		case 441:
			errorMessage += " USERNOTINCHANNEL " + parameter1 + " " + parameter2 + " :They aren't on that channel";
			break;
		case 442:
			errorMessage += " NOTONCHANNEL " + parameter1 + " :You're not on that channel";
			break;
		case 443:
			errorMessage += " USERONCHANNEL " + parameter1 + " " + parameter2 + " :is already on channel";
			break;
		case 461:
			errorMessage += " NEEDMOREPARAMS " + parameter1 + " :Not enough parameters";
			break;
		case 462:
			errorMessage += " ALREADYREGISTRED :You may not reregister";
			break;
		case 464:
			errorMessage += " PASSWDMISMATCH :Password incorrect";
			break;
		case 471:
			errorMessage += " " + parameter1 + " " + parameter2 + " :Cannot join channel (+l)";
			break;
		case 472:
			errorMessage += " " + parameter1 + " " + parameter2 + " :is not a recognised channel mode.";
			break;
		case 473:
			errorMessage += " ERR_INVITEONYLCHAN " + parameter1 + " :Cannot join channel. (+i)";
			break;
		case 475:
			errorMessage += " ERR_BADPASSWORD " + parameter2 + " :Cannot join channel (incorrect channel key)";
			break;
		case 482:
			errorMessage += + " " + parameter1 + " " + parameter2 + " :You're not a channel operator";
			break;
		case 501:
			errorMessage += " UMODEUNKNOWNFLAG :Unknown MODE flag";
			break;
		case 650:
			errorMessage += parameter1 + ":<channel> [:<topic>]";
			break;
		case 696:
			errorMessage += + " " + parameter1 + " " + parameter2 + " " + parameter3 + " *" + " :You must specify a parameter for the " + parameter3 + " mode";
			break;
		default:
			errorMessage += " " + parameter1 + " :Unknown error";
			break;
	}
	errorMessage = ":" + this->hostname + " " + errorMessage + "\r\n" ;
	write(fd, errorMessage.c_str(), errorMessage.length());
	return errorMessage;
}



std::string Server::rpl_msg(int msg_code, int fd, std::string parameter1="", std::string parameter2="", std::string parameter3="", std::string info="")
{
	User *current = (this->users).find(fd)->second;
	(void)info;
	std::string rpl_message = current->user_nick + " ";
	std::stringstream ss;
	ss << msg_code;

	switch(msg_code)
	{
		case 001:
			rpl_message += "\x1b[31mWelcome to 2drunk2code server!!!" + current->user_nick +
		"!~" + current->user_nick + "@" + this->hostname;
			break;
		case 331:
			rpl_message += parameter1 + " :no topic is set";
			break;
		case 332:
			rpl_message += parameter1 + " :" + parameter2;
			break;
		case 336:
			rpl_message += parameter1;
			break;
		case 324:
			rpl_message += parameter1 + " " + parameter2 + " :" + parameter3;
			break;
		case 341:
			rpl_message += parameter1 + " " + parameter2 + " :" + parameter3;
			break;
		case 346:
			rpl_message += parameter1 + " " + parameter2;
			break;
		case 500:
			rpl_message += parameter1 + " " + parameter2 + " :" + parameter3;
			// [ server : 6667 ] :dede!~jcarlen@freenode-o6d.g28.dc9e5h.IP MODE #adc :-it
			break;
		default:
			rpl_message += " this but a default msg ";
	}
	rpl_message = ":" + this->hostname + " " + ss.str() + " " + rpl_message + "\r\n" ;
	write(fd, rpl_message.c_str(), rpl_message.length());
	return rpl_message;
}

std::string Server:: first_message(int fd)
{
	User *current = (this->users).find(fd)->second;

	return (":"+ this->hostname + " 001 " + current->user_nick + " :" + "\x1b[31mWelcome to 2drunk2code server!!!" + current->user_nick +
		"!~" + current->user_nick + "@" + this->hostname + "\r\n");
}

int Server:: welcome_msg()
{
	std:: cout << "\x1b[31mLet's get started!" << std:: endl;
	std::cout << "\x1b[38;5;255m----------------------------------------\nServer started, listening on port " << this->port << "\n----------------------------------------" << std::endl;
	return (0);
}

std::string Server:: gen_base_msg(int fd)
{
	User *current = (this->users).find(fd)->second;

	this->base_msg = ":" + current->user_nick + "!~" + current->user_name + "@" + this->hostname + " ";
	return (this->base_msg);
}
