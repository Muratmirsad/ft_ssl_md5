NAME = ft_ssl
FLAGS =
SRCS = main.c md5.c utils.c commands.c

all: $(NAME)

$(NAME): $(SRCS)
	@gcc $(FLAGS) $(SRCS) -o $(NAME)
	# @clear
	@echo "program created."

clean:
	@rm -rf $(NAME)
	@echo "program cleaned."

fclean: clean

re: clean all


# valgrind --leak-check=full ./ft_ssl and -g