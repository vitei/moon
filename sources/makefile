BISON = bison
BISON_FLAGS = --verbose

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

%.o: %.cpp %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

../bin/moon: error.o lexer.o main.o parser.o tree/block.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: ../bin/moon

clean:
	rm -rf parser.output *.o tree/*.o ../bin/moon
