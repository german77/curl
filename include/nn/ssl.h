#pragma once

struct Connection
{
    void* filler[35];
};

extern void nnosSleepThread();
extern void nnsslInitialize();
extern void nnsslFinalize();
extern void nnsslContextGetContextId();
extern void nnsslConnectionCreate();
extern void nnsslContextDestroy();
extern void nnsslConnectionSetOption();
extern void nnsslConnectionSetSocketDescriptor();
extern void nnsslConnectionSetHostName();
extern void nnsslConnectionSetVerifyOption();
extern void nnsslConnectionSetSessionCacheMode();
extern void nnsslConnectionSetIoMode();
extern void nnsslConnectionDoHandshakeWithCertBuffer();
extern void nnsslConnectionGetVerifyCertError();
extern void nnsslConnectionGetConnectionId();
extern void nnsslConnectionDestroy();
extern void nnsslConnectionPeek();
extern size_t nnsslConnectionPending(const struct Connection*, int*);
