
.PHONY: compiler tests all clean
all: compiler tests

compiler:
	@$(MAKE) -C sources

tests:
	@$(MAKE) -C tests

clean:
	@$(MAKE) clean -C sources
	@$(MAKE) clean -C tests
