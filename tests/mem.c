#include "mem.h"
#include "tests.h"
#include <stdint.h>

START_TEST(test_mem_alloc) {
  void * ptr = mem_alloc(10);
  ck_assert(ptr != NULL);
  ck_assert(*((int8_t *)(ptr - sizeof(size_t) - sizeof(int8_t))) == 1);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) )) == 10);
  mem_free(ptr);

  ck_assert(NULL == mem_alloc(0));
}
END_TEST

START_TEST(test_mem_free) {
  mem_free(NULL);  
}
END_TEST

START_TEST(test_mem_cpy) {
  void *ptr = mem_alloc(10);
  void *ptr2 = mem_cpy(ptr);
  ck_assert(ptr != NULL);
  ck_assert(*((int8_t *)(ptr - sizeof(size_t) - sizeof(int8_t))) == 2);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) )) == 10);
  ck_assert(ptr2 != NULL);
  ck_assert(*((int8_t *)(ptr2 - sizeof(size_t) - sizeof(int8_t))) == 2);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) )) == 10);
  ck_assert(ptr == ptr2);
  mem_free(ptr);
  ptr = NULL;
  ck_assert(ptr2 != NULL);
  ck_assert(*((int8_t *)(ptr2 - sizeof(size_t) - sizeof(int8_t))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) )) == 10);
  mem_free(ptr2);
}
END_TEST

START_TEST(test_mem_dup) {
  void *ptr = mem_alloc(10);
  void *ptr2 = mem_dup(ptr);
  ck_assert(ptr != NULL);
  ck_assert(*((int8_t *)(ptr - sizeof(size_t) - sizeof(int8_t))) == 1);
  ck_assert(*((size_t *)(ptr - sizeof(size_t) )) == 10);
  ck_assert(ptr2 != NULL);
  ck_assert(*((int8_t *)(ptr2 - sizeof(size_t) - sizeof(int8_t))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) )) == 10);
  ck_assert(ptr != ptr2);
  mem_free(ptr);
  ptr = NULL;
  ck_assert(ptr2 != NULL);
  ck_assert(*((int8_t *)(ptr2 - sizeof(size_t) - sizeof(int8_t))) == 1);
  ck_assert(*((size_t *)(ptr2 - sizeof(size_t) )) == 10);
  mem_free(ptr2);
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
  
  suite_add_tcase(s, tc);
  
  return s;
}
