// compile with
//     gcc -Wall -o mystat mystat.c
// ./mystat mystat

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>  // for PATH_MAX
#include <assert.h>

static char *mode_letters(mode_t);
static char *show_local_time(time_t *);
static char *show_utc_time(time_t *t);

int
main(int argc, char *argv[])
{
    struct stat sb = {0};

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename1> ... <filenameN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        if(-1 == lstat(argv[i], &sb)) {
            perror("*** Could not stat file");
            continue;
        }
        fprintf(stdout, "File: %s\n", argv[i]);
        fprintf(stdout, " File type:            ");
        switch(sb.st_mode & S_IFMT) {
            case S_IFBLK:
                fprintf(stdout, "block device\n");
                break;
            case S_IFCHR:
                fprintf(stdout, "character device\n");
                break;
            case S_IFDIR:
                fprintf(stdout, "directory\n");
                break;
            case S_IFIFO:
                fprintf(stdout, "FIFO/pipe\n");
                break;
            case S_IFREG:
                fprintf(stdout, "regular file\n");
                break;
            case S_IFSOCK:
                fprintf(stdout, "socket\n");
                break;
            case S_IFLNK:
                {
                    struct stat lsb;
                    char buf[PATH_MAX] = {'\0'};
                    ssize_t buf_size = 0;

                    if (-1 == stat(argv[i], &lsb)) {
                        fprintf(stdout, "Symbolic link - "
                                "with dangling destination\n")
                    }
                    else {
                        memset(buf, 0, PATH_MAX);
                        buf_size = readlink(argv[i], buf, sizeof(buf));
                        assert(buf_size);
                        fprintf(stdout, "Symbolic link -> %s\n", buf);
                    }
                    break;
                }
            default:
                fprintf(stdout, "unkown?\n");
                break;
        }
        
        fprintf(stdout, "   I-node number:              %zd\n", sb.st_ino);
        {
            char oct[10];

            sprintf(oct, "%06o", sb.st_mode);
            fprintf(stdout, "   Mode:           %s      (0%s in octal)\n"
                        , mode_letters(sb.st_mode), &oct[3]);
        }
        fprintf(stdout, "   Link count:         %zd\n", sb.st_nlink);

        {
            struct passwd *uName;
            struct group *gName;

            uName = getpwuid(sb.st_uid);
            gName = getgrgid(sb.st_gid);
            fprintf(stdout, "   Ownder Id:      %-15s (UID = %u)\n"
                , uName->pw_name, sb.st_uid);
                
            fprintf(stdout, "   Group Id:      %-15s (GID = %u)\n"
                , gName->gr_name, sb.st_gid);
        }
        fprintf(stdout, "   File size:          %zd bytes\n", sb.st_size);
    }

    return EXIT_SUCCESS;
}

// This function is NOT re-entrant.
static char *
show_local_time(time_t *t)
{
    static char bl[80] = {'\0'};
    struct tm *tsl;

    // Make sure the buffer is set to all NULLs (zeroes).
    memset(bl, 0, sizeof(bl));
    // Get the local time from the epoch time that was passed.
    tsl = localtime(t);
    // This is how the time and date will be formatted
    //   2015-01-05 16:06:14 -0800 (PST) Mon
    strftime(bl, sizeof(bl), "%Y-%m-%d %H:%M:%S %z (%Z) %a", tsl);

    // This returns a pointer to statically allocated memory. If you call
    //   this routine multiple times, the contents of the memory can (and
    //   most likely will) change.
    return(bl);
}

// This function is not re-entrant.
static char *
show_utc_time(time_t *t)
{
    static char bg[80] = {'\0'};
    struct tm *tsg;

    // Make sure the buffer is set to all NULLs (zeroes).
    memset(bg, 0, sizeof(bg));
    // Get the GMT time (aka UTC) from the epoch time that was passed.
    tsg = gmtime(t);
    // This is how the time and date will be formatted
    //   2015-01-05 16:06:14 -0800 (GMT) Mon
    strftime(bg, sizeof(bg), "%Y-%m-%d %H:%M:%S %z (%Z) %a", tsg);

    // This returns a pointer to statically allocated memory. If you call
    //   this routine multiple times, the contents of the memory can (and
    //   most likely will) change.
    return(bg);
}

// This is not a reentrant function.
static char *
mode_letters(mode_t mode)
{
    // A pointer to this variable is returned from the function. The space
    //   is allocated once an reused for each call into the function. This
    //   is why the function is not reentrant.
    static char mode_str[20] = {'\0'};

    // Make sure the mode_str array of characters is all set to 0 (aka NULL).
    memset(mode_str, 0, sizeof(mode_str));

    // This block of code will place a leading character in the mode_str that
    //   indicates what type of file this is: regular file, directory, ... You
    //   can see a listing of the file types in the man page for the find
    //   command (search for '-type") or "man 2 stat".
    //
    //   Symbol	Meaning
    //   -	Regular file
    //   d	Directory
    //   l	Link
    //   c	Special file
    //   s	Socket
    //   p	Named pipe
    //   b	Block device
    //   D	Door (Solaris only)
    //
    if (S_ISREG(mode)) { // is it a regular file
        strcat(mode_str, "-");
    }
    else if (S_ISDIR(mode)) { // a directory
        // A directory created with mkdir.
        strcat(mode_str, "d");
    }
    else if (S_ISCHR(mode)) { // character device
        // These are character devices.
        strcat(mode_str, "c");
    }
    else if (S_ISBLK(mode)) { // block device
        // These are special block devices.
        strcat(mode_str, "b");
    }
    else if (S_ISFIFO(mode)) { // FIFO (named pipe)
        // Created with mkfifo command.
        strcat(mode_str, "p");
    }
    else if (S_ISLNK(mode)) { // symbolic link
        // Created with "ln -s file-name link-name"
        strcat(mode_str, "l");
    }
    else if (S_ISSOCK(mode)) { // socket
        // These are AF_UNIX sockets.
        strcat(mode_str, "s");
    }
#ifdef __sun
    // This is a Solaris specific section of code.
    // There is not a nice macro for this comparison.
    else if ((mode & S_IFMT) == S_IFDOOR) { // A Solaris "Door"
        // I've never heard of this one let alone seen it.
        strcat(mode_str, "D");
    }
#endif // __sun
    else {
        // This means that we don't know what type of object this is.
        // This should never occur.
        strcat(mode_str, "?");
    }

    // These are the user level permissions for the file. If the read-bit
    //   is set, a "r" is copied into the mode_str. Otherwise, a dash gets
    //   copied into the mode_str.
    strcat(mode_str, (mode & S_IRUSR) ? "r" : "-");
    strcat(mode_str, (mode & S_IWUSR) ? "w" : "-");
    if (mode & S_ISUID) {
        // Checking for the set-uid bit.
        if (S_ISDIR(mode)) {
            // A set-uid bit on a directory.
            strcat(mode_str, "s");
        }
        else if (S_ISREG(mode)) {
            // A set-uid bit on a file.
            strcat(mode_str, "S");
        }
        else {
            // not sure what to do here.
            strcat(mode_str, "?");
        }
    }
    else {
        // The execute bit is set.
        strcat(mode_str, (mode & S_IXUSR) ? "x" : "-");
    }

    // These are the group level permissions for the file. If the read-bit
    //   is set, a "r" is copied into the mode_str. Otherwise, a dash gets
    //   copied into the mode_str.
    strcat(mode_str, (mode & S_IRGRP) ? "r" : "-");
    strcat(mode_str, (mode & S_IWGRP) ? "w" : "-");
    if (mode & S_ISGID) {
        // Checking for the set-gid bit.
        if (S_ISDIR(mode)) {
            // A set-gid directory.
            strcat(mode_str, "s");
        }
        else if (S_ISREG(mode)) {
            // A set-gid file.
            strcat(mode_str, "S");
        }
        else {
            // not sure what to do here.
            strcat(mode_str, "?");
        }
    }
    else {
        strcat(mode_str, (mode & S_IXGRP) ? "x" : "-");
    }

    // These are the other level permissions for the file. If the read-bit
    //   is set, a "r" is copied into the mode_str. Otherwise, a dash gets
    //   copied into the mode_str.
    strcat(mode_str, (mode & S_IROTH) ? "r" : "-");
    strcat(mode_str, (mode & S_IWOTH) ? "w" : "-");
    if (mode & S_ISVTX) {
        // Checking for stickies.
        if (S_ISDIR(mode)) {
            // Sticky directory.
            strcat(mode_str, "t");
        }
        else if (S_ISREG(mode)) {
            // Sticky file.
            strcat(mode_str, "T");
        }
        else {
            // not sure what to do here.
            strcat(mode_str, "?");
        }
    }
    else {
        strcat(mode_str, (mode & S_IXOTH) ? "x" : "-");
    }

    // This returns a pointer to the staticly allocated string. It is efficient,
    //   but not reentrant. If being reentrant is important, you should pass a
    //   buffer into which the mode characters are are copied.
    return(mode_str);
}
