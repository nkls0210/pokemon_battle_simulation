CXX       := /usr/bin/g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -fdiagnostics-color=always -g

SRC     := src
INCLUDE := include

all: clean main.exe

main.exe: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@


run: all
	./main.exe

clean:
	rm -f *.exe
