#include "mtrack.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

static MTracker * _mk_new (MTracker * root) {
  MTracker * new = malloc(sizeof(*new));
  if (new != NULL) {
    memset(new->table, 0, sizeof(void *) * MTRACKER_SIZE * 2);
    new->next = NULL;
    new->nextFree = NULL;
    new->ownAlloc = 1;
  }

  /* prepend, it's faster and we are mostly filling this up */
  if (root != NULL) {
    new->next = root->next;
    root->next = new;
  }
  return new;
}

/* See tests/experiences/bfill.c */
static inline int _mk_search_free_ptr(const void * table[]) {
  int begin = 0;
  int end = MTRACKER_SIZE - 1;
  int prevEnd = end;
  unsigned int middle = 0; /* must be unsigned */
  
  while (1) {
    if (end < begin) {
      if (table[begin] == NULL) {
	return begin;
      } else {
	return -1; /* table full */
      }
    }
    middle = (end + begin) >> 1; /* divide by two, rounded down */
    if (table[middle] == NULL) {
      prevEnd = end;
      end = middle - 1;
    } else {
      begin = middle + 1;
      end = prevEnd - 1;
    }
  }
}

MTracker * mk_alloc(void) {
  MTracker * new =_mk_new(NULL);
  new->nextFree = new;
  return new;
}

void mk_init (MTracker * root) {
  if (root == NULL) { return; }
  memset(root->table, 0, sizeof(void *) * MTRACKER_SIZE * 2);
  root->nextFree = root;
  root->next = NULL;
  root->ownAlloc = 0;
}

int mk_track(MTracker * root, void * ptr, void (*ptr_free)(void * ptr)) {
  MTracker *nextFree = NULL;
  int freeSlot = -1;
  if (ptr == NULL) { return 0; }
  nextFree = root->nextFree;
  if (nextFree != NULL) {
    freeSlot = _mk_search_free_ptr((const void **)nextFree->table);
    if (freeSlot == -1) {
      if ((nextFree = _mk_new(root)) != NULL) {
	root->nextFree = nextFree;
	freeSlot = _mk_search_free_ptr((const void **)nextFree->table);
      }
    }
  } else {
    if ((nextFree = _mk_new(root)) != NULL) {
      root->nextFree = nextFree;
      freeSlot = _mk_search_free_ptr((const void **)nextFree->table);
    }
  }

  if (freeSlot >= 0) {
    nextFree->table[freeSlot] = ptr;
    nextFree->table[freeSlot + MTRACKER_SIZE] = ptr_free;
  }
  return 1;
}

int mk_free(MTracker * root, void * ptr) {
  MTracker * c = root;
  int i = 0;
  while (c != NULL) {
    for (i = 0; i < MTRACKER_SIZE; i++) {
      if (c->table[i] == ptr) {
	((void (*)(void *))c->table[i + MTRACKER_SIZE])(c->table[i]);
	c->table[i] = NULL;
	c->table[i + MTRACKER_SIZE] = NULL;
	goto done_jump_out_loops;
      }
    }
    c = c->next;
  }
  return 0;
 done_jump_out_loops:
  return 1;
}

void mk_free_all(MTracker * root) {
  MTracker * c = root;
  int i = 0;
  while (c != NULL) {
    for (i = 0; i < MTRACKER_SIZE; i++) {
      if (c->table[i] != NULL && c->table[i + MTRACKER_SIZE] != NULL) {
	((void (*)(void *))c->table[i + MTRACKER_SIZE])(c->table[i]);
	c->table[i] = NULL;
	c->table[i + MTRACKER_SIZE] = NULL;
      }
    }
    c = c->next;
  }
}

void mk_destroy(MTracker * root) {
  MTracker * c = root;
  MTracker * tmp = NULL;
  int i = 0;
  while (c != NULL) {
    for (i = 0; i < MTRACKER_SIZE; i++) {
      if (c->table[i] != NULL && c->table[i + MTRACKER_SIZE] != NULL) {
	((void (*)(void *))c->table[i + MTRACKER_SIZE])(c->table[i]);
	c->table[i] = NULL;
	c->table[i + MTRACKER_SIZE] = NULL;
      }
    }
    tmp = c;
    c = c->next;
    if (tmp->ownAlloc) {
      free(tmp);
    }
  }
}
