#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "test.h"
#include <stdlib.h>

// use memset, read, write, printf, open, close to test

int main() {
    printf("\nTESTS\n");

    if (basic() == 0) {
        printf("BASIC TEST - Passed.\n");
    }
    else {
        printf("BASIC TEST - Failed.\n");
    }

    printf("\n");
}

int basic() {
    // open driver
    int fd = open("/dev/driver", O_RDWR);

    if (fd == -1) {
        printf("BASIC TEST - Failed to open.\n");
        return -1;
    }

    // write
    const char str[] = "Hello world!\0";
    int w = write(fd, str, strlen(str));
    
    if (w != 0) {
        printf("BASIC TEST - Failed to write.\n");
        close(fd);
        return -1;
    }

    // read
    char *buf = malloc(sizeof(str)*sizeof(char));
    int r = read(fd, buf, sizeof(str));
    printf("BASIC TEST - Output: %s\n", buf);

    if (r != 0) {
        printf("BASIC TEST - Failed to read.\n");
        close(fd);
        free(buf);
        return -1;
    }

    if (strcmp(buf, str) != 0) {
        close(fd);
        free(buf);
        return -2;
    }

    // close driver
    close(fd);
    free(buf);
    return 0;
}