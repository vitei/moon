MOON = ../bin/moon
MOON_FLAGS = 

all: one.c

%.c: %.moon $(MOON)
	$(MOON) $(MOON_FLAGS) -o $@ $<

clean:
#	rm -rf *.o ../bin/compiler
#	rm -rf one
