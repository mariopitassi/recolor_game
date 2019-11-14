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

/* ************* Error handler *********** */ 

// TODO: test this function
void error(char[100] err_mess) {
    fprintf(stderr, "Error: " + err_mess);
    exit(EXIT_FAILURE);
}


/* ************* Author : Anouche *********** */ 

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

/* ************* Author : Mario *********** */ 

void game_set_cell_init(game g, uint x, uint y, color c) {

    if(g==NULL || g->tab_init == NULL || g->tab_cur == NULL) {
        error("Pointer NULL exception");
    }

    if(x<SIZE && y<SIZE && c < NB_COLORS) {
        g->tab_init[12*y+x] = c;
        g->tab_cur[12*y+x]  = c;
    }
}

void game_set_max_moves(game g, uint nb_max_moves) {
    if(g==NULL) {
        error("Pointer NULL exception");
    }

    g->move_max = nb_max_moves;
}


// TODO: finish implementation
void game_play_one_move(game g, color c) {
    if(g==NULL) {
        error("Pointer NULL exception");
    }

    if(c < NB_COLORS) {
        g->move_cur += 1;
    }
}


/* ************* Author : Mattias *********** */ 

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

/* ************* Author : Farouk *********** */ 

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