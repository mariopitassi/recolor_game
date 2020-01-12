#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

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
  int moves_max = 69;

  game g = game_new(tab, moves_max);

  if (game_nb_moves_max(g) != moves_max) {
    fprintf(stderr, "Error: invalid game nb moves max (game_new)!\n");
    game_delete(g);
    return false;
  }

  game g2 = game_new_empty();

  if (game_nb_moves_max(g2) != 0) {
    fprintf(stderr, "Error: invalid game nb moves max (game_new_empty)!\n");
    game_delete(g);
    game_delete(g2);
    return false;
  }

  game_delete(g);
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
      if (game_cell_current_color(g, x, y) != c) {
        fprintf(stderr, "Error: invalid current cell color!\n");
        game_delete(g);
        return false;
      }
    }
  }

  game_restart(g);

  // After restart, check if values match (if not : error)
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g, x, y) != tab[game_width(g) * y + x]) {
        fprintf(stderr, "Error: invalid init cell color!\n");
        game_delete(g);
        return false;
      }
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
      if (game_cell_current_color(g, x, y) != tab[game_width(g) * y + x]) {
        fprintf(stderr, "Error: invalid cell color! (game_new) \n");
        game_delete(g);
        return false;
      }
    }
  }

  game g2 = game_new_empty();

  // Check if every cells of g2 match 0 (if not : error)
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g2, x, y) != 0) {
        fprintf(stderr, "Error: invalid cell color! (game_new_empty)\n");
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

/* ********** GAME_IS_WRAPPING TEST ********** */

bool test_game_is_wrapping() {

  // Initialize an array which allows us to check if the game is truly wrapping
  // Allows us to check if game_play_one_move works as well
  color wrap_tab_test[] = { 0, 0, 1, 1, 0,
                            1, 0, 1, 0, 1,
                            0, 0, 1, 0, 1,
                            1, 0, 0, 0, 1,
                            1, 1, 1, 1, 1,
                            0, 1, 1, 1, 0 };

  game g = game_new_ext(5, 6, wrap_tab_test, 5, true);

  // Check if game_is_wrapping returns the correct value (if not : error)
  if (!game_is_wrapping(g)){
    fprintf(stderr, "Error : unvalid wrapping value g (false instead of true)\n");
    game_delete(g);
    return false;
  }

  game_play_one_move(g, 1);

  // Check if the game is truly wrapping (game_play_one_move is working)
  // (if not : error)
  if (!game_is_over(g)) {
    fprintf(stderr, "Error : game_play_one_move is not wrapping g");
    game_delete(g);
    return false;
  }

  game g2 = game_new_ext(5, 6, wrap_tab_test, 5, false);

  if (game_is_wrapping(g2)){
    fprintf(stderr, "Error : unvalid wrapping value g2 (true instead of false)\n");
    game_delete(g);
    game_delete(g2);
    return false;
  }

  game_play_one_move(g2, 1);

  if (game_is_over(g2)) {
    fprintf(stderr, "Error : game_play_one_move is wrapping g2");
    game_delete(g);
    game_delete(g2);
    return false;
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
  if (strcmp("game_nb_moves_max", argv[1]) == 0)
    ok = test_game_nb_moves_max();
  else if (strcmp("game_set_cell_init", argv[1]) == 0)
    ok = test_game_set_cell_init();
  else if (strcmp("game_cell_current_color", argv[1]) == 0)
    ok = test_game_cell_current_color();
  else if (strcmp("game_is_wrapping", argv[1]) == 0)
    ok = test_game_is_wrapping();
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
