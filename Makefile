CC = cc
ASM = nasm
LD = ld

CWARNINGS = -Wall -Wextra -Wpedantic
INCLUDES = -I$(HOME)/dev/fstdlib/

LDFLAGS = `pkg-config --libs xcb xcb-image` -lm -g
CFLAGS = -std=c11 $(CWARNINGS) $(INCLUDES) -g

NASMFLAGS = -felf64 -F dwarf -g

#ASMOBJFILES = multMatrix.o multVector.o normalizeTo3d.o doRotationMatrix.o setIdentityMatrix.o doTranslationMatrix.o
OBJFILES = mainX.o basic_draw.o asmUtil.o

mainx: $(OBJFILES) asmT
	$(CC) $(LDFLAGS) $(OBJFILES) -o $@

asmUtil.o: asm/*.o
	$(LD) $^ -r -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

asm/%.o: asm/%.asm
	$(ASM) $(NASMFLAGS) $<

clean:
	rm -f mainx
	rm -f *.o
	rm -f asm/*.o

.PHONY: clean asmT
