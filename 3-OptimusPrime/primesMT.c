// Alex Harris - CS 333 Winter 2022
#include "primesMT.h"

static int is_verbose = 0;
static unsigned int user_bounds = 10240;
static unsigned int user_threads = 1;
static unsigned int bit_arr_size = 0;

// User defined bounds. Defaults to 10240 if user doesn't enter a number.
#define UBOUNDS user_bounds
// User defined # of threads. Defaults to 1 if not specified by the user.
#define UTHREADS user_threads
#define ARRAYSIZE bit_arr_size
#define VERBOSE is_verbose
#define FILE_NAME "test.out"

void helpMe(void);

int main(int argc, char *argv[])
{
    // pthread_t *threads = NULL;
    // long tid = 0;
    FILE *op = NULL;
    BitBlock_t *BitArray;
    {
        int opt = -1;
        while ((opt = getopt(argc, argv, "t:u:vh")) != -1)
        {
            switch (opt)
            {
            case 't':
                UTHREADS = atoi(optarg);
                break;
            // bounds
            case 'u':
                UBOUNDS = atoi(optarg);
                break;
            case 'v':
                VERBOSE = 1;
                break;
            case 'h':
                helpMe();
                exit(EXIT_SUCCESS);
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }
    }
    ARRAYSIZE = (UBOUNDS / 32) + 1;
    BitArray = malloc(ARRAYSIZE * sizeof(BitBlock_t));

    op = fopen(FILE_NAME, "w");
    fprintf(op, "test");
    fclose(op);
    printf("\nuser_bounds = %u\nuser_threads = %u\nverbose = %d\n",
           UBOUNDS, UTHREADS, VERBOSE);

    printf("\nBitArray slots = %d\t BitArray[0] Size = %lu\n",
          ARRAYSIZE, sizeof(*BitArray));

    for(int i = 0; i < ARRAYSIZE; ++i){
        BitArray[i].bits = UINT32_MAX;
        printf("\n%u\n",BitArray[i].bits);
    }
    // uint32_t i = 1;
    // while(i != 0){
    //     if(BitArray[0].bits & i)
    //         printf("\nSuccess!, i = %u\n", i);
    //     else
    //         printf("\nWOMP WOMP :( \n");
    //     i = i << 1;
    // }
    exit(EXIT_SUCCESS);
}

void helpMe(void)
{
    printf("\n\n\nONLY HELP WAS CALLED\n\n\n");
    return;
}