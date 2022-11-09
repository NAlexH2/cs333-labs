// Alex Harris - CS 333 Winter 2022

# include <math.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <stdio.h>
# include <stddef.h>
# include <unistd.h>
# include <sys/time.h>
# include <math.h>
# include <string.h>
# include <getopt.h>
# include <pthread.h>

typedef struct BitBlock_s {
    uint32_t bits;
    pthread_mutex_t mutex;
} BitBlock_t;

