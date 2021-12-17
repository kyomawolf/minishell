
NAME = minishell
LIB = -Llib -lft -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -Wall -Werror -Wextra

#-fsanitize=address -fno-omit-frame-pointer -static-libsan

SNTZ	= -fsanitize=address -fno-omit-frame-pointer  -static-libsan


INC = -Iinclude -I$(HOME)/.brew/opt/readline/include
SRC = main.c input.c new.c parser/b_tree.c parser/tree_builder1.c \
      parser/max_lexer.c parser/parser_check.c free.c free2.c DEBUG_print.c \
	  executor/exec_expand_wildcard.c executor/pipe_count.c llist.c \
	  executor/executor.c executor/exec_expand.c global_utils.c \
	  wildcard/wild1.c wildcard/wild2.c wildcard/wild3.c parser/tree_builder2.c \
	  parser/tree_builder3.c \
	  executor/exec_builtin.c executor/exec_child.c executor/exec_io.c \
	  executor/exec_main.c executor/exec_parent.c executor/exec_path.c

#./parser/par_main.c ./parser/par_utils1.c ./parser/brackets.c b_tree.c

OBJ_DIR = obj/

#BONUS_SRC =
OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c,%.o,$(SRC)))

$(NAME) : lib/libft.a $(OBJ)
	@gcc $(FLAGS) -O0 $(INC) $^ $(LIB) -o $@

	@echo "compiled!"

all : lib/libft.a $(NAME)

debug : fclean lib/libft.a $(OBJ) $(SET)
	gcc $(FLAGS) -g -O0 $(INC) $^ $(LIB) -o $@

$(OBJ_DIR)%.o: %.c
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
	- rm -f $(OBJ)
	@echo "object directory cleaned!"

fclean :
	make fclean -silent -C lib/libft/
	rm -f $(OBJ)
	rm -f ./lib/*.a ./include/libft.h
	@echo "fully cleaned!"

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