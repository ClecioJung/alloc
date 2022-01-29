#ifndef ___ALLOC_H_
#define ___ALLOC_H_

#include <stdlib.h>

void init_alloc(void);
void end_alloc(void);
void *alloc(const size_t size);
void del(const void *const ptr);
void print_chunks(void);

#endif  // ___ALLOC_H_
