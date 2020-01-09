#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

// Author: PITASSI Mario

/* ************* Error handler *********** */
static bool error(char *err_mess) {
  fprintf(stderr, "Error: %s \n\n", err_mess);
  return true;
}

color tab[] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2,
               0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2,
               1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3,
               0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
               1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1,
               1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2,
               2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

/* ****** TEST GAME_NEW ***** */

bool test_game_new() {
  uint coups_max = 12;
  game g = game_new(tab, coups_max);

  if (g == NULL) {
    error("g is not a valid pointer.");
    return false;
  }

  for (int i = 0; i <= SIZE - 1; i++) {
    for (int j = 0; j <= SIZE - 1; j++) {
      if (game_cell_current_color(g, j, i) != tab[SIZE * i + j]) {
        error("cells aren't initialized with good values from tab.");
        game_delete(g);
        return false;
      }
    }
  }

  for (uint i = 0; i <= SIZE; i++) {
    game g2 = game_new(tab, i);
    if (g2 == NULL) {
      error("g2 is not a valid pointer.");
      game_delete(g);
      return false;
    }

    if (game_nb_moves_max(g2) != i) {
      error("max move not initialized.");
      game_delete(g2);
      game_delete(g);
      return false;
    }

    game_delete(g2);
  }

  uint coups_test = game_nb_moves_max(g);

  if (coups_max != coups_test) {
    error("max moves aren't the same so it doesn't change.");
    game_delete(g);
    return false;
  }

  uint coups_current_test = game_nb_moves_cur(g);

  if (coups_current_test != 0) {
    error("current moves aren't the same so it doesn't change.");
    game_delete(g);
    return false;
  }

  game_delete(g);
  return true;
}

/* ******  TEST SET_MAX_MOVES ****** */

bool test_set_max_moves() {
  uint coups_max = 12;
  game g = game_new(tab, coups_max);

  if (g == NULL) {
    error("g is not a valid pointer.");
    return false;
  }

  uint coups = 54;

  if (game_nb_moves_cur(g) > coups) {
    error("you can't put a max_move < current_move.");
    game_delete(g);
    return false;
  }

  game_set_max_moves(g, coups);

  uint coups_test = game_nb_moves_max(g);

  if (coups != coups_test) {
    error("max moves aren't the same, it doesn't change.");
    game_delete(g);
    return false;
  }

  game_delete(g);
  return true;
}

/* ****** TEST COPY ***** */
bool test_copy() {
  uint coups_max = 12;
  game g = game_new(tab, coups_max);

  bool problem = false;

  if (g == NULL) {
    error("g is not a valid pointer.");
    return false;
  }

  game_play_one_move(g, 1);

  game g2 = game_copy(g);

  if (g2 == NULL) {
    problem = error("g2 is not a valid pointer.");
  }

  for (int i = 0; i <= SIZE - 1; i++) {
    for (int j = 0; j <= SIZE - 1; j++) {
      if (game_cell_current_color(g, j, i) != game_cell_current_color(g2, j, i)) {
        problem = error("cells copied weren't initialized correctly.");
      }
    }
  }

  if (game_nb_moves_max(g) != game_nb_moves_max(g2)) {
    problem = error("max moves can't be different.");
  }

  if(game_is_wrapping(g) != game_is_wrapping(g2)) {
    problem = error("wrapping can't be different.");
  }

  if(game_height(g) != game_height(g2)) {
    problem = error("height can't be different.");
  }

  if(game_width(g) != game_width(g2)) {
    problem = error("width can't be different.");
  }

  if (game_nb_moves_cur(g) != game_nb_moves_cur(g2)) {
    problem = error("current moves can't be different.");
  }

  if(problem) {
    game_delete(g);
    game_delete(g2);
    return false;
  }
  return true;
}

/** TEST_HEIGHT **/

bool test_height() {
  uint width = 10;
  uint height = 25;
  game g = game_new_empty_ext(width, height, false);

  if(game_height(g) != height) {
    error("height isn't the same");
    return false;
  }

  return true;
}

/** TEST_WIDTH **/

bool test_width() {
  uint width = 10;
  uint height = 25;
  game g = game_new_empty_ext(width, height, false);

  if(game_width(g) != width) {
    error("height isn't the same");
    return false;
  }

  return true;
}
/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1)
    usage(argc, argv);

  // Start of tests.
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp("game_new", argv[1]) == 0)
    ok = test_game_new();
  else if (strcmp("set_max_moves", argv[1]) == 0) {
    ok = test_set_max_moves();
  } else if (strcmp("copy", argv[1]) == 0) {
    ok = test_copy();
  }else if (strcmp("height", argv[1]) == 0) {
    ok = test_height();
  }else if (strcmp("width", argv[1]) == 0) {
    ok = test_width();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // Résults of tests.
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "\nTest \"%s\" finished: FAILURE\n\n", argv[1]);
    return EXIT_FAILURE;
  }
}
