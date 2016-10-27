FLAGS = -Wall -Werror -ansi -pedantic
COMP = g++

all: 
      mkdir -p ./bin
      $(COMP) $(FLAGS) ./src/Command.cpp -o ./bin/rshell
      $(COMP) $(FLAGS) ./src/Connectors.cpp -o ./bin/rshell
      
rshell:
      
      $(COMP) $(FLAGS) ./src/Command.cpp -o ./bin/rshell
      $(COMP) $(FLAGS) ./src/Connectors.cpp -o ./bin/rshell
      
remove:

      rm -rf ./bin
