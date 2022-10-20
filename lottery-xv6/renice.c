#include "types.h"
#include "user.h"

int main (int argc, char * argv[])
{
    #ifdef LOTTERY
    if (argc <= 3) {
        printf(1, "\nERROR: Too few arguments to continue."
        "\nExpected a pid and new nice value to assign.\n");
        exit();
    }
    int nice = atoi(argv[1]);

    for(int i = 2; i < argc; ++i){
        renice(nice, atoi(argv[i]));
    }

    #endif // LOTTERY

    exit();
}