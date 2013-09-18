THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/makefile.settings


.PHONY: compiler runtime tests all clean
all: compiler runtime tests

compiler:
	@$(MAKE) -C sources/compiler

runtime:
	@$(MAKE) -C sources/runtime

tests:
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources/compiler
	@$(MAKE) clean -C sources/runtime
	@$(MAKE) clean -C tests
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LIB_DIR)
	@rm -rf $(BIN_DIR)
