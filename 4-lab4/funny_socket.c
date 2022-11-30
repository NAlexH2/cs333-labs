#include <fcntl.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char funny_name[] = "FUNNYsocket";
    char *sname = NULL;

    if (argc < 2) {
        sname = funny_name;
    }
    else {
        sname = argv[1];
    }
    {
        struct sockaddr_un namesock;
        int fd;

        namesock.sun_family = AF_UNIX;
        strncpy(namesock.sun_path, (char *)sname, sizeof(namesock.sun_path));
        fd = socket(AF_UNIX, SOCK_DGRAM, 0);
        bind(fd, (struct sockaddr *) &namesock, sizeof(struct sockaddr_un));

        close(fd);
    }

    return EXIT_SUCCESS;
}
