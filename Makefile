CC = cc
ASM = nasm
LD = ld

CWARNINGS = -Wall -Wextra -Wpedantic -Wno-trigraphs
INCLUDES = -I$(HOME)/dev/fstdlib/

LDFLAGS = `pkg-config --libs xcb xcb-image` -lm -g
CFLAGS = -std=c11 $(CWARNINGS) $(INCLUDES) -g -trigraphs

NASMFLAGS = -felf64 -F dwarf -g

ASMFILES = $(wildcard asm/*.asm)
OBJASMFILES = $(patsubst asm/%.asm,asm/%.o,$(ASMFILES)) a_draw.o
OBJFILES = mainX.o basic_draw.o asmUtil.o

mainx: $(OBJFILES) asmUtil.o
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJFILES) -o $@

regressionTest: regressionTest.c asmUtil.o
	$(CC) $(CFLAGS) $^ -lm -g -o $@
	./regressionTest

ham: ham.o
	$(CC) $< -o $@

a_draw.o: draw.asm
	$(ASM) $(NASMFLAGS) $< -o a_draw.o

asmUtil.o: $(OBJASMFILES)
	$(LD) $^ -r -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

asm/%.o: asm/%.asm
	$(ASM) $(NASMFLAGS) $<

clean:
	rm -f mainx regressionTest
	rm -f *.o
	rm -f asm/*.o

.PHONY: clean regressionTest
