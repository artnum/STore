#include "mem.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/**
 * A pointer can have up to 200 refcounted copy of itself. A room is
 * kept for duplicate (which increase refcount). Doing this, a pointer
 * can have 200 copies + 55 duplicates and each duplicate can have
 * 200 copies + 55 duplicates and so on.
 */
#define MEM_REFCOUNT_MAX 255
#define MEM_COPY_MAX 200

#define MEM_HEADER_LENGTH (sizeof(uint8_t) + sizeof(size_t) + \
			   sizeof(void *))
#define MEM_PTR(ptr) (void *)((ptr) + MEM_HEADER_LENGTH)
#define MEM_ORIGIN(ptr) (void *)((ptr) - MEM_HEADER_LENGTH)
#define MEM_REFCOUNT(ptr) (uint8_t *)((ptr) - sizeof(uint8_t) - \
				     sizeof(size_t) - sizeof(void *))
#define MEM_LENGTH(ptr) (size_t *)((ptr) - sizeof(size_t) - \
				   sizeof(void *))
#define MEM_PARENT(ptr) (void **)((ptr) - sizeof(void *))

void * _mem_alloc (const size_t len) {
  if (len <= 0) { return NULL; }
  void * ptr = malloc(len + MEM_HEADER_LENGTH);
  if (ptr == NULL) { return NULL; }
  
  *MEM_REFCOUNT(MEM_PTR(ptr)) = 1;
  *MEM_LENGTH(MEM_PTR(ptr)) = len;
  *MEM_PARENT(MEM_PTR(ptr)) = NULL;

  DEBUG("New allocation of size %ld with overhead of %ld (%f%%)", len, MEM_HEADER_LENGTH, (float)100 * MEM_HEADER_LENGTH / len);
  return MEM_PTR(ptr);
}

void *mem_alloc(const size_t len) {
  void *ptr = _mem_alloc(len);
  if (ptr == NULL) { return NULL; }
  memset(ptr, 0, len);
  return ptr;
}

void mem_free (void * ptr) {
  DEBUG("Free of %p", ptr);
  if (ptr == NULL) { return; }
  *MEM_REFCOUNT(ptr) -= 1;
  if (*MEM_REFCOUNT(ptr) == 0) {
    if (*MEM_PARENT(ptr) != NULL) {
      mem_free(*MEM_PARENT(ptr));
    }
    /* reset to zero memory before freeing */
    memset(ptr, 0, *MEM_LENGTH(ptr));
    DEBUG("Effective free of %p", ptr);
    free(MEM_ORIGIN(ptr));
  }
}

/* mem_cpy doesn't increment *MEM_REFCOUNT(*MEM_PARENT(ptr)) has copy
   is not another reference of parent but another reference of child
*/
void * mem_cpy (void * ptr) {
  void * new_ptr = NULL;
  if (*MEM_REFCOUNT(ptr) < MEM_COPY_MAX) {
    *MEM_REFCOUNT(ptr) += 1;
    new_ptr = ptr;
  } else {
    DEBUG("Maximum copy reached for %p", ptr);
    new_ptr = mem_dup(ptr);
  }
  return new_ptr;
}

void * _mem_dup (void * ptr, int pRefCountInc) {
  if (ptr == NULL) { return NULL; }
  void * new_ptr = _mem_alloc(*MEM_LENGTH(ptr));
  if (new_ptr) {
    memcpy(new_ptr, ptr, *MEM_LENGTH(ptr));
  }
  if (pRefCountInc && *MEM_REFCOUNT(ptr) < MEM_REFCOUNT_MAX) {
    *MEM_PARENT(new_ptr) = ptr;
    *MEM_REFCOUNT(ptr) += 1;
  }
  return new_ptr;
}

void * mem_dup (void * ptr) {
  return _mem_dup(ptr, 1);
}

void * mem_resize (void * ptr, const size_t len) {
  void * new_ptr = NULL;
  if (ptr == NULL && len < 0) { return NULL; }
  if (ptr == NULL && len > 0) { return mem_alloc(len); }
  if (ptr != NULL && len == 0) { mem_free(ptr); return NULL; }
  
  new_ptr = mem_alloc(len);
  if (*MEM_PARENT(ptr) != NULL) {
    *MEM_PARENT(new_ptr) = *MEM_PARENT(ptr);
    /* increase reference count of parent because ptr and new_ptr are
       distinct child of *MEM_PARENT(ptr). */
    *MEM_REFCOUNT(*MEM_PARENT(ptr)) += 1;
  }
  if (*MEM_LENGTH(new_ptr) <= *MEM_LENGTH(ptr)) {
    memcpy(new_ptr, ptr, *MEM_LENGTH(new_ptr));
  } else {
    memcpy(new_ptr, ptr, *MEM_LENGTH(ptr));
  }
  mem_free(ptr);
  
  return new_ptr;
}

void * mem_mine (void * ptr) {
  if (ptr == NULL) { return NULL; }
  
  if (*MEM_REFCOUNT(ptr) > 1) {
    *MEM_REFCOUNT(ptr) -= 1;
    return mem_dup(ptr);
  } else {
    return (void *)ptr;
  }
}

void * mem_reset (void * ptr) {
  void * parent = NULL;
  if (ptr == NULL) { return NULL; }

  if (*MEM_PARENT(ptr) != NULL) {
    parent = *MEM_PARENT(ptr);
    mem_free(ptr);
    if (*MEM_REFCOUNT(parent) + 1 < MEM_REFCOUNT_MAX) {
      *MEM_REFCOUNT(parent) += 1;
    } else {
      parent = _mem_dup(parent, 0);
    }
  } else {
    parent = ptr;
  }
  return parent;
}
