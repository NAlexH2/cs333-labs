// R. Jesse Chaney
// rchaney@pdx.edu

#include "vikalloc.h"

// macro for mem_block_t that may change overtime
#define BLOCK_SIZE (sizeof(mem_block_t))
// macro for where the data starts after mem_block_t
#define BLOCK_DATA(__curr) (((void *)__curr) + (BLOCK_SIZE))
// returns the "current" ptr to the user for a specific block of data
#define DATA_BLOCK(__data) ((mem_block_t *)(__data - BLOCK_SIZE))

#define IS_FREE(__curr) ((__curr->size) == 0)

#define PTR "0x%07lx"
#define PTR_T PTR "\t"

static mem_block_t *block_list_head = NULL;
static mem_block_t *block_list_tail = NULL;
static void *low_water_mark = NULL;
static void *high_water_mark = NULL;
// only used in next-fit algorithm
static mem_block_t *prev_fit = NULL;

static uint8_t isVerbose = FALSE;
static vikalloc_fit_algorithm_t fit_algorithm = FIRST_FIT;
static FILE *vikalloc_log_stream = NULL;

static void init_streams(void) __attribute__((constructor));

// My helpers
static void vikallocLowWaterMarkIsNull(size_t size, mem_block_t *curr);
static void vikallocAgain(size_t size, mem_block_t *curr);

static size_t min_sbrk_size = MIN_SBRK_SIZE;

static void
init_streams(void)
{
    vikalloc_log_stream = stderr;
}

size_t
vikalloc_set_min(size_t size)
{
    if (0 == size)
    {
        // just return the current value
        return min_sbrk_size;
    }
    if (size < (BLOCK_SIZE + BLOCK_SIZE))
    {
        // In the event that it is set to something silly small.
        size = MAX(BLOCK_SIZE + BLOCK_SIZE, SILLY_SBRK_SIZE);
    }
    min_sbrk_size = size;

    return min_sbrk_size;
}

void vikalloc_set_algorithm(vikalloc_fit_algorithm_t algorithm)
{
    fit_algorithm = algorithm;
    if (isVerbose)
    {
        switch (algorithm)
        {
        case FIRST_FIT:
            fprintf(vikalloc_log_stream, "** First fit selected\n");
            break;
        case BEST_FIT:
            fprintf(vikalloc_log_stream, "** Best fit selected\n");
            break;
        case WORST_FIT:
            fprintf(vikalloc_log_stream, "** Worst fit selected\n");
            break;
        case NEXT_FIT:
            fprintf(vikalloc_log_stream, "** Next fit selected\n");
            break;
        default:
            fprintf(vikalloc_log_stream, "** Algorithm not recognized %d\n", algorithm);
            fit_algorithm = FIRST_FIT;
            break;
        }
    }
}

void vikalloc_set_verbose(uint8_t verbosity)
{
    isVerbose = verbosity;
    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, "Verbose enabled\n");
    }
}

void vikalloc_set_log(FILE *stream)
{
    vikalloc_log_stream = stream;
}

void *
vikalloc(size_t size)
{
    mem_block_t *curr = NULL;
    int mul = ((size + BLOCK_SIZE) / min_sbrk_size) + 1;

    if (size == 0)
        return NULL;

    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, ">> %d: %s entry: size = %lu\n", __LINE__, __FUNCTION__, size);
    }

    if (low_water_mark == NULL)
    {
        low_water_mark = sbrk(0);
        if (size + BLOCK_SIZE < min_sbrk_size) // if size fits neatly in 2048 bytes
        {
            // allocate in multiples of 2048 + bs
            curr = sbrk(min_sbrk_size);
            curr->capacity = min_sbrk_size - BLOCK_SIZE;

            // Cut down on duplicate code with this one easy trick!
            vikallocLowWaterMarkIsNull(size, curr);
        }
        // otherwise, figure out the closest multiple of 2048 to fit size
        else
        {
            curr = sbrk((mul * min_sbrk_size));
            curr->capacity = (mul * min_sbrk_size) - BLOCK_SIZE;

            // Cut down on duplicate code with this one easy trick!
            vikallocLowWaterMarkIsNull(size, curr);
        }
    }
    /* ...otherwise...
     * see if mem fits in current block if so decrease size
     * if not make new block using curr, move tail, adjust high_water_mark?
     */
    else
    {
        curr = block_list_head;
        // while(curr != NULL && size > curr->capacity - curr->size)
        //     curr = curr->next;
        while (curr != NULL)
        {
            if (curr != NULL && curr->size == 0 && size < curr->capacity)
            {
                curr->size = size;
                return BLOCK_DATA(curr);
            }
            // split
            else if (curr != NULL && curr->size + size + BLOCK_SIZE < curr->capacity)
            {
                // math to move split pointer to the end of the bytes in curr size/capacity
                mem_block_t *split = BLOCK_DATA(curr) + curr->size;
                if (curr->next != NULL)
                {
                    split->next = curr->next;
                    curr->next->prev = split;
                }
                else
                {
                    block_list_tail = split;
                    split->next = NULL;
                }
                curr->next = split;
                split->prev = curr;
                split->size = size;
                split->capacity = curr->capacity - BLOCK_SIZE - curr->size;
                curr->capacity = curr->size;

                return BLOCK_DATA(split);
            }
            curr = curr->next;
        }
        // if it fits neatly into a block on its own

        if (size + BLOCK_SIZE < min_sbrk_size)
        {
            curr = sbrk(min_sbrk_size);
            curr->capacity = min_sbrk_size - BLOCK_SIZE;
            vikallocAgain(size, curr);
        }
        else
        {
            curr = sbrk((mul * min_sbrk_size));
            curr->capacity = (mul * min_sbrk_size) - BLOCK_SIZE;
            vikallocAgain(size, curr);
        }
    }

    return BLOCK_DATA(curr);
}

// helpers to cutdown repeat code in vikalloc
// Does the fun work of assigning head, tail, and high water mark
void vikallocLowWaterMarkIsNull(size_t size, mem_block_t *curr)
{
    block_list_head = curr;

    block_list_head->size = size; // set used memory

    // hwm should be the end of current avail block/only node
    high_water_mark = (void *)block_list_head + (block_list_head->capacity + BLOCK_SIZE);
    block_list_head->next = block_list_head->prev = NULL;
    block_list_tail = block_list_head;
}

// When low_water_mark is not null
void vikallocAgain(size_t size, mem_block_t *curr)
{
    curr->size = size;
    curr->next = NULL;
    curr->prev = block_list_tail;
    block_list_tail->next = curr;
    block_list_tail = curr;
    high_water_mark = (void *)block_list_tail + (block_list_tail->capacity + BLOCK_SIZE);
}

void vikfree(void *ptr)
{
    mem_block_t *curr = DATA_BLOCK(ptr);
    if (ptr == NULL)
        return;
    if (isVerbose)
    {
        // fprintf(vikalloc_log_stream, ">> %d: %s entry\n", __LINE__, __FUNCTION__);
        if (IS_FREE(curr))
        {
            fprintf(vikalloc_log_stream, "Block is already free: ptr = " PTR "\n", (long)(ptr - low_water_mark));
            return;
        }
    }

    curr->size = 0;
    if (curr->next && curr->next->size == 0)
    {
        curr->capacity += curr->next->capacity + BLOCK_SIZE;
        if (curr->next->next)
        {
            curr->next = curr->next->next;
            curr->next->prev = curr;
        }
        else
            curr->next = NULL;
    }
    if (curr->prev && curr->prev->size == 0)
    {
        curr = (void *)curr->prev + BLOCK_SIZE;
        vikfree(curr);
    }
    return;
}

///////////////

void vikalloc_reset(void)
{
    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, ">> %d: %s entry\n", __LINE__, __FUNCTION__);
    }

    if (low_water_mark != NULL)
    {
        if (isVerbose)
        {
            fprintf(vikalloc_log_stream, "*** Resetting all vikalloc space ***\n");
        }
        brk(low_water_mark);
        block_list_head = block_list_tail = low_water_mark = high_water_mark = NULL;
    }
}

void *
vikcalloc(size_t nmemb, size_t size)
{
    void *ptr = vikalloc(size * nmemb);

    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, ">> %d: %s entry\n", __LINE__, __FUNCTION__);
    }

    return ptr;
}

// This is like the regular realloc() call. See the man page for details.
// Pass in a pointer to already allocated memory and a new size you wish
// were allocated. If the new size exceeds the capacity of the existing
// block, a new block will be allocated, the old contents will be copied
// into the new block, and the old block deallocated.
// If you pass NULL as the pointer to old memory, this will behave the
// same as vikalloc.
void *
vikrealloc(void *ptr, size_t size)
{
    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, ">> %d: %s entry\n", __LINE__, __FUNCTION__);
    }

    if (ptr != NULL)
    {
        mem_block_t *curr = DATA_BLOCK(ptr);
        if (size > curr->capacity)
        {
            void *ptr2 = NULL;
            ptr2 = vikalloc(size);
            memcpy(ptr2, ptr, curr->size);
            vikfree(ptr);
            return ptr2;
        }
        curr->size = size;
        return ptr;
    }
    return (ptr = vikalloc(size));
}

void *
vikstrdup(const char *s)
{
    char *ptr = vikalloc(strlen(s) + 1);
    if (isVerbose)
    {
        fprintf(vikalloc_log_stream, ">> %d: %s entry\n", __LINE__, __FUNCTION__);
    }
    strcpy(ptr, s);

    return ptr;
}

#include "vikalloc_dump.c"
