// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h> // required to load transparent texture from PNG
#include <SDL_ttf.h>   // required to use TTF fonts

#include "asde_slist.h"
#include "asde_slist_utilitary_functions.h"
#include "game_io.h"
#include "game_rand.h"
#include "gui.h"
#include "solver.h"
#include "time.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

#define FONT "ressources/squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "ressources/bg.jpg"

/* **************************************************************** */

enum status { PLAYING, WIN, RESTART };

struct Text {
  TTF_Font *font;
  SDL_Color color;
  uint x;
  uint y;
  uint w;
  uint h;
};

struct Gui_color {
  SDL_Texture *bg;
  SDL_Color grid_line;
  SDL_Color *cells;
};

struct Env_t {
  game game;
  status game_state;
  uint win_width;
  uint win_height;
  uint grid_width;
  uint grid_height;
  uint cell_len;
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
static Gui_color *init_colors(SDL_Renderer *ren, game g) {

  Gui_color *gui = malloc(sizeof(Gui_color));
  error(gui == NULL, "Pointer NULLL");

  SDL_Texture *bg = IMG_LoadTexture(ren, BACKGROUND);
  if (!bg)
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  gui->bg = bg;

  gui->grid_line = (SDL_Color){70, 70, 70, 255};

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

  SDL_Color sample = {0, 127, 127, 255};

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

  uint len = env->cell_len;
  x *= len;
  y *= len;

  SDL_Rect cell = {x, y, len, len};
  SDL_Color color = env->colors->cells[c];

  SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(ren, &cell);
}

/**
 * @brief Draw or redraw grid (lines + columns + cells) of the game.
 *
 * @param win pointer to the window context
 * @param ren pointer to the renderer
 * @param env pointer to the environment
 */
static void draw_grid(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  // Cells drawing
  for (uint x = 0; x < env->grid_width; x++) {
    for (uint y = 0; y < env->grid_height; y++) {
      color c = game_cell_current_color(env->game, x, y);
      draw_cell(win, ren, env, x, y, c);
    }
  }

  // Grid lines
  SDL_Color gc = env->colors->grid_line;
  SDL_SetRenderDrawColor(ren, gc.r, gc.g, gc.b, 255);

  uint len = env->cell_len;

  for (uint y = 0; y < 1 + env->grid_height * len; y += len) {
    SDL_RenderDrawLine(ren, 0, y, env->grid_width * len, y);
  }

  // Grid rows
  for (uint x = 0; x < 1 + env->grid_width * len; x += len) {
    SDL_RenderDrawLine(ren, x, 0, x, env->grid_height * len);
  }
}

static Text *init_text(TTF_Font *font, SDL_Color color, uint x, uint y, uint w,
                       uint h) {

  Text *text = (Text *)malloc(sizeof(Text));
  error(text == NULL, "Not enough memory");

  text->font = font;
  text->color = color;
  text->x = x;
  text->y = y;
  text->w = w;
  text->h = h;

  return text;
}

static void draw_text(SDL_Window *win, SDL_Renderer *ren, Env *env, Text *text,
                      const char *msg) {
  SDL_Surface *surf = TTF_RenderText_Blended(text->font, msg, text->color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);

  SDL_Rect msg_rect;
  msg_rect.w = text->w;
  msg_rect.h = text->h;
  msg_rect.x = text->x;
  msg_rect.y = text->y;

  SDL_RenderCopy(ren, texture, NULL, &msg_rect);

  SDL_FreeSurface(surf);
}

static void free_text(Text *text) {
  TTF_CloseFont(text->font);
  free(text);
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {

  game g = NULL;

  // Handle executable arguments
  error(
      argc == 3 || argc > 6,
      "Usage: ./recolor_sdl [<w> <h> <nb_mov_max> [<nb_max_color>] [<S|N>]]\n");

  if (argc == 1) {
    g = game_load("data/default_game.rec");
  } else if (argc == 2) {
    g = game_load(argv[1]);
  } else {

    uint w = atoi(argv[1]);
    uint h = atoi(argv[2]);
    uint mov_max = atoi(argv[3]);
    uint nb_max_color = 4; // default
    uint is_wrap = false;  // default

    if (argc > 4)
      nb_max_color = atoi(argv[4]) > 16 ? 16 : atoi(argv[4]);
    if (argc == 6 && argv[5][0] == 'S')
      is_wrap = true;

    g = game_random_ext(w, h, is_wrap, nb_max_color, mov_max);
  }

  Env *env = malloc(sizeof(struct Env_t));
  error(env == NULL, "Pointer NULL");

  PRINT("Press ESC to quit.\nGood luck ! \n");

  // Get screen size info (width & height)
  SDL_MaximizeWindow(win);
  SDL_SetWindowPosition(win, 0, 0);

  // Get screen size info (width & height)
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // Env initialisation
  env->win_width = w;
  env->win_height = h;

  env->game = g;
  env->game_state = PLAYING;

  env->grid_width = game_width(g);
  env->grid_height = game_height(g);

  // Cell length depending some environment conditions
  if (env->grid_width > env->grid_height) {
    env->cell_len = env->win_width / env->grid_width;

    if (env->cell_len * env->grid_height > env->win_height)
      env->cell_len = env->win_height / env->grid_width;

  } else if (env->grid_width < env->grid_height) {
    env->cell_len = env->win_height / (env->grid_height);

    if (env->cell_len * env->grid_width > env->win_width)
      env->cell_len = env->win_width / env->grid_height;

  } else if (env->win_width < env->win_height) {
    env->cell_len = env->win_width / env->grid_width;

  } else {
    env->cell_len = env->win_height / (env->grid_height);
  }

  env->colors = (Gui_color *)init_colors(ren, g);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  status state = env->game_state;

  if (state == PLAYING) {
    SDL_RenderCopy(ren, env->colors->bg, NULL, NULL);

    draw_grid(win, ren, env);

  } else {
    SDL_RenderCopy(ren, env->colors->bg, NULL, NULL);

    SDL_Color c1 = {237, 237, 237, 255};
    SDL_Color c2 = {179, 179, 179, 255};

    // TODO: Responsive fonts
    TTF_Font *f1 = TTF_OpenFont(FONT, FONTSIZE);
    TTF_Font *f2 = TTF_OpenFont(FONT, FONTSIZE / 2);

    if (!f1 || !f2)
      ERROR("TTF_OpenFont: %s\n", FONT);

    // End game message (pos: window center)
    uint w = 600;
    uint h = 100;
    uint x = (env->win_width - w) / 2;
    uint y = (env->win_height - h) / 2;
    Text *txt1 = init_text(f1, c1, x, y, w, h);

    if (env->game_state == RESTART) {
      draw_text(win, ren, env, txt1, "Dommage perdu pour cette fois !");
    } else {
      draw_text(win, ren, env, txt1, "Bien joué tu as gagné");
    }

    free_text(txt1);

    Text *txt2 = init_text(f2, c2, x + 150, y + 100, w / 2, h / 2);

    draw_text(win, ren, env, txt2, "Appuie sur R pour rejouer");
    free_text(txt2);
  }
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
    case SDLK_q:
      return true;
      break;
    case SDLK_r:
      if (env->game_state == RESTART) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }
      break;
    }
  }

  if (e->type == SDL_MOUSEBUTTONUP && env->game_state == PLAYING) {
    uint x = e->button.x / (env->cell_len);
    uint y = e->button.y / (env->cell_len);

    if (x >= 0 && x < env->grid_width && y >= 0 && y < env->grid_height) {

      color c_cur = game_cell_current_color(env->game, x, y);

      if (game_nb_moves_cur(env->game) < game_nb_moves_max(env->game)) {
        game_play_one_move(env->game, c_cur);
      } else if (game_is_over(env->game)) {
        PRINT("GAGNÉ !\n");
        env->game_state = WIN;
      } else {
        PRINT("PERDU...\n");
        env->game_state = RESTART;
      }
    }
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  game_delete(env->game);
  free(env->colors->cells);
  free(env->colors);
  free(env);
}

/* **************************************************************** */