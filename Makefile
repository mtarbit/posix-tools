CFLAGS = -Wall -Iinclude
ARFLAGS = cr
VPATH = src

targets = cal cat echo env false ls pwd true
archive = libpos.a

.PHONY: all clean

all: $(targets)
$(targets): $(archive)
$(archive): $(archive)(lib_pos_err.o)

clean:
	-rm $(archive) $(targets)
