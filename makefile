
.PHONY: compiler all clean
all: compiler

compiler:
	@$(MAKE) -C sources

tests:
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources
	@$(MAKE) clean -C tests
