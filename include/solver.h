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
 * @brief Free a solution
 * @param s a solution
 */
void free_sol(sol s);

/**
 * @brief Print and free a solution in a file
 * NB: print "NO SOLUTION\n" if there is no solution
 * @param solution pointer to a solution
 * @param fname pointer to a file name
 */
void print_sol_in_file(sol solution, const char *fname);

/**
 * @brief print the number of solutions in a file
 * @param nb_sol the number of solutions
 * @param fname pointer to a file name
 */
void print_nb_sol_in_file(uint nb_sol, const char *fname);

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
