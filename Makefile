#OBJS specifies which files to compile as part of the project
OBJS = 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS_DEBUG = -std=c++20 -Wall -Wextra -g -Og -pedantic

COMPILER_FLAGS_RELEASE = -DRELEASE -O2 -std=c++20
#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME_DEBUG = something_d
OBJ_NAME_RELEASE = something 

#This is the target that compiles our executable
debug : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS) -o $(OBJ_NAME_DEBUG)

release : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS_RELEASE) $(LINKER_FLAGS) -o $(OBJ_NAME_RELEASE)

