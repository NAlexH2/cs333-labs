#include "rand.h"
#include "user.h"

int main(int argc, char *argv[])
{
#ifdef PROC_TIMES
    if(argc > 1)
        srand(atoi(argv[1]));
        
    for(int i = 0; i < 10; ++i)
        printf(1, "random number is: %u\n", rand());

#endif // PROC_TIMES
    exit();
}
