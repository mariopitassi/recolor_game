#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "game.h"
#include "game_io.h"
#include <stdbool.h>

/* ********************* LIST functions *************************** */

typedef color data_type;

typedef struct SList *SList;

// returns an empty list
extern SList asde_slist_create_empty(void);

// tests whether a SList is empty
extern bool asde_slist_isEmpty(SList L);

// adds a new element on the start of the list -> fonction insereEnTete(val L:
// liste d'objets, val x objet): liste d'objets;
extern SList asde_slist_prepend(SList L, data_type data);

// -> fonction supprimerEnTete(val L: liste d'objet): liste d'objet;
extern SList asde_slist_delete_first(SList L);

// -> fonction insererApres(val L:liste d'objet, val p : liste d'objets, val x:
// objet): liste d'objet;
extern SList asde_slist_insert_after(SList L, SList p, data_type data);

// -> fonction supprimerApres(val L:liste d'objet, val p : liste d'objets):
// liste d'objet;
extern SList asde_slist_delete_after(SList L, SList p);

// gets next element in a SList ->  fonction suivant(val L:liste d'objet): liste
// d'objet;
extern SList asde_slist_next(SList L);

// gets data in a SList ->  fonction valeur(val L:liste d'objet): objet;
extern data_type asde_slist_data(SList L);

// erases the entire list
extern SList asde_slist_delete_list(SList L);

// tests whether the element belongs to a SList
extern bool asde_slist_belongs_to(SList L, data_type data);

// gets the first position of the given element in a SList, -1 if not in the
// list
extern int asde_slist_position(SList L, data_type data);

// returns the element in a SList at the given position, starting with 0
extern data_type asde_slist_nth(SList L, int n);

// gets the last element in a SList
extern data_type asde_slist_last(SList L);

// adds an element at the end of the list
extern SList asde_slist_append(SList L, data_type data);

// prints the elements of a list of integers
extern void asde_slist_print_intList(SList L);

// deletes the last element in a SList
extern SList asde_slist_delete_last(SList L);

// deletes all occurrences of an element in a SList
extern SList asde_slist_delete_all(SList L, data_type data);

// adds L2 onto the end of L1
extern SList asde_slist_concat(SList L1, SList L2);

// reverses a SList and returns the new list
extern SList asde_slist_reverse(SList L);

// Returns a game according to the arguments passed
// Raise usage if incorrect number of arguments
game game_from_arg(int argc, char *argv[]);

#endif /* TOOLBOX_H */