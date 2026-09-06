CPP_COMPILER = g++
C_COMPILER = gcc
SRC_DIR = src

INCLUDE_DIR = -Idependencies
LIB_DIR = -Ldependencies/lib

LINKER_OPTIONS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

FLAGS = -Wall -Wextra -O2 $(INCLUDE_DIR) $(LIB_DIR) $(LINKER_OPTIONS)

SRC_FILES = $(foreach dir, $SRC_DIR, $(wildcard src/*.c src/*.cpp))
$(info $(SRC_FILES))
OBJECTS = $(patsubst %.cpp, %.o, $(patsubst %.c, %.o, $(SRC_FILES)))
$(info $(OBJECTS))

%.o: %.c
	$(C_COMPILER) $(FLAGS) $^ -c -o $@

%.o: %.cpp
	$(CPP_COMPILER) $(FLAGS) $^ -c -o $@

project: $(OBJECTS)
	$(CPP_COMPILER) $(FLAGS) $^ -o $@

all: project
	./project

clean:
	rm -rf src/glad.o src/main.o
