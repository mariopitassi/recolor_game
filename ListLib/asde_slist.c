#include "asde_slist.h"
#include <stdio.h>
#include <stdlib.h>

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
