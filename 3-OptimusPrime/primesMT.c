// Alex Harris - CS 333 Winter 2022
# include "primesMT.h"


static int isVerbose = 0;
static unsigned int user_bounds = 10240;
static unsigned int user_threads = 1;


// User defined bounds. Defaults to 10240 if user doesn't enter a number.
# define UBOUNDS user_bounds
// User defined # of threads. Defaults to 1 if not specified by the user.
# define UTHREADS user_threads
# define VERBOSE isVerbose
# define FILE_NAME "test.out"

void helpMe(void);

int main(int argc, char *argv[])
{
    // pthread_t *threads = NULL;
    // long tid = 0;
    FILE *op = NULL;
    {
        int opt = -1;
        while ((opt = getopt(argc, argv, "t:u:vh")) != -1)
        {
            switch (opt)
            {
            case 't':
                UTHREADS = atoi(optarg);
                break;
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
    printf("\nhello world!\n");
    op = fopen(FILE_NAME, "w");
    fprintf(op, "test");
    fclose(op);
    perror("\n\nThis is stderror\n\n");
    printf("\nuser_bounds = %u\nuser_threads = %u\nverbose = %d\n", 
            UBOUNDS, UTHREADS, VERBOSE);
    exit(EXIT_SUCCESS);
}

void helpMe(void) {
    printf("\n\n\nONLY HELP WAS CALLED\n\n\n");
    return;
}