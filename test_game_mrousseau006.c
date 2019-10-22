#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "game.h"

/* test game new empty */


bool test_game_new_empty (game* g){

    if (g == NULL){
        fprintf(stderr,"Error: le tableau du jeu n'est pas intialisé");
        return false;
    }

    for(int i=0; i<=SIZE-1; i++) {
        for(int j=0; j<=SIZE-1; j++) {

            if (game_cell_current_color (*g, j, i) != 0){
                fprintf(stderr,"Error:le tableau n'est pas initialisé à RED");
                return false;
            }
        }
    }

    if(game_nb_moves_max != 0){
        return false;
    }

    return true;
}



/* test delete game */

bool test_game_delete (game* g){

    if (g == NULL){
        fprintf(stderr,"Error:le tableau du jeu n'est pas intialisé");
        return false;
    }

    game_delete(g);

    if (g != NULL){
        return false;
    }
    return true;
}

/* test game current move */

bool test_game_nb_moves_cur(game* g, uint coups_max){

    if (g == NULL){
        fprintf(stderr,"Error: le tableau du jeu n'est pas intialisé");
        return false;
    }

    if (game_nb_moves_cur(g) != 0){
        return false;
    }

    for (int i = 0, i <= coups_max){
        int k = rand()%4;
        game_play_one_move(g, k);
        i++;

        if (game_nb_moves_cur(g) != i){
            return false;
        }

    }

    return true;
}


int main(int argc, char *argv[]) {

    // Initialisation d'un jeu.
    color tab[] = {
        0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0,
        0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0,
        1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2,
        2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0,
        2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0,
        0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3,
        1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2,
        1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3,
        0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0,
        1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0,
        1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2,
        2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};

    uint coups_max = 12;

    game jeu = game_new(tab, coups_max);

    //initialisation d'un tableau vide
    game jeu_vide = game_new_empty()

    // Démarrage des tests.
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;

    if (strcmp("game_new_empty", argv[1]) == 0)
        ok = test_game_new_empty(jeu_vide);
    else if(strcmp("game_delete", argv[1]) == 0) {
        ok = test_game_delete(jeu, coups_max);
    }else if(strcmp("game_nb_moves_cur", argv[1]) == 0) {
        ok = test_copy(jeu, coups_max,);
    }
    else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    game_delete(jeu);

    // Résultat des tests.
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
