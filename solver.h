#ifndef SOLVER_H
#define SOLVER_H
#include "game.h"
#include "game_io.h"
#include "ListLib/asde_slist.h"
#include <stdio.h>

typedef struct solution {
  color *moves;
  uint nb_moves;
}*sol;

void error(bool cond, char *err_mess);

sol sol_alloc();

void print(color *moves_arr, uint size);

// Joue plusieurs coups
void game_play_moves(game g, color *moves, uint nb_moves);

// Copie un tableau, je pense que ça peut etre utile lorsque l'on stockera notre
// tableau dans la structure solution
color *copy_arr(color *arr, uint len);

// Determine si le tableau passé en paramètre est une solution ou pas
bool is_solution(cgame g, color *moves, uint nb_moves);

// Retourne le nb de couleurs du jeu après avoir joué les couleurs du tableaux
// "moves" de "moves[0] à moves[nb_moves-1]"
uint nb_colors(cgame g, color *moves, uint nb_moves);

// Met à jour la liste "color_around" avec les couleurs adjacentes
void around(game g, uint x, uint y, color oldcolor, SList color_around, uint* nb_color_around);

// Retourne une liste de couleurs adjacentes après avoir joué les couleurs du
// tableaux "moves" de "moves[0] à moves[nb_moves-1]"
// Dernière optimisation que je vois est : trier cette liste
SList col_around(cgame g, color *moves, uint nb_moves);

// Affiche toutes les solutions possibles du jeu "g" sur "nb_moves" coups
// Cette fonctions marche pour "game_default" et "game_horizontal_2S" mais pas
// avec "2N"
void look_for_sol(cgame g, color *moves, SList c_around, uint nb_moves,
                  uint nb_moves_max, uint *nb_sol, sol s, bool look_nb_sol);

sol find_min(cgame g);

sol find_one(cgame g);

uint nb_sol(cgame g);

#endif // SOLVER_H
