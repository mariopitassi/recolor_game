#ifndef SOLVER_H
#define SOLVER_H
#include "asde_slist.h"
#include "game.h"
#include "game_io.h"
#include <stdio.h>

/**
 * @brief The structure pointer that stores the solution.
 */
typedef struct solution {
  color *moves;  // Tab of moves
  uint nb_moves; // Number of mobes / Length of tab
} * sol;

sol sol_alloc();

color *moves_alloc(uint nb_moves);


/**
 * @brief Error handler
 *
 * @param cond is a test that raise an error or not
 * @param err_mess an output message
 */
void error(bool cond, char *err_mess);

/**
 * @brief Updates the "color_around" list with the adjacent colors recursively
 * NB: The list is sorted
 *
 * @param g a game instance
 * @param x a position on the abscissa axis
 * @param y a position on the ordinate axis
 * @param oldcolor color to update
 * @param color_around list of color uptated...
 * @param counter represents the number of times a color is in the neighborhood
 */
void around(game g, uint x, uint y, color oldcolor, SList color_around,
            uint *nb_color_around);

/**
 * @brief Finds the solution sequence being the smallest
 *
 * @param g a game instance
 * @return sol structure with all params initialized if a solution is found
 * else sol->nb_moves = 0
 */
sol find_min(cgame g);

/**
 * @brief Get the first sequence of solution that is found
 *
 * @param g a game instance
 * @return sol structure with all params initialized
 */
sol find_one(cgame g);

/**
 * @brief Get the number of solutions from a a game instance
 *
 * @param g a game instance
 * @return uint number of solutions
 */
uint nb_sol(cgame g);

#endif // SOLVER_H
