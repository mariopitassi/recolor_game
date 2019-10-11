CC = gcc
CFLAGS = -g -Wall -std=c99
CPPFLAGS = -I
LDFLAGS = -L. -lgame

recolor_text : recolor_text.o libgame.a
	$(CC) -o recolor_text recolor_text.o $(LDFLAGS)

recolor_text.o : recolor_text.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

libgame.a: game_io.o game.o
	ar cr libgame.a game_io.o game.o

.PHONY: clean
clean :
	rm -f recolor_text.o recolor_text libgame.a
