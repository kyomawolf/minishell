
NAME = minishell

LIB = -Llib -lft
FLAGS = -Wall -Werror -Wextra
INC = -Iinclude
SRC = 	
BONUS_SRC = 
OBJ = $(patsubst %.c,%.o,$(SRC))

$(NAME) : $(OBJ)
	@gcc $(FLAGS) -O3 $(INC) $^ $(LIB) -o $@
	@echo "compiled!"

all : lib/libft.a $(NAME)

debug : fclean lib/libft.a $(OBJ) $(SET)
	gcc $(FLAGS) -g -O0 $(INC) $^ $(LIB) -o $@

%.o: %.c
	@gcc $(FLAGS) $(INC) -O3 -c $< -o $@ 

re : fclean all

bonus :

libs :
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
	rm -f *.out *.o *.~ $(NAME) ./parser/*.o
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