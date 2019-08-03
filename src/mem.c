#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MEM_REFCOUNT_MAX 127
#define MEM_HEADER_LENGTH (sizeof(int8_t) + sizeof(size_t))
#define MEM_PTR(ptr) (void *)((ptr) + MEM_HEADER_LENGTH)
#define MEM_ORIGIN(ptr) (void *)((ptr) - MEM_HEADER_LENGTH)
#define MEM_REFCOUNT(ptr) (int8_t *)((ptr) - sizeof(int8_t) - sizeof(size_t))
#define MEM_LENGTH(ptr) (size_t *)((ptr) - sizeof(size_t))

void * _mem_alloc (const size_t len) {
  if (len <= 0) { return NULL; }
  void * ptr = malloc(len + MEM_HEADER_LENGTH);
  if (ptr == NULL) { return NULL; }
  
  *MEM_REFCOUNT(MEM_PTR(ptr)) = 1;
  *MEM_LENGTH(MEM_PTR(ptr)) = len;
  
  return MEM_PTR(ptr);
}

void *mem_alloc(const size_t len) {
  void *ptr = _mem_alloc(len);
  if (ptr == NULL) { return NULL; }
  memset(ptr, 0, len);
  return ptr;
}

void mem_free (const void * ptr) {
  if (ptr == NULL) { return; }
  *MEM_REFCOUNT(ptr) -= 1;
  if (*MEM_REFCOUNT(ptr) == 0) {
    free(MEM_ORIGIN(ptr));
  }
}

void * mem_cpy (const void * ptr) {
  void * new_ptr = NULL;
  if (*MEM_REFCOUNT(ptr) < MEM_REFCOUNT_MAX) {
    *MEM_REFCOUNT(ptr) += 1;
    new_ptr = (void *)ptr;
  } else {
    new_ptr = mem_dup(ptr);
  }
  return new_ptr;
}

void * mem_dup (const void * ptr) {
  if (ptr == NULL) { return NULL; }
  void * new_ptr = _mem_alloc(*MEM_LENGTH(ptr));
  if (new_ptr) {
    memcpy(new_ptr, ptr, *MEM_LENGTH(ptr));
  }
  return new_ptr;
}

void * mem_resize (const void * ptr, const size_t len) {
  void * new_ptr = NULL;
  if (ptr == NULL && len < 0) { return NULL; }
  if (ptr == NULL && len > 0) { return mem_alloc(len); }
  if (ptr != NULL && len == 0) { mem_free(ptr); return NULL; }
  
  new_ptr = mem_alloc(len);
  if (*MEM_LENGTH(new_ptr) <= *MEM_LENGTH(ptr)) {
    memcpy(new_ptr, ptr, *MEM_LENGTH(new_ptr));
  } else {
    memcpy(new_ptr, ptr, *MEM_LENGTH(ptr));
  }
  mem_free(ptr);
  
  return new_ptr;
}
