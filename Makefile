##############################
### 	Declare some variables
##############################

CC = g++
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c++17
SRC_FILES = src/*.cpp src/*/*.cpp
INCLUDE_PATH = -I"./libs"
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OBJ_NAME = gameengine

##############################
### 	Make file rules
##############################

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SRC_FILES) $(INCLUDE_PATH) $(LINKER_FLAGS) -o ${OBJ_NAME};

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
