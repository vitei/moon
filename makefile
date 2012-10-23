
.PHONY: compiler all clean
all: compiler

compiler:
	@$(MAKE) -C sources

clean:
	@$(MAKE) clean -C sources
