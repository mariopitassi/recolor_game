#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* extend_fname(const char* fname, const char* extension){
  char* new_fname = malloc(strlen(fname)+strlen(extension)+1);  // make space for the new string
  strcpy(new_fname, fname); // copy fname into the new_fname
  strcat(new_fname, extension); // add the extension
  return new_fname;
}

void free_sol_and_print_in_file(sol solution, const char* fname){
  error(solution==NULL || fname==NULL, "Bad pointer\n");

  char* new_fname = extend_fname(fname, ".sol");

  FILE* f = fopen(new_fname, "w");
  error(f==NULL, "Allocation pb\n");

  if (solution->nb_moves > 0){
    for (uint i = 0; i < solution->nb_moves; i++){
      (i == 0) ?
        fprintf(f, "%u", solution->moves[i]):
        fprintf(f, " %u", solution->moves[i]);
    }
    free(solution->moves);
    free(solution);
  }
  else{
    fprintf(f, "NO SOLUTION\n");
    free(solution);
  }

  fclose(f);
  free(new_fname);
}

void print_nb_sol_in_file(uint nb_sol, const char* fname){
  error(fname==NULL, "Bad pointer\n");

  char* new_fname = extend_fname(fname, ".nbsol");

  FILE* f = fopen(new_fname, "w");
  error(f==NULL, "Allocation pb\n");

  fprintf(f, "NB_SOL = %u\n", nb_sol);

  fclose(f);
  free(new_fname);
}


static void raise_usage(int code) {

  switch (code) {
  case 1: // Cas FIND_ONE
    printf("FIND_ONE\n");
    break;
  case 2: // Cas NB_SOL
    printf("NB_SOL \n");
    break;
  case 3: // Cas FIN_MIN
    printf("FIND_MIN \n");
    break;
  default: // Cas défaut, mauvais argument
    printf(
        "Usage: ./recolor_solve FIND_ONE|NB_SOL|FIN_MIN <input> <output> \n");
    break;
  }

  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  if (argc != 4)
    raise_usage(0);

  game g = game_load(argv[2]);

  if (strcmp(argv[1], "FIND_ONE") == 0 || strcmp(argv[1], "FIND_MIN") == 0) {
    sol solution = (strcmp(argv[1], "FIND_ONE") == 0) ? find_one(g) : find_min(g);
    free_sol_and_print_in_file(solution, argv[3]);
  }
  else if (strcmp(argv[1], "NB_SOL") == 0) {
    print_nb_sol_in_file(nb_sol(g), argv[3]);
  }
  else {
    game_delete(g);
    raise_usage(0);
  }

  game_delete(g);
}

/**
int main(int argc, char *argv[]) {

  if (argc == 1)
    raise_usage(0);

  if (strcmp(argv[1], "FIND_ONE") == 0) {

    if (argc != 3)
      raise_usage(1);

  } else if (strcmp(argv[1], "NB_SOL") == 0) {

    if (argc != 3)
      raise_usage(2);

  } else if (strcmp(argv[1], "FIN_MIN") == 0) {

    if (argc != 3)
      raise_usage(3);

  } else
    raise_usage(0);
}
**/
