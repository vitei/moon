BISON = bison
BISON_FLAGS = --verbose

C = clang
C_FLAGS = 
CPP_FLAGS =

CXX = clang++
CXX_FLAGS = 

FLEX = flex
FLEX_FLAGS = 

.INTERMEDIATE: parser lexer

all: compiler

parser: moon.yy
	$(BISON) $(BISON_FLAGS) --defines=parser.h -o parser.cpp $<

parser.h: parser
parser.cpp: parser lexer.h

lexer: moon.ll
	$(FLEX) $(FLEX_FLAGS) --header-file=lexer.h -o lexer.cpp $<

lexer.h: lexer
lexer.cpp: lexer parser.h

%.o: %.cpp %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

../bin/moon: error.o lexer.o main.o parser.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: ../bin/moon

clean:
	rm -rf lexer.* parser.* *.o tree/*.o ../bin/moon
