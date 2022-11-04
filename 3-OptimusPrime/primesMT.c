// Alex Harris - CS 333 Winter 2022
# include "primesMT.h"


# define FILE_NAME "test.out"

int main(int argc, char *argv[])
{
    FILE *op = NULL;
    printf("\nhello world!\n");
    op = fopen(FILE_NAME, "w");
    fprintf(op, "test");
    fclose(op);
    exit(EXIT_SUCCESS);
}