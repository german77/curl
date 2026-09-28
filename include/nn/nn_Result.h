#pragma once

#include <nn/result/result_ResultBase.h>

typedef struct nnResult {
    uint32_t _value;
} nnResult;

extern bool nnResultIsFailure(struct nnResult );
extern bool nnResultIsSuccess(struct nnResult );
extern int nnResultGetModule(struct nnResult );
extern int nnResultGetDescription(struct nnResult );

static bool isResultDifferent(struct nnResult a, struct nnResult b)
{
    return nnResultGetModule(a) != nnResultGetModule(b) ||
           nnResultGetDescription(a) != nnResultGetDescription(b);
}

static bool isResultEqual(struct nnResult a, struct nnResult b)
{
    return nnResultGetModule(a) == nnResultGetModule(b) &&
           nnResultGetDescription(a) == nnResultGetDescription(b);
}
