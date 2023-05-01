NAME = serv

SRCS =	main.cpp parsing.cpp server.cpp user.cpp channel.cpp messages.cpp
OBJS = ${SRCS:.cpp=.o}

CXX  = g++

RM  = rm -f

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

run: $(NAME)
	@printf "\n\033[32;1mvvv output vvv \033[0m\n\n"
	@./serv
	@printf "\n\n\n"
	@ make clean

re:		fclean all

.PHONY:	all clean fclean re run