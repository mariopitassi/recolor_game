#include "game.h"
#include "game_io.h"
#include "solver.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Test a condition
 * @param cond The condition to be tested
 * @param err_msg The message to be displayed if condition true
 * @param g The game to be deleted if condition true
 * @return true if condition true
 * @return false if condition false
 */
static bool test(bool cond, char *err_msg, game g) {
  if (cond) {
    fprintf(stderr, "%s", err_msg);
    game_delete(g);
  }
  return cond;
}

/* ********** TAB TEST INITIALIZATION ********** */

color tab[] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2,
               0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2,
               1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3,
               0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
               1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1,
               1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2,
               2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

/* ********** GAME_NB_MOVES_MAX TEST********** */

bool test_game_nb_moves_max() {
  uint moves_max = 69;

  game g = game_new(tab, moves_max);
  if (test(game_nb_moves_max(g) != moves_max,
           "Error: invalid game nb moves max (game_new)!\n", g))
    return false;
  game_delete(g);

  game g2 = game_new_empty();
  if (test(game_nb_moves_max(g2) != 0,
           "Error: invalid game nb moves max (game_new_empty)!\n", g2))
    return false;
  game_delete(g2);

  return true;
}

/* ********** GAME_SET_CELL_INIT TEST ********** */

bool test_game_set_cell_init() {
  game g = game_new_empty();

  // Initialize every cells of g with the same values than tab
  // Then check if they have been correctly set (if not : error)
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      color c = tab[game_width(g) * y + x];
      game_set_cell_init(g, x, y, c);
      if (test(game_cell_current_color(g, x, y) != c,
               "Error: invalid current cell color!\n", g))
        return false;
    }
  }

  game_restart(g);

  // After restart, check if values match (if not : error)
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (test(game_cell_current_color(g, x, y) != tab[game_width(g) * y + x],
               "Error: invalid init cell color!\n", g))
        return false;
    }
  }

  game_delete(g);
  return true;
}

/* ********** GAME_CELL_CURRENT_COLOR TEST ********** */

bool test_game_cell_current_color() {
  game g = game_new(tab, 427);

  // Check if every cells of g match tab (if not : error)
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (test(game_cell_current_color(g, x, y) != tab[game_width(g) * y + x],
               "Error: invalid cell color! (game_new) \n", g))
        return false;
    }
  }
  game_delete(g);

  game g2 = game_new_empty();

  // Check if every cells of g2 match 0 (if not : error)
  for (uint y = 0; y < game_height(g2); y++) {
    for (uint x = 0; x < game_width(g2); x++) {
      if (test(game_cell_current_color(g2, x, y) != 0,
               "Error: invalid cell color! (game_new_empty)\n", g2))
        return false;
    }
  }
  game_delete(g2);
  return true;
}

/* ********** GAME_IS_WRAPPING TEST ********** */

bool test_game_is_wrapping() {
  // Initialize an array which allows us to check if the game is truly wrapping
  // Allows us to check if game_play_one_move works as well
  color wrap_tab_test[] = {0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
                           1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0};

  game g = game_new_ext(5, 6, wrap_tab_test, 5, true);

  // Check if game_is_wrapping returns the correct value (if not : error)
  if (test(!game_is_wrapping(g),
           "Error : unvalid wrapping value g (false instead of true)\n", g))
    return false;

  game_play_one_move(g, 1);

  // Check if the game is truly wrapping (game_play_one_move is working)
  // (if not : error)
  if (test(!game_is_over(g), "Error : game_play_one_move is not wrapping g", g))
    return false;

  game_delete(g);

  game g2 = game_new_ext(5, 6, wrap_tab_test, 5, false);
  if (test(game_is_wrapping(g2),
           "Error : unvalid wrapping value g2 (true instead of false)\n", g2))
    return false;

  game_play_one_move(g2, 1);
  if (test(game_is_over(g2), "Error : game_play_one_move is wrapping g2", g2))
    return false;

  game_delete(g2);
  return true;
}

/* ********** NB_SOL TEST ********** */
bool test_nb_sol() {
  game g = game_load("data/test_game_24sol.rec");
  uint nb_solution = nb_sol(g);
  if (test(nb_solution != 24,
           "Error : Nombre de solution différent de 24 pour test_game_446N\n",
           g))
    return false;

  game_set_max_moves(g, 3);
  nb_solution = nb_sol(g);
  if (test(nb_solution != 0, "Error : Nombre de solution différent de 0 pour "
                             "jeu impossible à gagner\n",
           g))
    return false;

  game_delete(g);

  game g2 = game_load("data/test_game_1sol.rec");
  nb_solution = nb_sol(g2);
  if (test(nb_solution != 1,
           "Error : Nombre de solution différent de 1 pour test_game_448N\n",
           g2))
    return false;

  game_delete(g2);

  game g3 = game_load("data/test_game_0.rec");
  nb_solution = nb_sol(g3);
  if (test(nb_solution != 0,
           "Error : Nombre de solution différent de 0 pour test_game_won\n",
           g3))
    return false;

  game_delete(g3);

  return true;
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {

  // start test
  fprintf(stderr, "=> Start test fakhoun\n");
  assert(test_game_nb_moves_max());
  assert(test_game_set_cell_init());
  assert(test_game_cell_current_color());
  assert(test_game_is_wrapping());
  assert(test_nb_sol());

  // print test result
  fprintf(stderr, "Test fakhoun finished: SUCCESS\n");
  return EXIT_SUCCESS;
}
