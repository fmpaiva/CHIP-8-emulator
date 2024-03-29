#OBJS specifies which files to compile as part of the project
OBJS = src/*.cpp 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
ERROR_FLAGS = -Wall -Wextra -Werror -Wconversion
COMPILER_FLAGS_DEBUG = -std=c++20 -g -Og
COMPILER_FLAGS_RELEASE = -std=c++20 -DRELEASE -O2

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME_DEBUG = bin/emulator_d
OBJ_NAME_RELEASE = bin/emulator 

#This is the target that compiles our executable
debug : $(OBJS)
	$(CC) $(OBJS) $(ERROR_FLAGS) $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS) -o $(OBJ_NAME_DEBUG)

release : $(OBJS)
	$(CC) $(OBJS) $(ERROR_FLAGS) $(COMPILER_FLAGS_RELEASE) $(LINKER_FLAGS) -o $(OBJ_NAME_RELEASE)

