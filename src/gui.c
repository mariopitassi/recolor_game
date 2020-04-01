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

#ifdef __ANDROID__
#define FONT "squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "background.png"
#else
#define FONT "ressources/squirk.ttf"
#define FONTSIZE 200
#define BACKGROUND "ressources/bg.jpg"
#endif

/* **************************************************************** */

enum status { PLAYING, WIN };
enum texture { BACKGROUND_TEXT, TITLE, RESTART, QUIT, WON };

struct Gui_color {
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
  SDL_Texture **textures;
  uint nb_textures;
};

/* **** Static functions (decription below main functions) **** */

static void usage(bool cond, char *err_mess);
static void update_env(SDL_Window *win, Env *env);
static Gui_color *init_colors(SDL_Renderer *ren, game g);
static void render_const_textures(SDL_Renderer *ren, Env *env);
static void render_status(SDL_Renderer *ren, Env *env);
static bool click_on_grid(SDL_Event *e, Env *env);
static bool click_under_grid(SDL_Event *e, Env *env);
static void draw_grid(SDL_Window *win, SDL_Renderer *ren, Env *env);
static void draw_cell(SDL_Window *win, SDL_Renderer *ren, Env *env, uint x,
                      uint y, color c);

/* ********************* Main functions *************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  srand(time(NULL));

  // Init game
  game g;

#ifdef __ANDROID__
  // Create a complete random game for the moment
  // We'll create a sort of a menu to choose the game we want in the future
  uint wrap = rand() % 2;
  g = game_random_ext(6 + rand() % 15, 6 + rand() % 15, wrap, 4 + rand() % 13,
                      12 + rand() % 100);
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

  // Get screen size info (width & height) and more
  update_env(win, env);

  SDL_MaximizeWindow(win);
  SDL_SetWindowPosition(win, 0, 0);

  // Init grid colors
  env->colors = init_colors(ren, g);

  // Allocate memory for textures tab
  env->nb_textures = 5; // Bg, title, restart, quit, won
  env->textures = malloc(5 * sizeof(SDL_Texture *));

  // Init background
  env->textures[BACKGROUND_TEXT] = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->textures[BACKGROUND_TEXT])
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  // Init text texture using squirk font
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font)
    ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font,
                   TTF_STYLE_NORMAL); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL

  SDL_Surface *title = TTF_RenderText_Blended(font, "Flood It", DIM_GREY);
  env->textures[TITLE] = SDL_CreateTextureFromSurface(ren, title);

  SDL_Surface *restart = TTF_RenderText_Blended(font, "Restart", DIM_GREY);
  env->textures[RESTART] = SDL_CreateTextureFromSurface(ren, restart);

  SDL_Surface *quit = TTF_RenderText_Blended(font, "Quit", DIM_GREY);
  env->textures[QUIT] = SDL_CreateTextureFromSurface(ren, quit);

  SDL_Surface *won = TTF_RenderText_Blended(font, "YOU WON !", BLACK);
  env->textures[WON] = SDL_CreateTextureFromSurface(ren, won);

  SDL_FreeSurface(title);
  SDL_FreeSurface(restart);
  SDL_FreeSurface(quit);
  SDL_FreeSurface(won);
  TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {

  update_env(win, env);

  // Render background texture
  SDL_RenderCopy(ren, env->textures[BACKGROUND_TEXT], NULL, NULL);

  // Render title, restart and quit
  render_const_textures(ren, env);

  // Render grid
  draw_grid(win, ren, env);

  // Render nb_cur / nb_max
  render_status(ren, env);

  status state = env->game_state;

  if (state == WIN) {
    SDL_Rect rect;

    // End game message (pos: grid center)
    rect.w = env->grid_width / 2;
    rect.h = 100;
    rect.x = env->grid_start_x + env->grid_width / 4;
    rect.y = (env->win_height - rect.h) / 2;
    SDL_QueryTexture(env->textures[WON], NULL, NULL, NULL, NULL);
    SDL_RenderCopy(ren, env->textures[WON], NULL, &rect);
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
    float button_x = e->tfinger.x * env->win_width;
    float button_y = e->tfinger.y * env->win_height;

    if (click_on_grid(e, env) && env->game_state == PLAYING) {
      // Convert tapped area to a valid x and y for game_cell_current_color()
      uint x = (button_x - env->grid_start_x) / env->cell_width;
      uint y = (button_y - env->grid_start_y) / env->cell_height;

      color c_cur = game_cell_current_color(env->game, x, y);

      game_play_one_move(env->game, c_cur);

      if (game_is_over(env->game))
        env->game_state = WIN;
    } else if (click_under_grid(e, env)) {
      // Tapped on 'Restart'
      if (button_x > env->grid_start_x &&
          button_x < env->grid_start_x + env->grid_width / 4) {
        game_restart(env->game);
        env->game_state = PLAYING;
      }
      // Tapped on 'Quit'
      else
        return (button_x > env->grid_start_x + 3 * env->grid_width / 4 &&
                button_x < env->grid_start_x + env->grid_width);
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
      else if (e->button.x > env->grid_start_x + 3 * env->grid_width / 4 &&
               e->button.x < env->grid_start_x + env->grid_width) {
        PRINT("DOMMAGE\n");
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

  for (uint text = 0; text < env->nb_textures; text++)
    SDL_DestroyTexture(env->textures[text]);

  free(env->textures);
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

  for (uint y = env->grid_start_y; y < 1 + env->grid_start_y + env->grid_height;
       y += env->cell_height) {
    SDL_RenderDrawLine(ren, env->grid_start_x, y,
                       env->grid_start_x + env->grid_width, y);
  }

  // Grid rows
  for (uint x = env->grid_start_x; x < 1 + env->grid_start_x + env->grid_width;
       x += env->cell_width) {
    SDL_RenderDrawLine(ren, x, env->grid_start_y, x,
                       env->grid_start_y + env->grid_height);
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
  rect.w = env->grid_width / 2;
  rect.h = rect.w / 4;
  if (rect.h > env->grid_start_y)
    rect.h = env->grid_start_y; // here grid_start_y == gap between top's edge
                                // window and grid
  rect.x = env->grid_start_x + (env->grid_width / 4);
  rect.y = env->grid_start_y - rect.h;
  SDL_QueryTexture(env->textures[TITLE], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[TITLE], NULL, &rect);

  // Save height for drawing lines
  int h = rect.h;

  // Same for Restart "button"
  rect.w = rect.w / 4;
  rect.h = rect.w / 2;
  rect.x = env->grid_start_x + rect.w / 2;
  rect.y = env->grid_start_y + env->grid_height + rect.h / 2;
  if (rect.y + rect.h >= env->win_height) {
    rect.h = env->grid_start_y / 2;
    rect.y = env->grid_start_y + env->grid_height + rect.h / 2;
  }
  SDL_QueryTexture(env->textures[RESTART], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[RESTART], NULL, &rect);

  // Same for Quit "button" (same w, h and y than 'Restart')
  rect.x = env->grid_start_x + 3 * env->grid_width / 4 + rect.w / 2;
  SDL_QueryTexture(env->textures[QUIT], NULL, NULL, NULL, NULL);
  SDL_RenderCopy(ren, env->textures[QUIT], NULL, &rect);

  // Draw lines to seperate them
  int start_line_x = env->grid_start_x + env->grid_width / 4;
  int start_line_y = env->grid_start_y + env->grid_height;
  int end_line_x = env->grid_start_x + env->grid_width / 4;
  int end_line_y = env->grid_start_y + env->grid_height + h;
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);

  start_line_x = env->grid_start_x + (3 * env->grid_width / 4);
  end_line_x = env->grid_start_x + (3 * env->grid_width / 4);
  SDL_RenderDrawLine(ren, start_line_x, start_line_y, end_line_x, end_line_y);
}

/**
 * @brief Renders 'nb_cur/nb_max_moves' under the grid
 * Add an '!' if game lost (nb_cur > nb_moves_max)
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

  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font)
    ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font,
                   TTF_STYLE_NORMAL); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL

  SDL_Surface *surface;
  if (game_is_over(env->game))
    surface = TTF_RenderText_Blended(font, str_status, LIME);
  else if (!lost)
    surface = TTF_RenderText_Blended(font, str_status, DIM_GREY);
  else
    surface = TTF_RenderText_Blended(font, str_status, RED_SDL);

  SDL_Texture *status = SDL_CreateTextureFromSurface(ren, surface);

  // Set position and size of : nb_cur / nb_moves_max
  rect.w = env->grid_width / 4;
  rect.h = rect.w / 2;
  if (rect.h > env->grid_start_y)
    rect.h = env->grid_start_y;
  rect.x = env->grid_start_x + env->grid_width / 4 + rect.w / 2;
  rect.y = env->grid_start_y + env->grid_height + rect.h / 16;

  SDL_RenderCopy(ren, status, NULL, &rect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(status);
  TTF_CloseFont(font);
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
  return (button_y > env->grid_start_y + env->grid_height &&
          button_y <
              env->grid_start_y + env->grid_height + env->grid_width / 8);
#endif
  return (e->button.y > env->grid_start_y + env->grid_height &&
          e->button.y <
              env->grid_start_y + env->grid_height + env->grid_width / 8);
}