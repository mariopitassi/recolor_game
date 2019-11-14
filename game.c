#include <stdlib.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"


typedef struct game_s {
    color *tab_init;
    color *tab_cur;
    uint size;
    uint moves_max;
    uint moves_cur;
} *game;



/**
 * @brief Gets the number of moves since the last start (or restart).
 * @param g the game
 * @return the value of the current number of moves
 * @pre @p g is a valid pointer toward a cgame structure
 **/
 
uint nb_moves_cur(cgame g){
    if (g==NULL){
        error("Pointeur nul");
    }

    return g->moves_cur;
}


/**
 * @brief Clones the game g
 * @param g a constant pointer on the game to clone
 * @return the clone of g
 * @pre @p g is a valid pointer toward a cgame structure
 **/

game game_copy(cgame g){
    if (g==NULL){
        error("Pointeur nul");
    }

    game g1 = game_new(g->tab_init, g->moves_max);
    /*game g1 = malloc(sizeof(game));
    if (g1==NULL){
        error("Probleme allocation structure");
    }

    g1->tab_cur = malloc((g->size*g->size)sizeof(color));
    if (g1->tab==NULL){
        error("Probleme allocation tab cur");
    }*/
    for (int i = 0; i < g->size; i++) {
        g1->tab_cur[i]=g->tab_cur[i]);
    }

    g1->moves_cur = g->moves_cur;

    return g1;    
}



/**
 * @brief Checks if the game is over.
 * @details The game is considered to be over if the all the cells in the grid
 * have the same color and the number of moves is smaller than or
 * equal to the maximum allowed number of moves
 * @return true, if the game is over, false otherwise
 * @param g the game
 * @pre @p g is a valid pointer toward a game structure
 **/

/* fonction aux pour game_is_over */
bool full_grid(cgame g){
    color c = game_cell_current_color(g, 0, 0);
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game_cell_current_color(g, j, i) != c) {
                return false;
            }
        }
    }

    return true;
}

bool game_is_over(cgame g){
    if (g==NULL){
        error("Pointeur nul");
    }

    if (g->moves_cur<=g->moves_max && full_gird(g)){
        return true;
    }

    return false;
}