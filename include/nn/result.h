#pragma once

#define NNSSL_RESULT_FATAL     0x1927b
#define NNSSL_RESULT_WOULDBLOCK     0x1987b

extern bool nnResultIsFailure(size_t result);
extern bool nnResultIsSuccess(size_t result);
extern int nnResultGetModule(size_t result);
extern int nnResultGetDescription(size_t result);
