NAME = serveur

HEADER = includes/serveur.h

SRC = src/serveur_main.c src/connection.c src/communication.c src/tools.c \
			src/cmd_clients_manager.c src/input_handler.c src/client_hdl.c \
			src/team_hdl.c src/serveur_data.c src/serveur_loop.c

OBJ = $(SRC:.c=.o)

CC = gcc -g -Wall -Werror -Wextra

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
