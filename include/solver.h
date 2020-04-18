#ifndef SOLVER_H
#define SOLVER_H

#include "game.h"
#include "game_io.h"
#include "toolbox.h"
#include <stdio.h>

/**
 * @brief The structure pointer that stores the solution.
 */
typedef struct solution {
  color *moves;  // Tab of moves
  uint nb_moves; // Number of moves / Length of tab
} * sol;

/**
 * @brief Error handler
 *
 * @param cond is a test that raise an error or not
 * @param err_mess an output message
 */
void error(bool cond, char *err_mess);

/**
 * @brief Allocate memory for a solution structure and initialized it to default
 * NB: The tab is not allocated (look_for_sol will do it if a solution is found)
 * @return sol* a pointer to the allocated solution structure
 */
sol sol_alloc();

/**
 * @brief Free a solution
 * @param s a solution
 */
void free_sol(sol s);

/**
 * @brief Play a hit board
 *
 * @param g a game instance
 * @param moves a tab of color to play iteratively
 * @param nb_moves total of moves
 */
void game_play_moves(game g, color *moves, uint nb_moves);

/**
 * @brief Play a hit board and count the number of color in the grid
 *
 * @param g a game instance
 * @param moves a colors array
 * @param nb_moves number of moves to play
 * @return uint number of color in the grid
 */
uint nb_colors(cgame g, color *moves, uint nb_moves);

/**
 * @brief Returns a list of adjacent colors after playing the colors in the
 * "moves" array from "moves [0] to moves [nb_moves-1]" NB : Last optimization
 * possible : sort this list
 * @param g an instance of a game
 * @param moves array of played moves
 * @param nb_moves nb of moves to play
 * @return SList a list of adjacent colors
 */
SList col_around(cgame g, color *moves, uint nb_moves);

/**
 * @brief Finds the solution sequence being the smallest
 *
 * @param g a game instance
 * @return sol structure with all params initialized if a solution is found
 * else sol->nb_moves = 0 and sol->moves = NULL
 */
sol find_min(cgame g);

/**
 * @brief Get the first sequence of solution that is found
 *
 * @param g a game instance
 * @param nb_moves_max the maximum length of the sequence
 * @return sol structure with all params initialized if a solution is found
 * else sol->nb_moves = 0 and sol->moves = NULL
 */
sol find_one(cgame g, uint nb_moves_max);

/**
 * @brief Get the number of solutions from a a game instance
 *
 * @param g a game instance
 * @return uint number of solutions
 */
uint nb_sol(cgame g);

/**
 * @brief Get the first sequence of solution that is found
 * for graphic interface (look for 0.5 second max)
 * NB: Same as find_one, just added a timer
 *
 * @param g a game instance
 * @param nb_moves_max the maximum length of the sequence
 * @return sol structure with all params initialized if a solution is found
 * else sol->nb_moves = 0 and sol->moves = NULL
 */
sol find_one_gui(cgame g, uint nb_moves_max);

/**
 * @brief Search minimal solution for 1 second maximum
 * Function used in graphic interface
 * NB: Same as find_min, just added a timer
 *
 * @param g a game instance
 * @return sol structure with all params initialized if a solution is found
 * else sol->nb_moves = 0 and sol->moves = NULL
 */
sol find_min_gui(cgame g);

#endif // SOLVER_H
