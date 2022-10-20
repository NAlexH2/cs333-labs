#include "user.h"
#include "types.h"
#include "proc.h"

int main (int argc, char * argv[])
{
    // #ifdef LOTTERY
    if (argc < 3) {
        printf(1, "\nERROR: Too few arguments to continue."
        "\nExpected a pid and new nice value to assign.\n");
        exit();
    }
    if(atoi(atoi(argv[1]) < MIN_NICE_VALUE || argv[1]) > MAX_NICE_VALUE)
    {
        printf(1, "\nERROR: renice value must be between %d and %d\n", 
        MIN_NICE_VALUE, MAX_NICE_VALUE);
        exit();
    }

    for(int i = 2; i < argc; ++i){
        renice(atoi(argv[1]), atoi(argv[i]));
    }

    // #endif // LOTTERY

    exit();
}