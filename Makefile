# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gt-serst <gt-serst@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/25 17:00:33 by gt-serst          #+#    #+#              #
#    Updated: 2023/08/25 17:01:56 by gt-serst         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				= philo

CC					= gcc

CFLAGS				= -Wall -Wextra -Werror

RM					= rm -rf

SRCS				= main.c \
					  init.c \
					  launch.c \
					  utils.c \
					  exit.c \

OBJS				= $(addprefix srcs/, $(SRCS:.c=.o))

.c.o:
					$(CC) $(CFLAGS) -c -I ./includes $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
					$(CC) -o $(NAME) $(OBJS)

clean:
					$(RM) $(OBJS)

fclean: clean
					$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
