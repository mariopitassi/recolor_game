CC = gcc
CFLAGS = -g -Wall -std=c99
CPPFLAGS = -I
LDFLAGS = -L. -lgame
USERNAMES = mrousseau006 mpitassi akasparian fakhoun

ALL: recolor_text test_game_mpitassi test_game_akasparian test_game_fakhoun test_game_mrousseau006

recolor_text : recolor_text.o libgame.a
	$(CC) -o recolor_text recolor_text.o $(LDFLAGS)

recolor_text.o : recolor_text.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

libgame.a: game_io.o game.o
	ar cr libgame.a game_io.o game.o

test_game_mrousseau006 : test_game_mrousseau006.o libgame.a
	$(CC) -o $@ $< $(LDFLAGS)

test_game_mrousseau006.o: test_game_mrousseau006.c game.h game_io.h
	$(CC) -c $(CFLAGS) $<

test_game_mpitassi: test_game_mpitassi.o libgame.a
	$(CC) -o $@ $< $(LDFLAGS)

test_game_mpitassi.o: test_game_mpitassi.c game.h game_io.h
	$(CC) -c $(CFLAGS) $<

test_game_akasparian: test_game_akasparian.o libgame.a
	$(CC) -o $@ $< $(LDFLAGS)

test_game_akasparian.o: test_game_akasparian.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

test_game_fakhoun: test_game_fakhoun.o libgame.a
	$(CC) -o $@ $< $(LDFLAGS)

test_game_fakhoun.o: test_game_fakhoun.c game_io.h game.h
	$(CC) -c $(CFLAGS) $<

.PHONY: clean test test_mpitassi test_akasparian test_fakhoun test

clean:
	rm -f recolor_text.o recolor_text libgame.a test_game_mpitassi test_game_mpitassi.o test_game_akasparian test_game_akasparian.o test_game_fakhoun test_game_fakhoun.o test_game_mrousseau006.o test_game_mrousseau006

test: test_mpitassi test_akasparian test_fakhoun test_mrousseau006

test_mpitassi: test_game_mpitassi
	./test_game_mpitassi game_new
	./test_game_mpitassi set_max_moves
	./test_game_mpitassi copy

test_akasparian: test_game_akasparian
	./test_game_akasparian game_play_one_move
	./test_game_akasparian game_is_over
	./test_game_akasparian game_restart

test_fakhoun: test_game_fakhoun
	./test_game_fakhoun nb_moves_max
	./test_game_fakhoun set_cell_init
	./test_game_fakhoun cell_current_color

test_mrousseau006: test_game_mrousseau006
	./test_game_mrousseau006 game_new_empty
	./test_game_mrousseau006 game_nb_moves_cur
	./test_game_mrousseau006 game_delete



