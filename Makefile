NAME = codexion
CCF = cc -Wall -Wextra -Werror -pthread
SRCS =	codexion.c 		\
		parsing.c 		\
		init_structs.c 	\
		actions.c 		\
		actions_utils.c \
		time.c			\
		scheduler.c     \
		routine.c
OBJS = $(SRCS:.c=.o)
RM = @rm -f

all: $(NAME)

$(NAME): $(OBJS) 
	@$(CCF) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	@$(CCF) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(OBJS)
	$(RM) $(NAME)
	clear

re: fclean all

r: all clean
	clear
help:
	@echo "\033[35mAvailable Make commands:\033[0m"
	@echo ""
	@echo "\033[33mmake\033[0m         Compile the program."
	@echo "\033[33mclean\033[0m        Remove objects (files.o) to keep the project environment clean."
	@echo "\033[33mfclean\033[0m       Remove objects (files.o), the program name and clears the terminal."
	@echo "\033[33mre\033[0m           Remove temporary files (objects, program name) and compile again the program."
	@echo "\033[33mr\033[0m            Compile the program, remove objects (files.o) and clear terminal."

.DEFAULT:
	@echo "\033[31mError: Unknown command.\033[0m"
	@echo "Use \033[33mmake help\033[0m to see all available commands."

.PHONY: all clean fclean re .DEFAULT help