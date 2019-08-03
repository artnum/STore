#ifndef MEM_H__
#define MEM_H__

#include <stdint.h>
#include <string.h>

/**
 * \fn void * mem_alloc(const size_t len)
 * \brief Drop-in replacement of malloc with reference counting and memory zeroing
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
void mem_free (const void * ptr);

/**
 * \fn void * mem_cpy (void * ptr)
 * \brief Copy memory. This is a zero-copy operation as long as reference counting is below 
 *        its maximum, in that case, it behave like mem_dup()
 *
 * \param[in] ptr Memory to copy
 * \return A pointer to the copied memory
 */
void * mem_cpy (const void * ptr);

/**
 * \fn void * mem_dup (void * ptr)
 * \brief Duplicate memory : a new space is allocated, of the same size, and data are copied
 *
 * \param[in] ptr Memory to duplicate
 * \return A pointer to the duplicated memory
 */
void * mem_dup (const void * ptr);

/**
 * \fn void * mem_resize (const void * ptr, const size_t len)
 * \brief Resize memory, a pointer is resized to a given size. The original pointer is freed.
 *
 * \param[in] ptr Memory to resize
 * \param[in] len New size
 * \return A new pointer to a resized memory location
 */
void * mem_resize (const void * ptr, const size_t len);
#endif /* MEM_H__ */
