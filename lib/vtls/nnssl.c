/*
 * Source file for all NNSSL-specific code for the TLS/SSL layer. No code
 * but vtls.c should ever call or use these functions.
 */

#include "curl_setup.h"

#ifdef USE_NNSSL

#include "urldata.h"

/* The last #include files should be: */
#include "curl_memory.h"
#include "nn/result.h"
#include "nn/ssl.h"

static void nnssl_destroy_ssl_connection(struct connectdata* conn, int sockindex)
{
    struct Connection* nnconn = &conn->ssl[sockindex].backend.connection;

    uint64_t id = 0;
    size_t result = nnsslConnectionGetConnectionId(nnconn, &id);
    if (!nnResultIsFailure(result & 0xffffffff) && id != 0)
    {
        nnResultIsFailure(nnsslConnectionDestroy(nnconn) & 0xffffffff);
        conn->ssl[sockindex].state = ssl_connection_none;
    }
}

static void nnssl_destroy_ssl_context(struct connectdata* conn, int sockindex)
{
    struct nnsslBackend* backend = &conn->ssl[sockindex].backend;
    struct Context* ctx = backend->pnnssl_context;

    uint64_t id = 0;
    if (ctx == NULL)
        return;

    bool result = nnResultIsFailure(nnsslContextGetContextId(ctx, &id) & 0xffffffff);
    if (id != 0 && !result)
        nnResultIsFailure(nnsslContextDestroy(backend->pnnssl_context) & 0xffffffff);
}

void Curl_nnssl_close(struct connectdata* conn, int sockindex)
{
    if (conn == NULL)
        return;

    nnssl_destroy_ssl_connection(conn, sockindex);
    if (!conn->ssl[sockindex].backend.using_external_ssl_context)
        nnssl_destroy_ssl_context(conn, sockindex);
}

int Curl_nnssl_init(void)
{
    size_t result = nnsslInitialize() & 0xffffffff;
    if (!nnResultIsFailure(result))
    {
        return CURLE_UNSUPPORTED_PROTOCOL;
    }

    if (nnResultGetModule(result) == nnResultGetModule(NNSSL_RESULT_FATAL) &&
        nnResultGetDescription(result) == nnResultGetDescription(NNSSL_RESULT_FATAL))
        return CURLE_UNSUPPORTED_PROTOCOL;

    return CURLE_OK;
}

void Curl_nnssl_cleanup(void)
{
    nnsslFinalize();
}

CURLcode Curl_nnssl_connect(struct connectdata* conn, int sockindex)
{
    if (conn == NULL)
        return CURLE_SSL_INVALIDREFERENCE;

    return CURLE_NOT_BUILT_IN;
}

CURLcode Curl_nnssl_connect_nonblocking(struct connectdata* conn, int sockindex, bool* done)
{
    if (conn == NULL || done == NULL)
        return CURLE_SSL_INVALIDREFERENCE;

    struct ssl_connect_data* connssl = &conn->ssl[sockindex];

    if (connssl->state == ssl_connection_complete)
    {
        *done = true;
        return CURLE_OK;
    }

    struct SessionHandle* data = conn->data;
    int value = connssl->backend.value;
    CURLcode result = CURLE_OK;

    if (connssl->backend.value == 0)
    {
        // TODO: finish implementation
    }
    return result;
}

size_t Curl_nnssl_version(char* buffer, size_t size)
{
    if (!buffer)
        return 0;

    return snprintf(buffer, size, "nn::ssl");
}

int Curl_nnssl_check_cxn(struct connectdata* conn)
{
    char buf[4];
    int got = 0;

    if (conn == NULL)
        return CURLE_SSL_INVALIDREFERENCE;

    size_t result =
        nnsslConnectionPeek(&conn->ssl[0].backend.connection, buf, &got, 1) & 0xffffffff;
    if (!nnResultIsFailure(result))
    {
        if (got > 0)
            return CURLE_UNSUPPORTED_PROTOCOL;
        if (got == 0)
            return CURLE_OK;
        return -1;
    }

    if (nnResultGetModule(result) != nnResultGetModule(NNSSL_RESULT_WOULDBLOCK))
        return -1;

    if (nnResultGetDescription(result) != nnResultGetDescription(NNSSL_RESULT_WOULDBLOCK))
        return -1;

    return 1;
}

int Curl_nnssl_seed(struct SessionHandle* data)
{
    return 0;
}

int Curl_nnssl_random(struct SessionHandle* data, unsigned char* entropy, size_t length)
{
    if (entropy == NULL || data == NULL)
        return 0;

    for (unsigned int i = 0; i < length; i++)
    {
        entropy[i] = 0;
    }
    return 0;
}

bool Curl_nnssl_cert_status_request(void)
{
    return false;
}

bool Curl_nnssl_false_start(void)
{
    return false;
}

bool Curl_nnssl_data_pending(const struct connectdata* conn, int sockindex)
{
    int pending = 0;
    int result = nnsslConnectionPending(&conn->ssl[sockindex].backend.connection, &pending);
    return nnResultIsSuccess(result & 0xffffffff) && pending > 0;
}

#endif /* USE_NNSSL */
