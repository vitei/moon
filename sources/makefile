BISON = bison
BISON_FLAGS = 

C = clang
C_FLAGS = 
CPP_FLAGS =

CXX = clang++
CXX_FLAGS = 

FLEX = flex
FLEX_FLAGS = 

.INTERMEDIATE: parser.cpp parser.h lexer.cpp

all: compiler

parser.cpp: moon.yy
	$(BISON) $(BISON_FLAGS) --defines=parser.h -o $@ $<

parser.h: parser.cpp

lexer.cpp: moon.ll parser.h
	$(FLEX) $(FLEX_FLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

compiler: error.o lexer.o main.o parser.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

clean:
	rm -rf *.o compiler
