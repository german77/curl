#pragma once

struct Connection
{ /* nn::ssl::Connection */
    void* filler[16];
};

struct Context
{ /* nn::ssl::Context */
    void* filler[16];
};

struct nnsslBackend  // TODO: Validate members
{
    struct Connection connection;
    struct Context context;
    struct Context* pnnssl_context; /* active context (embedded or external) */
    char using_external_ssl_context;
};

extern void nnosSleepThread();

extern size_t nnsslInitialize(void);
extern size_t nnsslFinalize(void);

extern size_t nnsslContextGetContextId(const struct Context* ctx, uint64_t* pOutId);
extern size_t nnsslContextDestroy(const struct Context* ctx);

extern size_t nnsslConnectionCreate(const struct Connection* conn, const struct Context* ctx);
extern size_t nnsslConnectionSetOption(const struct Connection* conn, int optionType, int enable);
extern size_t nnsslConnectionSetSocketDescriptor(const struct Connection* conn, int fd);
extern size_t nnsslConnectionSetHostName(const struct Connection* conn, const char* name,
                                         uint32_t len);
extern size_t nnsslConnectionSetVerifyOption(const struct Connection* conn, uint32_t verifyOption);
extern size_t nnsslConnectionSetSessionCacheMode(const struct Connection* conn, int mode);
extern size_t nnsslConnectionSetIoMode(const struct Connection* conn, int mode);
extern size_t nnsslConnectionDoHandshakeWithCertBuffer(const struct Connection* conn,
                                                       uint32_t* pCertDataSize,
                                                       uint32_t* pCertCount);
extern size_t nnsslConnectionGetVerifyCertError(const struct Connection* conn, uint32_t* pOut);
extern size_t nnsslConnectionGetConnectionId(const struct Connection* conn, uint64_t* pOutId);
extern size_t nnsslConnectionDestroy(const struct Connection* conn);
extern size_t nnsslConnectionPeek(const struct Connection* conn, void* buf, int* pOut,
                                  uint32_t len);
extern size_t nnsslConnectionPending(const struct Connection* connection, int* pending);
extern size_t nnsslConnectionRead(const struct Connection* conn, void* buf, int* pOutRead,
                                  uint32_t len);
extern size_t nnsslConnectionWrite(const struct Connection* conn, const void* buf, int* pOutWritten,
                                   uint32_t len);
