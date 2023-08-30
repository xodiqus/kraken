
#ifdef KRAKEN_DEBUG
#define kdebug_log(format, ...) kdebug_log_(__FILE__, __func__, __LINE__, format, __VA_ARGS__)

#define kdebug_code(...) __VA_ARGS__;

#define check_elapsed_sec(result, ...)          \
    const clock_t temp_##result = clock();      \
    __VA_ARGS__;                                \
    const long double result = (long double)(clock() - temp_##result) / CLOCKS_PER_SEC; \
    const bool can_use_##result = true;

#else 

#define kdebug_log(format, ...) 

#define kdebug_code(...)

#define check_elapsed_sec(result, ...)         \
    __VA_ARGS__;                               \
    const long double result = 0;              \
    const bool can_use_##result = false;       \

#endif

#include <stdio.h>

extern 
FILE* kdebug_log_stream;

void kdebug_log_(const char* file, const char* func, long long line, const char* format, ...);