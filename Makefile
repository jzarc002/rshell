FLAGS = -Wall -Werror -ansi -pedantic
COMP = g++

all: 
	mkdir -p ./bin
	$(COMP) $(FLAGS) ./src/main.cpp -o ./bin/rshell
rshell:
	$(COMP) $(FLAGS) ./src/main.cpp -o ./bin/rshell
clean:
	rm -rf ./bin