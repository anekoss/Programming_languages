ASM=nasm
ASMFLAGS=-f elf64
LD=ld

all: program

program: main.o lib.o dict.o
	$(LD) -o $@ $^

main.o: main.asm lib.inc words.inc dict.inc colon.inc

dict.o: dict.asm lib.inc

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -f *.o program

.PHONY: clean
