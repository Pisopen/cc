CC=gcc
CFLAGS=-O2 -Wall
INC=-Iinc

all: cc.exe

obj/%.o: src/%.c inc/%.h
	mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS) $(INC)

cc.exe: obj/cc.o
	$(CC) -o $@ $^

clean:
	rm -f obj/*
	rm -f cc

remake: clean all

.PHONY: all clean remake