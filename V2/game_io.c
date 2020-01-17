#include "game_io.h"
#include "game.h"
#include <stdlib.h>

#define MAXLINELEN 4096

/**
 * @brief Read one line
 * 
 * @param f 
 * @param size 
 * @return char* 
 */
static char* read_one_line(FILE* f, uint* size) {

}
/**
 * @brief Extract data from the first line of the file.
 * 
 * @param w 
 * @param h 
 * @param nb_move_max 
 * @param is_wrap 
 */
static void convert_first_line(uint* w, uint* h, uint* nb_move_max, bool* is_wrap) {

}

/**
 * @brief Creates a game by loading its description in a file
 *  NB : THE FORMAT WILL BE SPECIFIED LATER
 * @param filename
 * @return the loaded game
 **/
game game_load(char *filename) {

  FILE* f = fopen(filename, "r");


  if (f == NULL) {
    fprintf(stderr, "Bad pointer");
    exit(EXIT_FAILURE);
  }

  uint width;
  uint height;
  uint nb_move_max;
  bool is_wrap; 

  convert_first_line(&width, &height, &nb_move_max, &is_wrap);

  color* tab = malloc(sizeof(color)*width*height);

  fclose(f);
}

/**
 * @brief Save a game in a file
 *  NB : THE FORMAT WILL BE SPECIFIED LATER
 * @param g game to save
 * @param filename output file
 **/
void game_save(cgame g, char *filename) {}

