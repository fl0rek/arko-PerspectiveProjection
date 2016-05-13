CC = cc

sdl: mainSDL.c
	$(CC) $< -o $@
