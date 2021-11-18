# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/19 15:11:15 by jkasper           #+#    #+#              #
#    Updated: 2021/11/18 12:50:18 by jkasper          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isallnum.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
      ft_isdigit.c ft_isprint.c ft_itoa.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c \
      ft_memset.c ft_numlen.c ft_putchar_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_putnbbase.c ft_putendl_fd.c \
      ft_realloc.c ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
      ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_whitespaces.c \
	  ft_atol.c ft_char_arr_len.c \

BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstsize.c \
		ft_lstiter.c ft_lstmap.c

OBJ = $(patsubst %.c, %.o, $(SRC))

OBJ_BON = $(patsubst %.c, %.o, $(BONUS))

FLAGS = -Wall -Werror -Wextra

all : $(NAME) 
	
%.o : %.c
	gcc -c $(FLAGS) $< -o $@

$(NAME) : $(OBJ)
	ar -crs $(NAME) $(OBJ)

bonus : $(OBJ_BON) $(OBJ)
	ar -crs $(NAME) $(OBJ)

re : fclean all

fclean : clean
	rm -f $(NAME)

clean : 
	rm -f *.o *~ *.out
