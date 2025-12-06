#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

///reads an file in 8-bytes and conts char categories in (upper,lower,digits,space and other)


int main(int argc, char *argv[]) {
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];

    if (argc != 2) {
        printf("USAGE: parta FILEIN");
        return 1;
    }

    const char *filename = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("ERROR: %s not found", filename);
        return 2;
    }

    int upper = 0, lower = 0, number = 0, space = 0, other = 0;
    ssize_t nread;

    while ((nread = read(fd, buffer, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < nread; i++) {
            unsigned char c = (unsigned char)buffer[i];
            if (isupper(c))       upper++;
            else if (islower(c))  lower++;
            else if (isdigit(c))  number++;
            else if (isspace(c))  space++;
            else                  other++;
        }
    }

    close(fd);

    printf("Upper,%d\n", upper);
    printf("Lower,%d\n", lower);
    printf("Number,%d\n", number);
    printf("Space,%d\n", space);
    printf("Other,%d\n", other);

    return 0;

}
