# Defining makefile variables
CC = g++
CFLAGS = -g -Wall -std=c++11 -c -Iinclude -o
SRCFILES = ./src/Studio.cpp ./src/Action.cpp ./src/Customer.cpp ./src/Trainer.cpp ./src/Workout.cpp ./src/main.cpp
OBJFILES = ./bin/Studio.o ./bin/Action.o ./bin/Customer.o ./bin/Trainer.o ./bin/Workout.o ./bin/main.o

# Default target, executed directly after the "make" command
all: ./bin/studio

# Links the project's .o files into studio.exe
./bin/studio: ./bin/*.o
	@echo "Linking object files"
	g++ -o ./bin/studio $(OBJFILES)
	@echo "Finished linking"

# Compiles all the .cpp files in src dirctory
./bin/*.o: $(SRCFILES)
	@echo "Compiling source code"
	$(CC) $(CFLAGS) ./bin/Studio.o ./src/Studio.cpp
	$(CC) $(CFLAGS) ./bin/Action.o ./src/Action.cpp
	$(CC) $(CFLAGS) ./bin/Customer.o ./src/Customer.cpp
	$(CC) $(CFLAGS) ./bin/Trainer.o ./src/Trainer.cpp
	$(CC) $(CFLAGS) ./bin/Workout.o ./src/Workout.cpp
	$(CC) $(CFLAGS) ./bin/main.o ./src/main.cpp
	@echo "Finished compiling"

# Cleans all the files in bin directory
clean:
	@echo "Cleaning bin directory"
	rm -rf bin/*