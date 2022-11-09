// Alex Harris - CS 333 Winter 2022
#include "primesMT.h"

static int is_verbose = 0;
static unsigned int user_bounds = 10240;
static unsigned int user_threads = 1;
static unsigned int bit_arr_size = 0;
static unsigned int max_calc = 0;
BitBlock_t *BitArray = NULL;

// Prototypes
void helpMe(void);
void *compositor(void*);
void printPrimes(void);


int main(int argc, char *argv[])
{
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
    if(is_verbose)
        perror("\n**Verbose on**\n");
    max_calc = ceil(sqrt(user_bounds) + 1);
    bit_arr_size = (user_bounds / 32) + 1;
    BitArray = malloc(bit_arr_size * sizeof(BitBlock_t));
    threads = malloc(user_threads * sizeof(pthread_t));
    for(int i = 0; i < bit_arr_size; ++i){
        pthread_mutex_init(&BitArray[i].mutex, NULL);
        BitArray[i].bits = ~0;
    }
    // for(int i = 3; i < max_calc; i += 2){
    //     compositor(i);
    // }
    for(long i = 0, tid = 3; i < user_threads; ++i, tid += 2){
        pthread_create(&threads[i], NULL, compositor, (void *) tid);
    }

    for (tid = 0; tid < user_threads; ++tid) {
        pthread_join(threads[tid], NULL);
    }

    printPrimes();

    if(is_verbose) {
        fprintf(stderr, "\nuser_bounds = %u\nuser_threads = %u\nis_verbose = %d\n",
                user_bounds, user_threads, is_verbose);
        fprintf(stderr, "\nBitArray slots = %d\t BitArray[0] Size = %lu\n",
                bit_arr_size, sizeof(*BitArray));
        fprintf(stderr, "\nBitArray[0].bits = %u\t max_calc = %u\n",
                BitArray[0].bits, max_calc);
    }
    // de alloc
    exit(EXIT_SUCCESS);
}

void helpMe(void)
{
    printf("\n\n\nONLY HELP WAS CALLED\n\n\n");
    return;
}

// all composits are being flipped to 0
void *compositor(void * can)
{
    /* 
    * void * thread_func(void * arg) {
        long start_paws = (long) arg;

        for(long candy_date = start_paws; i < max_calc; candy_date += (2*user_threads)){
            for(long j = candy_date + candy_date; j <= user_bounds; j += candy_date)
        }
    }
    */
    long sp = (long) can;
    for (long i = sp; i < max_calc; i += (2 * user_threads))
    {
        for (long j = sp + sp; i <= user_bounds; i += sp)
        {
            int bb_index = j / 32;
            int bb_bit = j % 32;
            uint32_t mask = 0x1 << bb_bit;
            mask = ~mask;
            pthread_mutex_lock(&BitArray[bb_index].mutex);
            BitArray[bb_index].bits &= mask;
            pthread_mutex_unlock(&BitArray[bb_index].mutex);
        }
    }
    pthread_exit(EXIT_SUCCESS);
}

void printPrimes(void)
{
    printf("\n2\n");
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