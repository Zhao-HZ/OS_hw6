#include <stdio.h>
#include <stdlib.h>
#include "manager.h"

#define DENOMINATOR 10000
#define MALLOC(TYPE, SIZE) (TYPE*) malloc(sizeof(TYPE) * (SIZE + 1))

int page_hits_num = 0;
int page_misses_num = 0;
int page_faults_num = 0;

void debug_main();

int main(int argc, char **argv) {
    int frame;
    // FILE* store = fopen("BACKING.bin", "rb");
    if (argc != 2) {
        printf("ERROR!\n");
    }
    FILE *file = fopen(argv[1], "r");
    FILE *store = fopen("BACKING_STORE.bin", "rb");
    unsigned int addresses[10000]; // The address cannot be empty.
    PageTable pageTable;
    TLB tlb;
    PhysicalMemory pMemory;
    // pMemory.memory = MALLOC(Page, FRAME_SIZE);
    pMemory.memory = MALLOC(Page, 256);
    // pMemory.in_use = MALLOC(int, FRAME_SIZE);
    pMemory.in_use = MALLOC(int, 256);
    pMemory.size = FRAME_SIZE;
    
    for (int i = 0; i < 256; i++) {
        pageTable.table[i] = -1;
        pMemory.in_use[i] = -1;
    }
    for (int i = 0; i < MAX; i++) {
        tlb.pages[i] = -1;
    }
    for (int i = 0; i < 1000; i++) {
        fscanf(file, "%d", addresses + i);
    }
    for (int i = 0; i < 1000; i++) {
        int address = addresses[i];
        int mask = (1 << 16) - 1;
        address = address & mask;
        int page_num = get_page_num(address);
        int offset = get_offset(address);
        int flag = search_TLB(page_num, &tlb);
        if (flag != -1) {
            // Page hit
            page_hits_num++;
        } else {
            // Page miss
            page_misses_num++;
            frame = pageTable.table[page_num];
            if (frame == -1) {
                // Page fault
                page_faults_num++;
                frame = get_empty_frame(&pMemory);
                // pageTable[page_num]++;
                pMemory.in_use[frame] = 1;                
                fseek(store, page_num * 256, SEEK_SET);
                fread(pMemory.memory[frame].page, sizeof(Page), 1, store);
            }
            // page_replacement(page_num, frame, &tlb);
        }
        int physical_addr = (frame << 8) | offset;
        printf("VA: %d, PA: %d, value: %d\n", 
        address, physical_addr, pMemory.memory[frame].page[offset]);
    }
    printf("Frame size = %d\n", FRAME_SIZE);
    printf("Page hits rate = %.2f\n", (float) page_hits_num / DENOMINATOR);
    // printf("%.2f\n", (float)page_misses_num / DENOMINATOR);
    printf("Page faults rate = %.2f\n", (float) page_faults_num / DENOMINATOR);
    if(pMemory.memory != NULL) {
        free(pMemory.memory);
    }
    if (pMemory.in_use != NULL) {
        free(pMemory.in_use);
    }
    return 0;
}

void debug_main() {
    printf("%d\n", MAX);
}