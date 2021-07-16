CXX = g++
CXXFLAGS = -Wall -Wno-unknown-pragmas
CXXLINKFLAGS = -pthread

EXEC_NAME = main

SRC_FILES = main.cpp light.cpp surface.cpp vector_math.cpp

LIBS = sfml-graphics sfml-system sfml-window sfml-audio sfml-network

LIB_PATH = /usr/local/lib64
INCLUDE_PATH = /usr/local/include/ ./include

# The rest is madness

SRC_FILES := $(addprefix src/,$(SRC_FILES))
OBJ_FILES = $(patsubst src/%,build/obj/%.o,$(basename $(SRC_FILES))) #Make them independent to .cpp or .c

LIBS := $(addprefix -l,$(LIBS))
LIB_PATH := $(addprefix -L,$(LIB_PATH))
INCLUDE_PATH := $(addprefix -I,$(INCLUDE_PATH))

all: prog
	./build/bin/$(EXEC_NAME)

prog: $(OBJ_FILES)
	$(CXX) $(CXXLINKFLAGS) $(LIB_PATH) $(LIBS) $^ -o build/bin/$(EXEC_NAME)

build/obj/%.o: src/%.c
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -o $@ $< 

build/obj/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -o $@ $< 

clean:
	rm -rf build/bin/$(EXEC_NAME) build/obj/*.o

test:
	@echo Source Files: $(SRC_FILES)
	@echo Object Files: $(OBJ_FILES)
	@echo Libs: $(LIBS)
	@echo Lib Path: $(LIB_PATH)
	@echo Include Path: $(INCLUDE_PATH)
	@echo Executable Name: $(EXEC_NAME)
	@echo Compiler: $(CXX) $(CXXFLAGS)
