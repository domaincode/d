
NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address 
SRC = Auth_cmd/UTILS.cpp Auth_cmd/USER.cpp Auth_cmd/PASS.cpp Auth_cmd/NICK.cpp main.cpp Server.cpp Server_Utils.cpp Client.cpp \
	Client_Utils.cpp Functions.cpp Channel.cpp Channel_Utils.cpp Channel_cmd/JOIN.cpp Channel_cmd/MODE.cpp Channel_cmd/Mode_Utils.cpp \
	Channel_cmd/KICK.cpp Channel_cmd/TOPIC.cpp Channel_cmd/INVITE.cpp Channel_cmd/PRIVMSG.cpp Bot.cpp 
HEADERS = NonBlockingSocket.hpp\
			Server.hpp Channel.hpp\
			Client.hpp\

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)
	
%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean : 
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all