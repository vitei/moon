THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/../../makefile.settings

SOURCES:=$(shell find $(THIS_MAKEFILE_DIR) -type f -name "*.c")
OBJECTS:=$(patsubst $(THIS_MAKEFILE_DIR)/%,$(PLATFORM_OBJ_DIR)/%,$(SOURCES:.c=.o))
DEPS:=$(OBJECTS:.o=.deps)

all: $(LM)

# Dependencies
-include $(DEPS)

# Rules
include $(MOON_ROOT_DIR)/makefile.rules

$(LM): $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@$(AR) rcs $@ $^

clean:
	@rm -rf $(DEPS)
	@rm -rf $(OBJECTS)
	@rm -rf $(LM)
