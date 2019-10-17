#include <stdio.h>
#include <stdlib.h>

#include "game.h"

/* ****** TEST 1 ***** */

int main(int argc, char *argv[]) {
    if (argc == 1)
        usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test mpitassi \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("init_free", argv[1]) == 0)
        ok = test_init_free();
    else if (strcmp("push", argv[1]) == 0)
        ok = test_push(100);
    else
    {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
