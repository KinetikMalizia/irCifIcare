#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.h"
# define MAXLINE 4096
# define SERVER_PORT 6667
# define POLL_SIZE 10
# define RED \x1b[31m
typedef std::vector<std::string>	t_svec;

class User;
class Channel;

class Server
{
	private:

	public:
		Server();
		Server(std::string host_name, std:: string portnumber, std:: string password);
		void check_args(char **av);
		void remove_from_poll(struct pollfd fds[], int i, int nfds);
		int  create_socket();
		int  accept_connection(int listenfd);
		int add_modes();
		int	add_client(int client_fd);
		int Nick(t_svec token);
		void find_cmd(t_svec recToken, int fd);
		void print_users();
		std:: string first_msg();
		void send_msg(std:: string msg, int msg_code); // reply to client --> create a container with all the possible messages and codes
		//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu 
		int	FillUserInfo(t_svec tokens, int user_fd);
		int	isNickUsed(std::string nick);
		int	channelExists(std::string name);
		std::string first_message(int fd);
		int welcome_msg();
		void check_user_pings();


		int	translate(std::string nick);
		~Server();

		
		int				listenfd;
		int				pollfd;
		int				on;
		struct pollfd	fds[POLL_SIZE];
		int				nfds;


		//in most cases, std::string is just a placeholder
		std::map<int,User*>				users; // usernickname and pointer on the client
		std::map<std::string, Channel*>	channels; //name of channel and pointer to channel
		std::string 					password;
		int								port;
		std::string						default_mode; // set it at launch
		std::string						hostname;
};

#endif 


		// The user modes available: 
        //    i - marks a users as invisible;
        //    s - marks a user for receipt of server notices;
        //    w - user receives wallops;
        //    o - operator flag.
		// If a user attempts to make themselves an operator using the "+o"
		// flag, the attempt should be ignored.  

