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
        error("Pointeur est nul");
    }
    else {
        for(int i = 0; i < g->size*g->size ; i++){

        g->tab_cur[i] = g->tab_init[i];
        }

        g->move_cur = 0;
    }

}

uint game_nb_moves_max(game g){
    if (g == NULL){
        error("g is not a valid pointer");
        exit(EXIT_FAILURE);
    }
    uint nb_moves_max = g->move_max;

    return nb_moves_max;
}

void game_delete (game g){

    if(g == NULL){
        error("Pointeur est nul");
    }

    else {
        free(g->tab_cur);
        free(g);
    }
}

/**
 * @brief Creates a new game and initializes it in one call.
 * @param cells 1D array describing the color of each cell of the game. The storage is row by row
 * @param nb_max_moves the value of the maximum number of moves
 * @return the created game
 * @pre @p cells is an initialized array of size SIZE*SIZE.
 * @pre @p nb_max_moves > 0
 **/
game game_new(color *cells, uint nb_moves_max){                     
    game g = malloc(sizeof(game));
    if (g == NULL) 
        error("g allocation went wrong\n");
    g->tab_init = cells;
    g->tab_cur = malloc(SIZE*SIZE*sizeof(color));
    if (g->tab_cur == NULL)
        error("g->tab_cur allocation went wrong\n");
    for (int i = 0; i<SIZE*SIZE; i++){
        g->tab_cur[i] = cells[i];
    }
    g->size = SIZE;
    g->move_max = nb_moves_max;
    g->move_cur = 0;
    return g;
}

/**
 * @brief Creates an empty game.
 * @details Creates an empty game having SIZE rows and SIZE columns. All the cells will have the default color
 * (whose value is RED). The maximum number of moves is set to 0.
 * @return The created game
 **/
game game_new_empty(){
    game g = malloc(sizeof(game));
    if (g == NULL)
        error("g allocation went wrong\n");
    g->tab_init = calloc(SIZE*SIZE, sizeof(color));
    if (g->tab_init == NULL)
        error("g->tab_init allocation went wrong\n");
    g->tab_cur = calloc(SIZE*SIZE, sizeof(color));
    if (g->tab_cur == NULL)
        error("g->tab_cur allocation went wrong\n");
    g->size = SIZE;
    g->move_max = 0;
    g->move_cur = 0;
    return g;
}

/**
 * @brief Gets the color of the cell located at given coordinates.
 * @param g the game
 * @param x the first coordinate of the cell
 * @param y the second coordinate of the cell
 * @return the color of the cell
 * @pre @p g is a valid pointer toward a cgame structure
 * @pre @p x < SIZE
 * @pre @p y < SIZE
 **/
color game_cell_current_color(cgame g, uint x, uint y){
    if (g == NULL)
        error("g is not a valid pointer");
    if (x >= SIZE || y >= SIZE)
        error("x or y is higher than SIZE or equal");
    return g->tab_cur[SIZE*y+x];
}