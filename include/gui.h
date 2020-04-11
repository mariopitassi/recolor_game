#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...)                                                        \
  do {                                                                         \
    SDL_Log(STR, ##__VA_ARGS__);                                               \
  } while (0)
#define ERROR(STR, ...)                                                        \
  do {                                                                         \
    SDL_Log(STR, ##__VA_ARGS__);                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
#else
#define PRINT(STR, ...)                                                        \
  do {                                                                         \
    printf(STR, ##__VA_ARGS__);                                                \
  } while (0)
#define ERROR(STR, ...)                                                        \
  do {                                                                         \
    fprintf(stderr, STR, ##__VA_ARGS__);                                       \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
#endif

/* **************************************************************** */

#define RED_SDL                                                                \
  (SDL_Color) { 255, 0, 0, 255 }
#define LIME                                                                   \
  (SDL_Color) { 0, 255, 0, 255 }
#define BLUE_SDL                                                               \
  (SDL_Color) { 0, 0, 255, 255 }
#define YELLOW_SDL                                                             \
  (SDL_Color) { 255, 255, 0, 255 }
#define CYAN                                                                   \
  (SDL_Color) { 0, 255, 255, 255 }
#define MAGENTA                                                                \
  (SDL_Color) { 255, 0, 255, 255 }
#define ORANGE                                                                 \
  (SDL_Color) { 255, 165, 0, 255 }
#define SILVER                                                                 \
  (SDL_Color) { 192, 192, 192, 255 }
#define DARK_CYAN                                                              \
  (SDL_Color) { 0, 139, 139, 255 }
#define OLIVE                                                                  \
  (SDL_Color) { 128, 128, 0, 255 }
#define GREEN_SDL                                                              \
  (SDL_Color) { 0, 128, 0, 255 }
#define PURPLE                                                                 \
  (SDL_Color) { 128, 0, 128, 255 }
#define DIM_GREY                                                               \
  (SDL_Color) { 105, 105, 105, 255 }
#define PEACH_PUFF                                                             \
  (SDL_Color) { 255, 218, 185, 255 }
#define SIENNA                                                                 \
  (SDL_Color) { 160, 82, 45, 255 }
#define SALMON                                                                 \
  (SDL_Color) { 250, 128, 114, 255 }
#define BLACK                                                                  \
  (SDL_Color) { 0, 0, 0, 255 }
#define WHITE                                                                  \
  (SDL_Color) { 255, 255, 255, 255 }

/* **************************************************************** */

#define APP_NAME "Recolor Game"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define DELAY 100

/* **************************************************************** */

typedef struct Env_t Env;
typedef enum status status;
typedef struct Text Text;
typedef struct Gui_color Gui_color;
typedef struct Gui_solution Gui_solution;

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]);
void render(SDL_Window *win, SDL_Renderer *ren, Env *env);
void clean(SDL_Window *win, SDL_Renderer *ren, Env *env);
bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e);

/* **************************************************************** */

#endif
