/*
 * Source file for all NNSSL-specific code for the TLS/SSL layer. No code
 * but vtls.c should ever call or use these functions.
 */

#include "curl_setup.h"

#ifdef USE_NNSSL

#include "urldata.h"

/* The last #include files should be: */
#include "curl_memory.h"
#include "nnc/ssl.h"

static struct nnResult  NNSSL_RESULT_FATAL = {0x1927b};
static struct nnResult NNSSL_RESULT_WOULDBLOCK= {0x1987b};

static void nnssl_destroy_ssl_connection(struct connectdata* conn, int sockindex)
{
    union nnsslConnection* nnconn = &conn->ssl[sockindex].backend.connection;

    uint64_t id = 0;
    struct nnResult result = nnsslConnectionGetConnectionId(nnconn, &id);
    if (!nnResultIsFailure(result ) && id != 0)
    {
        nnResultIsFailure(nnsslConnectionDestroy(nnconn) );
        conn->ssl[sockindex].state = ssl_connection_none;
    }
}

static void nnssl_destroy_ssl_context(struct connectdata* conn, int sockindex)
{
    struct nnsslBackend* backend = &conn->ssl[sockindex].backend;
    union nnsslContext* ctx = backend->pnnssl_context;

    uint64_t id = 0;
    if (ctx == NULL)
        return;

    bool result = nnResultIsFailure(nnsslContextGetContextId(ctx, &id) );
    if (id != 0 && !result)
        nnResultIsFailure(nnsslContextDestroy(backend->pnnssl_context) );
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
    struct nnResult  result = nnsslInitialize() ;
    if (!nnResultIsFailure(result))
    {
        return CURLE_UNSUPPORTED_PROTOCOL;
    }

    if (!isResultDifferent(result, NNSSL_RESULT_FATAL))
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

ssize_t nnssl_recv(struct connectdata* conn, int sockindex, char* buf, size_t buffersize,
                   CURLcode* curlcode)
{
    int bytes = 0;
    struct nnResult result =
        nnsslConnectionRead(&conn->ssl[sockindex].backend.connection, buf, &bytes, buffersize);

    if (nnResultIsFailure(result))
    {
        *curlcode = isResultEqual(result, NNSSL_RESULT_WOULDBLOCK) ? CURLE_AGAIN : CURLE_RECV_ERROR;
        return -1;
    }

    return bytes;
}

ssize_t nnssl_send(struct connectdata* conn, int sockindex, const void* mem, size_t len,
                   CURLcode* curlcode)
{
    int bytes = 0;

    if (conn == NULL || mem == NULL || curlcode == NULL)
        return CURLE_SSL_INVALIDREFERENCE;

    struct nnResult result =
        nnsslConnectionWrite(&conn->ssl[sockindex].backend.connection, mem, &bytes, len);

    if (nnResultIsFailure(result))
    {
        *curlcode =
            isResultEqual(result, NNSSL_RESULT_WOULDBLOCK) ? CURLE_AGAIN : CURLE_WRITE_ERROR;
        return -1;
    }

    return bytes;
}

CURLcode Curl_nnssl_connect_nonblocking(struct connectdata* conn, int sockindex, bool* done)
{
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

    struct nnResult  result =
        nnsslConnectionPeek(&conn->ssl[0].backend.connection, buf, &got, 1) ;
    if (!nnResultIsFailure(result))
    {
        if (got > 0)
            return CURLE_UNSUPPORTED_PROTOCOL;
        if (got == 0)
            return CURLE_OK;
        return -1;
    }

    if (!isResultEqual(result, NNSSL_RESULT_WOULDBLOCK))
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
    struct nnResult result = nnsslConnectionPending(&conn->ssl[sockindex].backend.connection, &pending);
    return nnResultIsSuccess(result ) && pending > 0;
}

#endif /* USE_NNSSL */
