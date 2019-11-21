#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "game.h"

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

/* test game new empty */

bool test_game_new_empty()
{

    game g = game_new_empty();

    if (g == NULL)
    {
        fprintf(stderr, "Error: le tableau du jeu n'est pas intialisé");
        return false;
    }

    for (int i = 0; i <= SIZE - 1; i++)
    {
        for (int j = 0; j <= SIZE - 1; j++)
        {

            if (game_cell_current_color(g, j, i) != 0)
            {
                fprintf(stderr, "Error:le tableau n'est pas initialisé à RED");
                game_delete(g);
                return false;
            }
        }
    }

    if (game_nb_moves_max(g) != 0)
    {
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* test delete game */

bool test_game_delete()
{

    uint coups_max = 12;

    game g = game_new(tab, coups_max);

    if (g == NULL)
    {
        fprintf(stderr, "Error:le tableau du jeu n'est pas intialisé");
        return false;
    }

    game_delete(g);

    return true;
}

/* test game current move */

bool test_game_nb_moves_cur()
{

    uint coups_max = 12;

    game g = game_new(tab, coups_max);

    if (g == NULL)
    {
        fprintf(stderr, "Error: le tableau du jeu n'est pas intialisé");
        return false;
    }

    if (game_nb_moves_cur(g) != 0)
    {
        game_delete(g);
        return false;
    }

    for (int i = 0; i <= coups_max;)
    {
        int k = rand() % 4;
        game_play_one_move(g, k);
        i++;

        if (game_nb_moves_cur(g) != i)
        {
            game_delete(g);
            return false;
        }
    }

    game_restart(g);

    if (game_nb_moves_cur(g) != 0)
    {
        game_delete(g);
        return false;
    }

    game_delete(g);

    return true;
}
void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

    // Démarrage des tests.
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;

    if (strcmp("game_new_empty", argv[1]) == 0)
        ok = test_game_new_empty();
    else if (strcmp("game_delete", argv[1]) == 0)
    {
        ok = test_game_delete();
    }
    else if (strcmp("game_nb_moves_cur", argv[1]) == 0)
    {
        ok = test_game_nb_moves_cur();
    }
    else
    {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Résultat des tests.
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "\nTest \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
