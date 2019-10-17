#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "game.h"

/* test game new empty */


bool test_gameNewEmpty (game* g){

    if (g == NULL){
        fprintf(stderr,"le tableau du jeu n'est pas intialisé");
        return false;
    }

    for(int i=0; i<=SIZE-1; i++) {
        for(int j=0; j<=SIZE-1; j++) {

            if (game_cell_current_color (*g, j, i) != 0){
                fprintf(stderr,"le tableau n'est pas initialisé à 0");
                return false;
            }
        }
    }

    if()
}


