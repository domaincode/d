
NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 
SRC = Auth_cmd/UTILS.cpp Auth_cmd/USER.cpp Auth_cmd/PASS.cpp Auth_cmd/NICK.cpp main.cpp Server.cpp Server_Utils.cpp Client.cpp \
	Client_Utils.cpp Functions.cpp Channel.cpp Channel_Utils.cpp Channel_cmd/JOIN.cpp Channel_cmd/MODE.cpp Channel_cmd/Mode_Utils.cpp \
	Channel_cmd/KICK.cpp Channel_cmd/TOPIC.cpp Channel_cmd/INVITE.cpp Channel_cmd/PRIVMSG.cpp Bot.cpp 

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(SRC) -o $(NAME)

%.o: %.cpp 
	$(CC) $(CPPFLAGS) -c $< -o $@

clean : 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	

re: fclean all