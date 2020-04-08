#include "gui.h"
#include "game_io.h"
#include "game_rand.h"
#include "solver.h"
#include "time.h"
#include "toolbox.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

#ifdef __ANDROID__
#define FONT "squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "background.png"
#else
#define FONT "ressources/squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "ressources/background.jpg"
#endif

/* **************************************************************** */

enum status { MENU, PLAYING, WIN, LOSE };
enum texture {
  BACKGROUND_IMG,
  TITLE,
  RESTART,
  QUIT,
  WON,
  LOST,
  MENU_TEXT,
  SOLVE,
  DEF_GAME,
  GAME_2N,
  GAME_2S,
  RAND_GAME,
  NB_TEXTURES
};
enum env_width_height { WINDOW, GRID, CELL, NB_RESIZABLE_ELEMENTS };

struct Gui_color {
  SDL_Color grid_line; // Grid's lines color (not used in this version)
  SDL_Color *cells;    // Grid's cells colors
};

struct Env_t {
  game game;
  status game_state;      // PLAYING || WIN
  int *width;             // Array of all env. width (window, grid, cell)
  int *height;            // Array of all env. height (window, grid, cell)
  int grid_start_x;       // Grid's starting point (x-axis)
  int grid_start_y;       // Grid's starting point (y-axis)
  Gui_color *colors;      // Look at Gui_color struct above
  SDL_Texture **textures; // Array of all constant textures
};

/* **** Static functions (decription below main functions) **** */

static void usage(bool cond, char *err_mess);
static void update_env(SDL_Window *win, Env *env);
static Gui_color *init_colors(game g);
static void render_const_textures(SDL_Renderer *ren, Env *env);
static void render_menu(SDL_Renderer *ren, Env *env);
static void render_status(SDL_Renderer *ren, Env *env);
static void render_end_game(SDL_Renderer *ren, Env *env, status state);
static bool click_on_grid(SDL_Event *e, Env *env);
static bool click_under_grid(SDL_Event *e, Env *env);
static bool click_over_grid(SDL_Event *e, Env *env);
static void draw_grid(SDL_Window *win, SDL_Renderer *ren, Env *env);
static void draw_cell(SDL_Window *win, SDL_Renderer *ren, Env *env, uint x,
                      uint y, color c);
static bool full_grid(cgame g);
#ifdef __ANDROID__
static void copy_asset(char *src, char *dst);
static game game_load_android(char *FILE);
#endif

/* ********************* Main functions *************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  srand(time(NULL)); // For a different random game each time

  // Init game
  game g;

#ifdef __ANDROID__
  g = game_load_android("default_game.rec");
#else

  // Raise usage if number of arguments are wrong ('3' | '>6')
  usage(argc == 3 || argc > 6,
        "./recolor_sdl <w*> <h*> <nb_mov_max*> <nb_max_color> <S|N>");

  // Handle executable arguments
  if (argc == 1) {
    g = game_load("data/default_game.rec");
  } else if (argc == 2) {
    g = game_load(argv[1]);
  } else {

    uint w = atoi(argv[1]);
    uint h = atoi(argv[2]);
    uint mov_max = atoi(argv[3]);
    uint max_col = (argc >= 5) ? (atoi(argv[4]) < 16 ? atoi(argv[4]) : 16) : 4;
    bool is_wrap = (argc == 6) ? (argv[5][0] == 'S') : false;

    g = game_random_ext(w, h, is_wrap, max_col, mov_max);
  }
#endif

  // Init environment
  Env *env = malloc(sizeof(struct Env_t));
  error(env == NULL, "Pointer NULL");

  env->game = g;
  env->game_state = PLAYING;

  PRINT("Press 'ESC' or 'q' to quit.\nPress 'r' to restart\nGood luck !\n");

  SDL_MaximizeWindow(win);
  SDL_SetWindowPosition(win, 0, 0);

  // Allocate memory for all env. width & height (win, grid, cell)
  env->width = malloc(NB_RESIZABLE_ELEMENTS * sizeof(uint));
  env->height = malloc(NB_RESIZABLE_ELEMENTS * sizeof(uint));

  // Get screen size info (width & height) and update the environment
  update_env(win, env);

  // Init grid colors
  env->colors = init_colors(g);

  // Allocate memory for textures array
  env->textures = malloc(NB_TEXTURES * sizeof(SDL_Texture *));

  // Init background
  env->textures[BACKGROUND_IMG] = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->textures[BACKGROUND_IMG])
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  // Init text texture using squirk font
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font)
    ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  SDL_Surface *title = TTF_RenderText_Blended(font, "Flood It", SILVER);
  env->textures[TITLE] = SDL_CreateTextureFromSurface(ren, title);

  SDL_Surface *restart = TTF_RenderText_Blended(font, "Restart", SILVER);
  env->textures[RESTART] = SDL_CreateTextureFromSurface(ren, restart);

  SDL_Surface *quit = TTF_RenderText_Blended(font, "Quit", SILVER);
  env->textures[QUIT] = SDL_CreateTextureFromSurface(ren, quit);

  SDL_Surface *won = TTF_RenderText_Blended(font, "YOU WON !", BLACK);
  env->textures[WON] = SDL_CreateTextureFromSurface(ren, won);

  SDL_Surface *lost = TTF_RenderText_Blended(font, "YOU LOST !", BLACK);
  env->textures[LOST] = SDL_CreateTextureFromSurface(ren, lost);

  SDL_Surface *menu = TTF_RenderText_Blended(font, "MENU", SILVER);
  env->textures[MENU_TEXT] = SDL_CreateTextureFromSurface(ren, menu);

  SDL_Surface *solve = TTF_RenderText_Blended(font, "SOLVE", SILVER);
  env->textures[SOLVE] = SDL_CreateTextureFromSurface(ren, solve);

  SDL_Surface *def_game = TTF_RenderText_Blended(font, "DEFAULT GAME", SILVER);
  env->textures[DEF_GAME] = SDL_CreateTextureFromSurface(ren, def_game);

  SDL_Surface *game_2s = TTF_RenderText_Blended(font, "GAME H 2S", SILVER);
  env->textures[GAME_2S] = SDL_CreateTextureFromSurface(ren, game_2s);

  SDL_Surface *game_2n = TTF_RenderText_Blended(font, "GAME H 2N", SILVER);
  env->textures[GAME_2N] = SDL_CreateTextureFromSurface(ren, game_2n);

  SDL_Surface *rand_game = TTF_RenderText_Blended(font, "RANDOM GAME", SILVER);
  env->textures[RAND_GAME] = SDL_CreateTextureFromSurface(ren, rand_game);

  SDL_FreeSurface(title);
  SDL_FreeSurface(restart);
  SDL_FreeSurface(quit);
  SDL_FreeSurface(won);
  SDL_FreeSurface(lost);
  SDL_FreeSurface(menu);
  SDL_FreeSurface(solve);
  SDL_FreeSurface(def_game);
  SDL_FreeSurface(game_2s);
  SDL_FreeSurface(game_2n);
  SDL_FreeSurface(rand_game);
  TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  update_env(win, env);

  status state = env->game_state;

  // Render background texture
  SDL_RenderCopy(ren, env->textures[BACKGROUND_IMG], NULL, NULL);

  // Render title, restart and quit
  render_const_textures(ren, env);

  if (state == MENU) {
    render_menu(ren, env);
  } else {

    // Render grid
    draw_grid(win, ren, env);

    // Render nb_cur / nb_max
    render_status(ren, env);

    // Render end game message if game is won or lost
    if (state == WIN || state == LOSE)
      render_end_game(ren, env, state);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {

  update_env(win, env);

  if (e->type == SDL_QUIT)
    return true;

#ifdef __ANDROID__

  // Tapped on screen
  if (e->type == SDL_FINGERDOWN) {

    // Convert coordinates as tfinger.x/y is normalized in [0..1]
    float button_x = e->tfinger.x * env->width[WINDOW];
    float button_y = e->tfinger.y * env->height[WINDOW];

    if (click_on_grid(e, env) && env->game_state == PLAYING) {

      // Convert tapped area to a valid x and y for game_cell_current_color()
      uint x = (button_x - env->grid_start_x) / env->width[CELL];
      uint y = (button_y - env->grid_start_y) / env->height[CELL];

      // Check if x or y is really valid (if not just do as nothing happened)
      // Can happen due to conversion float->uint & click is just on the edge
      if (x >= game_width(env->game) || y >= game_height(env->game))
        return false;

      color c_cur = game_cell_current_color(env->game, x, y);

      // For more fun just play color != than current cell (0,0)
      if (c_cur != game_cell_current_color(env->game, 0, 0))
        game_play_one_move(env->game, c_cur);

      if (game_is_over(env->game))
        env->game_state = WIN;

      else if (full_grid(env->game))
        env->game_state = LOSE;

    }

    else if (click_on_grid(e, env) && env->game_state == MENU) {

      game g;

      if (button_y < env->grid_start_y + env->height[GRID] / 2) {

        // Tapped on 'Default game'
        if (button_x < env->grid_start_x + env->width[GRID] / 2)
          g = game_load_android("default_game.rec");

        // Tapped on 'Random game'
        else {
          uint w = 6 + (uint)rand() % 15;
          uint h = 6 + (uint)rand() % 15;
          bool wrap = (uint)rand() % 2;
          uint max_color = 4 + (uint)rand() % 13;
          uint max_moves = (max_color > 10) ? w + h + max_color : w + h;
          g = game_random_ext(w, h, wrap, max_color, max_moves);
        }
      }

      else {

        // Tapped on 'Game_2S'
        if (button_x < env->grid_start_x + env->width[GRID] / 2)
          g = game_load_android("horizontal_game2S.rec");

        // Tapped on 'Game_2N'
        else
          g = game_load_android("horizontal_game2N.rec");
      }

      free(env->colors->cells);
      free(env->colors);
      game_delete(env->game);

      env->game = g;
      env->colors = init_colors(env->game);

      env->game_state = PLAYING;
    }

    else if (click_over_grid(e, env)) {

      // Tapped on 'Menu'
      if (button_x > env->grid_start_x &&
          button_x < env->grid_start_x + env->width[GRID] / 4) {
        env->game_state = (env->game_state == MENU) ? PLAYING : MENU;
      }
    }

    else if (click_under_grid(e, env)) {

      // Tapped on 'Restart'
      if (button_x > env->grid_start_x &&
          button_x < env->grid_start_x + env->width[GRID] / 4) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }

      // Tapped on 'Quit'
      else
        return (button_x > env->grid_start_x + 3 * env->width[GRID] / 4 &&
                button_x < env->grid_start_x + env->width[GRID]);
    }
  }
#else

  else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
    case SDLK_ESCAPE:
      PRINT("SEE YOU!\n");
      return true;
      break;
    case SDLK_q:
      PRINT("SEE YOU\n");
      return true;
      break;
    case SDLK_r:
      game_restart(env->game);
      env->game_state = PLAYING;
      break;
    }
  }

  // Clicked on screen
  else if (e->type == SDL_MOUSEBUTTONUP) {

    if (click_on_grid(e, env) && env->game_state == PLAYING) {

      // Convert clicked area to a valid x and y for game_cell_current_color()
      uint x = (e->button.x - env->grid_start_x) / (env->width[CELL]);
      uint y = (e->button.y - env->grid_start_y) / (env->height[CELL]);

      // Check if x or y is really valid (if not just do as nothing happened)
      // Can happen due to conversion float->uint & click is just on the edge
      if (x >= game_width(env->game) || y >= game_height(env->game))
        return false;

      color c_cur = game_cell_current_color(env->game, x, y);

      // For more fun just play color != than current cell (0,0)
      if (c_cur != game_cell_current_color(env->game, 0, 0))
        game_play_one_move(env->game, c_cur);

      if (game_is_over(env->game)) {
        PRINT("CONGRATS !\n");
        env->game_state = WIN;
      } else if (full_grid(env->game)) {
        PRINT("TOO BAD !\n");
        env->game_state = LOSE;
      }

    }

    else if (click_on_grid(e, env) && env->game_state == MENU) {

      game g;

      if (e->button.y < env->grid_start_y + env->height[GRID] / 2) {

        // Clicked on 'Default game'
        if (e->button.x < env->grid_start_x + env->width[GRID] / 2)
          g = game_load("data/default_game.rec");

        // Clicked on 'Random game'
        else {
          uint w = 6 + (uint)rand() % 15;
          uint h = 6 + (uint)rand() % 15;
          bool wrap = (uint)rand() % 2;
          uint max_color = 4 + (uint)rand() % 13;
          uint max_moves = (max_color > 10) ? w + h + max_color : w + h;
          g = game_random_ext(w, h, wrap, max_color, max_moves);
        }
      }

      else {

        // Clicked on 'Game_2S'
        if (e->button.x < env->grid_start_x + env->width[GRID] / 2)
          g = game_load("data/horizontal_game2S.rec");

        // Clicked on 'Game_2N'
        else
          g = game_load("data/horizontal_game2N.rec");
      }

      free(env->colors->cells);
      free(env->colors);
      game_delete(env->game);

      env->game = g;
      env->colors = init_colors(env->game);

      env->game_state = PLAYING;
    }

    else if (click_over_grid(e, env)) {

      // Clicked on 'Menu'
      if (e->button.x > env->grid_start_x &&
          e->button.x < env->grid_start_x + env->width[GRID] / 4) {
        env->game_state = (env->game_state == MENU) ? PLAYING : MENU;
      }

    }

    else if (click_under_grid(e, env)) {

      // Clicked on 'Restart'
      if (e->button.x > env->grid_start_x &&
          e->button.x < env->grid_start_x + env->width[GRID] / 4) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }

      // Clicked on 'QUIT'
      else if (e->button.x > env->grid_start_x + 3 * env->width[GRID] / 4 &&
               e->button.x < env->grid_start_x + env->width[GRID]) {
        PRINT("SEE YOU\n");
        return true;
      }
    }
  }
#endif

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  game_delete(env->game);

  for (uint t = 0; t < NB_TEXTURES; t++)
    SDL_DestroyTexture(env->textures[t]);

  free(env->textures);
  free(env->width);
  free(env->height);
  free(env->colors->cells);
  free(env->colors);
  free(env);
}

/* ************* Static functions description & code *************** */

// Basic usage function
static void usage(bool cond, char *err_mess) {
  if (cond) {
    fprintf(stderr, "Usage: %s \n\n", err_mess);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Update some environment variables according to window's size for
 * responsive purpose.
 *
 * @param win pointer to a window
 * @param env pointer to a renderer
 */
static void update_env(SDL_Window *win, Env *env) {
  int win_width, win_height;
  SDL_GetWindowSize(win, &win_width, &win_height);

  uint nb_x_cells = game_width(env->game);
  uint nb_y_cells = game_height(env->game);

  env->width[WINDOW] = win_width;
  env->height[WINDOW] = win_height;

  env->width[CELL] = win_width / nb_x_cells;
  env->height[CELL] =
      (win_height / nb_y_cells) * 0.8; // 0.1 for title and 0.1 for status

  // Adjust width and height to get squares
  // Take smaller length (grid may not fit the screen otherwise)
  if (env->width[CELL] < env->height[CELL])
    env->height[CELL] = env->width[CELL];
  else
    env->width[CELL] = env->height[CELL];

  env->width[GRID] = nb_x_cells * env->width[CELL];
  env->height[GRID] = nb_y_cells * env->height[CELL];

  // Divide free space (win_width - grid_width) by 2 to center the grid
  env->grid_start_x = (env->width[WINDOW] - env->width[GRID]) / 2;
  env->grid_start_y = (env->height[WINDOW] - env->height[GRID]) / 2;
}

/**
 * @brief This function initialize and allocate Gui_color struct. At first, it
 * puts all GUI colors in and then pick some colors from an array for game
 * cells.
 *
 * @param g a game instance
 * @return Gui_color* pointer
 */
static Gui_color *init_colors(game g) {

  Gui_color *gui = malloc(sizeof(Gui_color));
  error(gui == NULL, "Pointer NULLL");

  gui->grid_line = DIM_GREY;

  // Count number of colors in game
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

  // Init colors
  SDL_Color col_arr[] = {RED_SDL,   LIME,       BLUE_SDL,  YELLOW_SDL,
                         CYAN,      MAGENTA,    ORANGE,    SILVER,
                         DARK_CYAN, OLIVE,      GREEN_SDL, PURPLE,
                         DIM_GREY,  PEACH_PUFF, SIENNA,    SALMON};

  for (uint x = 0; x < nb_col; x++)
    cells[x] = col_arr[x];

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
  // Update x and y with real coordinates
  x = (x * env->width[CELL]) + env->grid_start_x;
  y = (y * env->height[CELL]) + env->grid_start_y;

  // Create the cell
  SDL_Rect cell = {x, y, env->width[CELL], env->height[CELL]};

  // Draw and fill it with its color
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
  for (uint x = 0; x < game_width(env->game); x++) {
    for (uint y = 0; y < game_height(env->game); y++) {
      color c = game_cell_current_color(env->game, x, y);
      draw_cell(win, ren, env, x, y, c);
    }
  }

  /** Version without lines
  // Grid lines
  SDL_Color gc = env->colors->grid_line;
  SDL_SetRenderDrawColor(ren, gc.r, gc.g, gc.b, 255);

  for (uint y = env->grid_start_y; y < 1 + env->grid_start_y +
  env->height[GRID]; y += env->height[CELL]) {
    SDL_RenderDrawLine(ren, env->grid_start_x, y,
                       env->grid_start_x + env->width[GRID], y);
  }

  // Grid rows
  for (uint x = env->grid_start_x; x < 1 + env->grid_start_x + env->width[GRID];
       x += env->width[CELL]) {
    SDL_RenderDrawLine(ren, x, env->grid_start_y, x,
                       env->grid_start_y + env->height[GRID]);
  }
  **/
}

/**
 * @brief Renders title, restart, and quit (not mutable textures)
 * Draw 2 lines at the bottomto seperate : Restart | (nb_cur/nb_max) | Quit
 *
 * @param ren pointer to renderer
 * @param env pointer to environment
 */
static void render_const_textures(SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;

  // Set Titles's position and size then renders it
  rect.w = env->width[GRID] / 2;
  rect.h = rect.w / 4;
  if (rect.h > env->grid_start_y)
    rect.h = env->grid_start_y; // grid_start_y == gap between top's edge
                                // window and grid
  rect.x = env->grid_start_x + (env->width[GRID] / 4);
  rect.y = env->grid_start_y - rect.h;
  SDL_QueryTexture(env->textures[TITLE], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[TITLE], NULL, &rect);

  // Save height for drawing separation lines
  int h = rect.h;

  // Menu "button"
  rect.w = rect.w / 4;
  rect.h = rect.w / 2;
  if (rect.h > env->grid_start_y / 2)
    rect.h = env->grid_start_y / 2;
  rect.x = env->grid_start_x + rect.w / 2;
  rect.y = env->grid_start_y - 3 * rect.h / 2;
  SDL_QueryTexture(env->textures[MENU_TEXT], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[MENU_TEXT], NULL, &rect);

  // Solve "button"
  rect.x = env->grid_start_x + 3 * env->width[GRID] / 4 + rect.w / 2;
  rect.y = env->grid_start_y - 3 * rect.h / 2;
  SDL_QueryTexture(env->textures[SOLVE], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[SOLVE], NULL, &rect);

  // Quit "button" (same w, h and y than 'Restart')
  rect.y = env->grid_start_y + env->height[GRID] + rect.h / 2;
  if (rect.y + rect.h >= env->height[WINDOW]) {
    rect.h = env->grid_start_y / 2;
    rect.y = env->grid_start_y + env->height[GRID] + rect.h / 2;
  }
  SDL_QueryTexture(env->textures[QUIT], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[QUIT], NULL, &rect);

  // Restart "button"
  rect.x = env->grid_start_x + rect.w / 2;
  rect.y = env->grid_start_y + env->height[GRID] + rect.h / 2;
  SDL_QueryTexture(env->textures[RESTART], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[RESTART], NULL, &rect);

  // Draw lines to seperate : Restart | nb_cur/nb_max | Quit
  int start_line_x = env->grid_start_x + env->width[GRID] / 4;
  int start_line_y = env->grid_start_y + env->height[GRID];
  int end_line_x = env->grid_start_x + env->width[GRID] / 4;
  int end_line_y = env->grid_start_y + env->height[GRID] + h;
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);

  start_line_x = env->grid_start_x + (3 * env->width[GRID] / 4);
  end_line_x = env->grid_start_x + (3 * env->width[GRID] / 4);
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);
}

/**
 * @brief Renders 'nb_cur/nb_max_moves' under the grid
 * LIME color if game won
 * RED color if game lost (add an '!' at the end too)
 * SILVER color otherwise
 *
 * @param ren a renderer
 * @param env an environment
 */
static void render_status(SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;

  bool lost = game_nb_moves_cur(env->game) >= game_nb_moves_max(env->game) &&
              !game_is_over(env->game);

  // Convert (nb_cur / nb_moves_max) to string.
  // Print "!" at the end if game lost
  char str_status[15];
  if (!lost)
    sprintf(str_status, "%u/%u", game_nb_moves_cur(env->game),
            game_nb_moves_max(env->game));
  else
    sprintf(str_status, "%u/%u !", game_nb_moves_cur(env->game),
            game_nb_moves_max(env->game));

  // Init texture and color it according to status game
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font)
    ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  SDL_Surface *surface;
  if (game_is_over(env->game))
    surface = TTF_RenderText_Blended(font, str_status, LIME);
  else if (!lost)
    surface = TTF_RenderText_Blended(font, str_status, SILVER);
  else
    surface = TTF_RenderText_Blended(font, str_status, RED_SDL);

  SDL_Texture *status = SDL_CreateTextureFromSurface(ren, surface);

  // Set position and size
  rect.w = env->width[GRID] / 4;
  rect.h = rect.w / 2;
  if (rect.h > env->grid_start_y)
    rect.h = env->grid_start_y;
  rect.x = env->grid_start_x + env->width[GRID] / 4 + rect.w / 2;
  rect.y = env->grid_start_y + env->height[GRID] + rect.h / 16;

  // Render it then destroy it
  SDL_RenderCopy(ren, status, NULL, &rect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(status);
  TTF_CloseFont(font);
}

/**
 * @brief Renders a menu where you can load all the games
 *
 * @param ren a renderer
 * @param env an environment
 */
static void render_menu(SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;

  // Set default game's button position and size then renders it
  rect.w = env->width[GRID] / 4;
  rect.h = env->height[GRID] / 8;
  rect.x = env->grid_start_x + env->width[GRID] / 8;
  rect.y = env->grid_start_y + env->height[GRID] / 4 - rect.h / 2;
  SDL_QueryTexture(env->textures[DEF_GAME], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[DEF_GAME], NULL, &rect);

  // Random game "button"
  rect.x = rect.x + env->width[GRID] / 2;
  SDL_QueryTexture(env->textures[RAND_GAME], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[RAND_GAME], NULL, &rect);

  // Horizontal_game2N "button"
  rect.y = rect.y + env->height[GRID] / 2;
  SDL_QueryTexture(env->textures[GAME_2N], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[GAME_2N], NULL, &rect);

  // Horizontal_game2S "button"
  rect.x = rect.x - env->width[GRID] / 2;
  SDL_QueryTexture(env->textures[GAME_2S], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[GAME_2S], NULL, &rect);

  // Draw lines to seperate different games
  int start_line_x = env->grid_start_x + env->width[GRID] / 2;
  int start_line_y = env->grid_start_y;
  int end_line_x = start_line_x;
  int end_line_y = env->grid_start_y + env->height[GRID];
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);

  start_line_x = env->grid_start_x;
  start_line_y = env->grid_start_y + env->height[GRID] / 2;
  end_line_x = env->grid_start_x + env->width[GRID];
  end_line_y = start_line_y;
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);
}

/**
 * @brief Renders 'YOU LOSE' || 'YOU WON' when the grid is full
 * (state == WIN || state == LOSE)
 *
 * @param ren a renderer
 * @param env an environment
 * @param state the game's state (WIN || LOSE)
 */
static void render_end_game(SDL_Renderer *ren, Env *env, status state) {
  SDL_Rect rect;

  // Render end game message (pos: grid center)
  rect.w = env->width[GRID] / 2;
  rect.h = 100;
  rect.x = env->grid_start_x + env->width[GRID] / 4;
  rect.y = (env->height[WINDOW] - rect.h) / 2;

  if (state == WIN) {
    SDL_QueryTexture(env->textures[WON], NULL, NULL, NULL, NULL);
    SDL_RenderCopy(ren, env->textures[WON], NULL, &rect);
  } else if (state == LOSE) {
    SDL_QueryTexture(env->textures[LOST], NULL, NULL, NULL, NULL);
    SDL_RenderCopy(ren, env->textures[LOST], NULL, &rect);
  }
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
  float button_y = e->tfinger.y * env->height[WINDOW];
  float button_x = e->tfinger.x * env->width[WINDOW];
  return (button_y >= env->grid_start_y &&
          button_y <= env->grid_start_y + env->height[GRID] &&
          button_x <= env->width[GRID] + env->grid_start_x &&
          button_x >= env->grid_start_x);
#endif
  return (e->button.y >= env->grid_start_y &&
          e->button.y <= env->grid_start_y + env->height[GRID] &&
          e->button.x >= env->grid_start_x &&
          e->button.x <= env->width[GRID] + env->grid_start_x);
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
  float button_y = e->tfinger.y * env->height[WINDOW];
  return (button_y > env->grid_start_y + env->height[GRID] &&
          button_y <
              env->grid_start_y + env->height[GRID] + env->width[GRID] / 8);
#endif
  return (e->button.y > env->grid_start_y + env->height[GRID] &&
          e->button.y <
              env->grid_start_y + env->height[GRID] + env->width[GRID] / 8);
}

/**
 * @brief Checks if a click is over the grid (in the menu | title | solve area)
 *
 * @param e pointer to an SDL_Event
 * @param env pointer to an environment
 * @return true if click is over the grid
 * @return false otherwise
 */
static bool click_over_grid(SDL_Event *e, Env *env) {
#ifdef __ANDROID__
  float button_y = e->tfinger.y * env->height[WINDOW];
  return (button_y < env->grid_start_y &&
          button_y > env->grid_start_y - env->width[GRID] / 8);
#endif
  return (e->button.y < env->grid_start_y &&
          e->button.y > env->grid_start_y - env->width[GRID] / 8);
}

/**
 * @brief Checks if all the cells have the same color
 * @param g the game
 * @return false if a cell is different from the first cell
 */
static bool full_grid(cgame g) {
  color c = game_cell_current_color(g, 0, 0);

  for (int y = 0; y < game_height(g); y++) {
    for (int x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g, x, y) != c) {
        return false;
      }
    }
  }

  return true;
}

/* ***** copy file from assets/ to Android internal storage ***** */

#ifdef __ANDROID__
static void copy_asset(char *src, char *dst) {
  SDL_RWops *file = SDL_RWFromFile(src, "r");
  if (!file)
    ERROR("[ERROR] SDL_RWFromFile: %s\n", src);
  int size = SDL_RWsize(file);
  PRINT("copy file %s (%d bytes) into %s\n", src, size, dst);
  char *buf = (char *)malloc(size + 1);
  if (!buf)
    ERROR("[ERROR] malloc\n");
  int r = SDL_RWread(file, buf, 1, size);
  PRINT("read %d\n", r);
  if (r != size)
    ERROR("[ERROR] fail to read all file (%d bytes)\n", r);
  FILE *out = fopen(dst, "w+");
  if (!out)
    ERROR("[ERROR] fail to create file %s\n", dst);
  int w = fwrite(buf, 1, r, out);
  if (r != w)
    ERROR("[ERROR] fail to write all file (%d bytes)\n", w);
  fclose(out);
  SDL_RWclose(file);
  free(buf);
}
/**
 * @brief Loads a game for android devices
 *
 * @param FILE the game to be load
 * @return the loaded game
 */
static game game_load_android(char *FILE) {
  const char *dir = SDL_AndroidGetInternalStoragePath();
  /* const char * dir = SDL_AndroidGetExternalStoragePath(); */
  char filename[1024];
  sprintf(filename, "%s/%s", dir, FILE);
  copy_asset(FILE, filename);
  game g = game_load(filename);
  return g;
}
#endif