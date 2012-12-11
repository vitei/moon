THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/makefile.settings


.PHONY: compiler tests all clean
all: compiler tests

obj_dir:
	@mkdir -p $(OBJ_DIR)

compiler: obj_dir
	@$(MAKE) -C sources/compiler

tests: obj_dir
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources/compiler
	@$(MAKE) clean -C tests
	@rm -rf $(OBJ_DIR)
