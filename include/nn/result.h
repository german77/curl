#pragma once

#define NNSSL_RESULT_WOULDBLOCK     0x1987b  /* WANT_READ/WRITE - retry */

extern bool nnResultIsFailure(size_t result);
extern bool nnResultIsSuccess(size_t result);
extern int nnResultGetModule(size_t result);
extern int nnResultGetDescription(size_t result);
