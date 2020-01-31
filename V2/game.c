#include "game.h"
#include "game_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct game_s {
  color *tab_init;
  color *tab_cur;
  uint size_x;
  uint size_y;
  uint moves_max;
  uint moves_cur;
  bool wrap;
} * game;

/* ************* Error handler *********** */

static void error(bool cond, char *err_mess) {
  if(cond) {
    fprintf(stderr, "Error: %s \n\n", err_mess);
    exit(EXIT_FAILURE);
  }
}

/* ************* Author : Anouche *********** */

uint game_nb_moves_cur(cgame g) {
  error(g == NULL,"g is not a valid pointer");
  return g->moves_cur;
}

game game_copy(cgame g) {
  error(g == NULL, "g is not a valid pointer");

  game g1 =
      game_new_ext(g->size_x, g->size_y, g->tab_init, g->moves_max, g->wrap);

  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g1->tab_cur[i] = g->tab_cur[i];
  }

  g1->moves_cur = g->moves_cur;

  return g1;
}

/**
 * @brief Checks if all the cells have the same color
 * @param g the game
 * @return false if a cell is different from the first cell
 */
static bool full_grid(cgame g) {
  color c = game_cell_current_color(g, 0, 0);

  for (int i = 0; i < g->size_y; i++) {
    for (int j = 0; j < g->size_x; j++) {
      if (game_cell_current_color(g, j, i) != c) {
        return false;
      }
    }
  }

  return true;
}

bool game_is_over(cgame g) {
  error(g == NULL, "g is not a valid pointer");

  if (g->moves_cur <= g->moves_max && full_grid(g)) {
    return true;
  }

  return false;
}

game game_new_ext(uint width, uint height, color *cells, uint nb_moves_max,
                  bool wrapping) {
  game g = malloc(sizeof(struct game_s));
  
  error(g == NULL,"g allocation went wrong");

  error(width == 0 || height == 0, "width or height can't be 0");

  g->size_x = width;
  g->size_y = height;
  g->moves_max = nb_moves_max;
  g->moves_cur = 0;

  g->tab_init = malloc(g->size_x * g->size_y * sizeof(color));

  error(g->tab_init == NULL, "g->tab_init allocation went wrong");

  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_init[i] = cells[i];
  }

  g->tab_cur = malloc(g->size_x * g->size_y * sizeof(color));

  error(g->tab_cur == NULL, "g->tab_cur allocation went wrong");

  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_cur[i] = cells[i];
  }

  g->wrap = wrapping;

  return g;
}

/* ************* Author : Mario *********** */

void game_set_cell_init(game g, uint x, uint y, color c) {
  error(g == NULL || g->tab_init == NULL || g->tab_cur == NULL, "g is not a valid pointer");

  if (x < g->size_x && y < g->size_y) {
    g->tab_init[g->size_x * y + x] = c;
    g->tab_cur[g->size_x * y + x] = c;
  }
}

void game_set_max_moves(game g, uint nb_max_moves) {
  error(g == NULL, "g is not a valid pointer");

  g->moves_max = nb_max_moves;
}

uint game_height(cgame g) {

  error(g == NULL, "g allocation went wrong");

  return g->size_y;
}

uint game_width(cgame game) {
  error(game == NULL, "g allocation went wrong");

  return game->size_x;
}

/* ************* Author : Everyone *********** */

/**
 * @brief Recursively fills with the new color all the adjacent cells of (x,y)
 * having the same old color If game is wrapping, this function do the job.
 *
 * @param g the game
 * @param x the first coordinate of the cell
 * @param y the second coordinate of the cell
 * @param oldcolor color to be changed
 * @param newcolor color to be used
 */
void static floodFill(game g, uint x, uint y, color oldcolor, color newcolor) {

  // Check if the cell (x,y) needs to be changed
  if (g->tab_cur[g->size_x * y + x] == oldcolor && newcolor != oldcolor) {
    g->tab_cur[g->size_x * y + x] = newcolor;

    // Recursivity
    if (x < g->size_x - 1)
      floodFill(g, x + 1, y, oldcolor, newcolor); // Going right until edge
    if (y < g->size_y - 1)
      floodFill(g, x, y + 1, oldcolor, newcolor); // Going down until edge
    if (x > 0)
      floodFill(g, x - 1, y, oldcolor, newcolor); // Going left until edge
    if (y > 0)
      floodFill(g, x, y - 1, oldcolor, newcolor); // Going up until edge

    // if game_is_wrapping, recursivity on opposite edge when an edge is reached
    if (game_is_wrapping(g)) {
      if (x == g->size_x - 1)
        floodFill(g, (x + 1 - g->size_x), y, oldcolor, newcolor);
      if (y == g->size_y - 1)
        floodFill(g, x, (y + 1 - g->size_y), oldcolor, newcolor);
      if (x == 0)
        floodFill(g, (g->size_x - 1), y, oldcolor, newcolor);
      if (y == 0)
        floodFill(g, x, (g->size_y - 1), oldcolor, newcolor);
    }
  }
}

void game_play_one_move(game g, color c) {
  error(g == NULL, "g is not a valid pointer");

  floodFill(g, 0, 0, g->tab_cur[0], c);
  g->moves_cur++;
}

/* ************* Author : Mattias *********** */

void game_restart(game g) {

  error(g == NULL, "g is not a valid pointer");

  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_cur[i] = g->tab_init[i];
  }

  g->moves_cur = 0;
}

uint game_nb_moves_max(cgame g) {
  error(g == NULL, "g is not a valid pointer");

  uint nb_moves_max = g->moves_max;
  return nb_moves_max;
}

void game_delete(game g) {
  error(g == NULL, "g is not a valid pointer");

  free(g->tab_init);
  free(g->tab_cur);
  free(g);
}

game game_new_empty_ext(uint width, uint height, bool wrapping) {
  game g = malloc(sizeof(struct game_s));

  error(g == NULL, "g allocation went wrong");

  g->size_x = width;
  g->size_y = height;
  g->moves_max = 0;
  g->moves_cur = 0;

  g->tab_init = calloc(g->size_x * g->size_y, sizeof(color));

  error(g->tab_init == NULL, "g->tab_init allocation went wrong");

  g->tab_cur = calloc(g->size_x * g->size_y, sizeof(color));

  error(g->tab_cur == NULL, "g->tab_cur allocation went wrong");

  g->wrap = wrapping;

  return g;
}

/* ************* Author : Farouk *********** */

game game_new(color *cells, uint nb_moves_max) {
  game g = malloc(sizeof(struct game_s));
  
  error(g == NULL, "g allocation went wrong");

  error(nb_moves_max == 0, "nb_max_move can't be 0");

  g->size_x = SIZE;
  g->size_y = SIZE;
  g->moves_max = nb_moves_max;
  g->moves_cur = 0;

  g->tab_init = malloc(g->size_x * g->size_y * sizeof(color));
  
  error(g->tab_init == NULL, "g->tab_init allocation went wrong");

  for (int i = 0; i < g->size_x * g->size_y; i++)
    g->tab_init[i] = cells[i];

  g->tab_cur = malloc(g->size_x * g->size_y * sizeof(color));
  
  error(g->tab_cur == NULL, "g->tab_cur allocation went wrong");

  for (int i = 0; i < g->size_x * g->size_y; i++)
    g->tab_cur[i] = cells[i];

  g->wrap = false;

  return g;
}

game game_new_empty() {
  game g = malloc(sizeof(struct game_s));
  
  error(g == NULL, "g allocation went wrong");

  g->size_x = SIZE;
  g->size_y = SIZE;
  g->moves_max = 0;
  g->moves_cur = 0;

  g->tab_init = calloc(g->size_x * g->size_y, sizeof(color));
  
  error(g->tab_init == NULL, "g->tab_init allocation went wrong");

  g->tab_cur = calloc(g->size_x * g->size_y, sizeof(color));
  
  error(g->tab_cur == NULL, "g->tab_cur allocation went wrong");

  g->wrap = false;

  return g;
}

color game_cell_current_color(cgame g, uint x, uint y) {
  
  error(g == NULL, "g is not a valid pointer");
  
  error(x >= g->size_x || y >= g->size_y, "x or y is higher than it should be");

  return g->tab_cur[g->size_x * y + x];
}

bool game_is_wrapping(cgame g) {

  error(g == NULL, "g allocation went wrong");

  return g->wrap;
}
