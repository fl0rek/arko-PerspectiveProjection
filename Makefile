CC = cc
CWARNINGS = -Wall -Wextra -Wpedantic
INCLUDES = -I$(HOME)/dev/fstdlib/
LDFLAGS = `pkg-config --libs xcb xcb-image` -lm -g
CFLAGS = -std=c11 $(CWARNINGS) $(INCLUDES) -g
OBJFILES = mainX.o basic_draw.o

mainx: $(OBJFILES)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f mainx
	rm -f *.o

.PHONY: clean
