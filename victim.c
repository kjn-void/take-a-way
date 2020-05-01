#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "takeaway.h"

static bool move_to_next_char = false;

static void wd_cb(int signum) {
    if (signum == SIGALRM) {
        move_to_next_char = true;
    }
}

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s SECRET_STRING\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *secret = argv[1];
    const size_t len = strlen(secret);
    char * ptr = aligned_alloc(MEM_ALIGNMENT, MEM_ALIGNMENT);

    expect(ptr != NULL, "Could not allocate %u bytes", MEM_ALIGNMENT);
    expect(signal(SIGALRM, wd_cb) != SIG_ERR, "Failed to install signal handler");

    for (size_t i = 0; i < len; i++) {
        printf("V: %c\n", secret[i]);
        const char ch = secret[i] - '0';
        alarm(1);
        while (!move_to_next_char) {
            volatile char _ = ptr[ch * TAG_SEL];
        }
        move_to_next_char = false;
    }

    free(ptr);
    printf("Done\n");
}
