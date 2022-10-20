#include "user.h"
#include "types.h"

int main (int argc, char * argv[])
{
    // #ifdef LOTTERY
    if (argc < 3) {
        printf(1, "\nERROR: Too few arguments to continue."
        "\nExpected a pid and new nice value to assign.\n");
        exit();
    }
    if(argv[1] == 0)
    {
        printf(1, "\nERROR: renice value cannot be 0\n");
        exit();
    }

    for(int i = 2; i < argc; ++i){
        renice(atoi(argv[1]), atoi(argv[i]));
    }

    // #endif // LOTTERY

    exit();
}