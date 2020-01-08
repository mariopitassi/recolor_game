#include "game_io.h"
#include "game.h"

/* ************* FICHIER A IGNORER POUR L'INSTANT *********** */

/**
 * @brief Creates a game by loading its description in a file
 *  NB : THE FORMAT WILL BE SPECIFIED LATER
 * @param filename
 * @return the loaded game
 **/
game game_load(char *filename) {
  game g = game_new_empty();
  return g;
}

/**
 * @brief Save a game in a file
 *  NB : THE FORMAT WILL BE SPECIFIED LATER
 * @param g game to save
 * @param filename output file
 **/
void game_save(cgame g, char *filename) {}
