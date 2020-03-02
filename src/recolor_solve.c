#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Add an extension to a file name
 * @param fname a pointer to a file name
 * @param extension pointer to a string
 * @return the extended file name
 */
static char *extend_fname(const char *fname, const char *extension) {
  error(fname == NULL || extension == NULL,
        "Invalid filename or extension pointer\n");
  char *new_fname = malloc(strlen(fname) + strlen(extension) +
                           1);  // make space for the new string
  strcpy(new_fname, fname);     // copy fname into the new_fname
  strcat(new_fname, extension); // add the extension
  return new_fname;
}

/**
 * @brief Print and free a solution in a file
 * NB: print "NO SOLUTION\n" if there is no solution
 * @param solution pointer to a solution
 * @param fname pointer to a file name
 */
void print_sol_in_file(sol solution, const char *fname) {
  error(solution == NULL || fname == NULL, "Bad pointer\n");

  char *new_fname = extend_fname(fname, ".sol");

  FILE *f = fopen(new_fname, "w");
  error(f == NULL, "Allocation pb\n");

  if (solution->nb_moves > 0) { // Si il y a une solution on print
    for (uint i = 0; i < solution->nb_moves; i++) {
      (i == 0) ? fprintf(f, "%u", solution->moves[i])
               : fprintf(f, " %u", solution->moves[i]);
    }
  } else {
    fprintf(f, "NO SOLUTION\n");
  }

  fclose(f);
  free(new_fname);
}

/**
 * @brief print the number of solutions in a file
 * @param nb_sol the number of solutions
 * @param fname pointer to a file name
 */
void print_nb_sol_in_file(uint nb_sol, const char *fname) {
  error(fname == NULL, "Bad pointer\n");

  char *new_fname = extend_fname(fname, ".nbsol");

  FILE *f = fopen(new_fname, "w");
  error(f == NULL, "Allocation pb\n");

  fprintf(f, "NB_SOL = %u\n", nb_sol);

  fclose(f);
  free(new_fname);
}

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
