#ifndef DEBUG_H__
#define DEBUG_H__

#ifdef NDEBUG
#define DEBUG(fmt, ...) fprintf(stderr, "\tDEBUG %s:%d@%s -> \"" fmt \
			     "\"\n", __FILE__, __LINE__,  __func__, \
			     __VA_ARGS__)
#else
#define DEBUG(fmt, ...) do { if (0) fprintf(stderr, \
					 "DEBUG %s:%d@%s -> \"" \
					 fmt "\"\n", __FILE__, \
					 __LINE__,  __func__,	\
					 __VA_ARGS__); } while(0)
#endif /* NDEBUG */
#endif /* DEBUG_H__ */
