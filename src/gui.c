// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h> // required to load transparent texture from PNG
#include <SDL_ttf.h>   // required to use TTF fonts

#include "asde_slist.h"
#include "asde_slist_utilitary_functions.h"
#include "game_io.h"
#include "gui.h"
#include "solver.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

typedef struct Gui_color {
  SDL_Color bg;
  SDL_Color grid_bg;
  SDL_Color grid_line;
  SDL_Color *cells;
} Gui_color;

struct Env_t {
  game game;
  uint win_width;
  uint win_height;
  uint grid_width;
  uint grid_height;
  uint cell_len;
  double cell_ratio;
  Gui_color *colors;
};

/* ********************* Static functions ************************* */

/**
 * @brief This function initialize and allocate Gui_color struct. At first, it
 * puts all GUI colors in and then it creates a random tab of colors for game
 * cells.
 *
 * @param g a game instance
 * @return Gui_color* pointer
 */
static Gui_color *init_colors(game g) {

  Gui_color *gui = malloc(sizeof(Gui_color));
  error(gui == NULL, "Pointer NULLL");

  gui->bg = (SDL_Color){};
  gui->grid_bg = (SDL_Color){};
  gui->grid_line = (SDL_Color){};

  uint nb_col = 0;
  SList colors = asde_slist_create_empty();

  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (!asde_slist_belongs_to(colors, game_cell_current_color(g, x, y))) {
        colors = asde_slist_prepend(colors, game_cell_current_color(g, x, y));
        nb_col += 1;
      }
    }
  }

  asde_slist_delete_list(colors);

  SDL_Color *cells = malloc(sizeof(SDL_Color) * nb_col);
  error(cells == NULL, "Pointer NULL");

  SDL_Color sample = {128, 128, 128, 255};

  for (uint x = 0; x < nb_col; x++) {
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;

    SDL_Color obj;

    obj.r = (r + sample.a) / 2;
    obj.g = (g + sample.g) / 2;
    obj.b = (b + sample.b) / 2;
    obj.a = 255;

    cells[x] = obj;
  }

  gui->cells = cells;

  return gui;
}

/**
 * @brief This function draw a cell in the grid according to (x,y) coordonates
 * and a color
 *
 * @param win pointer to the window
 * @param ren pointer to a renderer
 * @param env pointer to environment content
 * @param x int of x-axis on the grid
 * @param y int of y-axis on the grid
 * @param c color of the cell
 */
static void draw_cell(SDL_Window *win, SDL_Renderer *ren, Env *env, uint x,
                      uint y, color c) {

  uint len = env->cell_len * env->cell_ratio;
  SDL_Rect cell = {x, y, len, len};

  SDL_SetRenderDrawColor(ren, env->colors->cells[c].r, env->colors->cells[c].g,
                         env->colors->cells[c].b, env->colors->cells[c].a);
  SDL_RenderFillRect(ren, &cell);
}

static void draw_grid(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  if (env->win_width > env->win_height) {

    // Cells drawing
    for (uint x = 0; x < env->grid_width; x++) {
      for (uint y = 0; y < env->grid_height; y++) {
        color c = game_cell_current_color(env->game, x, y);
        draw_cell(win, ren, env, x * env->cell_len * env->cell_ratio,
                  y * env->cell_len * env->cell_ratio, c);
      }
    }

    // Grid lines
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    for (uint y = 0; y < 1 + env->grid_height * env->cell_len * env->cell_ratio;
         y += env->cell_len * env->cell_ratio) {
      SDL_RenderDrawLine(ren, 0, y,
                         env->grid_width * env->cell_len * env->cell_ratio, y);
    }

    // Grid rows
    for (uint x = 0; x < 1 + env->grid_width * env->cell_len * env->cell_ratio;
         x += env->cell_len * env->cell_ratio) {
      SDL_RenderDrawLine(ren, x, 0, x,
                         env->grid_height * env->cell_len * env->cell_ratio);
    }

  } else {
  }
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  error(argc != 2, "Usage: ./recolor_sdl <name>.rec");

  Env *env = malloc(sizeof(struct Env_t));
  error(env == NULL, "Pointer NULL");

  game g = game_load(argv[1]);

  PRINT("Press ESC to quit.\nGood luck ! \n");

  // Get screen size info (width & height)
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);

  // Set window size to screen size (a "fullscreen")
  SDL_SetWindowSize(win, DM.w, DM.h);
  SDL_SetWindowPosition(win, 0, 0);

  // Env initialisation
  env->win_width = DM.w;
  env->win_height = DM.h;

  env->game = g;

  env->grid_width = game_width(g);
  env->grid_height = game_height(g);

  env->cell_ratio = 0.5;
  env->cell_len = (env->win_width > env->win_height)
                      ? (env->win_height / env->grid_height)
                      : (env->win_width / env->grid_width);

  env->colors = (Gui_color *)init_colors(g);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  draw_grid(win, ren, env);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {

  if (e->type == SDL_QUIT)
    return true;

  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
    case SDLK_ESCAPE:
      return true;
      break;
    }
  }

  if (e->type == SDL_MOUSEBUTTONUP) {
    uint x = e->button.x / (env->cell_len * env->cell_ratio);
    uint y = e->button.y / (env->cell_len * env->cell_ratio);

    if (x >= 0 && x < env->grid_width && y >= 0 && y < env->grid_height) {

      color c_cur = game_cell_current_color(env->game, x, y);

      if (game_nb_moves_cur(env->game) < game_nb_moves_max(env->game)) {
        game_play_one_move(env->game, c_cur);
        PRINT("CONTINUE ! Current coups : %d \n", game_nb_moves_cur(env->game));
      } else if (game_is_over(env->game)) {
        PRINT("WINNNNNNN !\n");
      } else {
        PRINT("C'EST REPARTI POUR UNE NOUVELLE PARTIE.... RECOMMENCE....\n");
        game_restart(env->game);
      }
    }

    // PRINT("Coordonnées de la grille de ce clic : (%d,%d)\n", x, y);
    // PRINT("Coordonnées du clic : (%d,%d) \n", e->button.x, e->button.y);
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) { free(env); }

/* **************************************************************** */