#include "mem.h"

int main(int argc, char ** argv) {
  void * ptr = mem_alloc(10);
  void * ptr2 = mem_alloc(10);
  void * ptr3 = mem_alloc(10);
  void * ptr4 = NULL;
  void * ptr5 = NULL;


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
  
  return 0;
}
