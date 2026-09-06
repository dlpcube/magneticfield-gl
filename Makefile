CPP_COMPILER = g++
C_COMPILER = gcc
SRC_DIR = src

INCLUDE_DIR = -Idependencies/include
CPP_FLAGS = -Wall -Wextra -O2 $(INCLUDE_DIR)
C_FLAGS = -Wall -Wextra -O2 $(INCLUDE_DIR)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	MAC_GLFW_FLAGS := $(shell pkg-config --libs glfw3 2>/dev/null)
	ifeq ($(strip $(MAC_GLFW_FLAGS)),)
		MAC_GLFW_FLAGS = -Ldependencies/library -lglfw.3.4
	endif
	LINKER_OPTIONS = $(MAC_GLFW_FLAGS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	OUTPUT = project
else ifeq ($(OS),Windows_NT)
	LINKER_OPTIONS = -lglfw3 -lopengl32 -lgdi32
	OUTPUT = project.exe
else
	LINKER_OPTIONS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	OUTPUT = project
endif

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(patsubst %.c, %.o, $(SRC_FILES)))

%.o: %.c
	$(C_COMPILER) $(C_FLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -c $< -o $@

$(OUTPUT): $(OBJECTS)
	$(CPP_COMPILER) $(OBJECTS) $(LINKER_OPTIONS) -o $@

all: $(OUTPUT)

clean:
	rm -f $(OBJECTS) project project.exe
