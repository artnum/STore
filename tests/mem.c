#include "mem.h"
#include "tests.h"
#include <stdint.h>
#include <stdlib.h>

START_TEST(test_mem_alloc) {
  void * ptr = mem_alloc(10);
  ck_assert(ptr != NULL);
  ck_assert(*((uint8_t *)(ptr - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 1);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) - sizeof(void *))) == 10);
  ck_assert(*((void **)(ptr - sizeof(void *))) == NULL);
  mem_free(ptr);

  ck_assert(NULL == mem_alloc(0));
}
END_TEST

START_TEST(test_mem_free) {
  mem_free(NULL);  
}
END_TEST

START_TEST(test_mem_cpy) {
  int *i1 = mem_alloc(sizeof(*i1));
  int *i2 = NULL;		     
  void *ptr = mem_alloc(10);
  void *ptr2 = mem_cpy(ptr);
  ck_assert(ptr != NULL);
  ck_assert(*((uint8_t *)(ptr - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 2);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) - sizeof(void *))) == 10);
  ck_assert(*((void **)(ptr - sizeof(void *))) == NULL);
  ck_assert(ptr2 != NULL);
  ck_assert(*((uint8_t *)(ptr2 - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 2);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) - sizeof(void *))) == 10);
  ck_assert(*((void **)(ptr2 - sizeof(void *))) == NULL);
  ck_assert(ptr == ptr2);
  mem_free(ptr);
  ptr = NULL;
  ck_assert(ptr2 != NULL);
  ck_assert(*((uint8_t *)(ptr2 - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) - sizeof(void *))) == 10);
  mem_free(ptr2);

  i2 = mem_cpy(i1);
  *i1 = 5;
  ck_assert(*i1 == *i2);
  mem_free(i1);
  mem_free(i2);
}
END_TEST

START_TEST(test_mem_dup) {
  void *ptr = mem_alloc(10);
  void *ptr2 = mem_dup(ptr);
  ck_assert(ptr != NULL);
  ck_assert(*((uint8_t *)(ptr - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 2);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) - sizeof(void *))) == 10);
  ck_assert(ptr2 != NULL);
  ck_assert(*((uint8_t *)(ptr2 - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) - sizeof(void *))) == 10);
  ck_assert(ptr != ptr2);
  mem_free(ptr);
  ptr = NULL;
  ck_assert(ptr2 != NULL);
  ck_assert(*((uint8_t *)(ptr2 - sizeof(size_t) - sizeof(uint8_t) - sizeof(void *))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) - sizeof(void *))) == 10);
  mem_free(ptr2);
}
END_TEST

START_TEST(test_mem_tree) {
  int i = 0;
  int * tmp = NULL;
  int * ptr1 = mem_alloc(sizeof(*ptr1) * 100000);
  int * ptr2 = NULL;
  int * ptr3 = NULL;

  tmp = ptr1;
  for (i = 0; i < 100000; i++) {
    *tmp++ = rand();
  }
  *ptr1 = 1;
  ptr2 = mem_dup(ptr1);
  ptr3 = mem_cpy(ptr2);
  mem_free(ptr2);
  mem_free(ptr1);

  /* ptr1 is not freed as long as ptr2 is not freed */
  ck_assert(*ptr1 == 1); /* mem_free set memory to 0 when effective free happend */
  ck_assert(*ptr2 == 1);
  ck_assert(*ptr3 == 1);
  mem_free(ptr2);
}
END_TEST

START_TEST(test_mem_reset) {
  int *ptr1 = mem_alloc(sizeof(*ptr1));
  int *ptr2 = NULL;

  *ptr1 = 5;
  ptr2 = mem_dup(ptr1);
  ck_assert(*ptr2 == 5);
  *ptr2 = 6;
  ck_assert(*ptr1 != *ptr2);
  ptr2 = mem_reset(ptr2);
  ck_assert(*ptr1 == *ptr2);
  mem_free(ptr2);
  mem_free(ptr1);
}
END_TEST

Suite * mem_test_suite (void) {
  Suite * s = NULL;
  TCase * tc = NULL;

  s = suite_create("Memory Test");
  tc = tcase_create("Core");

  tcase_add_test(tc, test_mem_alloc);
  tcase_add_test(tc, test_mem_free);
  tcase_add_test(tc, test_mem_cpy);
  tcase_add_test(tc, test_mem_dup);
  tcase_add_test(tc, test_mem_tree);
  tcase_add_test(tc, test_mem_reset);
  
  suite_add_tcase(s, tc);
  
  return s;
}
