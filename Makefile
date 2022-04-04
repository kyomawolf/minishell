
NAME = minishell

NAME_RDL = minishell_wrd

RDLINE := 0
LIB = -Llib -lft
RDLINE_LIB = -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -Wall -Wextra -Werror

#-fsanitize=address -fno-omit-frame-pointer -static-libsan

SNTZ	= -fsanitize=address -fno-omit-frame-pointer  -static-libsan


INC = -Iinclude -I$(HOME)/.brew/opt/readline/include
SRC = main.c input.c new.c get_next_line.c get_next_line_utils.c \
	  parser/b_tree.c parser/tree_builder1.c parser/tree_builder2.c \
	  parser/tree_builder3.c parser/tree_builder4.c \
	  parser/parser_io.c parser/parser_check1.c parser/parser_check2.c \
	  parser/lexer_append1.c parser/lexer_append2.c parser/lexer_append2_helper.c\
	  parser/lexer_append3.c parser/lexer_heredoc1.c parser/lexer_heredoc2.c \
	  parser/lexer_main.c \
	  parser/lexer_tokenlist.c parser/lexer_utils1.c parser/lexer_utils2.c \
	  parser/lexer_utils3.c parser/lexer_utils4.c \
	  executor/exec_builtin.c executor/exec_child.c executor/exec_executor.c\
	  executor/exec_expand_wildcard.c executor/pipe_count.c\
	  executor/exec_expand_heredoc1.c executor/exec_expand_heredoc2.c \
	  executor/exec_expand_io1.c executor/exec_expand_io2.c \
	  executor/exec_io_adjust_pipes.c \
	  executor/exec_expand_utils.c executor/exec_expand1.c \
	  executor/exec_expand2.c executor/exec_expand3.c\
	  executor/exec_expand4.c executor/exec_expand5.c executor/exec_io1.c \
	  executor/exec_io2.c executor/exec_main.c executor/exec_parent.c \
	  executor/exec_path.c executor/exec_pipeline.c executor/exec_utils.c \
	  executor/exec_utils2.c \
	  wildcard/newwild1.c wildcard/newwild2.c wildcard/newwild3.c wildcard/newwild4.c\
	  builtin/cd.c builtin/export1.c builtin/export2.c builtin/echo.c \
	  builtin/unset.c builtin/env.c builtin/exit.c builtin/pwd.c \
	  free.c free2.c free3.c global_utils.c llist.c DEBUG_print.c


OBJ_DIR = obj/

#BONUS_SRC =
OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c,%.o,$(SRC)))

$(NAME) : lib/libft.a $(OBJ)
ifeq ($(RDLINE),1)
	@gcc $(FLAGS) -O3 $(INC) $^ $(LIB) $(RDLINE_LIB) -o $@
else
	@gcc $(FLAGS) -O3 $(INC) $^ $(LIB) -o $@
endif
	@echo "compiled!"


all : $(NAME)

debug : fclean lib/libft.a $(OBJ) $(SET)
ifeq ($(RDLINE),1)
	@gcc $(FLAGS) -g $(INC) $^ $(LIB) $(RDLINE_LIB) -o $@
else
	@gcc $(FLAGS) -g $(INC) $^ $(LIB) -o $@
endif
	@echo "compiled!"

$(OBJ_DIR)%.o: %.c
ifeq ($(RDLINE),1)
	@gcc $(FLAGS) -DREADLINE $(INC) -O3 -c $< -o $@
else
	@gcc $(FLAGS) $(INC) -O3 -c $< -o $@
endif


re : fclean all

bonus : all

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
	rm -f ./minishell
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