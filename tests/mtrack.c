#include "mtrack.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>
START_TEST(test_mtrack_init) {
  MTracker t;
  int i = 0;
  mk_init(&t);
  ck_assert(t.nextFree == &t);
  ck_assert(t.next == NULL);
  ck_assert(t.ownAlloc == 0);
  for (i = 0; i < MTRACKER_SIZE * 2; i++) {
    ck_assert(t.table[i] == NULL);
  }
  mk_destroy(&t);
}
END_TEST

START_TEST(test_mtrack_alloc) {
  MTracker * t;
  int i = 0;
  t = mk_alloc();
  ck_assert(t->nextFree == t);
  ck_assert(t->next == NULL);
  ck_assert(t->ownAlloc != 0);
  for (i = 0; i < MTRACKER_SIZE * 2; i++) {
    ck_assert(t->table[i] == NULL);
  }
  mk_destroy(t);
}
END_TEST

START_TEST(test_mtrack_track) {
  MTracker t;
  int i = 0;
  mk_init(&t);

  for (i = 0; i < MTRACKER_SIZE; i++) {
    ck_assert(mk_track(&t, malloc(1000), free) == 1);
  }
  for (i = 0; i < MTRACKER_SIZE; i++) {
    ck_assert(t.table[i] != NULL);
  }
  mk_free_all(&t);

  for (i = 0; i < MTRACKER_SIZE >> 1;  i++) {
    ck_assert(mk_track(&t, malloc(1000), free) == 1);
  }
  for (i = 0; i < MTRACKER_SIZE; i++) {
    ck_assert(i < MTRACKER_SIZE >> 1 ? t.table[i] != NULL : t.table[i] == NULL);
  }
  mk_destroy(&t);
  mk_init(&t);
  for (i = 0; i < MTRACKER_SIZE + 1; i++) {
    ck_assert(mk_track(&t, malloc(1000), free) == 1);
  }
  for (i = 0; i < MTRACKER_SIZE; i++) {
    ck_assert(t.table[i] != NULL);
  }
  ck_assert(t.nextFree != &t);
  ck_assert(t.nextFree->table[0] != NULL);
  for (i = 1; i < MTRACKER_SIZE; i++) {
    ck_assert(t.nextFree->table[1] == NULL);
  }
  mk_destroy(&t);
}
END_TEST;

Suite * mtrack_test_suite (void) {
  Suite * s = NULL;
  TCase * tc = NULL;

  s = suite_create("Memory tracker test");
  tc = tcase_create("Core");

  tcase_add_test(tc, test_mtrack_init);
  tcase_add_test(tc, test_mtrack_alloc);
  tcase_add_test(tc, test_mtrack_track);
  suite_add_tcase(s, tc);
  return s;
}
