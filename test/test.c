#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "test.h"

// use memset, read, write, printf, open, close to test

int main() {
    if (basic() == 0) {
        printf("BASIC TEST - Passed.\n");
    }
    else {
        printf("BASIC TEST - Failed.\n");
    }
}

int basic() {
    // open driver
    int fd = open("/dev/driver", O_RDWR);

    if (fd == -1) {
        printf("BASIC TEST - Failed to open.\n");
        return -1;
    }

    // write
    const char *str = "Good morning!";
    int written = write(fd, str, strlen(str));
    
    if (written == -1) {
        printf("BASIC TEST - Failed to write.\n");
        return -1;
    }

    // read
    char buf[1024];
    int r = read(fd, buf, 1024);

    if (r == -1) {
        printf("BASIC TEST - Failed to read.\n");
        return -1;
    }

    // close driver
    close(fd);
    return 0;
}