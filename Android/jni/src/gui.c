#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

#define FONT "squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "background.png"

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
  SDL_Texture *bg; // Background
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
  uint cell_width;
  uint cell_height;
  uint grid_start_x;
  uint grid_start_y;
  Gui_color *colors;
  SDL_Texture *text;
};

/* ********************* Usage infos ************************* */
static void usage(bool cond, char *err_mess) {
  if (cond) {
    fprintf(stderr, "Usage: %s \n\n", err_mess);
    exit(EXIT_FAILURE);
  }
}

/* ********************* Static functions ************************* */

/**
 * @brief This function initialize and allocate Gui_color struct. At first, it
 * puts all GUI colors in and then pick some colors from an array for game
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

  SDL_Color col_arr[16] = {
      {255, 0, 0, 255} /*RED*/,           {0, 255, 0, 255} /*LIME*/,
      {0, 0, 255, 255} /*BLUE*/,          {255, 255, 0, 255} /*YELLOW*/,
      {0, 255, 255, 255} /*CYAN*/,        {255, 0, 255, 255} /*MAGENTA*/,
      {255, 140, 0, 255} /*DARK_ORANGE*/, {192, 192, 192, 255} /*SILVER*/,
      {0, 139, 139, 255} /*DARK_CYAN*/,   {128, 128, 0, 255} /*OLIVE*/,
      {0, 128, 0, 255} /*GREEN*/,         {128, 0, 128, 255} /*PURPLE*/,
      {105, 105, 105, 255} /*DIM_GREY*/,  {255, 218, 185, 255} /*PEACH_PUFF*/,
      {160, 82, 45, 255} /*SIENNA*/,      {255, 215, 0, 255} /*GOLD*/
  };

  for (uint x = 0; x < nb_col; x++)
    cells[x] = col_arr[x];

  gui->cells = cells;

  return gui;
}

/**
 * @brief Update some environment variables (ones likely to be changed) for more
 * flexibility
 *
 * @param win pointer to a windows
 * @param env pointer to a renderer
 */
static void update_env(SDL_Window *win, Env *env) {
  int win_width, win_height;
  SDL_GetWindowSize(win, &win_width, &win_height);

  uint nb_x_cells = game_width(env->game);
  uint nb_y_cells = game_height(env->game);

  env->win_width = win_width;
  env->win_height = win_height;

  env->cell_width = win_width / nb_x_cells;
  env->cell_height =
      (win_height / nb_y_cells) * 0.8; // 0.1 for title and 0.1 for status

  // Adjust width and height to get squares
  if (env->cell_width < env->cell_height)
    env->cell_height = env->cell_width;
  else
    env->cell_width = env->cell_height;

  env->grid_width = nb_x_cells * env->cell_width;
  env->grid_height = nb_y_cells * env->cell_height;

  // Divide free space (win_width - grid_width) by 2 to center the grid
  env->grid_start_x = (env->win_width - env->grid_width) / 2;
  env->grid_start_y = (env->win_height - env->grid_height) / 2;
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
  // Update x and y with real coordinates
  x = (x * env->cell_width) + env->grid_start_x;
  y = (y * env->cell_height) + env->grid_start_y;

  // Create the cell
  SDL_Rect cell = {x, y, env->cell_width, env->cell_height};
  SDL_Color color = env->colors->cells[c];

  // Draw and fill it
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

  update_env(win, env);

  // Cells drawing
  for (uint x = 0; x < game_width(env->game); x++) {
    for (uint y = 0; y < game_height(env->game); y++) {
      color c = game_cell_current_color(env->game, x, y);
      draw_cell(win, ren, env, x, y, c);
    }
  }

  // Grid lines
  SDL_Color gc = env->colors->grid_line;
  SDL_SetRenderDrawColor(ren, gc.r, gc.g, gc.b, 255);

  /** Functional but I prefer without lines
    for (uint y = env->grid_start_y; y < 1 + env->grid_start_y +
  env->grid_height; y += env->cell_height) {
      SDL_RenderDrawLine(ren, env->grid_start_x, y, env->grid_start_x +
  env->grid_width, y);
    }

    // Grid rows
    for (uint x = env->grid_start_x; x < 1 + env->grid_start_x +
  env->grid_width; x += env->cell_width) {
      SDL_RenderDrawLine(ren, x, env->grid_start_y, x, env->grid_start_y +
  env->grid_height);
    }
  **/
}

/**
 * @brief This function allocate and initialize all params required to create a
 * text
 *
 * @param font pointer to a TTF_Font initialized
 * @param color of the text
 * @param x horizontal position in the window
 * @param y vertical position in the window
 * @param w fixed width
 * @param h fixed height
 * @return Text*
 */
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

/**
 * @brief Draws the text in a renderer with a Text already allocate and
 * initialize
 *
 * @param win pointer to the window context
 * @param ren pointer to the renderer context
 * @param env pointer to the environment
 * @param text pointer to Text structure initialized with init_text()
 * @param msg a const char with the content of the text to draw.
 */
static void draw_text(SDL_Renderer *ren, Env *env, Text *text,
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

/**
 * @brief Draws status game (restart | nb_cur / nb_moves_max | quit) at the
 * bottom of the sreen
 *
 * @param ren pointer to renderer
 * @param env pointer to environment
 * @param f pointer to font used
 * @param c pointer to color used
 */
static void draw_status(SDL_Renderer *ren, Env *env, TTF_Font *f, SDL_Color c) {
  // Convert (nb_cur / nb_moves_max) to string. NB : maybe optimize it later
  char nb_cur_s[5];
  SDL_itoa(game_nb_moves_cur(env->game), nb_cur_s, 10);
  char nb_max_s[5];
  SDL_itoa(game_nb_moves_max(env->game), nb_max_s, 10);
  char status[15];
  strcpy(status, nb_cur_s);
  strcat(status, "/");
  strcat(status, nb_max_s);

  // Set position and size of : nb_cur / nb_moves_max
  uint w = env->grid_width / 4;
  uint h = (env->win_height - env->grid_height) / 4;
  uint x = env->grid_start_x + env->grid_width / 4 + w / 2;
  uint y = env->grid_start_y + env->grid_height + h / 2;
  Text *nb_cur_max = init_text(f, c, x, y, w, h);
  draw_text(ren, env, nb_cur_max, status);
  free(nb_cur_max);

  // Set position and size of : Restart "button" (same h than 'status')
  w = env->grid_width / 8;
  x = env->grid_start_x + w / 2;
  y = env->grid_start_y + env->grid_height + h / 2;
  Text *r = init_text(f, c, x, y, w, h);
  draw_text(ren, env, r, "Restart");
  free(r);

  // Set position and size of : Quit "button" (same w, h and y than 'Restart')
  x = env->grid_start_x + 3 * env->grid_width / 4 + w / 2;
  Text *q = init_text(f, c, x, y, w, h);
  draw_text(ren, env, r, "Quit");
  free(q);

  // Draw lines to seperate them (2 lasts ones are optionals)
  SDL_RenderDrawLine(ren, env->grid_start_x + env->grid_width / 4,
                     env->grid_start_y + env->grid_height,
                     env->grid_start_x + env->grid_width / 4, env->win_height);
  SDL_RenderDrawLine(ren, env->grid_start_x + (3 * env->grid_width / 4),
                     env->grid_start_y + env->grid_height,
                     env->grid_start_x + (3 * env->grid_width / 4),
                     env->win_height);
  // SDL_RenderDrawLine(ren, env->grid_start_x, env->grid_start_y +
  // env->grid_height, env->grid_start_x, env->win_height);
  // SDL_RenderDrawLine(ren, env->grid_start_x + env->grid_width,
  // env->grid_start_y + env->grid_height, env->grid_start_x + env->grid_width,
  // env->win_height);
}

/**
 * @brief Draws game's name
 *
 * @param ren pointer to renderer
 * @param env pointer to environment
 * @param f pointer to font used
 * @param c pointer to color used
 */
static void draw_title(SDL_Renderer *ren, Env *env, TTF_Font *f, SDL_Color c) {
  uint w = env->grid_width / 2;
  uint h = (env->win_height - env->grid_height) / 2;
  uint x = env->grid_start_x + (w / 2);
  uint y = 0;
  Text *text = init_text(f, c, x, y, w, h);
  draw_text(ren, env, text, "Flood It");
  free(text);
}

/**
 * @brief Checks if a click is on the grid or not.
 *
 * @param e pointer to an SDL_Event
 * @param env pointer to an environment
 * @return true if click is on the grid
 * @return false otherwise
 */
static bool click_on_grid(SDL_Event *e, Env *env) {
#ifdef __ANDROID__
  float button_y = e->tfinger.y * env->win_height;
  float button_x = e->tfinger.x * env->win_width;
  return (button_y >= env->grid_start_y &&
          button_y <= env->grid_start_y + env->grid_height &&
          button_x <= env->grid_width + env->grid_start_x &&
          button_x >= env->grid_start_x);
#endif
  return (e->button.y >= env->grid_start_y &&
          e->button.y <= env->grid_start_y + env->grid_height &&
          e->button.x >= env->grid_start_x &&
          e->button.x <= env->grid_width + env->grid_start_x);
}

/**
 * @brief Check if a click is under the grid (in the 'restart | status | quit'
 * area)
 *
 * @param e pointer to an SDL_Event
 * @param env pointer to an environment
 * @return true if click is under the grid
 * @return false otherwise
 */
static bool click_under_grid(SDL_Event *e, Env *env) {
#ifdef __ANDROID__
  float button_y = e->tfinger.y * env->win_height;
  return (button_y > env->grid_start_y + env->grid_height);
#endif
  return (e->button.y > env->grid_start_y + env->grid_height);
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  // Init game
  game g;

#ifdef __ANDROID__
  srand(time(NULL));
  g = game_random_ext(12, 12, true, rand() % 16, 50);
#else
  // Handle executable arguments
  if (argc == 1) {
    g = game_load("data/default_game.rec");
  } else if (argc == 2) {
    g = game_load(argv[1]);
  } else {
    usage(argc < 4 || argc > 6,
          "./recolor_sdl <w*> <h*> <nb_mov_max*> <nb_max_color> <S|N>");

    uint w = atoi(argv[1]);
    uint h = atoi(argv[2]);
    uint mov_max = atoi(argv[3]);
    uint max_col = (argc >= 5) ? atoi(argv[4]) : 4;
    bool is_wrap = (argc == 6) ? (argv[5][0] == 'S') : false;

    g = game_random_ext(w, h, is_wrap, max_col, mov_max);
  }
#endif
  // Init environment
  Env *env = malloc(sizeof(struct Env_t));
  error(env == NULL, "Pointer NULL");

  PRINT("Press ESC to quit.\nGood luck ! \n");

  // Get screen size info (width & height)
  int win_width, win_height;
  SDL_GetWindowSize(win, &win_width, &win_height);

  SDL_MaximizeWindow(win);
  SDL_SetWindowPosition(win, 0, 0);
  /**
  uint nb_x_cells = game_width(g);
  uint nb_y_cells = game_height(g);

  env->cell_width = win_width / nb_x_cells;
  env->cell_height = (win_height / nb_y_cells) * 0.8;

  env->grid_width = nb_x_cells * env->cell_width;
  env->grid_height = nb_y_cells * env->cell_height;
  **/
  env->game = g;
  env->game_state = PLAYING;
  env->colors = (Gui_color *)init_colors(ren, g);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  update_env(win, env);

  SDL_Color c = {105, 105, 105, 255};

  TTF_Font *f = TTF_OpenFont(FONT, FONTSIZE);
  if (!f)
    ERROR("TTF_OpenFont: %s\n", FONT);

  // Display game's name, grid and status
  SDL_RenderCopy(ren, env->colors->bg, NULL, NULL);
  draw_title(ren, env, f, c);
  draw_grid(win, ren, env);
  draw_status(ren, env, f, c);

  status state = env->game_state;

  if (state == WIN) {
    // End game message (pos: grid center)
    uint w = env->grid_width / 2;
    uint h = 100;
    uint x = env->grid_start_x + env->grid_width / 4;
    uint y = (env->win_height - h) / 2;
    Text *txt1 = init_text(f, c, x, y, w, h);
    draw_text(ren, env, txt1, "BRAVO");
    free(txt1);
  }

  TTF_CloseFont(f);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {

  update_env(win, env);

  if (e->type == SDL_QUIT)
    return true;

#ifdef __ANDROID__
  // Tapped on screen
  else if (e->type == SDL_FINGERDOWN && env->game_state == PLAYING) {
    if (click_on_grid(e, env)) {
      // Convert tapped area to a valid x and y for game_cell_current_color()
      uint x =
          (e->tfinger.x * env->win_width - env->grid_start_x) / env->cell_width;
      uint y = (e->tfinger.y * env->win_height - env->grid_start_y) /
               env->cell_height;

      color c_cur = game_cell_current_color(env->game, x, y);

      game_play_one_move(env->game, c_cur);

      if (game_is_over(env->game))
        env->game_state = WIN;
    } else if (click_under_grid(e, env)) {
      // Tapped on 'Restart'
      if (e->tfinger.x * env->win_width > env->grid_start_x &&
          e->tfinger.x * env->win_width <
              env->grid_start_x + env->grid_width / 4) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }
      // Tapped on 'Quit'
      else
        return (e->tfinger.x * env->win_width >
                    env->grid_start_x + (3 * env->grid_width / 4) &&
                e->tfinger.x * env->win_width <
                    env->grid_start_x + env->grid_width);
    }
  }
#else

  else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
    case SDLK_ESCAPE:
      PRINT("DOMMAGE\n");
      return true;
      break;
    case SDLK_q:
      PRINT("DOMMAGE\n");
      return true;
      break;
    case SDLK_r:
      game_restart(env->game);
      env->game_state = PLAYING;
      break;
    }
  }

  else if (e->type == SDL_MOUSEBUTTONUP) {
    if (click_on_grid(e, env) && env->game_state == PLAYING) {
      uint x = (e->button.x - env->grid_start_x) / (env->cell_width);
      uint y = (e->button.y - env->grid_start_y) / (env->cell_height);

      color c_cur = game_cell_current_color(env->game, x, y);

      game_play_one_move(env->game, c_cur);

      if (game_is_over(env->game)) {
        PRINT("BRAVO\n");
        env->game_state = WIN;
      }
    } else if (click_under_grid(e, env)) {
      // Clicked on 'Restart'
      if (e->button.x > env->grid_start_x &&
          e->button.x < env->grid_start_x + env->grid_width / 4) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }
      // Clicked on 'QUIT'
      else if (e->button.x > env->grid_start_x + (3 * env->grid_width / 4) &&
               e->button.x < env->grid_start_x + env->grid_width)
        return true;
    }
  }
#endif

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  game_delete(env->game);
  free(env->colors->cells);
  free(env->colors);
  free(env);
}