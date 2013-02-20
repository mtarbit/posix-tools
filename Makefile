CFLAGS = -Wall

objects = true false pwd echo

all: $(objects)

clean:
	rm $(objects)
