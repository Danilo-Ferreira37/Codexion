NAME = codexion
CCF = cc -Wall -Wextra -Werror -pthread
SRCS =	codexion.c 		\
		parsing.c 		\
		init_structs.c 	\
		actions.c 		\
		actions_utils.c \
		time.c			\
		routine.c
OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CCF) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CCF) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re