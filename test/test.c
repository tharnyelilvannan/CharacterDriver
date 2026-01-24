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

    if (basic_circular_buffer() == 0) {
        printf("BASIC CIRCULAR BUFFER TEST - Passed.\n");
    }
    else {
        printf("BASIC CIRCULAR BUFFER TEST - Failed.\n");
    }

    if (basic_mutex() == 0) {
        printf("BASIC MUTEX TEST - Passed.\n");
    }
    else {
        printf("BASIC MUTEX TEST - Failed.\n");
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

int basic_circular_buffer_test() {
    int fd = open("/dev/driver", O_RDWR);

    if (fd == -1) {
        printf("BASIC CIRCULAR BUFFER TEST - Failed to open.\n");
        return -1;
    }

    // large string to write to test buffer
    const char str[] = "It is a truth universally acknowledged, that a single man in possession of a good fortune, must be in want of a wife. However little known the feeligns or views of such a man may be on his first entering a neighbourhood, this truth is so well fixed in the minds of the surrounding families that he is considered as the rightful property of some one or other of their daughters. \"My dear Mr. Bennet,\" said his lady to him one day, \"have you heard that Netherfield Park is let at last?\" Mr. Bennet replied that he had not. \"But it is,\" returned she; \"for Mrs. Long has just been here, and she told me all about it.\" Mr. Bennet made no answer. \"Do not you want to know who had taken it?\" cried his wife impatiently. \"You want to tell me, and I have no objection to hearing it.\" This was invitation enough. \"Why, my dear, you must know, Mrs. Long says that Netherfield is taken by a young man of large fortune from the north of England; that he cam edown on Monday in a chaise and four to see the place, and was so much delighted with it, that he agreed with Mr. Morris immediately; that he is to take possession before Michaelmas, and some of his servants are to be in the house by the end of next week.\"\0";
    int w = write(fd, str, strlen(str));
    
    if (w != 0) {
        printf("BASIC CIRCULAR BUFFER TEST - Failed to write.\n");
        close(fd);
        return -1;
    }

    // read
    char *buf = malloc(sizeof(str)*sizeof(char));
    int r = read(fd, buf, sizeof(str));
    printf("BASIC CIRCULAR BUFFER TEST - Output: %s\n", buf);

    if (r != 0) {
        printf("BASIC CIRCULAR BUFFER TEST - Failed to read.\n");
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

int basic_mutex_test() {
    return 0;
}