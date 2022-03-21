CC=g++
CFLAGS=-g -Wall -Werror

C_SOURCES=$(wildcard *.cpp)

tlbcontroller: ${C_SOURCES}
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm -f tlbcontroller
