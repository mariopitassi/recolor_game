#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

// test game_play_one_move                pr le moment teste juste la 1ere case
// (+nb moves curr)

bool test_game_play_one_move(game g) {
  int moves_hyp =
      game_nb_moves_cur(g) + 1; // prediction du nb de coups post move
  int c = rand() % 4;           // random color
  game_play_one_move(g, c);
  if (moves_hyp != game_nb_moves_cur(g)) {
    fprintf(stderr, "Error: invalid number of moves\n");
    game_delete(g);
    return false;
  }
  if (game_cell_current_color(g, 0, 0) != c) {
    fprintf(stderr, "Error: invalid cell color\n");
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

// test game_is_over

bool test_game_is_over(game g) {
  if (g == NULL) {
    fprintf(stderr, "Error: pb memoire\n");
    return false;
  }

  bool camarche = false;

  if (game_is_over(g)) {
    fprintf(stderr, "Error: over mais meme pas commence\n");
    game_delete(g);
    return false;
  }
  // joue jusqu'à la fin et gagne
  color moves[] = {3, 1, 3, 1, 0, 3, 1, 0, 1, 3, 2, 0};
  for (int i = 0; i < 11; i++) {
    game_play_one_move(g, moves[i]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: over mais pas encore fini\n");
      game_delete(g);
      return false;
    }
  }
  game_play_one_move(g, moves[11]);
  if (game_is_over(g)) {
    camarche = true;
  }

  // relance partie + joue coups aléatoires
  game_restart(g);
  color ran_moves[] = {2, 1, 0, 3, 0, 1, 2, 3, 1, 0, 2, 0};
  for (int n = 0; n < 12; n++) {
    game_play_one_move(g, ran_moves[n]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: over mais pas gagne\n");
      game_delete(g);
      return false;
    }
  }

  // continue de jouer apres coups max
  color next_moves[] = {2, 1, 0, 3, 2, 0, 1};
  for (int n = 0; n < 7; n++) {
    game_play_one_move(g, next_moves[n]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: over mais coups max depasse\n");
      game_delete(g);
      return false;
    }
  }
  // return game_is_over(g) && true;
  if (camarche == false) {
    fprintf(stderr, "Error: devrait etre over\n");
  }
  game_delete(g);
  return camarche;
}

// test game_restart

bool test_game_restart(game g, color *tab) {
  // même chose que mario et game new???????
  if (g == NULL) {
    fprintf(stderr, "Error: pb memoire\n");
    game_delete(g);
    return false;
  }
  game_restart(g);

  for (int i = 0; i <= SIZE - 1; i++) {
    for (int j = 0; j <= SIZE - 1; j++) {
      if (game_cell_current_color(g, j, i) != tab[12 * i + j]) {
        fprintf(stderr, "Error: pb initialisation cellules\n");
        game_delete(g);
        return false;
      }
    }
  }

  if (game_nb_moves_cur(g) != 0 || game_nb_moves_max(g) != 12) {
    fprintf(stderr, "Error: pb nb moves\n");
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

// usage (given)
void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1)
    usage(argc, argv);

  // initialisation game pour les tests
  color tab[] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2,
                 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2,
                 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3,
                 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
                 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1,
                 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2,
                 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

  uint coups_max = 12;

  game game_test = game_new(tab, coups_max);

  // start test
  fprintf(stderr, "=> Start test akasparian \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp("game_play_one_move", argv[1]) == 0)
    ok = test_game_play_one_move(game_test);
  else if (strcmp("game_is_over", argv[1]) == 0)
    ok = test_game_is_over(game_test);
  else if (strcmp("game_restart", argv[1]) == 0)
    ok = test_game_restart(game_test, tab);
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