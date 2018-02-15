CF = -Wall --std=c++14 -g $(shell pkg-config --cflags)
LF = -Wall --std=c++14  -lGL -lGLEW -lSDL2 -lSDL2_image -lassimp
CXX = g++
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TRG = animation


all: $(TRG)


obj/%.o: src/%.cpp Makefile
	@mkdir -p obj/
	$(CXX) $(CF) $< -c -o $@


$(TRG): $(OBJ) Makefile
	$(CXX) $(OBJ) $(LF) -o $@


clean:
	rm -rf $(TRG) obj/
