#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"

/** Co-authors :
 * PITASSI Mario
 * KASPARIAN Anouche
 * ROUSSEAU Mattias
 * AKHOUN Farouk
 */

void grid_display(game g);
void play_game(game g);

int main(void) {
  // Initializing the game grid
  color tab[12*15];

  for (int i; i < 12*15; i++){
    tab[i] = rand()%16;
  }

  // Initializing a game with the grid above, SIZE and Coups_max
  game g = game_new_ext(12, 15, tab, 100, true);

  // Starting the game
  play_game(g);

  // Free the game
  game_delete(g);

  return EXIT_SUCCESS;
}

// Display game grid
void grid_display(game g) {

  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {

      color c = game_cell_current_color(g, x, y);

      if(c < 10) {
        printf("%d", c);
      }else {
        char c2 = 'A' + (c - 10);
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
    // Display the current state of the game
    printf("nb coups joués : %d ; nb coups max : %d \n", game_nb_moves_cur(g),
           game_nb_moves_max(g));

    // Display the current game grid
    grid_display(g);

    // Checking the current state of the game to avoid displaying useless information
    printf(
        "Jouer un coup : (0,1,2,3,r ou q; r pour redémarrer ou q pour "
        "quitter)\n");

    // Each round, we wait for the player to choose a color
    char coup;
    scanf(" %c", &coup);

    printf("\n");

    if (coup == 'r') {
      game_restart(g);
    }else if (coup == 'q') {
      printf("DOMMAGE \n");
      we_play = false;
    }else if (coup >='0' && coup <= '9') {  // Converting the char into the corresponding int using the ASCII table
      game_play_one_move(g, coup - '0');
    }else if (coup >= 'A' && coup <= 'F'){  // Converting a char into an int
      game_play_one_move(g, coup - 'A' + 10);
    }

    // If the game is over, display the grid and 'BRAVO'.
    if (game_is_over(g)) {
      printf("nb coups joués : %d ; nb coups max : %d \n", game_nb_moves_cur(g),
             game_nb_moves_max(g));
      grid_display(g);
      printf("BRAVO \n");
      we_play = false;
    }
  }
}
