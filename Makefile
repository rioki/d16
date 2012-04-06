
CC      = gcc
LEX     = flex
CFLAGS += -g -Wall -O0


.PHONY: all clean

all: dasm16 dcpu16

# TODO clean should be platform independent
# http://stackoverflow.com/questions/10024486/portable-make-clean-rule
clean: 
	rm dasm16.exe dcpu16.exe *.o

dasm16: dasm16.o scan.o parse.o variant.o cpu.o generate.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	
dcpu16: dcpu16.o cpu.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
    
dasm16.o: dasm16.c parse.h generate.h
scan.o: scan.l scan.h variant.h
parse.o: parse.c parse.h scan.h variant.h cpu.h
variant.o: variant.c variant.h
generate.o: generate.c generate.h

dcpu16.o: dcpu16.c cpu.h
cpu.o: cpu.c cpu.h
