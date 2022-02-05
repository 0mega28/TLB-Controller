CC=g++
CFLAGS=-g -Wall -Werror

main: main.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm -f main