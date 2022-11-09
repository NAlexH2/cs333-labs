// Alex Harris - CS 333 Winter 2022
#include "primesMT.h"

static int is_verbose = 0;
static unsigned int user_bounds = 10240;
static unsigned int user_threads = 1;
static unsigned int bit_arr_size = 0;
static unsigned int max_calc = 0;
BitBlock_t *BitArray = NULL;


// User defined bounds. Defaults to 10240 if user doesn't enter a number.
//# define UBOUNDS user_bounds
// User defined # of threads. Defaults to 1 if not specified by the user.
//# define UTHREADS user_threads
//# define ARRSIZE bit_arr_size
//# define VERBOSE is_verbose
//# define FILE_NAME "test.out"

// Prototypes
void helpMe(void);
void compositor(int);
void printPrimes();


int main(int argc, char *argv[])
{
    // pthread_t *threads = NULL;
    // long tid = 0;
    FILE *op = NULL;
    pthread_t *threads = NULL;
    long tid = 0;

    {
        int opt = -1;
        while ((opt = getopt(argc, argv, "t:u:vh")) != -1)
        {
            switch (opt)
            {
            case 't':
                user_threads = atoi(optarg);
                break;
            // bounds
            case 'u':
                user_bounds = atoi(optarg);
                break;
            case 'v':
                is_verbose = 1;
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
    max_calc = ceil(sqrt(user_bounds) + 1);
    bit_arr_size = (user_bounds / 32) + 1;
    BitArray = malloc(bit_arr_size * sizeof(BitBlock_t));
    threads = malloc(user_threads * sizeof(pthread_t));
    for(int i = 0; i < bit_arr_size; ++i){
        pthread_mutex_init(&BitArray[i].mutex, NULL);
        BitArray[i].bits = ~0;
    }
    // printf("\nBitArray[0].bits = %u\t max_calc = %u\n", 
    //         BitArray[0].bits, max_calc);
    //BitArray[0].bits = BitArray[0].bits ^ 1;
    //BitArray[0].bits = BitArray[0].bits ^ (1<<1);
    for(int i = 3; i < max_calc; i += 2){
        compositor(i);
    }


    printf("\nuser_bounds = %u\nuser_threads = %u\nis_verbose = %d\n",
            user_bounds, user_threads, is_verbose);
    printf("\nBitArray slots = %d\t BitArray[0] Size = %lu\n",
        bit_arr_size, sizeof(*BitArray));
    printf("\nBitArray[0].bits = %u\t max_calc = %u\n", 
            BitArray[0].bits, max_calc);
    printPrimes(BitArray);

    exit(EXIT_SUCCESS);
}

void helpMe(void)
{
    printf("\n\n\nONLY HELP WAS CALLED\n\n\n");
    return;
}

// all composits are being flipped to 0
void compositor(int can)
{
    for (int i = can + can; i <= user_bounds; i += can) {
        int bb_index = i / 32;
        int bb_bit = i % 32;
        unsigned int mask = 0x1 << bb_bit;
        mask = ~mask;
        BitArray[bb_index].bits &= mask;
    }
    return;
}

void printPrimes()
{
    printf("2,\t");
    for(int i = 3; i < user_bounds; i += 2)
    {
        int bb_index = i / 32;
        int bb_bit = i % 32;
        unsigned int mask = 0x1 << bb_bit;
        if ((BitArray[bb_index].bits & mask) != 0)
            printf("%d,\t", bb_bit);
    }

    return;
}