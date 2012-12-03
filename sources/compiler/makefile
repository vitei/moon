THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/../../makefile.settings

.INTERMEDIATE: generated_lexer.cpp generated_lexer.h generated_parser.cpp generated_parser.h

all: compiler

generated_parser.cpp: moon.yy moon.ll
	$(BISON) $(BISON_FLAGS) --defines=generated_parser.h -o generated_parser.cpp $<

generated_lexer.cpp: moon.ll moon.yy
	$(FLEX) $(FLEX_FLAGS) --header-file=generated_lexer.h -o generated_lexer.cpp $<

generated_lexer.o: generated_parser.cpp
generated_parser.o: generated_lexer.cpp

%.o: %.cpp
	$(CXX) -c $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $<

$(BIN_DIR)/moon: generated_lexer.o generated_parser.o error.o loader.o main.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: $(BIN_DIR)/moon

clean:
	rm -rf generated_lexer.* generated_parser.* *.o $(BIN_DIR)/moon
