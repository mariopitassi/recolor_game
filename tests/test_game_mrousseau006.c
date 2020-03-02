#include "game.h"
#include "solver.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialisation d'un jeu.
color tab[] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2,
               0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2,
               1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3,
               0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
               1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1,
               1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2,
               2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

/* test game new empty */
/**
 * @brief the function tests if the tested function worked correctly
 *
 * @return true if all the game structure's fields have been correctly
 * initialised
 * @return false if a field of the game structure hasn't been correctly
 * initialised
 */
bool errorJEU(char *err_mess, game g) {
  fprintf(stderr, "Error: %s \n\n", err_mess);
  game_delete(g);
  return false;
}
bool errorSOL(char *err_mess, game g, sol SOL) {
  fprintf(stderr, "Error: %s \n\n", err_mess);
  free_sol(SOL);
  game_delete(g);
  return false;
}
bool test_game_new_empty() {
  // initializing a new empty game and test if the game is correctly initialized
  game g = game_new_empty();

  if (g == NULL) {
    return errorJEU("Error: invalid pointer", g);
  }
  // checking if all the cells have been initialized to 0
  for (int i = 0; i <= SIZE - 1; i++) {
    for (int j = 0; j <= SIZE - 1; j++) {
      if (game_cell_current_color(g, j, i) != 0) {
        return errorJEU("Error: The array isn't initialized to 0", g);
      }
    }
  }
  // checking if the max nb moves has been initialized to 0
  if (game_nb_moves_max(g) != 0) {
    return errorJEU("Error :Number max move is wrong", g);
  }
  // free the created game
  game_delete(g);
  return true;
}
/* test game new empty ext */
/**
 * @brief the function tests if the tested function worked correctly
 *
 * @return true if all the game structure's fields have been correctly
 * initialised
 * @return false if a field of the game structure hasn't been correctly
 * initialised or the game hasn't been created
 */
bool test_game_new_empty_ext() {
  // creating testing parameters
  uint width = 8;
  uint height = 10;
  bool wrapping = true;
  // initializing a new game for the test with the testing parameters
  game g = game_new_empty_ext(width, height, wrapping);
  if (g == NULL) {
    return errorJEU(" Error: the game isn't initialized", g);
  }
  // checking if the game width has been correctly initialized
  if (game_width(g) != width) {
    return errorJEU("Error : Game width isn't initialized ", g);
  }
  // checking if the game height has been correctly initialized
  if (game_height(g) != height) {
    return errorJEU("Error: Game height isn't initialized ", g);
  }
  // checking if all the cells have been initialized to 0
  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g, x, y) != 0) {
        return errorJEU("Error: the array isn't initialized to 0", g);
      }
    }
  }
  // checking if the max nb moves has been initialized to 0
  if (game_nb_moves_max(g) != 0) {
    return errorJEU("Error : Number max move isn't initialized to 0", g);
  }
  // checking if the wrapping bool has been correctly initialized
  if (game_is_wrapping(g) != wrapping) {
    return errorJEU("Error: Wrapping bool isn't initialized correctly", g);
  }
  // free the created game
  game_delete(g);
  return true;
}
/* test delete game */
/**
 * @brief Tests is the game is fully deleted with the game_delete function
 *
 * @return true if the game has been fully deleted
 * @return false if the game doesn't exist
 */

bool test_game_delete() {
  // creating a testing parameter
  uint coups_max = 12;
  // initializing a new game
  game g = game_new(tab, coups_max);
  // checking if the game exists and has been correctly initialized
  if (g == NULL) {
    return errorJEU("Error: The game point isn't initialized", g);
  }
  // free the created game
  game_delete(g);

  return true;
}

/* test game current move */
/**
 * @brief Tests if the current nb_moves has been correctly initialized to 0 at
 the begginning of a game and then tests if the current nb_moves is correctly
 incremented after playing a move
 *
 * @return true if the current nb_moves is correctly initialized and
 incremented, also when the game is restarted
 * @return false if the game hasn't been correctly initialized or incremented.
 Also return false if the game doesn't exist
 */
bool test_game_nb_moves_cur() {
  uint coups_max = 12;
  // initializing a new game and testing if it has been correctly created
  game g = game_new(tab, coups_max);

  if (g == NULL) {
    return errorJEU("Error: The game pointer isn't initialized", g);
  }
  // checking current nb moves = 0 at the beggining of the game
  if (game_nb_moves_cur(g) != 0) {
    return errorJEU("Error : Number current moves isn't initialized to 0", g);
  }
  // playing random moves to increment current nb moves
  for (int i = 0; i <= coups_max;) {
    int k = rand() % 4;
    game_play_one_move(g, k);
    i++;
    // cheking if the current nb moves is correctly incremented after playing a
    // move
    if (game_nb_moves_cur(g) != i) {
      return errorJEU(
          "Error: Number current moves hasn't been incremented by 1", g);
    }
  }
  // restart the game
  game_restart(g);

  // checking current mb moves = 0 after restarting the game
  if (game_nb_moves_cur(g) != 0) {
    return errorJEU("Error : number current moves hasn't been reset", g);
  }
  // free the created game
  game_delete(g);

  return true;
}

/* test find_min */
bool test_find_min() {
  // uint coup_max = 12;
  // creating an array of solutions known
  color tabTest[] = {0, 1, 2, 2, 1, 2, 1, 1, 2, 2, 2, 2};

  // creating the game that we want to test
  game game_test = game_new_ext(3, 4, tabTest, 12, false);
  // finding the minimal solution, with this game there are only 2 solution with
  // different sizes
  sol stest = find_min(game_test);
  // giving the minimal solution
  color Solmin[] = {1, 2};
  uint Nb_min = 2;
  // compare the number of moves of the solution we found with the real min
  // solution
  if (stest->nb_moves != Nb_min) {
    return errorSOL("Error: The number of moves of the solution found is wrong",
                    game_test, stest);
  }
  // compare the solution we found witht he real solution
  for (int i = 0; i < stest->nb_moves; i++) {
    if (stest->moves[i] != Solmin[i]) {
      return errorSOL("Error: The minimal solution is wrong", game_test, stest);
    }
  }

  free_sol(stest);
  game_delete(game_test);

  return true;
}
/* test de find one */
bool test_find_one() {
  // creating an array of solutions known
  color tabTest[] = {0, 1, 2, 2, 1, 2, 1, 1, 2, 2, 2, 2};
  // creating the game with the array of testing
  game game_test = game_new_ext(3, 4, tabTest, 12, false);
  // finding a solution to test
  sol stest = find_one(game_test);
  // testing the solution
  for (int i = 0; i < stest->nb_moves; i++) {
    game_play_one_move(game_test, stest->moves[i]);
  }
  // verifying that the game is over after playing the solution we found
  if (!game_is_over(game_test)) {
    return errorSOL(
        "Error: the solution returned isn't a solution for the game", game_test,
        stest);
  }
  free_sol(stest);
  game_delete(game_test);
  return true;
}
/* Test free_sol */

bool test_free_sol() {
  // creating an array of solutions known
  color tabTest[] = {0, 1, 2, 2, 1, 2, 1, 1, 2, 2, 2, 2};
  // creating the game with the array of testing
  game game_test = game_new_ext(3, 4, tabTest, 12, false);
  // finding a solution to test
  sol stest = find_one(game_test);

  free_sol(stest);

  game_delete(game_test);
  return true;
}

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1)
    usage(argc, argv);

  // starting tests.
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp("game_new_empty", argv[1]) == 0)
    ok = test_game_new_empty();
  else if (strcmp("game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  } else if (strcmp("game_nb_moves_cur", argv[1]) == 0) {
    ok = test_game_nb_moves_cur();
  } else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  } else if (strcmp("find_min", argv[1]) == 0) {
    ok = test_find_min();
  } else if (strcmp("find_one", argv[1]) == 0) {
    ok = test_find_one();
  } else if (strcmp("free_sol", argv[1]) == 0) {
    ok = test_free_sol();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // Tests results.
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "\nTest \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
