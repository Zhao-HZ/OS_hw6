#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define PROMPT "allocator>"
#define MAX_CHAR_COM 200

extern void request(int i, long size, char T);
extern void release(int i);
extern void stats();
extern void get_max(long max);
extern void compact();
extern short mark[2000000 + 1];

long max; // max size of the memory

void debug(int i, char c, long size) {
    printf("%d\n", i);
    printf("%c\n", c);
    printf("%ld\n", size);
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("ERROR\n");
        return 0;
    }
    // Enter the value in bytes.
    max = (long) atoi(argv[1]);
    get_max(max);

    for (int i = 0; i <= max + 1000; i++) {
        mark[i] = -1;
    }

    while (1) {
        char buf[MAX_CHAR_COM];
        char c;
        char T; // Algorithm for the request.
        int k = 0;
        int i; // order of the process
        long size; // size of the process
        printf("%s", PROMPT);
        while ((c = getchar()) != (int) '\n') {
            buf[k++] = c;
        }
        if (buf[0] == 'X') { // Exit
            return 0;
        } else if (buf[0] == 'C') { // Compaction
            compact();
        } else if (buf[0] == 'R' && buf[1] == 'Q') { 
            buf[5] = '\0';
            int i = atoi(buf + 4); // order of the process.
            int j = 6;
            for (j; buf[j] != ' '; j++);
            buf[j] = '\0';
            size = atoi(buf + 6);
            char T = buf[j + 1];
            request(i, size, T);
        } else if (buf[0] == 'R' && buf[1] == 'L') {
            buf[5] = '\0';
            int i = atoi(buf + 4);
            release(i);
        } else if (buf[0] == 'S') {
            stats();
        } 
    }
    return 0;
}

