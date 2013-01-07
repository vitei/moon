THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/../../makefile.settings

SOURCES := $(shell find $(THIS_MAKEFILE_DIR) -type f -name "*.cpp")
OBJECTS := $(OBJ_DIR)/generated_lexer.o $(OBJ_DIR)/generated_parser.o
OBJECTS += $(patsubst $(THIS_MAKEFILE_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:.cpp=.o))
DEPS := $(OBJECTS:.o=.deps)

.INTERMEDIATE: generated_lexer.cpp generated_parser.cpp

all: compiler

# Dependencies
-include $(DEPS)

generated_parser.cpp: moon.yy moon.ll
	@echo $@: $<
	@mkdir -p $(INC_DIR)/generated
	@$(BISON) $(BISON_FLAGS) --defines=$(INC_DIR)/generated/parser.h -o generated_parser.cpp $<

generated_lexer.cpp: moon.ll moon.yy
	@echo $@: $<
	@mkdir -p $(INC_DIR)/generated
	@$(FLEX) $(FLEX_FLAGS) --header-file=$(INC_DIR)/generated/lexer.h -o generated_lexer.cpp $<

$(OBJ_DIR)/generated_lexer.o: generated_parser.cpp
$(OBJ_DIR)/generated_parser.o: generated_lexer.cpp

# Rules
include $(MOON_ROOT_DIR)/makefile.rules

$(BIN_DIR)/moon: $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ $^

compiler: $(BIN_DIR)/moon

clean:
	@rm -rf generated_*
	@rm -rf $(INC_DIR)/generated
	@rm -rf $(DEPS)
	@rm -rf $(OBJECTS)
	@rm -rf $(BIN_DIR)/moon
