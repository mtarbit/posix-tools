CFLAGS = -Wall -Iinclude
ARFLAGS = cr
VPATH = src

targets = basename cal cat dirname echo env false ls pwd true wc
archive = libpos.a

.PHONY: all clean

all: $(targets)

$(targets): $(archive)
$(archive): $(archive)(lib_pos_err.o)

wc: -lm

clean:
	-rm $(archive) $(targets)
