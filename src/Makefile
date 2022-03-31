CC=g++
CFLAGS=-g -Wall -Werror

C_SOURCES=$(wildcard *.cpp)
H_SOURCES=$(wildcard *.hpp)

INPUT_FILES=$(wildcard input/*)
OUTPUT_FILES=$(addprefix output/,$(notdir ${INPUT_FILES}))
outfile=tlbstatistics.txt

all: ${OUTPUT_FILES}
	@sed -n '/#define/s/#define//p' config.hpp > ${outfile}

tlbcontroller: ${C_SOURCES} ${H_SOURCES}
	$(CC) $(CFLAGS) ${C_SOURCES} -o $@

output/%: input/% tlbcontroller
	@rm -f $@
	./tlbcontroller $< | tee $@ >> ${outfile}

clean:
	rm -f tlbcontroller ${OUTPUT_FILES} ${outfile}
