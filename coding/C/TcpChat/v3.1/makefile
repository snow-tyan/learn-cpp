SRCS:=$(wildcard *.c)                                                     
ELFS:=$(SRCS:%.c=%)
CC:=gcc
CFLAGS:=-g -Wall
all:$(ELFS)

%:%.c
	$(CC) $< -o $@

clean:
	rm -rf $(ELFS)