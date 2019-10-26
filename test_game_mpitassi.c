#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "game.h"

// Author: PITASSI Mario

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

/* ****** TEST GAME_NEW ***** */

bool test_game_new() {

    uint coups_max = 12;
    game g = game_new(tab, coups_max);

    if(g == NULL) {
        fprintf(stderr, "Error : Table de la structure game non initialisé.");
        return false;
    }

    for (int i = 0; i <= SIZE - 1; i++) {
        for (int j = 0; j <= SIZE - 1; j++) {
            if (game_cell_current_color(g, j, i) != tab[12*i+j]) {
                fprintf(stderr, "Error : Les cellules ne sont pas initialisées avec les bonnes valeurs du tableau.");
                game_delete(g);
                return false;
            }
        }
    }

    for(uint i=0; i<=SIZE;i++) {
        game g2 = game_new(tab,i);
        if(g2 == NULL) {
            fprintf(stderr,"Error: Pointeur NULL");
            game_delete(g);
            return false;
        }
      
        if(game_nb_moves_max(g2) != i) {
            fprintf(stderr, "Error: max moves not initialize");
            game_delete(g2);
            game_delete(g);
            return false;
        }
          
        game_delete(g2);
    }

    uint coups_test = game_nb_moves_max(g);

    if (coups_max != coups_test) {
        fprintf(stderr,"Error : Le nombre de coups max ne coïncide pas : %d != %d", coups_max, coups_test);
        game_delete(g);
        return false;
    }

    uint coups_current_test = game_nb_moves_cur(g);

    if(coups_current_test != 0){
        fprintf(stderr,"Error : Le nombre initial de coups joué ne coïncide pas : %d != %d", coups_current_test, 0);
        game_delete(g);
        return false;
    }

    game_delete(g);
    return true;
}

/* ******  TEST SET_MAX_MOVES ****** */

bool test_set_max_moves() {

    uint coups_max = 12;
    game g = game_new(tab, coups_max);

    if(g == NULL) {
        fprintf(stderr, "Error : Table de la structure game non initialisé.");
        return false;
    }

    uint coups = 54;

    if(game_nb_moves_cur(g) > coups) {
        fprintf(stderr,"Error : Impossible de définir un coups_max = %d < coups_cur =  %d", coups, game_nb_moves_cur(g));
        game_delete(g);
        return false;     
    }
    
    game_set_max_moves(g, coups);

    uint coups_test = game_nb_moves_max(g);

    if (coups != coups_test) {
        fprintf(stderr,"Error : Le nombre de coups max ne coïncide pas : %d != %d", coups, coups_test);
        game_delete(g);
        return false;
    }

    game g2 = game_new_empty();

    game_set_max_moves(g2, coups);

    uint coups_test2 = game_nb_moves_max(g2);

    if(coups_test2 != coups) {
        fprintf(stderr,"Error : Le nombre de coups max ne coïncide pas sur game_new_empty : %d != %d", coups, coups_test2);
        game_delete(g2);
        return false;
    }

    game_delete(g);
    game_delete(g2);
    return true;
}

/* ****** TEST COPY ***** */
bool test_copy() {

    uint coups_max = 12;
    game g = game_new(tab, coups_max);

    if (g == NULL) {
        fprintf(stderr, "Error : Partie d'origine non initialisée : pas assez de mémoire");
        return false;
    }

    game g2 = game_copy(g);

    if (g2 == NULL) {
        fprintf(stderr, "Error : Partie copiée non initialisé : pas assez de mémoire");
        return false;
    }

    for (int i = 0; i <= SIZE - 1; i++) {
        for (int j = 0; j <= SIZE - 1; j++) {
            if (game_cell_current_color(g, j, i) != game_cell_current_color(g2, j, i)) {
                fprintf(stderr, "Error : Les cellules copiées ne sont pas initialisées correctement.");
                game_delete(g);
                game_delete(g2);
                return false;
            }
        }
    }

    if (game_nb_moves_max(g) != game_nb_moves_max(g2)) {
        fprintf(stderr, "Error : Le nombre de coups max est différent");
        game_delete(g);
        game_delete(g2);
        return false;
    }

    if (game_nb_moves_cur(g) != game_nb_moves_cur(g2)) {
        fprintf(stderr, "Error : Le nombre de coups max est différent");
        game_delete(g);
        game_delete(g2);
        return false;
    }

    game_delete(g);
    game_delete(g2);
    return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}


/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
    if (argc == 1) usage(argc, argv);

    // Démarrage des tests.
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;

    if (strcmp("game_new", argv[1]) == 0)
        ok = test_game_new();
    else if(strcmp("set_max_moves", argv[1]) == 0) {
        ok = test_set_max_moves();
    }else if(strcmp("copy", argv[1]) == 0) {
        ok = test_copy();
    }else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Résultat des tests.
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }else {
        fprintf(stderr, "\nTest \"%s\" finished: FAILURE\n\n", argv[1]);
        return EXIT_FAILURE;
    }
}
