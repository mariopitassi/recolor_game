#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ********** INITIALISATION DU TABLEAU ********** */

color tab[] = {
        0,0,0,2,0,2,1,0,1,0,3,0,
        0,3,3,1,1,1,1,3,2,0,1,0,
        1,0,1,2,3,2,3,2,0,3,3,2,
        2,3,1,0,3,2,1,1,1,2,2,0,
        2,1,2,3,3,3,3,2,0,1,0,0,
        0,3,3,0,1,1,2,3,3,2,1,3,
        1,1,2,2,2,0,0,1,3,1,1,2,
        1,3,1,3,1,0,1,0,1,3,3,3,
        0,3,0,1,0,0,2,1,1,1,3,0,
        1,3,1,0,0,0,3,2,3,1,0,0,
        1,3,3,1,1,2,2,3,2,0,0,2,
        2,0,2,3,0,1,1,1,2,3,0,1 
};


/* ********** TEST DE GAME_NB_MOVES_MAX ********** */

bool test_nb_moves_max(int k){
    game g = game_new(tab, k);

    if (g == NULL){
        fprintf(stderr, "Error: invalid game init!\n");
        return false;
    }

    if (game_nb_moves_max(g) != k) {
        fprintf(stderr, "Error: invalid game nb moves max (game_new)!\n");
        game_delete(g);
        return false;
    }

    game g2 = game_new_empty();

    if (g2 == NULL){
        fprintf(stderr, "Error: invalid game init!\n");
        return false;
    }

    if (game_nb_moves_max(g2) != 0) {
        fprintf(stderr, "Error: invalid game nb moves max (game_new_empty)!\n");
	game_delete(g);
        game_delete(g2);
        return false;
    }

    game_set_max_moves(g, 2*k);

    if (game_nb_moves_max(g) != 2*k) {
        fprintf(stderr, "Error: invalid game nb moves max (game_set_max_moves(g))!\n");
        game_delete(g);
	game_delete(g2);
        return false;
    }

    game_set_max_moves(g2, k);

    if (game_nb_moves_max(g2) != k) {
        fprintf(stderr, "Error: invalid game nb moves max (game_set_max_moves(g2))!\n");
	game_delete(g);
        game_delete(g2);
        return false;
    }



    game_delete(g);
    game_delete(g2);

    return true;
}

/* ********** TEST DE GAME_SET_CELL_INIT ********** */

bool test_set_cell_init(uint k) {
    game g = game_new(tab, k);

    if (g == NULL){
        fprintf(stderr, "Error: invalid game init!\n");
        return false;
    }

    for (uint i = 0; i < SIZE; i++) {
        for (uint j = 0; j < SIZE; j++) {
            color col = rand()%4;
            game_set_cell_init(g, i, j, col);
            if (game_cell_current_color(g, i, j) != col) {
                fprintf(stderr, "Error: invalid cell color!\n");
                game_delete(g);
                return false;
            }
        }
    }
	
    game g2 = game_new_empty();

    if (g2 == NULL){
        fprintf(stderr, "Error: invalid game init!\n");
        return false;
    }

    for (uint i = 0; i < SIZE; i++) {
        for (uint j = 0; j < SIZE; j++) {
            color col = rand()%4;
            game_set_cell_init(g2, i, j, col);
            if (game_cell_current_color(g2, i, j) != col) {
                fprintf(stderr, "Error: invalid cell color!\n");
		game_delete(g);
                game_delete(g2);
                return false;
            }
        }
    }

    /* essai anouche */

    //game g3 = game_new(tab, k);
    game g3 = game_new_empty();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            color col = tab[12*j+i];
            game_set_cell_init(g3, i, j, col);
        }
    }
    color ran_moves[]={2,1,0,3,0,1,2,3,1,0,2,0};
    for (int n=0; n<12; n++){
        game_play_one_move(g3, ran_moves[n]);
    }
    game_restart(g3);
    for (int i = 0; i <= SIZE - 1; i++) {
        for (int j = 0; j <= SIZE - 1; j++) {
            if (game_cell_current_color(g3, i, j) != tab[12*j+i]) {
                fprintf(stderr, "Error: pb apres restart\n");
		game_delete(g);	
		game_delete(g2);
                game_delete(g3);
                return false;
            }
        }
    }
    game_delete(g);
    game_delete(g2);
    game_delete(g3);
    return true;
}

/* ********** TEST DE GAME_CELL_CURRENT_COLOR ********** */

bool test_cell_current_color() {
    game g = game_new(tab, 25);

    if (g == NULL){
        fprintf(stderr, "Error: invalid game init! (game_new) \n");
        return false;
    }

    for (uint i = 0; i < SIZE; i++) {
        for (uint j = 0; j < SIZE; j++) {
            if (game_cell_current_color(g, i, j) != tab[12*j+i]) {
                fprintf(stderr, "Error: invalid cell color! (game_new) \n");
                game_delete(g);
                return false;
            }
	    color col = rand()%4;
	    game_set_cell_init(g, i, j, col);
	    if (game_cell_current_color(g, i, j) != col) {
                fprintf(stderr, "Error: invalid cell color! (game_new--set_cell_init)\n");
                game_delete(g);
                return false;
            }
        }
    }

    game g2 = game_new_empty();

    if (g2 == NULL){
        fprintf(stderr, "Error: invalid game init! (game_new_empty)\n");
        return false;
    }

    for (uint i = 0; i < SIZE; i++) {
        for (uint j = 0; j < SIZE; j++) {
            if (game_cell_current_color(g2, i, j) != 0) {
                fprintf(stderr, "Error: invalid cell color! (game_new_empty)\n");
		game_delete(g);
                game_delete(g2);
                return false;
            }
	    color col = rand()%4;
	    game_set_cell_init(g2, i, j, col);
	    if (game_cell_current_color(g2, i, j) != col) {
                fprintf(stderr, "Error: invalid cell color! (game_new_empty--set_cell_init)\n");
		game_delete(g);
                game_delete(g2);
                return false;
            }
        }
    }

    game_delete(g);
    game_delete(g2);
    return true;
}


/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}


/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
    if (argc == 1) usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test fakhoun \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("nb_moves_max", argv[1]) == 0)
        ok = test_nb_moves_max(100);
    else if (strcmp("set_cell_init", argv[1]) == 0)
        ok = test_set_cell_init(100);
    else if (strcmp("cell_current_color", argv[1]) == 0)
        ok = test_cell_current_color();
    else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
