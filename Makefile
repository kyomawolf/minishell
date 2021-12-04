
NAME = minishell
LIB = -Llib -lft -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -Wall -Werror -Wextra
#-fsanitize=address -fno-omit-frame-pointer -static-libsan

SNTZ	= -fsanitize=address -fno-omit-frame-pointer  -static-libsan

INC = -Iinclude -I$(HOME)/.brew/opt/readline/include
SRC = main.c input.c new.c parser/b_tree.c parser/tree_builder.c \
      parser/max_lexer.c wild.c parser/parser_check.c free.c DEBUG_print.c \
	  executor/expander.c executor/expander_wildcard.c

#./parser/par_main.c ./parser/par_utils1.c ./parser/brackets.c b_tree.c

BONUS_SRC =
OBJ = $(patsubst %.c,%.o,$(SRC))

$(NAME) : $(OBJ) lib/libft.a
	@gcc $(FLAGS) -O0 $(INC) $^ $(LIB) -o $@

	@echo "compiled!"

all : lib/libft.a $(NAME)

debug : fclean lib/libft.a $(OBJ) $(SET)
	gcc $(FLAGS) -g -O0 $(INC) $^ $(LIB) -o $@

%.o: %.c
	@gcc $(FLAGS) $(INC) -O0 -c $< -o $@

re : fclean all

bonus :

lib/libft.a :
	@make -silent -C lib/libft
	@cp -f lib/libft/libft.a lib
	@cp -f lib/libft/libft.h include
	@echo "libraries ready!"

test : libs
	@gcc $(SRC) $(INC) $(LIB)
	@echo "compiled!"

clean :
	make clean -silent -C lib/libft/
	rm -f *.o *.~ *.d

fclean :
	make fclean -silent -C lib/libft/
	rm -f *.out *.o *.~ $(NAME) ./parser/*.o ./executor/*.o
	rm -f ./lib/*.a ./include/libft.h

help :
	@echo "rules:	all [default]"
	@echo "	re"
	@echo "	test"
	@echo "	clean"
	@echo "	fclean"
	@echo "	bonus"
	@echo "	tester"

-include $(DEP)

.PHONY : tester bonus fclean test re all clean