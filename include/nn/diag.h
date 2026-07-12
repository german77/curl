#pragma once

__attribute__((noreturn)) extern void nndetailDiagAbortFormatImpl(const char* type,
                                                                  const char* function,
                                                                  const char* file, int line,
                                                                  const char* format, ...);

#define NN_ABORT_UNSUPPORTED()              \
    nndetailDiagAbortFormatImpl(            \
        "NNC_ABORT",                        \
        __FUNCTION__,                       \
        __FILE__,                           \
        __LINE__,                           \
        "unsupported function (%s) called", \
        __FUNCTION__ + 11                   \
    );


#define NN_ASSERT(cond, format, ...)        \
    if (!(cond)) {                          \
        nndetailDiagAbortFormatImpl(        \
            "NNC_ABORT",                    \
            __FUNCTION__,                   \
            __FILE__,                       \
            __LINE__,                       \
            format,                         \
            ##__VA_ARGS__                   \
        );                                  \
    }
