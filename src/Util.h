#pragma once

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_backtrace() {
    void *buffer[10];
    int nptrs = backtrace(buffer, 10);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

// Writes message to stderr, file name, file number,
// If possible it tries to get backtrace info
// Aborts the process, causing a core dump if possible
#define PANIC(msg)                                                        \
    do {                                                                  \
        fprintf(stderr, "PANIC at %s:%d: '%s'", __FILE__, __LINE__, msg); \
        fprintf(stderr, "\n");                                            \
        print_backtrace();                                                \
        abort();                                                          \
    } while (0)

#define PANIC_FMT(format, ...)                                   \
    do {                                                         \
        fprintf(stderr, "PANIC at %s:%d: ", __FILE__, __LINE__); \
        fprintf(stderr, (format), __VA_ARGS__);                  \
        fprintf(stderr, "\n");                                   \
        print_backtrace();                                       \
        abort();                                                 \
    } while (0)