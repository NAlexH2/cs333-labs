// Alex Harris - CS 333 Winter 2022
#include "primesMT.h"

static int is_verbose = 0;
static unsigned int user_bounds = 10240;
static unsigned int user_threads = 1;
static unsigned int bit_arr_size = 0;
static unsigned int max_calc = 0;
BitBlock_t *BitArray = NULL;

// Prototypes
void *compositor(void*);
void printPrimes(void);


int main(int argc, char *argv[])
{
    pthread_t *threads = NULL;

    {
        int opt = -1;
        while ((opt = getopt(argc, argv, "t:u:vh")) != -1)
        {
            switch (opt)
            {
            case 't':
                user_threads = atoi(optarg);
                break;
            case 'u':
                user_bounds = atoi(optarg);
                break;
            case 'v':
                is_verbose = 1;
                break;
            case 'h':
                printf("\n\t\tHelp:\n-h\tDisplay this information\n"
                "-t#\tnumber of threads to use (default: 1)\n"
                "-u#\tThe highest value to calculate primes to (default: 10240)\n"
                "-v\tVerbose mode."
                " Display additional information as program runs\n\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }
    }
    if(is_verbose)
        fprintf(stderr, "\n**Verbose on**\n");
    max_calc = ceil(sqrt(user_bounds) + 1);
    bit_arr_size = (user_bounds / 32) + 1;
    BitArray = malloc(bit_arr_size * sizeof(BitBlock_t));
    threads = malloc(user_threads * sizeof(pthread_t));

    for(int i = 0; i < bit_arr_size; ++i){
        pthread_mutex_init(&BitArray[i].mutex, NULL);
        BitArray[i].bits = ~0;
    }

    for(long i = 0, can = 3; i < user_threads; ++i, can += 2){
        pthread_create(&threads[i], NULL, compositor, (void *) can);
    }

    for (long tid = 0; tid < user_threads; ++tid) {
        pthread_join(threads[tid], NULL);
    }

    printPrimes();

    if(is_verbose) {
        fprintf(stderr, "\nuser_bounds = %u\nuser_threads = %u\nis_verbose = %d\n",
                user_bounds, user_threads, is_verbose);
        fprintf(stderr, "\nBitArray slots = %d\t BitArray[0] Size = %lu\n",
                bit_arr_size, sizeof(*BitArray));
        fprintf(stderr, "\nBitArray[0].bits = %u\t max_calc = %u\n\n",
                BitArray[0].bits, max_calc);
    }
    // de alloc
    free(threads);
    free(BitArray);
    exit(EXIT_SUCCESS);
}

// all composits are being flipped to 0
void *compositor(void * can)
{
    long sp = (long) can;
    for (long i = sp; i <= max_calc; i += (2 * user_threads))
    {
        if (is_verbose) {
            fprintf(stderr, "\n\nmax_calc: %d\tsp: %li\ti: %ld", max_calc, sp, i);
        }
        for (long j = i + i; j <= user_bounds; j += i)
        {
            uint32_t bb_index = j / 32;
            uint32_t bb_bit = j % 32;
            uint32_t mask = 0x1 << bb_bit;
            mask = ~mask;
            pthread_mutex_lock(&BitArray[bb_index].mutex);
            BitArray[bb_index].bits &= mask;
            pthread_mutex_unlock(&BitArray[bb_index].mutex);
            if(is_verbose) {
                fprintf(stderr, "\n\tbb_index: %u\tbb_bit: %u\tthread pos: %lu\tj: %ld", 
                bb_index, bb_bit, i, j);
            }
        }
    }
    pthread_exit(EXIT_SUCCESS);
}

void printPrimes(void)
{
    printf("2\n");
    for(int i = 3; i < user_bounds; i += 2)
    {
        int bb_index = i / 32;
        int bb_bit = i % 32;
        int actual = 0;
        uint32_t mask = 0x1 << bb_bit;
        if(bb_index > 0)
            actual = (bb_index * 32) + bb_bit;
        else
            actual = bb_bit;
        if ((BitArray[bb_index].bits & mask) != 0)
            printf("%d\n", actual);
    }

    return;
}