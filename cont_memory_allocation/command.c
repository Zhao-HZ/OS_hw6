#include "command.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY (long)2000000
#define MAX_PROCESSES 10
#define PROCESS "Process"
#define ADDR "Addresses"

long max_;

extern int linear_search(long start);

// Aim to track the usage of the memory.
// range: 0 ... max_(Closed interval).
short address[MAX_MEMORY + 1];
short mark[MAX_MEMORY + 1];

// extern int linear_search(long start);

// It simulates one process.
typedef struct {
    // The region of the process involves start
    long start, size;
    
    int i;
    // The name of the process should be
    // P_{i} where i \in {0, 1, ..., 9}. 
    // We should store i in one structure.
} Process;

#define MALLOC_STRUCT (Process *)malloc(sizeof(Process));

Process *processes[MAX_PROCESSES];

void get_max(long max) {
    max_ = max;
}

int request(int i, long size, char T) {
    Process *p;
    int flag = 0;
    if (processes[i] == NULL) {
        p = MALLOC_STRUCT;
        p->i = i;
        p->size = size;
        if (T == 'F') { // First fit
            // TODO
            int ptr = 0;
            while (ptr <= max_) {
                if (address[ptr] == 0) {
                    if (ptr + size - 1 > max_) {
                        flag = 0;
                        ptr++;
                    } else {
                        flag = 1;
                        for (int j = ptr; j <= ptr + size - 1; j++) {
                            if (address[j] == 1) {
                                flag = 0;
                                break;
                            }
                        }
                        if (flag == 1) { // Success
                            p->start = ptr;
                            mark[ptr] = i;
                            break;
                        } else {
                            ptr++;
                        }
                    }
                } else {
                    ptr++;
                }
            }
            if (flag) {
                for (int j = ptr; j <= ptr + size - 1; j++) {
                    address[j] = 1;
                }
                processes[i] = p;
            }
        } else if (T == 'B') { // Best fit
            // TODO
            long min_size_block = 2 * max_;
            long cur_size_block;
            long ptr = 1;
            long min_ptr;
            // int is_hole = 0;
            long start = 0;
            // is_hole = address[0] == 0 ? 1 : 0;
            for (ptr; ptr <= max_; ptr++) {
                if (address[ptr] == 0) { 
                    if (address[ptr + 1] == 1) {
                        if (size <= ptr - start + 1) {
                            if (min_size_block > ptr - start + 1) {
                                min_size_block = ptr - start + 1;
                                min_ptr = start;
                            }
                        }
                    } else if (ptr + 1 == max_) {
                        if (size <= max_ - start + 1) {
                            if (min_size_block > max_ - start + 1) {
                                min_size_block = max_ - start + 1;
                                min_ptr = start;
                            }
                        }
                    }
                }
                if (address[ptr] == 1) {
                    if (address[ptr + 1] == 0) {
                        start = ptr + 1; 
                    }
                }
            }
            if (min_size_block >= size) {
                p->start = min_ptr;
                mark[min_ptr] = i;
                for (int i = min_ptr; i <= min_ptr + size - 1; i++) {
                    address[i] = 1;
                }
                processes[i] = p;
                flag = 1;
            } else {
                flag = 0;
            }

        } else if (T == 'W') { // Worst fit
            // TODO
            long max_size_block = 0;
            long cur_size_block;
            long ptr = 1;
            long max_ptr;
            int is_hole = 0;
            long start = 0;
            is_hole = address[0] == 0 ? 1 : 0;
            for (ptr; ptr <= max_; ptr++) {
                if (address[ptr] == 0) { 
                    if (address[ptr + 1] == 1) {
                        if (max_size_block < ptr - start + 1) {
                            max_size_block = ptr - start + 1;
                            max_ptr = start;
                        }
                    } else if (ptr + 1 == max_) {
                        if (max_size_block < max_ - start + 1) {
                            max_size_block = max_ - start + 1;
                            max_ptr = start;
                        }
                    }
                }
                if (address[ptr] == 1) {
                    if (address[ptr + 1] == 0) {
                        start = ptr + 1; 
                    }
                }
            }
            if (max_size_block >= size) {
                p->start = max_ptr;
                mark[max_ptr] = i;
                for (int i = max_ptr; i <= max_ptr + size - 1; i++) {
                    address[i] = 1;
                }
                processes[i] = p;
                flag = 1;
            } else {
                flag = 0;
            }
        } else {
            flag = 0;
            printf("Error! Unknown name of algorithm.\n");
        }
    } else {
        flag = 0;
        printf("The process P%d has already exists.\n", i);
    }
    return flag;
}

void release(int i) {
    if (processes[i] != NULL) {
        int start = processes[i]->start;
        int size = processes[i]->size;
        mark[processes[i]->start] = -1;
        for (int j = start; j <= start + size - 1; j++) {
            address[j] = 0;
        }
        free(processes[i]);
        processes[i] = NULL;
    }
}

void compact() {
    // TODO
    int start_hole = 0;
    int end_hole;
    int i = 1;
    // for (int i = 1; i <= max_; i++) {
    if (address[i] == 0) {
        start_hole = 0;
    }
    while (i <= max_) {
        if (address[i] == 0 && address[i - 1] == 1) {
            start_hole = i;
            i++;
        } else if (address[i] == 1 && address[i - 1] == 0) {
            end_hole = i - 1;
            int j = mark[i]; // get the order of the process from its start position.
            // Erase the region of the current process P_j
            for (int k = i; k < i + processes[j]->size; k++) {
                address[k] = 0;
            }
            if (start_hole <= end_hole) {
                for (
                    int k = start_hole;
                    k <= start_hole + processes[j]->size - 1;
                    k++
                ) {
                    address[k] = 1;
                }
            }
            mark[i] = -1;
            mark[start_hole] = j;
            processes[j]->start = start_hole;
            while (address[i] != 0) {
                i++;
            }
        } else {
            i++;
        }
    }
}

void stats() {
    // int is_hole;
    // int diff;
    // is_hole = address[0] == 0 ? 1 : 0;
    long start = 0;
    int j;
    for (long i = 1; i <= max_; i++) {
        // int k = linear_search(i);
        if (address[i] == 1 && address[i - 1] == 0) {
            // is_hole = 0;
            printf("%s [%ld:%ld] Unused\n", ADDR, start, i - 1);
            start = i;
        } else if (address[i] == 0 && address[i - 1] == 1) {
            // is_hole = 1;
            // int j = linear_search(start);
            j = mark[start];
            printf("%s [%ld:%ld] Process %d\n", ADDR, start, i - 1, j);
            start = i;
        } else if (mark[i] != -1) {
            // int j = linear_search(start);
            j = mark[start];
            printf("%s [%ld:%ld] Process %d\n", ADDR, start, i - 1, j);
            start = i;
        }
    }
    if (address[max_] == 0) {
        printf("%s [%ld:%ld] Unused\n", ADDR, start, max_);
    } else {
        int i = linear_search(start);
        printf("%s [%ld:%ld] Process %d\n", ADDR, start, max_, i);
    }
}

int linear_search(long start) {
    int i;
    for (i = 0; i < MAX_MEMORY; i++) {
        if (processes[i]->start == start) {
            return i;
        }
    } 
    // It is impossible indeed.
    return -1;
}

void debug_command() {

}