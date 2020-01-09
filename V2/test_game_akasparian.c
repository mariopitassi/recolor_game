#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"



/**
 * Compares a game grid and a given array of cells
 * Returns false if there is a difference between the two
 * true otherwise
**/
bool same_grid(game g, color* tab){
  for (int i = 0; i < game_height(g); i++) {
    for (int j = 0; j < game_width(g); j++) {
      if (game_cell_current_color(g, j, i) != tab[game_height(g) * i + j]) {
        return false;
      }
    }
  }
  return true;
}


/**
 * Creates a game with a random size and maximum number of moves
 * and a given wrapping value
**/
/**
game random_game(color *tab, bool wrap){
  int m = rand();
  int x = rand();
  int y = rand();
  game g = game_new_ext(x, y, tab, m, wrap);
  return g;
}**/


/* ****** TEST GAME_PLAY_ONE_MOVE ***** */        //(for now only checks the 1st square)

bool test_game_play_one_move(game g) {
  // predicted number of moves played post-move
  int moves_hyp =
      game_nb_moves_cur(g) + 1;

  int c = rand() % 4;     // random color

  game_play_one_move(g, c);

  // checks if the result matches what we infered
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


/* ****** TEST GAME_IS_OVER ***** */

bool test_game_is_over(game g) {
  if (g == NULL) {
    fprintf(stderr, "Error: memory problem\n");
    return false;
  }

  bool camarche = false;

  if (game_is_over(g)) {
    fprintf(stderr, "Error: game over before it even started\n");
    game_delete(g);
    return false;
  }

  // plays until the end and wins
  color moves[] = {3, 1, 3, 1, 0, 3, 1, 0, 1, 3, 2, 0};
  for (int i = 0; i < 11; i++) {
    game_play_one_move(g, moves[i]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: game over while the grid is not finished\n");
      game_delete(g);
      return false;
    }
  }
  game_play_one_move(g, moves[11]);
  if (game_is_over(g)) {
    camarche = true;
  }

  // restarts the game and plays random moves
  game_restart(g);
  color ran_moves[] = {2, 1, 0, 3, 0, 1, 2, 3, 1, 0, 2, 0};
  for (int n = 0; n < 12; n++) {
    game_play_one_move(g, ran_moves[n]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: game over while it was not won\n");
      game_delete(g);
      return false;
    }
  }

  // keeps playing after reaching the maximum number of moves allowed
  color next_moves[] = {2, 1, 0, 3, 2, 0, 1};
  for (int n = 0; n < 7; n++) {
    game_play_one_move(g, next_moves[n]);
    if (game_is_over(g)) {
      fprintf(stderr, "Error: game over while too many moves were played\n");
      game_delete(g);
      return false;
    }
  }
  
  if (camarche == false) {
    fprintf(stderr, "Error: should be over but is not\n");
  }
  game_delete(g);
  return camarche;
}


/* ****** TEST GAME_RESTART ***** */

bool test_game_restart(color *tab) {

  game g = game_new_ext(12, 12, tab, 15, true);
  color ran_moves[] = {2, 1, 0, 3, 0, 1, 2, 3, 1, 0, 2, 0};
  for (int n = 0; n < 12; n++) {
    game_play_one_move(g, ran_moves[n]);
  }

  // restarts and checks if all the parameters have been properly reset
  game_restart(g);

  if(!same_grid(g, tab)){
    fprintf(stderr, "Error: cells not initialized properly\n");
    game_delete(g);
    return false;
  }

  if (game_nb_moves_cur(g) != 0 || game_nb_moves_max(g) != 15) {
    fprintf(stderr, "Error: incorrect number of moves\n");
    game_delete(g);
    return false;
  }

  if(game_is_wrapping(g)!=true){
    fprintf(stderr, "Error: should be wrapping\n");
    game_delete(g);
    return false;
  }

  game_delete(g);
  return true;
}


/* ****** TEST GAME_NEW_EXT ***** */

bool test_game_new_ext(color *tab){
  //creates a new game with random parameters, wrapping
  game g = game_new_ext(12, 12, tab, 15, true);

  // checks if everything has been properly set
  if(game_width(g)!=12){
    fprintf(stderr, "Error: incorrect width\n");
    game_delete(g);
    return false;
  }

  if(game_height(g)!=12){
    fprintf(stderr, "Error: incorrect height\n");
    game_delete(g);
    return false;
  }

  if(game_nb_moves_max(g)!=15){
    fprintf(stderr, "Error: incorrect maximum number of moves\n");
    game_delete(g);
    return false;
  }

  if(game_nb_moves_cur(g)!=0){
    fprintf(stderr, "Error: incorrect current number of moves\n");
    game_delete(g);
    return false;
  }

  if(game_is_wrapping(g)!=true){
    fprintf(stderr, "Error: game should be wrapping and is not\n");
    game_delete(g);
    return false;
  }

  if(!same_grid(g, tab)){
    fprintf(stderr, "Error: cells not initialized properly\n");
    game_delete(g);
    return false;
  }

  game_delete(g);

  // creates a new game NOT wrapping
  game gf = game_new_ext(12, 12, tab, 15, false);

  if(game_is_wrapping(gf)!=false){
    fprintf(stderr, "Error: game is wrapping while it should not be\n");
    game_delete(gf);
    return false;
  }

  game_delete(gf);
  return true;

}


/* ****** USAGE ***** */
void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  // game initialisation for tests
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
    ok = test_game_restart(tab);
  else if(strcmp("game_new_ext", argv[1]) == 0)
    ok = test_game_new_ext(tab);
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test results
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}