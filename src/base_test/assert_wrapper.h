#include <assert.h>
#ifdef NDEBUG
#undef assert
#define assert(expr) if(!(expr)) {fprintf(stderr, "%s %s %d\n", #expr, __FILE__, __LINE__);};
#endif
