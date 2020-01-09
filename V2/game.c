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

static void error(char *err_mess) {
  fprintf(stderr, "Error: %s \n\n", err_mess);
  exit(EXIT_FAILURE);
}

/* ************* Author : Anouche *********** */

uint game_nb_moves_cur(cgame g) {
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  return g->moves_cur;
}

game game_copy(cgame g) {
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  game g1 = game_new_ext(g->size_x, g->size_y, g->tab_init, g->moves_max, g->wrap);

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
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  if (g->moves_cur <= g->moves_max && full_grid(g)) {
    return true;
  }

  return false;
}

game game_new_ext(uint width, uint height, color *cells, uint nb_moves_max, bool wrapping) {
  game g = malloc(sizeof(struct game_s));

  if (g == NULL)
    error("g allocation went wrong");

  g->size_x = width;
  g->size_y = height;
  g->moves_max = nb_moves_max;
  g->moves_cur = 0;

  g->tab_init = malloc(g->size_x * g->size_y * sizeof(color));
  
  if (g->tab_init == NULL)
    error("g->tab_init allocation went wrong");
  
  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_init[i] = cells[i];
  }

  g->tab_cur = malloc(g->size_x * g->size_y * sizeof(color));
  
  if (g->tab_cur == NULL)
    error("g->tab_cur allocation went wrong");
  
  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_cur[i] = cells[i];
  }

  g->wrap = wrapping;

  return g;
}

/* ************* Author : Mario *********** */

void game_set_cell_init(game g, uint x, uint y, color c) {
  if (g == NULL || g->tab_init == NULL || g->tab_cur == NULL)
    error("g is not a valid pointer");

  if (x < g->size_x && y < g->size_y && c < NB_COLORS) {
    g->tab_init[g->size_x * y + x] = c;
    g->tab_cur[g->size_x * y + x] = c;
  }
}

void game_set_max_moves(game g, uint nb_max_moves) {
  if (g == NULL)
    error("g is not a valid pointer");

  g->moves_max = nb_max_moves;
}

uint game_height(cgame g) {
  if (g == NULL)
    error("g allocation went wrong");

  return g->size_y;
}

uint game_width(cgame game) {
  if (game == NULL)
    error("g allocation went wrong");

  return game->size_x;
}

/* ************* Author : Everyone *********** */

/**
 * @brief Takes the game, a position (x, y), oldcolor, newcolor and recursively
 * fills with
 * the new color all the adjacent cells having the same old color
 *
 * If game is wrapping, this function do the job.
 *
 * @param g the game
 * @param x value on the x-axis of the grid
 * @param y value on the y-axis of the grid
 * @param oldcolor
 * @param newcolor
 */
void static floodFill(game g, uint x, uint y, color oldcolor, color newcolor) {

  if (g->tab_cur[g->size_x * y + x] == oldcolor && newcolor != oldcolor) {
    g->tab_cur[g->size_x * y + x] = newcolor;

    if (x < g->size_x - 1)
      floodFill(g, x + 1, y, oldcolor, newcolor);
    if (y < g->size_y - 1)
      floodFill(g, x, y + 1, oldcolor, newcolor);
    if (x > 0)
      floodFill(g, x - 1, y, oldcolor, newcolor);
    if (y > 0)
      floodFill(g, x, y - 1, oldcolor, newcolor);

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
  if (g == NULL)
    error("g is not a valid pointer");

  floodFill(g, 0, 0, g->tab_cur[0], c);
  g->moves_cur++;
}

/* ************* Author : Mattias *********** */

void game_restart(game g) {
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_cur[i] = g->tab_init[i];
  }

  g->moves_cur = 0;
}

uint game_nb_moves_max(cgame g) {
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  uint nb_moves_max = g->moves_max;
  return nb_moves_max;
}

void game_delete(game g) {
  if (g == NULL) {
    error("g is not a valid pointer");
  }

  free(g->tab_init);
  free(g->tab_cur);
  free(g);
}

game game_new_empty_ext(uint width, uint height, bool wrapping) {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    error("g allocation went wrong");
  }

  g->size_x = width;
  g->size_y = height;
  g->moves_max = 0;
  g->moves_cur = 0;

  g->tab_init = calloc(g->size_x * g->size_y, sizeof(color));

  if (g->tab_init == NULL) {
    error("g->tab_init allocation went wrong");
  }

  g->tab_cur = calloc(g->size_x * g->size_y, sizeof(color));
  if (g->tab_cur == NULL) {
    error("g->tab_cur allocation went wrong");
  }

  g->wrap = wrapping;

  return g;
}

/* ************* Author : Farouk *********** */

game game_new(color *cells, uint nb_moves_max) {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL)
    error("g allocation went wrong");

  g->size_x = SIZE;
  g->size_y = SIZE;
  g->moves_max = nb_moves_max;
  g->moves_cur = 0;

  g->tab_init = malloc(g->size_x * g->size_y * sizeof(color));
  if (g->tab_init == NULL)
    error("g->tab_init allocation went wrong");
  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_init[i] = cells[i];
  }

  g->tab_cur = malloc(g->size_x * g->size_y * sizeof(color));
  if (g->tab_cur == NULL)
    error("g->tab_cur allocation went wrong");
  for (int i = 0; i < g->size_x * g->size_y; i++) {
    g->tab_cur[i] = cells[i];
  }

  g->wrap = false;

  return g;
}

game game_new_empty() {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL)
    error("g allocation went wrong");

  g->size_x = SIZE;
  g->size_y = SIZE;
  g->moves_max = 0;
  g->moves_cur = 0;

  g->tab_init = calloc(g->size_x * g->size_y, sizeof(color));
  if (g->tab_init == NULL)
    error("g->tab_init allocation went wrong");

  g->tab_cur = calloc(g->size_x * g->size_y, sizeof(color));
  if (g->tab_cur == NULL)
    error("g->tab_cur allocation went wrong");

  g->wrap = false;

  return g;
}

color game_cell_current_color(cgame g, uint x, uint y) {
  if (g == NULL)
    error("g is not a valid pointer");

  if (x >= g->size_x || y >= g->size_y)
    error("x or y is higher than it should be");

  return g->tab_cur[g->size_x * y + x];
}

bool game_is_wrapping(cgame g) {
  if (g == NULL)
    error("g allocation went wrong");

  return g->wrap;
}