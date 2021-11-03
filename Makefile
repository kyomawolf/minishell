
NAME = minishell

LIB = -Llib -lft
FLAGS = -Wall -Werror -Wextra
INC = -Iinclude
SRC = 	pushswap.c sim.c sort.c utils.c utils_2.c utils_3.c input.c alg_one_1.c alg_one_2.c alg_two.c alg_hard.c \
		alg_two_helper.c reverse_five.c alg_five.c check.c chunks.c
INST = ./instructionset/instructionset.c
BONUS_SRC = ./tester/main.c ./tester/helper.c ./tester/check.c ./tester/helper_p2.c ./tester/input.c \
            ./tester/sorted.c ./tester/tester_instructions.c ./tester/fetch.c
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

bonus : tester


tester : libs
	@gcc $(FLAGS) $(BONUS_SRC) $(INC) $(LIB) -o c_tester
	@echo "compiled!"

libs :
	@make -silent -C lib/libft
	@cp -f lib/libft/libft.a lib
	@cp -f lib/libft/libft.h include
	@echo "libraries ready!"

test : libs
	@gcc $(SRC) $(INST) $(INC) $(LIB)
	@echo "compiled!"

clean :
	make clean -silent -C lib/libft/
	rm -f *.o *.~ *.d

fclean :
	make fclean -silent -C lib/libft/
	rm -f *.out *.o *.~ push_swap
	rm -f ./lib/*.a ./include/lbft.h
	rm -f ./instructionset/*.out ./instructionset/*.o ./instructionset/*.~

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