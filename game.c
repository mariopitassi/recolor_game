#include <stdlib.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"


typedef struct game_s {
    color *tab_init;
    color *tab_cur;
    uint size;
    uint move_max;
    uint move_cur;
} *game;


// Function to handle error
void error(char[100] err_mess) {
    fprintf(stderr, "Error: " + err_mess);
    exit(EXIT_FAILURE);
}

void game_restart(game g){
    
    if(g == NULL){
        error(Pointeur est nul);
    }

    for(int i = 0; i < g->size*g->size ; i++){

        g->tab_cur[i] = g->tab_init[i];
    }

    g->move_cur = 0;
}

uint game_nb_moves_max(game g){

    uint nb_moves_max = g->move_max;

    return nb_moves_max;
}

void game_delete (game g){

    if(g == NULL){
        error(Pointeur est nul);
    }

    else {
        free(g->tab_cur);
        free(g);
    }
}