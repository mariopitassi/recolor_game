#include "toolbox.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ********************* LIST functions *************************** */

struct SList {
  data_type data;
  struct SList *next;
};

void memoryError(void) {
  fprintf(stderr, "Not enough memory!\n");
  exit(EXIT_FAILURE);
}

SList asde_slist_alloc(void) {
  SList p = (SList)malloc(sizeof(struct SList));
  if (p == NULL)
    memoryError();
  p->next = NULL;
  return p;
}

void asde_slist_free(SList L) {
  if (L != NULL) {
    free(L);
    L = NULL;
  }
}

SList asde_slist_create_empty(void) { return NULL; }

bool asde_slist_isEmpty(SList L) { return (L == NULL); }

SList asde_slist_prepend(SList L, data_type data) {
  SList p = asde_slist_alloc();
  p->data = data;
  p->next = L;
  return p;
}

SList asde_slist_delete_first(SList L) {
  if (!asde_slist_isEmpty(L)) {
    SList next = L->next;
    asde_slist_free(L);
    L = next;
  }
  return L;
}

SList asde_slist_insert_after(SList L, SList p, data_type data) {
  if (!asde_slist_isEmpty(p)) {
    SList new = asde_slist_alloc();
    new->data = data;
    new->next = p->next;
    p->next = new;
  } else {
    fprintf(stderr, "insert_after on empty list!\n");
    exit(EXIT_FAILURE);
  }
  return L;
}

SList asde_slist_delete_after(SList L, SList p) {
  if (!asde_slist_isEmpty(p)) {
    if (!asde_slist_isEmpty(p->next)) {
      SList to_be_deleted = p->next;
      p->next = to_be_deleted->next;
      asde_slist_free(to_be_deleted);
    }
  } else {
    fprintf(stderr, "delete_after on empty list!\n");
    exit(EXIT_FAILURE);
  }
  return L;
}

SList asde_slist_next(SList L) {
  if (asde_slist_isEmpty(L)) {
    fprintf(stderr, "next on empty list!\n");
    exit(EXIT_FAILURE);
  }
  return L->next;
}

data_type asde_slist_data(SList L) {
  if (asde_slist_isEmpty(L)) {
    fprintf(stderr, "Data on empty list!\n");
    exit(EXIT_FAILURE);
  }
  return L->data;
}

// erases the entire list
SList asde_slist_delete_list(SList L) {
  while (!asde_slist_isEmpty(L)) {
    L = asde_slist_delete_first(L);
  }
  return L;
}

// tests whether the element belongs to a SList
bool asde_slist_belongs_to(SList L, data_type data) {
  SList p = L;
  while (!asde_slist_isEmpty(p)) {
    if (asde_slist_data(p) == data)
      return true;
    p = asde_slist_next(p);
  }
  return false;
}

// gets the first position of the given element in a SList, -1 if not in the
// list
int asde_slist_position(SList L, data_type data) {
  int i = 0;
  while (!asde_slist_isEmpty(L)) {
    if (asde_slist_data(L) == data)
      return i;
    L = asde_slist_next(L);
    i++;
  }
  return -1;
}

// returns the element in a SList at the given position, starting with 0
data_type asde_slist_nth(SList L, int n) {
  if (asde_slist_isEmpty(L) || (n < 0)) {
    fprintf(stderr, "asde_slist_nth : wrong query");
    exit(EXIT_FAILURE);
  }
  if (n == 0)
    return asde_slist_data(L);
  return asde_slist_nth(asde_slist_next(L), n - 1);
}

// gets the last element in a SList
data_type asde_slist_last(SList L) {
  if (asde_slist_isEmpty(L)) {
    fprintf(stderr, "asde_slist_nth : wrong query");
    exit(EXIT_FAILURE);
  }
  if (asde_slist_isEmpty(asde_slist_next(L)))
    return asde_slist_data(L);
  return asde_slist_last(asde_slist_next(L));
}

// adds an element at the end of the list
SList asde_slist_append(SList L, data_type data) {
  if (asde_slist_isEmpty(L))
    return asde_slist_prepend(L, data);
  SList p = L;
  while (!asde_slist_isEmpty(asde_slist_next(p)))
    p = asde_slist_next(p);
  return asde_slist_insert_after(L, p, data);
}

// prints the elements of a list of integers
static void asde_slist_print_rec_intList(SList L) {
  if (!asde_slist_isEmpty(L)) {
    printf(" %d ", asde_slist_data(L));
    asde_slist_print_rec_intList(asde_slist_next(L));
  }
}

void asde_slist_print_intList(SList L) {
  if (!asde_slist_isEmpty(L)) {
    printf("[");
    asde_slist_print_rec_intList(L);
    printf("]");
  } else
    printf("liste vide");
}

// deletes the last element in a SList
// SList asde_slist_delete_last (SList L);

// deletes all occurrences of an element in a SList
SList asde_slist_delete_all_ter(SList L, SList p, data_type data) {
  if (asde_slist_isEmpty(asde_slist_next(p)))
    return L;
  if (asde_slist_data(asde_slist_next(p)) == data)
    return asde_slist_delete_all_ter(L, asde_slist_delete_after(L, p), data);
  return asde_slist_delete_all_ter(L, asde_slist_next(p), data);
}

SList asde_slist_delete_all(SList L, data_type data) {
  if (asde_slist_isEmpty(L))
    return L;
  if (asde_slist_data(L) == data)
    return asde_slist_delete_all(asde_slist_delete_first(L), data);
  return asde_slist_delete_all_ter(L, L, data);
}

// adds L2 onto the end of L1
SList asde_slist_concat(SList L1, SList L2) {
  if (asde_slist_isEmpty(L2))
    return L1;
  return asde_slist_concat(asde_slist_append(L1, asde_slist_data(L2)),
                           asde_slist_next(L2));
}

// reverses a SList and returns the new list
SList asde_slist_reverse(SList L) {
  if (asde_slist_isEmpty(L))
    return L;
  if (asde_slist_isEmpty(asde_slist_next(L))) {
    SList L_aux = asde_slist_create_empty();
    return asde_slist_prepend(L_aux, asde_slist_data(L));
  }
  return asde_slist_append(asde_slist_reverse(asde_slist_next(L)),
                           asde_slist_data(L));
}



/* ********************* GAME_RAND function *************************** */

game game_random_ext(uint width, uint height, bool is_wrapping, uint max_color,
                     uint nb_max_moves) {
  game g = game_new_empty_ext(width, height, is_wrapping);
  for (uint x = 0; x < width; x++)
    for (uint y = 0; y < height; y++) {
      uint c = (uint)rand() % max_color;
      game_set_cell_init(g, x, y, c);
    }
  game_restart(g);
  game_set_max_moves(g, nb_max_moves);
  return g;
}