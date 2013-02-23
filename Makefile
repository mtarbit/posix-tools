CFLAGS = -Wall -Iinclude
ARFLAGS = cr
VPATH = src

targets = echo env false pwd true
archive = libpos.a

.PHONY: all clean

all: $(targets)
$(targets): $(archive)
$(archive): $(archive)(lib_pos_err.o)

clean:
	-rm $(archive) $(targets)
