CFLAGS = -Wall -Iinclude
ARFLAGS = cr
VPATH = src

ifeq ($(shell uname), Darwin)
	.LIBPATTERNS = lib%.dylib lib%.so lib%.a
endif

targets = basename cal cat dirname echo env false ls pwd rm true wc
archive = lib-posix-tools.a

.PHONY: all clean

all: $(targets)

$(targets): $(archive)
$(archive): $(archive)(posix-tools.o)

wc: -lm

clean:
	-rm $(archive) $(targets)
