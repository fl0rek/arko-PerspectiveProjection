CC = cc
CWARNINGS = -Wall -Wextra -Wpedantic
INCLUDES = -I$(HOME)/dev/fstdlib/
LDFLAGS = `pkg-config --libs xcb xcb-image` -lm
CFLAGS = -std=c11 $(CWARNINGS) $(INCLUDES)
OBJFILES = mainX.o basic_draw.o

mainx: $(OBJFILES)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f mainx
	rm -f *.o

.PHONY: clean
