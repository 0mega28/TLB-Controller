CC=g++
CFLAGS=-g -Wall -Werror

C_SOURCES=$(wildcard *.cpp)

tlbcontroller: ${C_SOURCES}
	$(CC) $(CFLAGS) $^ -o $@

run: scripts/runall.sh
	./scripts/runall.sh

clean:
	rm -f tlbcontroller
