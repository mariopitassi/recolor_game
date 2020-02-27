#include "ListLib/asde_slist.h"
#include "ListLib/asde_slist_utilitary_functions.h"
#include "game.h"
#include "game_io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct solution {
  color *sol_arr;
  int nb_moves;
} * sol;

void print(color *moves_arr, uint size) {
  // printf("\n");
  for (int i = 0; i < size; i++) {
    printf("%d ", (moves_arr[i]));
  }
  printf("\n");
}

// Joue plusieurs coups
void game_play_moves(game g, color *moves, uint nb_moves) {
  for (uint i = 0; i < nb_moves; i++) {
    game_play_one_move(g, moves[i]);
  }
}

// Copie un tableau, je pense que ça peut etre utile lorsque l'on stockera notre
// tableau dans la structure solution
color *copy_arr(color *arr, uint nb_moves) {
  color *copy_arr = malloc(nb_moves * sizeof(color));
  for (int i = 0; i < nb_moves; i++) {
    copy_arr[i] = arr[i];
  }
  return copy_arr;
}

// Determine si le tableau passé en paramètre est une solution ou pas
bool is_solution(cgame g, color *moves_arr, uint nb_moves) {
  game g2 = game_copy(g);
  for (int i = 0; i < nb_moves; i++) {
    game_play_one_move(g2, moves_arr[i]);
    if (game_is_over(g2)) {
      game_delete(g2);
      return true;
    }
  }
  game_delete(g2);
  return false;
}

// Retourne le nb de couleurs du jeu après avoir joué les couleurs du tableaux
// "moves" de "moves[0] à moves[nb_moves-1]"
uint nb_colors(cgame g, color *moves, uint nb_moves) {
  game g2 = game_copy(g);
  game_play_moves(g2, moves, nb_moves);

  uint nb_col = 0;

  SList colors = asde_slist_create_empty();
  colors = asde_slist_prepend(colors, 999);

  for (uint y = 0; y < game_height(g2); y++) {
    for (uint x = 0; x < game_width(g2); x++) {
      if (!asde_slist_belongs_to(colors, game_cell_current_color(g2, x, y))) {
        colors = asde_slist_append(colors, game_cell_current_color(g2, x, y));
        nb_col += 1;
      }
    }
  }

  game_delete(g2);
  asde_slist_delete_list(colors);

  return nb_col;
}

// Met à jour la liste "color_around" avec les couleurs adjacentes
void around(game g, uint x, uint y, color oldcolor, SList color_around) {

  if (game_cell_current_color(g, x, y) == oldcolor) {
    game_set_cell_init(g, x, y, 999);

    if (x < game_width(g) - 1)
      around(g, x + 1, y, oldcolor, color_around);
    if (y < game_height(g) - 1)
      around(g, x, y + 1, oldcolor, color_around);
    if (x > 0)
      around(g, x - 1, y, oldcolor, color_around);
    if (y > 0)
      around(g, x, y - 1, oldcolor, color_around);

    if (game_is_wrapping(g)) {
      if (x == game_width(g) - 1)
        around(g, (x + 1 - game_width(g)), y, oldcolor, color_around);
      if (y == game_height(g) - 1)
        around(g, x, (y + 1 - game_height(g)), oldcolor, color_around);
      if (x == 0)
        around(g, (game_width(g) - 1), y, oldcolor, color_around);
      if (y == 0)
        around(g, x, (game_height(g) - 1), oldcolor, color_around);
    }
  }

  else {
    if (!asde_slist_belongs_to(color_around, game_cell_current_color(g, x, y)))
      color_around =
          asde_slist_append(color_around, game_cell_current_color(g, x, y));
  }
}

// Retourne une liste de couleurs adjacentes après avoir joué les couleurs du
// tableaux "moves" de "moves[0] à moves[nb_moves-1]"
// Dernière optimisation que je vois est : trier cette liste
SList col_around(cgame g, color *moves, uint nb_moves) {
  game g2 = game_copy(g);
  game_play_moves(g2, moves, nb_moves);

  SList color = asde_slist_create_empty();
  color = asde_slist_prepend(color, 999);

  around(g2, 0, 0, game_cell_current_color(g2, 0, 0), color);

  color = asde_slist_delete_first(color);

  game_delete(g2);

  return color;
}

// Affiche toutes les solutions possibles du jeu "g" sur "nb_moves" coups
// Cette fonctions marche pour "game_default" et "game_horizontal_2S" mais pas
// avec "2N"
void nb_sol_rec(cgame g, color *moves, SList c_around, uint nb_moves,
                uint *nb_sol) {
  uint moves_left = game_nb_moves_max(g) - nb_moves;
  uint nb_col = nb_colors(g, moves, nb_moves);

  if (moves_left <
      nb_col - 1) { // On arrete car inutile de jouer (sûr de perdre)
    // print(moves, nb_moves);
    return;
  }

  if (nb_moves == game_nb_moves_max(g) || nb_col == 1) { // On a gagné
    *nb_sol += 1;
    printf("SOLUTION : ");
    print(moves, nb_moves);
    printf("NUMBER   : %u\n", *nb_sol);
    printf("NB COUPS : %u\n\n", nb_moves);
    return;
  }

  while (!asde_slist_isEmpty(c_around)) {
    moves[nb_moves] = asde_slist_data(c_around);
    SList next_c_around = col_around(
        g, moves,
        nb_moves + 1); //"nb_moves+1" car on vient d'ajouter un coup à "moves"
    nb_sol_rec(g, moves, next_c_around, nb_moves + 1, nb_sol);
    asde_slist_delete_list(next_c_around);
    c_around = asde_slist_next(c_around);
  }

  asde_slist_delete_list(c_around);
}

int main(void) {
  /**
    color test_arr[] = {0, 0, 1, 4, 0,
                        1, 0, 1, 0, 1,
                        0, 0, 1, 0, 1,
                        1, 0, 0, 0, 1,
                        1, 1, 1, 1, 1,
                        3, 2, 1, 1, 0};
  **/

  // game g = game_new_ext(5, 6, test_arr, 12, false);
  game g = game_load("data/default_game.rec");
  // game g = game_load("data/horizontal_game2S.rec");

  color *moves = malloc(game_nb_moves_max(g) * sizeof(color));
  uint nb_sol = 0;
  SList c_around = col_around(g, moves, 0);

  nb_sol_rec(g, moves, c_around, 0, &nb_sol);
  printf("\n\n NB SOL = %u\n", nb_sol);

  asde_slist_delete_list(c_around);
  free(moves);

  game_delete(g);
  return EXIT_SUCCESS;
}
