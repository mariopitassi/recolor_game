#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void raise_usage() {
  printf("Usage: ./recolor_solve FIND_ONE|NB_SOL|FIN_MIN <input> <output> \n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  if (argc != 4)
    raise_usage();

  game g = game_load(argv[2]);

  if (strcmp(argv[1], "FIND_ONE") == 0 || strcmp(argv[1], "FIND_MIN") == 0) {
    sol solution =
        (strcmp(argv[1], "FIND_ONE") == 0) ? find_one(g) : find_min(g);
    print_sol_in_file(solution, argv[3]);
    free_sol(solution);
  } else if (strcmp(argv[1], "NB_SOL") == 0) {
    print_nb_sol_in_file(nb_sol(g), argv[3]);
  } else {
    game_delete(g);
    raise_usage();
  }

  game_delete(g);
}
