# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcarlen <jcarlen@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/19 10:02:46 by fmalizia          #+#    #+#              #
#    Updated: 2023/04/20 13:40:43 by jcarlen          ###   ########.ch        #
#                                                                              #
# **************************************************************************** #

NAME = serv

SRCS =	main.cpp
OBJS = ${SRCS:.cpp=.o}

CXX  = g++

RM  = rm -f

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

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