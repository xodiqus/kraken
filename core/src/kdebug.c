#include "kdebug.h"

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

FILE* kdebug_log_stream;

void kdebug_log_(const char* file, const char* func, long long line, const char* format, ...) {
    assert(kdebug_log_stream != NULL);
    assert(strlen(format) < 100);

    va_list args;
    va_start(args, format);

    char buf[256];

    vsprintf(buf, format, args);
    va_end(args);

    fprintf(kdebug_log_stream, "[%s : %lld]: %s.\n", func, line, buf);
}