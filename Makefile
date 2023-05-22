NAME = ircserv

SRCS = main.cpp parsing.cpp server.cpp user.cpp channel.cpp messages.cpp \
		handle_cmd.cpp commands.cpp commands2.cpp
OBJS = ${SRCS:.cpp=.o}

CXX = g++

RM = rm -f

# Rainbow colors
COLOR_RESET = \033[0m
COLOR_RED = \033[0;31m
COLOR_ORANGE = \033[0;33m
COLOR_YELLOW = \033[1;33m
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_INDIGO = \033[0;36m
COLOR_VIOLET = \033[1;35m

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(COLOR_BLUE)Linking$(COLOR_RESET) $@"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@printf "$(COLOR_GREEN)[####################]$(COLOR_RESET) 100%%\n"

%.o: %.cpp
	@printf "\n"
	@printf "              $(COLOR_GREEN)%s$(COLOR_RESET)\n" "$(subst .cpp,.o,$<)"
	@printf "             ╱\n"
	@printf "      ╭─────╯\n"
	@printf "      │\n"
	@printf "      ├─$(COLOR_RED)Compiling$(COLOR_RESET)\n"
	@printf "      │\n"
	@printf "      ├─$(COLOR_YELLOW)[███        ]$(COLOR_RESET)\r"
	@sleep 0.02
	@printf "      ├─$(COLOR_ORANGE)[█████      ]$(COLOR_RESET)\r"
	@sleep 0.02
	@printf "      ├─$(COLOR_YELLOW)[███████    ]$(COLOR_RESET)\r"
	@sleep 0.02
	@printf "      ├─$(COLOR_GREEN)[█████████  ]$(COLOR_RESET)\r"
	@sleep 0.02
	@printf "      ├─$(COLOR_BLUE)[███████████]$(COLOR_RESET)\r"
	@sleep 0.02
	@printf "      │\n"
	@printf "      └─$(COLOR_GREEN)Compiled$(COLOR_RESET)\n"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

run: $(NAME)
	@printf "\n\033[32;1mvvv output vvv \033[0m\n\n"
	@printf "\n\033[32;1m>>>no password setup<<<< \033[0m\n\n"
	@./$(NAME) 6667 NOPASSWD
	@printf "\n\n\n"
	@make clean

re: fclean all

.PHONY: all clean fclean re run
