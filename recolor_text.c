#include <stdlib.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"

/* Co-auteurs : PITASSI Mario
                KASPARIAN Anouche
*/

void grid_display(game* g);
void game_start(game g);

int main(void) {

    // Initialisation de la grille de début de partie.
    color tab[] = {
        0,0,0,2,0,2,1,0,1,0,3,0,
        0,3,3,1,1,1,1,3,2,0,1,0,
        1,0,1,2,3,2,3,2,0,3,3,2,
        2,3,1,0,3,2,1,1,1,2,2,0,
        2,1,2,3,3,3,3,2,0,1,0,0,
        0,3,3,0,1,1,2,3,3,2,1,3,
        1,1,2,2,2,0,0,1,3,1,1,2,
        1,3,1,3,1,0,1,0,1,3,3,3,
        0,3,0,1,0,0,2,1,1,1,3,0,
        1,3,1,0,0,0,3,2,3,1,0,0,
        1,3,3,1,1,2,2,3,2,0,0,2,
        2,0,2,3,0,1,1,1,2,3,0,1 };


    // Inititialisation d'une partie avec la grille ci-dessus et de SIZE coups max
    game g = game_new(tab, SIZE);

    // Lancement de la partie 'g'
    game_start(g);

    // Suppression de la partie 'g'
    game_delete(g);
    
    return EXIT_SUCCESS;
}

// Affichage de la grille de jeu
void grid_display(game* g) {
    int i, j;

    for(i=0; i<=SIZE-1; i++) {
        for(j=0; j<=SIZE-1; j++) {

            printf("%d", game_cell_current_color(*g, j, i));

            if (j == 11) {
                printf("\n");
            }
        }
    }

    printf("\n \n");
}

void game_start(game g) {
    bool do_game = true;
    while (do_game) {

        // Affichage du status actuel de la partie
        uint coups_joues = game_nb_moves_cur(g);
        uint coups_max = game_nb_moves_max(g);
        printf("nb coups joués : %d ; nb coups max : %d \n", coups_joues, coups_max);

        // Affichage de la grille en cours
        grid_display(&g);
        
        // Si la partie est terminée, affichage de la grille, 'BRAVO' et rien d'autre.
        if (game_is_over(g)) {
            printf("BRAVO \n");
            do_game = false;
        }

        // Vérification du status de la partie afin d'éviter de proposer de jouer un coup 
        // alors que c'est terminé.
        if (do_game) printf("Jouer un coup : (0,1,2,3,r ou q; r pour redémarrer ou q pour quitter) \n");
        
        // À chaque tour, on attends la valeur saisie par le joueur (r, q, 0, 1, 2, 3).
        const char coup = getchar();
        printf("\n"); 

        if (coup == 'r') {
            game_restart(g);
        }else if (coup == 'q') {
            printf("DOMMAGE \n");
            do_game = false;
        }else if (coup == '0' || coup == '1' || coup == '2' || coup == '3') 
            game_play_one_move(g, atoi(&coup));
        
    }
}
