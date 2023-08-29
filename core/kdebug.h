
#ifndef KRAKEN_DEBUG
#define kdebug_log(format, ...) kdebug_log_(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#else 
#define kdebug_log(format, ...) 
#endif

#include <stdio.h>

extern 
FILE* kdebug_log_stream;

void kdebug_log_(const char* file, const char* func, long long line, const char* format, ...);