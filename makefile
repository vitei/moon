THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/makefile.settings


.PHONY: compiler documentation tests all clean
all: compiler documentation tests

compiler:
	@$(MAKE) -C sources/compiler

documentation:
	@$(MAKE) html -C documentation

tests:
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources/compiler
	@$(MAKE) clean -C documentation
	@$(MAKE) clean -C tests
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
