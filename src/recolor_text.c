#include "game.h"
#include "game_io.h"
#include "game_rand.h"
#include <stdio.h>
#include <stdlib.h>

/** Co-authors :
 * PITASSI Mario
 * KASPARIAN Anouche
 * ROUSSEAU Mattias
 * AKHOUN Farouk
 */

void grid_display(game g);
void play_game(game g);

int main(int argc, char *argv[]) {
  game g;

  // Load the default game or a selected game in ./data
  // g = (argc == 1) ? game_load("data/default_game.rec") : game_load(argv[1]);

  if (argc == 3 || argc > 6) {
    fprintf(stderr, "Usage: ./recolor_text [<w> <h> <nb_mov_max> "
                    "[<nb_max_color>] [<S|N>]]\n");
    return EXIT_FAILURE;
  } else if (argc == 1) {
    g = game_load("data/default_game.rec");
  } else if (argc == 2) {
    g = game_load(argv[1]);
  } else {

    uint w = atoi(argv[1]);
    uint h = atoi(argv[2]);
    uint mov_max = atoi(argv[3]);
    uint nb_max_color = 4; // default
    uint is_wrap = false;  // default

    if (argc > 4)
      nb_max_color = atoi(argv[4]);
    if (argc == 6 && argv[5][0] == 'S')
      is_wrap = true;

    g = game_random_ext(w, h, is_wrap, nb_max_color, mov_max);
  }
  // Starting the game
  play_game(g);

  // Free the game
  game_delete(g);

  return EXIT_SUCCESS;
}

// Display game grid
void grid_display(game g) {

  // Display the current state of the game
  printf("nb coups joués : %u ; nb coups max : %u \n", game_nb_moves_cur(g),
         game_nb_moves_max(g));

  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {

      color c = game_cell_current_color(g, x, y);

      if (c < 10) {
        printf("%u", c);
      } else {
        char c2 = 'A' + (c - 10); // Converting an int into a char (ASCII table)
        printf("%c", c2);
      }

      if (x == game_width(g) - 1) { // We have reached an edge
        printf("\n");
      }
    }
  }

  printf("\n \n");
}

void play_game(game g) {
  bool we_play = true;
  while (we_play) {

    grid_display(g);

    // Checking the current state of the game to avoid displaying useless
    // information
    printf("Jouer un coup : (0,1,2,3,r ou q; r pour redémarrer ou q pour "
           "quitter)\n");

    // Each round, we wait for the player to choose a color
    char coup;
    scanf(" %c", &coup);
    printf("\n");

    if (coup == 'r') {
      game_restart(g);
    } else if (coup == 'q') {
      printf("DOMMAGE \n");
      we_play = false;
    } else if (coup >= '0' && coup <= '9') {
      game_play_one_move(
          g, coup - '0'); // Converting a char into an int (ASCII table)
    } else if (coup >= 'A' && coup <= 'F') {
      game_play_one_move(
          g, coup - 'A' + 10); // Converting a char into an int (ASCII table)
    }

    // If the game is over, display the grid and 'BRAVO'.
    if (game_is_over(g)) {
      grid_display(g);
      printf("BRAVO \n");
      we_play = false;
    }
  }
}
