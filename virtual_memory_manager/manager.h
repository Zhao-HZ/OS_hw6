#ifndef MANAGER_H_
#define MANAGER_H_

#define MAX 20
#define FRAME_SIZE 256
// #define FRAME_SIZE 128

// It contains
// a mapping from page number to frame
typedef struct {
    int table[256];
} PageTable;

// This structure simulates a single page
// for physical memory.
typedef struct {
    char page[256];
} Page;

typedef struct {
    // The size of the two below
    // should be FRAME_SIZE.
    Page *memory;
    int *in_use; // -1: not in use.
                 // 1: in use.
    int size;
} PhysicalMemory;
// The physical should contain multiple
// pages.

typedef struct {
    // The size of the two below
    // should be 16
    // according to the title.
    int pages[MAX];
    int frames[MAX];
    int next;
} TLB;

int get_page_num(int address);
int get_offset(int address);

// 0: does not find
// otherwise return the corresponding frame
int search_TLB(int page_num, TLB *tlb);

int get_empty_frame(PhysicalMemory *pMemory);

// void page_replacement(int page_num, int frame, TLB *tlb);

void debug();
#endif