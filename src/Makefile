CC = gcc

CFLAGS = -ansi -Wall
OBJECTS = $(SOURCES:.c=.o)

default:
	@echo 'specify target'

image1.out: MUBARAKA-image1.c
	$(CC) $(CFLAGS) -o $@ $^

image2.out: MUBARAKA-image2.c
	$(CC) $(CFLAGS) -o $@ $^

image3.out: MUBARAKA-image3.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@rm -r *.out
