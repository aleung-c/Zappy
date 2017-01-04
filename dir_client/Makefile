NAME = client

HEADER = includes/ClientMain.hpp

SRC = src/main.cpp \
src/ClientInputHandler.cpp \
src/ClientConnection.cpp \
src/ClientPlayer.cpp \
src/CustomException.cpp \
src/ClientController.cpp


OBJ = $(SRC:.cpp=.o)

CC = g++ -g -Wall -Werror -Wextra

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
