#ifndef ASDE_SLIST_UTILITARY_H
#define ASDE_SLIST_UTILITARY_H

#include "game.h"
#include "game_io.h"
#include <stdbool.h>

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

#endif /* ASDE_SLIST_UTILITARY */
