CC?=gcc

all: elf  ptrace

program: elf.c
	$(CC) $< -o $@

trasse: ptrace.c
	$(CC) $< -o $@

run :  elf ptrace
	 ./elf ./nbody0
	./ptrace ./nbody0

clean:
	rm -f *.o ptrace elf

.PHONY: clean
