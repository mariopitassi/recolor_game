#include "solver.h"
#include "ListLib/asde_slist.h"
#include "ListLib/asde_slist_utilitary_functions.h"
#include "game.h"
#include "game_io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(bool cond, char *err_mess) {
  if (cond) {
    fprintf(stderr, "Error: %s \n\n", err_mess);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Allocate memory for a solution structure and initialized it to default
 * NB: The tab is not allocated (look_for_sol will do it if a solution is found)
 * @return sol* a pointer to the allocated solution structure
 */
static sol sol_alloc() {
  sol s = malloc(sizeof(struct solution));
  error(s == NULL, "Allocation went wrong\n");
  s->nb_moves = 0;
  return s;
}

/**
 * @brief Allocate memory for a color array
 *
 * @param nb_moves number of moves to store
 * @return color* a pointer to the allocated tab
 */
static color *moves_alloc(uint nb_moves) {
  color *moves = malloc(nb_moves * sizeof(color));
  error(moves == NULL, "Allocation went wrong\n");
  return moves;
}

/**
 * @brief Play a hit board
 *
 * @param g a game instance
 * @param moves a tab of color to play iteratively
 * @param nb_moves total of moves
 */
static void game_play_moves(game g, color *moves, uint nb_moves) {
  error(moves == NULL, "Invalid pointer\n");
  for (uint i = 0; i < nb_moves; i++) {
    game_play_one_move(g, moves[i]);
  }
}

/**
 * @brief Copy an array of color
 * NB : useful when storing our table in the solution structure
 *
 * @param arr a color array
 * @param len the length of the array
 * @return color*
 */
static color *copy_arr(color *arr, uint len) {
  error(arr == NULL, "Invalid pointer\n");
  color *copy_arr = malloc(len * sizeof(color));
  for (int i = 0; i < len; i++) {
    copy_arr[i] = arr[i];
  }
  return copy_arr;
}

/**
 * @brief Play a hit board and count the number of color in the grid
 *
 * @param g a game instance
 * @param moves a colors array
 * @param nb_moves number of moves to play
 * @return uint number of color in the grid
 */
static uint nb_colors(cgame g, color *moves, uint nb_moves) {
  error(moves == NULL, "Unvalid pointer\n");
  game g2 = game_copy(g);
  game_play_moves(g2, moves, nb_moves);

  uint nb_col = 0;

  SList colors = asde_slist_create_empty();

  for (uint y = 0; y < game_height(g2); y++) {
    for (uint x = 0; x < game_width(g2); x++) {
      if (!asde_slist_belongs_to(colors, game_cell_current_color(g2, x, y))) {
        colors = asde_slist_prepend(colors, game_cell_current_color(g2, x, y));
        nb_col += 1;
      }
    }
  }

  game_delete(g2);
  asde_slist_delete_list(colors);

  return nb_col;
}

void around(game g, uint x, uint y, color oldcolor, SList color_around,
            uint *counter) {

  color current_cell = game_cell_current_color(g, x, y);

  if (current_cell == oldcolor) {
    game_set_cell_init(g, x, y, 99); // On initialise cette couleur pour pas que
                                     // la fonction tourne à l'infini
    // (99 car improbable d'avoir un jeu à 100 couleurs)
    if (x < game_width(g) - 1)
      around(g, x + 1, y, oldcolor, color_around, counter);
    if (y < game_height(g) - 1)
      around(g, x, y + 1, oldcolor, color_around, counter);
    if (x > 0)
      around(g, x - 1, y, oldcolor, color_around, counter);
    if (y > 0)
      around(g, x, y - 1, oldcolor, color_around, counter);

    if (game_is_wrapping(g)) {
      if (x == game_width(g) - 1)
        around(g, (x + 1 - game_width(g)), y, oldcolor, color_around, counter);
      if (y == game_height(g) - 1)
        around(g, x, (y + 1 - game_height(g)), oldcolor, color_around, counter);
      if (x == 0)
        around(g, (game_width(g) - 1), y, oldcolor, color_around, counter);
      if (y == 0)
        around(g, x, (game_height(g) - 1), oldcolor, color_around, counter);
    }
  }
  // Tri de la liste
  // On met les couleurs qui reviennent le plus en début de liste
  else if (current_cell != 99) {
    counter[current_cell] += 1;
    SList p = asde_slist_delete_all(color_around, current_cell);
    while (!asde_slist_isEmpty(asde_slist_next(p)) &&
           counter[current_cell] <
               counter[asde_slist_data(asde_slist_next(p))]) {
      p = asde_slist_next(p);
    }
    color_around = asde_slist_insert_after(color_around, p, current_cell);
  }
}

/**
 * @brief Returns a list of adjacent colors after playing the colors in the
 * "moves" array from "moves [0] to moves [nb_moves-1]" NB : Last optimization
 * possible : sort this list
 * @param g an instance of a game
 * @param moves array of played moves
 * @param nb_moves nb of moves to play
 * @return SList a list of adjacent colors
 */
static SList col_around(cgame g, color *moves, uint nb_moves) {
  error(moves == NULL, "Unvalid pointer\n");
  game g2 = game_copy(g);
  game_play_moves(g2, moves, nb_moves);

  SList colors_around = asde_slist_create_empty();
  colors_around = asde_slist_prepend(colors_around, 99);

  color *counter = calloc(100, sizeof(color));

  around(g2, 0, 0, game_cell_current_color(g2, 0, 0), colors_around, counter);

  colors_around = asde_slist_delete_first(colors_around);

  free(counter);
  game_delete(g2);

  return colors_around;
}

/**
 * @brief This function finds all solutions (or just one) it can.
 * It returns nothing but it will update some paramaters during the search...
 * NB: It works with "game_default" and "game_horizontal_2S" but not with "2N"
 * @param g an instance of a game
 * @param moves a tab of move played by the solver
 * @param c_around a list of all color around the first cell
 * @param nb_moves number of current move played
 * @param nb_moves_max number of move max
 * @param nb_sol number of solution(s) found
 * @param s a pointer to a blank solution which could be updated or unused
 * (NULL)
 * @param look_nb_sol bool used to stop or not the search when the first
 * solution is found
 */
static void look_for_sol(cgame g, color *moves, SList c_around, uint nb_moves,
                         uint nb_moves_max, uint *nb_sol, sol s,
                         bool look_nb_sol) {
  uint moves_left = nb_moves_max - nb_moves;
  uint nb_col = nb_colors(g, moves, nb_moves);

  if (moves_left < nb_col - 1)
    return; // We stop because it's useless to play (sure to lose)

  if (nb_moves == nb_moves_max || nb_col == 1) { // Solution found
    *nb_sol += 1;
    if (!look_nb_sol) {
      s->moves = copy_arr(moves, nb_moves);
      s->nb_moves = nb_moves;
    }
    return;
  }

  while (!asde_slist_isEmpty(c_around)) {
    moves[nb_moves] = asde_slist_data(c_around);
    SList next_c_around = col_around(
        g, moves,
        nb_moves + 1); //"nb_moves + 1" because we just added a move to "moves"
    look_for_sol(g, moves, next_c_around, nb_moves + 1, nb_moves_max, nb_sol, s,
                 look_nb_sol);
    asde_slist_delete_list(next_c_around);
    c_around = asde_slist_next(c_around);
    if (*nb_sol > 0 && !look_nb_sol)
      return; // we stop when we find a solution and we dont search nb sol
  }

  asde_slist_delete_list(c_around);
}

sol find_min(cgame g) {
  sol s = sol_alloc();
  color move_min[] = {0};
  uint nb_moves_min = nb_colors(g, move_min, 0) -
                      1; // (Number of game colors - 1) before playing
  for (uint nb_moves = nb_moves_min; nb_moves <= game_nb_moves_max(g);
       nb_moves++) {
    color *moves = moves_alloc(nb_moves);
    SList c_around = col_around(g, moves, 0);
    uint nb_sol = 0;
    bool look_nb_sol = false;
    look_for_sol(g, moves, c_around, 0, nb_moves, &nb_sol, s, look_nb_sol);
    asde_slist_delete_list(c_around);
    free(moves);
    if (nb_sol > 0)
      return s;
  }
  return s;
}

sol find_one(cgame g) {
  sol s = sol_alloc();
  color *moves = moves_alloc(game_nb_moves_max(g));
  SList c_around = col_around(g, moves, 0);
  uint nb_sol = 0;
  bool look_nb_sol = false;
  look_for_sol(g, moves, c_around, 0, game_nb_moves_max(g), &nb_sol, s,
               look_nb_sol);
  asde_slist_delete_list(c_around);
  free(moves);
  return s;
}

uint nb_sol(cgame g) {
  sol s = NULL; // NULL because it will not be used
  color *moves = moves_alloc(game_nb_moves_max(g));
  SList c_around = col_around(g, moves, 0);
  uint nb_sol = 0;
  bool look_nb_sol = true;
  look_for_sol(g, moves, c_around, 0, game_nb_moves_max(g), &nb_sol, s,
               look_nb_sol);
  asde_slist_delete_list(c_around);
  free(moves);
  return nb_sol;
}
