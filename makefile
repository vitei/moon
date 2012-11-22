THIS_MAKEFILE_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})
include $(THIS_MAKEFILE_DIR)/makefile.settings


.PHONY: compiler tests all clean
all: compiler tests

compiler:
	@$(MAKE) -C sources/compiler

tests:
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources/compiler
	@$(MAKE) clean -C tests
