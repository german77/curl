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
    size_t result =
        nnsslConnectionRead(&conn->ssl[sockindex].backend.connection, buf, &bytes, buffersize) &
        0xffffffff;

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

    size_t result =
        nnsslConnectionWrite(&conn->ssl[sockindex].backend.connection, mem, &bytes, len) &
        0xffffffff;

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
    struct nnsslBackend* pnVar1;
    uint uVar2;
    int uVar3;
    int iVar4;
    int iVar5;
    uint uVar6;
    struct Context* pCVar7;
    ulong uVar8;
    size_t sVar9;
    long lVar10;
    struct code* pcVar12;
    CURLcode result;
    struct SessionHandle* pSVar13;
    struct Context** ppCVar14;
    char* __s;
    void* pvVar15;
    long local_88;
    ulong local_80;
    ulong local_78;
    long local_70;
    int local_68;
    short local_64;
    char local_62;
    struct ssl_connection_state* state;
    int* value;

    if (conn == NULL || done == NULL)
        return CURLE_SSL_INVALIDREFERENCE;

    struct ssl_connect_data* connssl = &conn->ssl[sockindex];

    if (connssl->state == ssl_connection_complete)
    {
        *done = true;
        return CURLE_OK;
    }

    state = &conn->ssl[sockindex].state;
    value = &conn->ssl[sockindex].backend.value;
    if (conn->ssl[sockindex].backend.value == 0)
    {
        pSVar13 = conn->data;
        local_88 = 0;
        if ((pSVar13->set).ssl.filler[0] == 0)
        {
            pCVar7 = &conn->ssl[sockindex].backend.context;
            conn->ssl[sockindex].backend.using_external_ssl_context = FALSE;
            conn->ssl[sockindex].backend.pnnssl_context = pCVar7;
            uVar3 = nnsslContextGetContextId(pCVar7, &local_88);
            if (nnResultIsFailure(uVar3) != 0)
                goto GOTOFATALERROR;
            ppCVar14 = &conn->ssl[sockindex].backend.pnnssl_context;
            if (local_88 != 0)
            {
                uVar3 = nnsslContextDestroy(*ppCVar14);
                if (nnResultIsFailure(uVar3) != 0)
                    goto GOTOFATALERROR;
            }
            if (((pSVar13->set).ssl.fsslctx == 0) && ((pSVar13->set).ssl.filler[0] == 0))
            {
                result = CURLE_SSL_CTXFUNCTION;
            }
            else
            {
                result = (*(pSVar13->set).ssl.fsslctx)(conn->data, *ppCVar14,
                                                       (conn->data->set).ssl.fsslctxp);
                if (result == CURLE_OK)
                {
                    pCVar7 = *ppCVar14;
                    goto LAB_710071d484;
                }
            LAB_710071d668:
                result = CURLE_ABORTED_BY_CALLBACK;
            }
        }
        else
        {
            ppCVar14 = &conn->ssl[sockindex].backend.pnnssl_context;
            conn->ssl[sockindex].backend.pnnssl_context = pCVar7;
            conn->ssl[sockindex].backend.using_external_ssl_context = TRUE;
        LAB_710071d484:
            uVar3 = nnsslContextGetContextId(pCVar7, &local_88);
            if (nnResultIsFailure(uVar3) == 0)
            {
                if (local_88 == 0)
                {
                    result = CURLE_SSL_INVALIDCONTEXT;
                }
                else
                {
                    pnVar1 = &conn->ssl[sockindex].backend;
                    uVar3 = nnsslConnectionCreate(pnVar1, *ppCVar14);
                    if (nnResultIsFailure(uVar3) == 0)
                    {
                        nnsocketFcntl(conn->sock[sockindex], 4, 0);
                        uVar3 = nnsslConnectionSetOption(pnVar1, 0, 1);
                        if (nnResultIsFailure(uVar3) == 0)
                        {
                            uVar3 =
                                nnsslConnectionSetSocketDescriptor(pnVar1, conn->sock[sockindex]);
                            if (nnResultIsFailure(uVar3) == 0)
                            {
                                __s = (conn->host).dispname;
                                sVar9 = strlen(__s);
                                uVar3 = nnsslConnectionSetHostName(pnVar1, __s, sVar9);
                                if (nnResultIsFailure(uVar3) == 0)
                                {
                                    pSVar13 = conn->data;
                                    /*uVar2._0_1_ = (pSVar13->set).ssl.verifypeer;
                                    uVar2._1_1_ = (pSVar13->set).ssl.verifyhost;
                                    uVar2._2_1_ = (pSVar13->set).ssl.verifystatus;
                                    uVar2._3_1_ = (pSVar13->set).ssl.verifynnOption;*/
                                    uVar6 = uVar2 & 0xff;
                                    if ((uVar2 & 0xff00) != 0)
                                    {
                                        uVar6 = uVar2 & 0xff | 2;
                                    }
                                    if (uVar2 >> 0x18 != 0)
                                    {
                                        uVar6 = uVar6 | 4;
                                    }
                                    uVar3 = nnsslConnectionSetVerifyOption(pnVar1, uVar6);
                                    if (nnResultIsFailure(uVar3) == 0)
                                    {
                                        uVar3 = nnsslConnectionSetSessionCacheMode(
                                            pnVar1, (conn->data->set).ssl.sessionid);
                                        if (nnResultIsFailure(uVar3) == 0)
                                        {
                                            pSVar13 = conn->data;
                                            pcVar12 = (pSVar13->set).ssl.filler[2];
                                            if ((pcVar12 != 0) //&&
                         /*(iVar4 = (*pcVar12)(pSVar13,pnVar1,(pSVar13->set).ssl.filler[4]),
                         iVar4 != 0)*/)
                                                goto LAB_710071d668;
                                            uVar3 = nnsslConnectionSetIoMode(pnVar1, 2);
                                            if (nnResultIsFailure(uVar3) != 0)
                                            {
                                                result = CURLE_SSL_CONNECT_ERROR;
                                                goto error;
                                            }
                                            *value = 1;
                                            goto LAB_710071d698;
                                        }
                                        result = CURLE_SSL_CONNECT_ERROR;
                                    }
                                    else
                                    {
                                        result = CURLE_SSL_CONNECT_ERROR;
                                    }
                                }
                                else
                                {
                                    result = CURLE_SSL_CONNECT_ERROR;
                                }
                            }
                            else
                            {
                                result = CURLE_SSL_CONNECT_ERROR;
                            }
                        }
                        else
                        {
                            result = CURLE_SSL_CONNECT_ERROR;
                        }
                    }
                    else
                    {
                        result = CURLE_SSL_CONNECT_ERROR;
                    }
                }
            }
            else
            {
            GOTOFATALERROR:
                result = CURLE_SSL_FATALERROR;
            }
        }
    }
    else
    {
    LAB_710071d698:
        pSVar13 = conn->data;
        if (Curl_timeleft(pSVar13, 0, 1) < 0)
        {
            result = CURLE_OPERATION_TIMEDOUT;
            goto error;
        }
        (pSVar13->set).ssl.filler[1] = 0;
        (pSVar13->set).ssl.certverifyresult = 0;
        local_62 = 0;
        local_64 = 0;
        local_68 = 0;
        local_70 = 0;
        uVar8 = nnsslConnectionDoHandshakeWithCertBuffer(&connssl->backend.connection,
                                                         (long)&local_70 + 4, &local_70);
        pvVar15 = (void*)(uVar8 & 0xffffffff);
        result = CURLE_OK;
        if (nnResultIsFailure(pvVar15) == 0)
        {
        LAB_710071d7b0:
            pcVar12 = (pSVar13->set).ssl.filler[3];
            if (pcVar12 != 0)
            {
                local_88 =
                    CONCAT35(CONCAT12(local_62, local_64), CONCAT41(local_68, result == CURLE_OK));
                local_80 = local_70 >> 0x20;
                local_78 = local_70 & 0xffffffff;
                // iVar4 = (*pcVar12)(conn->data,pnVar1,(conn->data->set).ssl.filler[4],&local_88);
                if (iVar4 != 0)
                {
                    result = CURLE_ABORTED_BY_CALLBACK;
                }
            }
        }
        else
        {
            if (nnResultGetModule(uVar8) != nnResultGetModule(0x1987b))
            {
            gotoblock:
                (pSVar13->set).ssl.filler[1] = pvVar15;
                result = CURLE_SSL_CONNECT_ERROR;
                if (nnResultGetModule(uVar8) == nnResultGetModule(0x19e7b))
                {
                    result = CURLE_PEER_FAILED_VERIFICATION;
                    if (nnResultGetDescription(uVar8) != nnResultGetDescription(0x19e7b))
                    {
                        result = CURLE_SSL_CONNECT_ERROR;
                    }
                }
                goto LAB_710071d7b0;
            }
            if (nnResultGetDescription(uVar8) != nnResultGetDescription(0x1987b))
                goto gotoblock;
            result = CURLE_AGAIN;
        }
        if (result == CURLE_OK)
        {
            *done = true;
            conn->recv[sockindex] = nnssl_recv;
            conn->send[sockindex] = nnssl_send;
            connssl->state = ssl_connection_complete;
            *value = 0;
            return CURLE_OK;
        }
    }
    if (result == CURLE_AGAIN)
    {
        return CURLE_OK;
    }

    if (result == CURLE_PEER_FAILED_VERIFICATION)
    {
        uVar3 = nnsslConnectionGetVerifyCertError(&conn->ssl[sockindex].backend, &local_88);
        if (nnResultIsFailure(uVar3) == 0)
        {
            (conn->data->set).ssl.certverifyresult = local_88 & 0xffffffff;
            if (isResultEqual(local_88 & 0xffffffff, 0x25a7b))
                goto failedVerification;

            if (isResultEqual(local_88 & 0xffffffff, 0x2607b))
                goto failedVerification;

            if (isResultEqual(local_88 & 0xffffffff, 0x25e7b))
                goto failedVerification;

            if (isResultEqual(local_88 & 0xffffffff, 0x2867b))
                goto GOTOCERTPROBLEM;

            if (isResultEqual(local_88 & 0xffffffff, 0x2687b))
            {
                result = CURLE_SSL_CACERT;
                goto error;
            }

            if (isResultEqual(local_88 & 0xffffffff, 0xbca7b))
                goto GOTOCERTPROBLEM;

            if (isResultEqual(local_88 & 0xffffffff, 0xbd07b))
                goto GOTOCERTPROBLEM;

            if (isResultEqual(local_88 & 0xffffffff, 0xbce7b))
                goto GOTOCERTPROBLEM;
        }
    failedVerification:
        result = CURLE_PEER_FAILED_VERIFICATION;
    }
    goto error;
GOTOCERTPROBLEM:
    result = CURLE_SSL_CERTPROBLEM;
    goto error;
error:
    nnssl_destroy_ssl_connection(conn, sockindex);
    if (!conn->ssl[sockindex].backend.using_external_ssl_context)
        nnssl_destroy_ssl_context(conn, sockindex);
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
    int result = nnsslConnectionPending(&conn->ssl[sockindex].backend.connection, &pending);
    return nnResultIsSuccess(result & 0xffffffff) && pending > 0;
}

#endif /* USE_NNSSL */
