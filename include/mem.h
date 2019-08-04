/**
 * \brief Reference counted memory 
 *
 * Doesn't remove the burden on allocating and freeing memory but add
 * some features like zero-copy, take care of size tracking, allow
 * a bit of "version management" as when you duplicate or copy, you
 * can still revert value at the current parent value.
 */
#ifndef MEM_H__
#define MEM_H__

#include <stdint.h>
#include <string.h>

/**
 * \fn void * mem_alloc(const size_t len)
 * \brief Drop-in replacement of malloc with reference counting and
 * memory zeroing
 *
 * \param[in] len Length of memory to be allocated
 * \return A pointer to a newly allocated memory space
 */
void *mem_alloc(const size_t len);

/**
 * \fn void mem_free(const void * ptr)
 * \brief Free reference counted memory
 *
 * \param[in] ptr A pointer to free
 */
void mem_free (void * ptr);

/**
 * \fn void * mem_cpy (void * ptr)
 * \brief Copy memory. This is a zero-copy operation as long as
 * reference counting is below its maximum, in that case, it behave
 * like mem_dup()
 *
 * \param[in] ptr Memory to copy
 * \return A pointer to the copied memory
 */
void * mem_cpy (void * ptr);

/**
 * \fn void * mem_dup (void * ptr)
 * \brief Duplicate memory : a new space is allocated, of the same
 * size, and data are copied
 *
 * \param[in] ptr Memory to duplicate
 * \return A pointer to the duplicated memory
 */
void * mem_dup (void * ptr);

/**
 * \fn void * mem_resize (const void * ptr, const size_t len)
 * \brief Resize memory, a pointer is resized to a given size. The
 * original pointer is freed.
 *
 * \param[in] ptr Memory to resize
 * \param[in] len New size
 * \return A new pointer to a resized memory location
 */
void * mem_resize (void * ptr, const size_t len);

/**
 * \fn void * mem_mine (const void * ptr)
 * \brief Make sure that this memory address is all mine. So if
 * refcount > 1, it returns a new copy,
 *
 * \param[in] ptr Memory to acquire
 * \return A pointer with a refcount of 1
 */
void * mem_mine (void * ptr);

/**
 * \fn void * mem_reset(void * ptr)
 * \brief If a pointer was duplicated from another pointer, it returns
 * its original and free the current. Else it returns ptr.
 *
 * \param[in] ptr Pointer to reset
 * \return Original pointer or ptr if there is no original
 *
 * It allows to make a copy, work on it (modifiying its content) and
 * then, when finish, just discard all modifications and get to
 * original value. The original value is parent pointer with
 * refcount++, so there is no copy, only a free.
 * ptr is not freed as long as there is references on it.
 */
void * mem_reset(void * ptr);
#endif /* MEM_H__ */
