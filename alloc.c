#define _DEFAULT_SOURCE

#include "alloc.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#ifdef DEBUG
#define DEBUG_PRINT(...)                                                 \
    do {                                                                 \
        fprintf(stderr, "%s:%d:%s(): \n", __FILE__, __LINE__, __func__); \
        fprintf(stderr, __VA_ARGS__);                                    \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) \
    do {                      \
    } while (0)
#endif

#define HEAP_SIZE 4096
#define WORD_SIZE sizeof(void *)

static void *heap = NULL;

static size_t get_chunk_size(const void *const ptr) {
    size_t size = *((size_t *)ptr);
    return (size & (SIZE_MAX - 1));
}

static void alloc_chunk(const void *const ptr, const size_t size) {
    *((size_t *)ptr) = size | 1;
}

static void free_chunk(const void *const ptr, const size_t size) {
    *((size_t *)ptr) = size;
}

static int is_allocated(const void *const ptr) {
    return (*((size_t *)ptr) & 1);
}

static void *get_address_from_chunk(const void *const ptr) {
    return ((size_t *)ptr + 1);
}

static void *get_chunk_from_address(const void *const ptr) {
    return ((size_t *)ptr - 1);
}

static void *next_chunk(const void *const ptr) {
    const size_t size = get_chunk_size(ptr);
    return ((u_int8_t *)ptr + size);
}

static int is_valid_chunk(const void *const ptr) {
    return ((size_t)((u_int8_t *)ptr - (u_int8_t *)heap + WORD_SIZE) < HEAP_SIZE);
}

static void coalesce_next_chunks(const void *const chunk_ptr) {
    size_t total_size = get_chunk_size(chunk_ptr);
    const void *next_ptr = next_chunk(chunk_ptr);
    while (!is_allocated(next_ptr) && is_valid_chunk(next_ptr)) {
        total_size += get_chunk_size(next_ptr);
        next_ptr = next_chunk(next_ptr);
    }
    free_chunk(chunk_ptr, total_size);
}

void init_alloc(void) {
    heap = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap == NULL || heap == MAP_FAILED) {
        DEBUG_PRINT("[ERROR] Could not allocate initial memory: %s\n", strerror(errno));
        return;
    }
    DEBUG_PRINT("[DEBUG] Heap initial address: %p\n", heap);
    free_chunk(heap, HEAP_SIZE - WORD_SIZE);
}

void end_alloc(void) {
    if (munmap(heap, HEAP_SIZE) == -1) {
        DEBUG_PRINT("[ERROR] Could not unmap allocated memory: %s\n", strerror(errno));
    }
    heap = NULL;
}

void *alloc(const size_t size) {
    if (heap == NULL || heap == MAP_FAILED) {
        DEBUG_PRINT("[ERROR] The alloc library was not properly initialized (heap = %p)\n", heap);
        return NULL;
    }

    const size_t rounded_size = ((size + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
    const size_t total_size = rounded_size + WORD_SIZE;
    void *chunk_ptr = heap;
    while (is_valid_chunk(chunk_ptr)) {
        if (!is_allocated(chunk_ptr)) {
            coalesce_next_chunks(chunk_ptr);
            size_t chunk_size = get_chunk_size(chunk_ptr);
            if (total_size <= chunk_size) {
                alloc_chunk(chunk_ptr, rounded_size);
                if (total_size < chunk_size) {
                    const void *chunk_rest = next_chunk(chunk_ptr);
                    free_chunk(chunk_rest, chunk_size - rounded_size);
                }
                return get_address_from_chunk(chunk_ptr);
            }
        }
        chunk_ptr = next_chunk(chunk_ptr);
    }
    return NULL;
}

void del(const void *const ptr) {
    if (ptr != NULL) {
        void *chunk_ptr = get_chunk_from_address(ptr);
        if (is_valid_chunk(chunk_ptr)) {
            size_t chunk_size = get_chunk_size(chunk_ptr);
            free_chunk(chunk_ptr, chunk_size);
        }
    }
}

void print_chunks(void) {
#ifdef DEBUG
    DEBUG_PRINT("[DEBUG] Resume of the chunks administrated by the alloc library:\n");
    for (void *chunk_ptr = heap; is_valid_chunk(chunk_ptr); chunk_ptr = next_chunk(chunk_ptr)) {
        void *address = get_address_from_chunk(chunk_ptr);
        size_t chunk_size = get_chunk_size(chunk_ptr);
        char *state = is_allocated(chunk_ptr) ? "allocated" : "freed";
        fprintf(stderr, "address = %p, size = %04ld bytes, %s\n", address, chunk_size, state);
    }
#endif
}