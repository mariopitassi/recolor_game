#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

/* ********** INITIALISATION DU TABLEAU ********** */

color tab[] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2,
               0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2,
               1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3,
               0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
               1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1,
               1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2,
               2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

/* ********** TEST DE GAME_NB_MOVES_MAX ********** */

bool test_game_nb_moves_max() {
  int moves_max = 50;

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

/* ********** TEST DE GAME_SET_CELL_INIT ********** */

bool test_game_set_cell_init() {
  int moves_max = 50;
  
  game g = game_new(tab, moves_max);

  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      color col = rand() % 4;
      game_set_cell_init(g, x, y, col);
      if (game_cell_current_color(g, x, y) != col) {
        fprintf(stderr, "Error: invalid cell color!\n");
        game_delete(g);
        return false;
      }
    }
  }

  game g2 = game_new_empty();

  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      color col = rand() % 4;
      game_set_cell_init(g2, x, y, col);
      if (game_cell_current_color(g2, x, y) != col) {
        fprintf(stderr, "Error: invalid cell color!\n");
        game_delete(g);
        game_delete(g2);
        return false;
      }
    }
  }

  game g3 = game_new_empty();
  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {
      color col = tab[game_height(g) * y + x];
      game_set_cell_init(g3, x, y, col);
    }
  }
  color ran_moves[] = {2, 1, 0, 3, 0, 1, 2, 3, 1, 0, 2, 0};
  for (int n = 0; n < 12; n++) {
    game_play_one_move(g3, ran_moves[n]);
  }
  game_restart(g3);
  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g3, x, y) != tab[game_width(g) * y + x]) {
        fprintf(stderr, "Error: pb apres restart\n");
        game_delete(g);
        game_delete(g2);
        game_delete(g3);
        return false;
      }
    }
  }

  game_delete(g);
  game_delete(g2);
  game_delete(g3);
  return true;
}

/* ********** TEST DE GAME_CELL_CURRENT_COLOR ********** */

bool test_game_cell_current_color() {
  game g = game_new(tab, 50);

  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g, x, y) != tab[game_width(g) * y + x]) {
        fprintf(stderr, "Error: invalid cell color! (game_new) \n");
        game_delete(g);
        return false;
      }
      color col = rand() % 4;
      game_set_cell_init(g, x, y, col);
      if (game_cell_current_color(g, x, y) != col) {
        fprintf(stderr,
                "Error: invalid cell color! (game_new--set_cell_init)\n");
        game_delete(g);
        return false;
      }
    }
  }

  game g2 = game_new_empty();


  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g2, x, y) != 0) {
        fprintf(stderr, "Error: invalid cell color! (game_new_empty)\n");
        game_delete(g);
        game_delete(g2);
        return false;
      }
      color col = rand() % 4;
      game_set_cell_init(g2, x, y, col);
      if (game_cell_current_color(g2, x, y) != col) {
        fprintf(stderr,
                "Error: invalid cell color! (game_new_empty--set_cell_init)\n");
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

/* ********** TEST DE GAME_IS_WRAPPING********** */

bool test_game_is_wrapping() {
  game g = game_new_empty_ext(12, 15, true);

  if (!game_is_wrapping(g)){
    fprintf(stderr, "Error : unvalid wrapping value g (false instead of true)\n");
    game_delete(g);
    return false;
  }

  game g2 = game_new_empty_ext(12, 15, false);

  if (game_is_wrapping(g2)){
    fprintf(stderr, "Error : unvalid wrapping value g2 (true instead of false)\n");
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
  if (strcmp("test_game_nb_moves_max", argv[1]) == 0)
    ok = test_game_nb_moves_max(100);
  else if (strcmp("test_game_set_cell_init", argv[1]) == 0)
    ok = test_game_set_cell_init(100);
  else if (strcmp("test_game_cell_current_color", argv[1]) == 0)
    ok = test_game_cell_current_color();
  else if (strcmp("test_game_is_wrapping", argv[1]) == 0)
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
