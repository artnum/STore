#ifndef MTRACK_H__
#define MTRACK_H__

/**
 * \file mtrack.h
 * \brief Memory tracker
 *
 * This tracker is to track memory allocation inside a module or a
 * function where you do a lot of allocation. You add up pointer and,
 * when done, you free them at once.
 * Not designed to search a particular pointer and free that pointer
 * even if there is a function for it.
 */

#include <stdint.h>

#define MTRACKER_SIZE 32

typedef struct _s_MTracker MTracker;
struct _s_MTracker {
  /* double size, we store the pointer and the function pointer to
     free it
  */
  void * table[MTRACKER_SIZE * 2];
  MTracker * next;
  MTracker * nextFree; /*< only apply to root node */
  char ownAlloc;
};

/**
 * \fn MTracker * mk_alloc(void)
 * \brief Allocate a memory tracker
 *
 * \return A new memory tracker
 */
MTracker * mk_alloc(void);

/**
 * \fn void mk_init(MTracker * root)
 * \brief Init an already allocated memory tracker. Init always assume
 * it's a root node.
 *
 * \param[in] root A pointer to a root tracker
 * \retun void
 */
void mk_init(MTracker * root);

/**
 * \fn int mk_track(MTracker * root, void * ptr,
 * void (*ptr_free)(void * ptr))
 * \brief Set a pointer to be tracked.
 *
 * \param[in] root Tracker in which you want to add you pointer
 * \param[in] ptr The pointer you want to track
 * \param[in] ptr_free The free function you want to use to free your
 * tracked pointer. If NULL, the standard free is
 * used
 */
int mk_track(MTracker * root, void * ptr, void (*ptr_free)
	     (void * ptr));

/**
 * \fn int mk_free(MTracker * root, void * ptr)
 * \brief Free a specific pointer if found
 * 
 * \param[in] root Tracker in which to search pointer
 * \param[in] ptr Pointer to free.
 * \return 1 if success, 0 if fail.
 * \warn Pointer are not ordered, so searching takes time, avoid 
 * using this function
 */
int mk_free(MTracker * root, void * ptr);

/**
 * \fn void mk_free_all(MTracker * root)
 * \brief Free all pointers but keeps tracker structure
 *
 * \param[in] root Tracker to free content
 * \return void
 */
void mk_free_all(MTracker * root);

/**
 * \fn void mk_destroy(MTracker * root)
 * \brief Free every pointer and all self allocated tracker
 *
 * \param[in] root Tracker to free
 * \return void
 */
void mk_destroy(MTracker * root);
#endif /* MTRACK_H__ */
