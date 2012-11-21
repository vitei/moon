BISON = bison
BISON_FLAGS = --verbose

C = clang
C_FLAGS = 
CPP_FLAGS =

CXX = clang++
CXX_FLAGS = 

FLEX = flex
FLEX_FLAGS = 

.INTERMEDIATE: _parser _lexer

all: compiler

_parser: moon.yy
	$(BISON) $(BISON_FLAGS) --defines=_parser.h -o _parser.cpp $<

_parser.h: _parser
_parser.cpp: _parser _lexer.h

_lexer: moon.ll
	$(FLEX) $(FLEX_FLAGS) --header-file=_lexer.h -o _lexer.cpp $<

_lexer.h: _lexer
_lexer.cpp: _lexer _parser.h

%.o: %.cpp %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

../bin/moon: _lexer.o _parser.o error.o loader.o main.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: ../bin/moon

clean:
	rm -rf _lexer.* _parser.* *.o tree/*.o ../bin/moon
