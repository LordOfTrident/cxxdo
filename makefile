SRC  = $(wildcard src/*.cc)
DEPS = $(wildcard src/*.hh)
OBJ  = $(addsuffix .o,$(subst src/,bin/,$(basename $(SRC))))

BIN = ./bin
OUT = $(BIN)/app

CXX       = g++
CXX_VER   = c++17
CXX_FLAGS = -O3 -std=$(CXX_VER) -Wall -Wextra -Werror \
            -pedantic -Wno-deprecated-declarations
CXX_LIBS = -lncurses

compile: $(BIN) $(OBJ) $(SRC)
	$(CXX) $(CXX_FLAGS) -o $(OUT) $(OBJ) $(CXX_LIBS)

bin/%.o: src/%.cc $(DEPS)
	$(CXX) -c $< $(CXX_FLAGS) -o $@

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -r $(BIN)/*

install:
	sudo cp -f $(OUT) /usr/bin/cxxdo

all:
	@echo compile, clean, install
