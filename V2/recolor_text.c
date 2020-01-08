#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_io.h"

/** Co-auteurs :
 * PITASSI Mario
 * KASPARIAN Anouche
 * ROUSSEAU Mattias
 * AKHOUN Farouk
 */

void grid_display(game g);
void play_game(game g);

int main(void) {
  // Initialisation de la grille de début de partie.
  color tab[12*15];

  for (int i; i < 12*15; i++){
    tab[i] = rand()%16;
  }

  // Initialisation d'une partie avec la grille ci-dessus et de SIZE coups max
  game g = game_new_ext(12, 15, tab, 100, true);

  // Lancement de la partie 'g'
  play_game(g);

  // Suppression de la partie 'g'
  game_delete(g);

  return EXIT_SUCCESS;
}

// Affichage de la grille de jeu
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

      if (x == game_width(g) - 1) { // We have reached the edge
        printf("\n");
      }
    }
  }

  printf("\n \n");
}

void play_game(game g) {
  bool we_play = true;
  while (we_play) {
    // Affichage du statut actuel de la partie
    printf("nb coups joués : %d ; nb coups max : %d \n", game_nb_moves_cur(g),
           game_nb_moves_max(g));

    // Affichage de la grille en cours
    grid_display(g);

    // Vérification du statut de la partie afin d'éviter de proposer de jouer un
    // coup alors que c'est terminé.
    printf(
        "Jouer un coup : (0,1,2,3,r ou q; r pour redémarrer ou q pour "
        "quitter)\n");

    // À chaque tour, on attend la valeur saisie par le joueur (r, q, 0, 1, 2,
    // 3).
    char coup;
    scanf(" %c", &coup);

    printf("\n");

    if (coup == 'r') {
      game_restart(g);
    }else if (coup == 'q') {
      printf("DOMMAGE \n");
      we_play = false;
    }else if (coup >='0' && coup <= '9') {  // On convertit le char en nombre grâce à la table ASCII
      game_play_one_move(g, coup - '0');
    }else if (coup >= 'A' && coup <= 'F'){  // Conversion d'un char en int
      game_play_one_move(g, coup - 'A' + 10);
    }

    // Si la partie est terminée, affichage de la grille et 'BRAVO'.
    if (game_is_over(g)) {
      printf("nb coups joués : %d ; nb coups max : %d \n", game_nb_moves_cur(g),
             game_nb_moves_max(g));
      grid_display(g);
      printf("BRAVO \n");
      we_play = false;
    }
  }
}
