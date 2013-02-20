MOON = ../bin/moon
MOON_FLAGS = -B

all: one

one: one.c
	@$(CC) $(CC_FLAGS) $(CXX_FLAGS) -o $@ $<

%.c: %.moon $(MOON)
	$(MOON) $(MOON_FLAGS) -o $@ $<

clean:
#	rm -rf *.o ../bin/compiler
	rm -rf one
