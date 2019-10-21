#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "game.h"


// test game_play_one_move                pr le moment teste juste la 1ere case (+nb moves curr)

// done & working

bool test_game_play_one_move(game g){      
    int moves_hyp = game_nb_moves_cur(g)+1;
    color c = 0; //à randomiser?
    game_play_one_move(g,c);
    if (moves_hyp!=game_nb_moves_cur(g)){
        fprintf(stderr, "Error: invalid number of moves\n");
        return false;
    }
    if (game_cell_current_color(g,0,0)!=c){
        fprintf(stderr, "Error: invalid cell color\n");
        return false;
    }
    return true;
}



// test game_is_over                -> done thx to mario

bool test_game_is_over(game g){
    if(g == NULL) {
        fprintf(stderr, "Error: pb memoire\n");
        return false;
    }
    //jouer jusqu'à la fin w/ if dans boucle jeu? puis 2e if hors boucle
    color moves[]={3,1,3,1,0,3,1,0,1,3,2,0};
    for (int i=0; i<12; i++){
        game_play_one_move(g, moves[i]);
    }

    return game_is_over(g) && true;
}


// test game_restart          -> done & working

bool test_game_restart(game g, color* tab){
    //même chose que mario et game new???????
    if(g == NULL) {
        fprintf(stderr, "Error: pb memoire\n");
        return false;
    }

    for (int i = 0; i <= SIZE - 1; i++) {
        for (int j = 0; j <= SIZE - 1; j++) {
            if (game_cell_current_color(g, j, i) != tab[12*i+j]) {
                fprintf(stderr, "Error: pb initialisation cellules\n");
                return false;
            }
        }
    }

    if (game_nb_moves_cur(g)!=0||game_nb_moves_max(g)!=12){
        fprintf(stderr, "Error: pb nb moves\n");
        return false;
    }
    return true;
}


// usage (given)
void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}



int main(int argc, char *argv[]) {
    if (argc == 1) usage(argc, argv);

    //initialisation game pour les tests
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

    game game_test = game_new(tab, coups_max);



    // start test
    fprintf(stderr, "=> Start test akasparian \"%s\"\n", argv[1]);
    bool ok = false;

    if (strcmp("game_play_one_move", argv[1]) == 0)
        ok = test_game_play_one_move(game_test);
    else if (strcmp("game_is_over", argv[1]) == 0)
        ok = test_game_is_over(game_test);
    else if (strcmp("game_restart", argv[1]) == 0)
        ok = test_game_restart(game_test, tab);
    else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }


}