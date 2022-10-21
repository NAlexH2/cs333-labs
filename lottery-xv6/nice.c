#include "types.h"
#include "user.h"


int main (int argc, char * argv[]){
    #ifdef LOTTERY
    if (argc < 3) {
        printf(1, "\nERROR: Too few arguments to continue."
        "\nExpected a nice value and process to run.\n");
        exit();
    }
    int nice_pid = getpid();
    renice(atoi(argv[1]), nice_pid);
    exec(argv[2], (argv+2));
    #endif //LOTTERY
    exit();

}