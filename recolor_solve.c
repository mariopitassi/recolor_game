#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Raise a  usage 
 * 
 * @param code 
 */
static void raise_usage(int code) {

    switch(code) {
        case 1: // Cas FIND_ONE
            printf("FIND_ONE\n");
            break;
        case 2: // Cas NB_SOL
            printf("NB_SOL \n");
            break;
        case 3: // Cas FIN_MIN
            printf("FIN_MIN \n");
            break;
        default: // Cas défaut, mauvais argument
            printf("Usage: ./recolor_solve FIND_ONE|NB_SOL|FIN_MIN <input> <output> \n");
            break;
    }

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  if (argc == 1)
    raise_usage(0);

  if (strcmp(argv[1], "FIND_ONE") == 0) {

    if (argc != 3)
      raise_usage(1);

  } else if (strcmp(argv[1], "NB_SOL") == 0) {

    if (argc != 3)
      raise_usage(2);

  } else if (strcmp(argv[1], "FIN_MIN") == 0) {

    if (argc != 3)
      raise_usage(3);

  } else
    raise_usage(0);
}