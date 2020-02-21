#include "game.h"
#include "game_io.h"
#include "ListLib/asde_slist.h"
#include "ListLib/asde_slist_utilitary_functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct solution {
  color* sol_arr;
  int nb_moves;
} * sol;


void print(color *moves_arr, uint size) {
  printf("\n");
  for (int i = 0; i < size; i++) {
    printf("%u", (moves_arr[i]));
  }
}

//Joue plusieurs coups
void game_play_moves(game g, color* moves, uint nb_moves){
  for (uint i = 0; i<nb_moves; i++){
    game_play_one_move(g, moves[i]);
  }
}


// Détermine si la séquence (ici moves_arr) est la dernière ou pas
// Par exemple pour un jeu à 4 couleurs et 3 coups, la dernière séquence sera "3 2 3"
bool end(color *moves_arr, color last_color, color b_last_color, uint size_arr) {
  if (size_arr % 2 == 1){
    for (int i = 0; i < size_arr; i++) {
      if (i % 2 == 0){
        if (moves_arr[i] != last_color) return false;
      }
      else {
        if (moves_arr[i] != b_last_color) return false;
      }
    }
  }
  else{
    for (int i = 0; i < size_arr; i++) {
      if (i % 2 == 0){
        if (moves_arr[i] != b_last_color) return false;
      }
      else {
        if (moves_arr[i] != last_color) return false;
      }
    }
  }
  return true;
}

//Retourne la prochaine couleurs après c dans la tableau c_arr
//A améliorer avec la fonction is_useful_to_play
color next_color(color c, color *c_arr, uint len_c_arr) {
  for (int i = 0; i < len_c_arr; i++) {
    if (c_arr[i] == c)
      return c_arr[(i + 1) % len_c_arr];
  }
  fprintf(stderr, "No next color found\n");
  exit(EXIT_FAILURE);
}

// Copie un tableau, je pense que ça peut etre utile lorsque l'on stockera notre tableau dans la structure solution
color *copy_arr(color *arr, uint nb_moves) {
  color *copy_arr = malloc(nb_moves * sizeof(color));
  for (int i = 0; i < nb_moves; i++) {
    copy_arr[i] = arr[i];
  }
  return copy_arr;
}

// Determine si le tableau passé en paramètre est une solution ou pas
bool is_solution(game g, color *moves_arr, uint nb_moves) {
  for (int i = 0; i < nb_moves; i++) {
    game_play_one_move(g, moves_arr[i]);
    if (game_is_over(g)) {
      return true;
    }
  }
  game_restart(g);
  return false;
}

// Retourne le nb de couleurs du jeu
// Je suis parti du principe que les couleurs sont numéroté de 0 à nb-colors-1
// Peut etre faire une vrai fonction qui compte toutes le couleurs
// A demander au prof
uint nb_colors(cgame g) {
  color color_max = 0;
  for (uint y = 0; y < game_height(g); y++) {
    for (uint x = 0; x < game_width(g); x++) {
      if (game_cell_current_color(g, x, y) > color_max) {
        color_max = game_cell_current_color(g, x, y);
      }
    }
  }
  return color_max + 1;
}

// Créer un tableau de couleur allant de 0 à nb_colors
// Il ne va pas jusqu'à nb_colors-1 car ma fonction find_one à besoin d'une couleur suppémentaire pour tourner
color *create_color_arr(uint nb_colors) {
  color *arr = malloc((nb_colors + 1) * sizeof(color));
  for (uint c = 0; c <= nb_colors; c++) {
    arr[c] = c;
  }
  return arr;
}

// fonction qui met à jour la list color_around avec les couleurs aux alentours 
// Obliger de mettre 999 sur chaque case qui est égale è old_color car sinon récursivité infini
// La liste aura donc un élémént égale à 999 mais ce n'est pas grave
// A optimiser après
void around(game g, uint x, uint y, color oldcolor, SList color_around) {

  if (game_cell_current_color(g,x,y) == oldcolor) {
    game_set_cell_init(g,x,y,999);

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
    if (!asde_slist_belongs_to(color_around, game_cell_current_color(g,x,y)))
      color_around = asde_slist_append(color_around, game_cell_current_color(g,x,y));
  }
}

// Nous dit si la couleur c est utile (elle est aux alentours) après avoir jouer des coups allant de 0 à pos_c - 1
// Fonction à utiliser pour optimisation
bool is_useful_to_play(game g, color c, color* moves_arr, uint pos_c){
  game g2 = game_copy(g);

  game_play_moves(g2, moves_arr, pos_c);

  if (game_cell_current_color(g2,0,0) == c) return false;

  SList color_around = asde_slist_create_empty();
  color_around = asde_slist_prepend(color_around, 999); // je prepend 999 car sinon la fonction around me renvoie une liste vide, à modifier plus tard
  around(g2, 0, 0, game_cell_current_color(g2,0,0), color_around);

  if (!asde_slist_belongs_to(color_around, c)) {
    game_delete(g2);
    asde_slist_delete_list(color_around);
    return false;
  }

  game_delete(g2);
  asde_slist_delete_list(color_around);
  return true;
}


/**
void find_one_rec(game g, color *moves_arr, uint size_moves_arr, color *c_arr,
              uint len_c_arr) {
  // print(moves_arr, size_moves_arr);

  if (is_solution(g, moves_arr, size_moves_arr)) {
    printf("\n\nSOLUTION FOUND\n");
    for (uint i = 0; i < size_moves_arr; i++) {
      printf("%d ", moves_arr[i]);
    }
    printf("\n\n");
  }

  if (end(moves_arr, c_arr[len_c_arr - 2], c_arr[len_c_arr - 3], size_moves_arr)) {
    return;
  }

  moves_arr[0] = next_color(moves_arr[0], c_arr, len_c_arr);

  for (int i = 0; i < size_moves_arr; i++) {
    if (moves_arr[i] == c_arr[len_c_arr - 1]) {
      moves_arr[i] = c_arr[0];
      moves_arr[i + 1] = next_color(moves_arr[i + 1], c_arr, len_c_arr);
    }
  }

  find_one_rec(g, moves_arr, size_moves_arr, c_arr, len_c_arr);
}
**/

void find_one_itr(game g, color *moves_arr, uint size_moves_arr, color *c_arr, uint len_c_arr){
  while(!end(moves_arr, c_arr[len_c_arr - 2], c_arr[len_c_arr - 3], size_moves_arr)){
    //print(moves_arr, size_moves_arr);

    if (is_solution(g, moves_arr, size_moves_arr)) { // à mettre dans un fichier .sol au lieu d'afficher
      printf("\n\nSOLUTION FOUND\n\n");
      for (int i = 0; i < size_moves_arr; i++) {
        printf("%u ", moves_arr[i]);
      }
      printf("\n\n");
      break;
    }

    moves_arr[0] = next_color(moves_arr[0], c_arr, len_c_arr);

    for (int i = 0; i < size_moves_arr; i++) {
      if (i < size_moves_arr -1 && moves_arr[i] == moves_arr[i+1]){ // Si 2 mm couleurs de suite, buggé mais réduit quand mm le nombre d'itération
        moves_arr[i] = next_color(moves_arr[i], c_arr, len_c_arr);
      }

      if (moves_arr[i] == c_arr[len_c_arr - 1]) { // Si on est sur la couleur égale à nb_colors
        moves_arr[i] = c_arr[0];
        moves_arr[i + 1] = next_color(moves_arr[i + 1], c_arr, len_c_arr);
      }
    }
    // Si le premier coup est similaire à la couleur de (0,0)
    if (moves_arr[0] == game_cell_current_color(g,0,0)) moves_arr[0] = next_color(moves_arr[0], c_arr, len_c_arr);
  }
  if (is_solution(g, moves_arr, size_moves_arr)) { // Duplication mais essentiel si la dernière séquence est la bonne
      printf("\n\nSOLUTION FOUND\n\n");
      for (int i = 0; i < size_moves_arr; i++) {
        printf("%u ", moves_arr[i]);
      }
      printf("\n\n");
    }
}

// Créer le premier tableau avec des coups différents minimaux
// Exemple pour un jeu à nb_coups 3 et 4 couleurs : "0 1 0"
color* first_arr(uint size, color *c_arr) {
  color *arr = malloc(size * sizeof(color));

  if (size % 2 == 0) {
    for (uint i = 0; i < size; i++) {
      if (i % 2 == 0)
        arr[i] = c_arr[1];
      else
        arr[i] = c_arr[0];
    }
  }
  else {
    for (uint i = 0; i < size; i++) {
      if (i % 2 == 0)
        arr[i] = c_arr[0];
      else
        arr[i] = c_arr[1];
    }
  }
  return arr;
}

//Trouve la plus petite solution
void find_min(game g, uint size_max, color *c_arr, uint len_c_arr) {
  for (uint size = 1; size <= size_max; size++) {
    uint *s = first_arr(size, c_arr);
    find_one_itr(g, s, size, c_arr, len_c_arr);
    free(s);
  }
}

int main(void) {

  game g = game_load("data/default_game.rec");
  color* col_arr = create_color_arr(nb_colors(g));
  color* moves = first_arr(game_nb_moves_max(g), col_arr);
  //find_min(g, game_nb_moves_max(g), col_arr, nb_colors(g)+1);
  find_one_itr(g, moves, game_nb_moves_max(g), col_arr, nb_colors(g)+1);
  free(col_arr);
  free(moves);
  game_delete(g);
  return EXIT_SUCCESS;
}
