THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/../../makefile.settings

.INTERMEDIATE: __parser.cpp __parser.h __lexer.cpp __lexer.h

all: compiler

__parser: moon.yy
	$(BISON) $(BISON_FLAGS) --defines=__parser.h -o __parser.cpp $<

__parser.h: __parser
__parser.cpp: __parser __lexer.h

__lexer: moon.ll
	$(FLEX) $(FLEX_FLAGS) --header-file=__lexer.h -o __lexer.cpp $<

__lexer.h: __lexer
__lexer.cpp: __lexer __parser.h

%.o: %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

$(BIN_DIR)/moon: __lexer.o __parser.o error.o loader.o main.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: $(BIN_DIR)/moon

clean:
	rm -rf __lexer.* __parser.* *.o frontend/*.o tree/*.o $(BIN_DIR)/moon
