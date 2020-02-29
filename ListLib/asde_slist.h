#ifndef ASDE_SLIST_H
#define ASDE_SLIST_H

#include "../game.h"
#include "../game_io.h"
#include <stdbool.h>

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

#endif /* ASDE_SLIST_H */
