#include "game_io.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINELEN 4096

/* ************* Error handler *********** */
static void error(bool cond, char *err_mess) {
  if (cond) {
    fprintf(stderr, "Error: %s \n\n", err_mess);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Read one line
 *
 * @param f pointer to a FILE
 * @param size
 * @return char*
 */
static char *read_one_line(FILE *f, long *size) {
  char *s = malloc(MAXLINELEN * sizeof(char));
  error(s == NULL, "Memory allocation error");

  long old_pos = ftell(f);
  if (fgets(s, MAXLINELEN, f) != NULL) {
    long len = 0
    len = ftell(f) - old_pos;
    if (s[len - 1] == '\n') {
      s[len - 1] = '\0';
      len--;
    } else {
      if (!feof(f)) {
        fprintf(stderr, "Line too long...");
        exit(EXIT_FAILURE);
      }
    }
    *size = len;
    return s;
  }
  free(s);
  return NULL;
}

/**
 * @brief extract data from a given line
 *
 * @param line
 * @param size
 * @return color* format color* pck je m'en sers que pour le color tab mais à
 * voir dans le futur
 */
static color *convert_line(char *line, long *size) {
  color *tab = malloc((*size) * sizeof(color));
  error(tab == NULL, "Pointer exception");

  long tab_s = 0;
  char *token = strtok(line, " ");
  while (token != NULL) {
    char *endval = token;
    long val = strtol(token, &endval, 10);
    if ((*endval) == '\0') {
      color cval = (color)val;
      tab[tab_s] = cval;
      tab_s++;
    } else {
      free(tab);
      return NULL;
    }
    token = strtok(NULL, " ");
  }
  return tab;
}

/**
 * @brief extract the color tab from the file
 *
 * @param fname filename
 * @param width of the grid game
 * @param height of the grid game
 * @param psize
 * @return color* array of colors for the game to load
 */
static color *read_tab(char *fname, int width, int height, long *psize) {
  FILE *f = fopen(fname, "r");
  error(f == NULL, "Problem opening file");

  color *tab = (color *)malloc(width * height * sizeof(color));
  error(tab == NULL, "Pointer exception");

  // jump the first line
  char *line = read_one_line(f, psize);
  free(line);

  for (int h = 0; h < height; h++) {
    line = read_one_line(f, psize);
    color *arr = convert_line(line, psize);
    for (int i = 0; i < width; i++) {
      tab[i + h * width] = arr[i];
    }
    free(arr);
    free(line);
  }
  fclose(f);
  return tab;
}

/**
 * @brief extract data from the first line of the file
 *
 * @param fname filename
 * @param psize
 * @return int* int* array with the basic info for the game
 *          (width, height, max_moves, swap)
 */
static int *read_infos(char *fname, long *psize) {
  FILE *f = fopen(fname, "r");
  error(f == NULL, "Problem opening file");

  char *line = read_one_line(f, psize);

  // j'avais un pb de segfault quand j'utilisais convert_line
  // là mais je sais pas pourquoi... (ca cassait sur le dernier caractère)
  // en attendant de trouver j'en ai fait une version qui bloque pas
  // mais c'est pas très flexible du coup

  int *tab = (int *)malloc(4 * sizeof(int)); // pr stocker les infos
  error(tab == NULL, "Pointer exception");

  char *token = strtok(line, " ");
  for (int i = 0; i < 3; i++) {
    char *endval = token;
    long val = strtol(token, &endval, 10);
    if ((*endval) == '\0') {
      int ival = (int)val;
      tab[i] = ival;
    } else {
      free(tab);
      free(line);
      fclose(f);
      return NULL;
    }
    token = strtok(NULL, " ");
  }

  int swap = (*token == 'N') ? 0 : 1;
  tab[3] = (int)swap;

  free(line);
  fclose(f);
  return tab;
}

/* *********** GAME_LOAD *********** */

/**
 * @brief Creates a game by loading its description in a file
 *  NB : THE FORMAT WILL BE SPECIFIED LATER
 * @param filename
 * @return the loaded game
 **/
game game_load(char *filename) {

  FILE *f = fopen(filename, "r");
  error(f == NULL, "Bad pointer");
  fclose(f);

  /* lire 1ere ligne puis utiliser taille
      pr allouer mem pr un tab
    lire le reste et tt stocker ds said tab
  */

  long size = 0; // pr readnextline

  // 1st line
  int *infos = read_infos(filename, &size);
  int width = infos[0];
  int height = infos[1];
  int nb_mm = infos[2];
  bool swap = (infos[3] != 0);

  // color tab
  color *game_tab = read_tab(filename, width, height, &size);

  game g = game_new_ext(width, height, game_tab, nb_mm, swap);

  free(infos);
  free(game_tab);
  return g;
}

/* *********** GAME_SAVE *********** */

/**
 * @brief Save a game in a file
 * @param g game to save
 * @param filename output file
 **/
void game_save(cgame g, char *filename) {
  error(g == NULL || filename == NULL, "Bad pointer\n");

  FILE *f = fopen(filename, "w");
  error(f == NULL, "Allocation pb\n");

  char wrap = game_is_wrapping(g) ? 'S' : 'N';

  fprintf(f, "%u %u %u %c\n", game_width(g), game_height(g),
          game_nb_moves_max(g), wrap);

  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (x == 0)
        fprintf(f, "%u", game_cell_current_color(g, x, y));
      else
        fprintf(f, " %u", game_cell_current_color(g, x, y));
    }
    fprintf(f, "\n");
  }

  fclose(f);
}
