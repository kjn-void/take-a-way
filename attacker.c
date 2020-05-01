#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

#include "takeaway.h"

#define PROBE_CNT           32
#define RETRIES             50000
#define ALPHABET_SZ         ('z' - '0' + 1)

typedef unsigned long long clk_t; 

static void panic(const char *fmt, va_list args) {
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static void expect(bool pred, const char *fmt, ...) {
    if (!pred) {
        va_list args;
        va_start(args, fmt);
        panic(fmt, args);
        va_end(args);
    }
}

clk_t probe(char * ptr, size_t idx) {
    unsigned int cpu_id;
    clk_t start = __builtin_ia32_rdtsc();
    for (unsigned cnt = 0; cnt < PROBE_CNT; cnt++) {
        volatile char _ = ptr[idx * TAG_SEL];
    }
    return __builtin_ia32_rdtscp(&cpu_id) - start;
}

int main() {
    char * ptr = aligned_alloc(MEM_ALIGNMENT, MEM_ALIGNMENT);
    clk_t histogram[ALPHABET_SZ];

    expect(ptr != NULL, "Could not allocate %u bytes", MEM_ALIGNMENT);

    for (;;) {
        memset(histogram, 0, sizeof histogram);
        for (int x = 0; x < RETRIES; x++) {
            for (size_t idx = 0; idx < ALPHABET_SZ; idx++) {
                histogram[idx] += probe(ptr, idx);
            }
        }
        clk_t max_idx = 0;
        for (size_t idx = 1; idx < ALPHABET_SZ; idx++) {
            if (histogram[max_idx] < histogram[idx]) {
                max_idx = idx;
            }
        }
        printf("Letter is: %c\n", (char)(max_idx + '0'));
    }

    free(ptr);
}
