#include "rand.h"

#ifdef PROC_TIMES

static unsigned long next = 1;

unsigned int rand()
{
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % RAND_MAX);
}

void srand(unsigned int seed)
{
    next = seed;
}

#endif // PROC_TIMES