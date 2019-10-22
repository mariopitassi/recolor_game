#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "game.h"

/* ****** TEST GAME_NEW ***** */

bool test_game_new(game g, color* tab, uint coups) {

    if(g == NULL) {
        fprintf(stderr, "Error : Table de la structure game non initialisé.");
        return false;
    }

    for (int i = 0; i <= SIZE - 1; i++) {
        for (int j = 0; j <= SIZE - 1; j++) {
            if (game_cell_current_color(g, j, i) != tab[12*i+j]) {
                fprintf(stderr, "Error : Les cellules ne sont pas initialisées avec les bonnes valeurs du tableau.");
                return false;
            }
        }
    }

    uint coups_test = game_nb_moves_max(g);

    if (coups != coups_test)
    {
        fprintf(stderr,"Error : Le nombre de coups max ne coïncide pas : %d != %d", coups, coups_test);
        game_restart(g);
        return false;
    }

    game_restart(g);
    return true;
}

/* ******  TEST SET_MAX_MOVES ****** */

bool test_set_max_moves(game g) {
    if(g == NULL) {
        fprintf(stderr, "Error : Table de la structure game non initialisé.");
        return false;
    }

    uint coups = 54;
    
    game_set_max_moves(g, coups);

    uint coups_test = game_nb_moves_max(g);

    if (coups != coups_test) {
        fprintf(stderr,"Error : Le nombre de coups max ne coïncide pas : %d != %d", coups, coups_test);
        game_restart(g);
        return false;
    }

    return true;
}

/* ****** TEST COPY ***** */
bool test_copy(game g, uint coup) {

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
                return false;
            }
        }
    }

    if (game_nb_moves_max(g) != game_nb_moves_max(g2)) {
        fprintf(stderr, "Error : Le nombre de coups max est différent");
        game_delete(g2);
        game_restart(g);
        return false;
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

    // Démarrage des tests.
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;

    if (strcmp("game_new", argv[1]) == 0)
        ok = test_game_new(jeu, tab, coups_max);
    else if(strcmp("set_max_moves", argv[1]) == 0) {
        ok = test_set_max_moves(jeu);
    }else if(strcmp("copy", argv[1]) == 0) {
        ok = test_copy(jeu, coups_max);
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
        fprintf(stderr, "\nTest \"%s\" finished: FAILURE\n\n", argv[1]);
        return EXIT_FAILURE;
    }
}
