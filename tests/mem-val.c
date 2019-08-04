#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#define PTRS_COUNT 500

int main(int argc, char ** argv) {
  int i = 0;
  int j = 0;
  int * iptr = 0;
  void * ptr = mem_alloc(10);
  void * ptr2 = mem_alloc(10);
  void * ptr3 = mem_alloc(10);
  void * ptr4 = NULL;
  void * ptr5 = NULL;
  void * ptrs[PTRS_COUNT] = { NULL };

  /* basic alloc / free */
  ptr5 = mem_cpy(ptr);
  ptr4 = mem_cpy(ptr2);
  mem_free(ptr);
  ptr = NULL;
  mem_free(ptr2);
  ptr2 = NULL;
  ptr = mem_dup(ptr3);
  mem_free(ptr3);
  ptr3 = NULL;

  ptr5 = mem_resize(ptr5, 20);
  ptr4 = mem_resize(ptr4, 0);
  ptr4 = NULL;

  mem_free(ptr);
  mem_free(ptr2);
  mem_free(ptr3);
  mem_free(ptr4);
  mem_free(ptr5);

  /* test for large copy / duplicate */
  for (i = 0; i < 210; i++) {
    ptrs[i] = mem_alloc(100 * sizeof(int));
    for (j = 0; j < 100; j++) {
      ((int *)ptrs[i])[j] = rand();
    }
  }

  for (i = 0; i < 210; i++) {
    ptrs[i + 210] = mem_cpy(ptrs[i]);
  }
  for (i = 0; i < PTRS_COUNT; i++) {
    mem_free(ptrs[i]);
  }

  ptr = mem_alloc(100 * sizeof(int));
  for (j = 0; j < 100; j++) {
    *((int *)(ptr + j)) = rand();    
  }
  for (i = 0; i < PTRS_COUNT; i++) {
    ptrs[i] = mem_cpy(ptr);
  }
  for(i = 0; i < PTRS_COUNT; i++) {
    mem_free(ptrs[i]);
  }

  mem_free(ptr);

  ptr = mem_alloc(sizeof(int));
  *(int *)ptr = 1;
  for (i = 0; i < 500; i++) {
    if (i == 0) {
      ptrs[i] = mem_dup(ptr);
    } else {
      ptrs[i] = mem_dup(ptrs[i - 1]);
    }
  }

  for (i = 0; i < 500; i++) {
    mem_free(ptrs[i]);
  }
  mem_free(ptr);

  return 0;
}
