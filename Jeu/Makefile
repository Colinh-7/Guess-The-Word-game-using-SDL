Jeux : main.o sdl.o data_struct.o strmanip.o
	gcc main.o sdl.o data_struct.o strmanip.o -o Jeux -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o : main.c sdl.h data_struct.h
	gcc main.c -c
sdl.o : sdl.c sdl.h strmanip.h
	gcc sdl.c -c
data_struct.o : data_struct.c data_struct.h strmanip.h
	gcc data_struct.c -c
strmanip.o : strmanip.c strmanip.h data_struct.h
	gcc strmanip.c -c
