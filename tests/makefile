THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/../makefile.settings

OUTPUT_DIR:=$(THIS_MAKEFILE_DIR)/bin
PASS_TESTS_DIR:=$(THIS_MAKEFILE_DIR)/should_pass

SOURCES:=$(shell find $(PASS_TESTS_DIR) -type f -name "*.moon")
BINARIES:=$(patsubst $(PASS_TESTS_DIR)/%,$(OUTPUT_DIR)/%,$(SOURCES:.moon=$(EXE_EXTENSION)))

MOON_FLAGS+=-B -I$(THIS_MAKEFILE_DIR)/includes -U$(THIS_MAKEFILE_DIR)/uses

all: $(BINARIES)

$(PASS_TESTS_DIR)/%.c: $(PASS_TESTS_DIR)/%.moon
	@echo $(<:.moon=$(EXE_EXTENSION)): $<
	@$(MOON) $(MOON_FLAGS) -o $@ $<

$(OUTPUT_DIR)/%: $(PASS_TESTS_DIR)/%.c
	@echo $(<:.c=$(EXE_EXTENSION)): $<
	@mkdir -p $(shell dirname $@)
	@$(CC) -o $@ $<

clean:
	@rm -rf $(OUTPUT_DIR)

