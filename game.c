#include <stdlib.h>
#include <stdio.h>
#include "game_io.h"
#include "game.h"


typedef struct game_s {
    color *tab_init;
    color *tab_cur;
    uint size;
    uint move_max;
    uint move_cur;
} *game;