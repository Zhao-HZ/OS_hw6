#ifdef COMMAND_H_
#define COMMAND_H_

/**
char T:
F -- First fit.
B -- Best fit.
W -- Worst fit.

int i:
order of the process.
*/
int request(int i, long size, char T);

// int: the order of the process.
void release(int i);

void compact();

void stats();

// Get the order of the process from its start time.
int linear_search(long start);

void debug_command();

void get_max(long max);

#endif 