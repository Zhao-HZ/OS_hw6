#include <stdio.h>
#include "manager.h"

int hello = 0;

int get_page_num(int address) {
    int num = address >> 8;
    return num;
}

int get_offset(int address) {
    int mask = (1 << 8) - 1;
    int result = address & mask;
    return result;
}

int search_TLB(int page_num, TLB *tlb) {
    for (int i = 0; i < 16; i++) {
        if (tlb->pages[i] == page_num) {
            return tlb->frames[i];
        }
    }
    return -1;
}

int get_empty_frame(PhysicalMemory *pMemory) {
    static int ptr;
    for (int i = 0; i < pMemory->size; i++) {
        if (pMemory->in_use[i] == -1) {
            return i;
        }
    }
    // If we cannot find the free block, 
    // we have to select one page to be replaced
    // by FIFO algorithm
    // In the following code, we indeed implement the page replacement.
    // i.e. the last part of the project.
    int result = ptr;
    if (ptr == pMemory->size) {
        ptr = 0;
    } else {
        ptr++;
    }
    return result;
}

// void page_replacement(int page_num, int frame, TLB *tlb) {

// }

void debug() {
}
