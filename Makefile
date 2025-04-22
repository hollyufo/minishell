NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline -L/opt/homebrew/opt/readline/lib
CPPFLAGS = -I/opt/homebrew/opt/readline/include -Iincludes -ILibft

SRCS = srcs/main.c \
       srcs/execution.c \
       srcs/builtins.c \
       srcs/environment.c \
       srcs/parsing.c \
       srcs/utils.c

OBJS = $(SRCS:.c=.o)

LIBFT = Libft/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C Libft

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(MAKE) -C Libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C Libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
