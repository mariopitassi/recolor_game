CC = gcc
CFLAGS = -g -Wall -std=c99
CPPFLAGS = -I
LDFLAGS = -L. -lgame
USERNAMES = mrousseau006 mpitassi akasparian fakhoun

all : recolor_text test_game_akasparian

test : test_akasparian

recolor_text : recolor_text.o libgame.a
	$(CC) -o recolor_text recolor_text.o $(LDFLAGS)

recolor_text.o : recolor_text.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

libgame.a: game_io.o game.o
	ar cr libgame.a game_io.o game.o

#test_mrousseau006 : test_game_mrousseau006.o libgame.a
#	$(CC) -o $@ $< $(LDFLAGS)

#test_mpitassi.o: test_mpitassi.c game_io.h game.h
#	$(CC) -c $(CFLAGS) $<

test_game_akasparian : test_game_akasparian.o libgame.a
	$(CC) -o $@ $< $(LDFLAGS)

test_game_akasparian.o: test_game_akasparian.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

#test_mpitassi : test_game_mpitassi.o libgame.a
#	$(CC) -o $@ $< $(LDFLAGS)

#test_fakhoun : test_game_fakhoun.o libgame.a
#	$(CC) -o $@ $< $(LDFLAGS)


.PHONY: clean
clean :
	rm -f recolor_text.o recolor_text libgame.a test_akasparian

.PHONY: test_akasparian
test_akasparian : test_game_akasparian
	./test_game_akasparian game_play_one_move
	./test_game_akasparian game_is_over
	./test_game_akasparian game_restart
