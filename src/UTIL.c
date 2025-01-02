#include <UTIL.h>

void print_backtrace() {
    void *buffer[10];
    int nptrs = backtrace(buffer, 10);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}