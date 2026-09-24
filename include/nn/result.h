#pragma once

#define NNSSL_RESULT_FATAL 0x1927b
#define NNSSL_RESULT_WOULDBLOCK 0x1987b
#define NNSSL_RESULT_FAILEDVERIFICATION1 0x19e7b
#define NNSSL_RESULT_FAILEDVERIFICATION2 0x25a7b
#define NNSSL_RESULT_ERROREXIPREDCERTIFICATE 0x25e7b
#define NNSSL_RESULT_ERRORREVOKEDCERTIFICATE 0x2607b
#define NNSSL_RESULT_ERRORKUNKNOWNCA 0x2687b
#define NNSSL_RESULT_FAILEDVERIFICATION3 0x2867b
#define NNSSL_RESULT_ALERTBADCERTIFICATE 0xbca7b
#define NNSSL_RESULT_ALERTCERTIFICATEREVOKED 0xbce7b
#define NNSSL_RESULT_ALERTCERTIFICATEEXIPRED 0xbd07b

extern bool nnResultIsFailure(size_t result);
extern bool nnResultIsSuccess(size_t result);
extern int nnResultGetModule(size_t result);
extern int nnResultGetDescription(size_t result);

static bool isResultDifferent(size_t a, size_t b)
{
    return nnResultGetModule(a) != nnResultGetModule(b) ||
           nnResultGetDescription(a) != nnResultGetDescription(b);
}

static bool isResultEqual(size_t a, size_t b)
{
    return nnResultGetModule(a) == nnResultGetModule(b) &&
           nnResultGetDescription(a) == nnResultGetDescription(b);
}
