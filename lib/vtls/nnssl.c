/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

/*
 * Source file for all OpenSSL-specific code for the TLS/SSL layer. No code
 * but vtls.c should ever call or use these functions.
 */

/*
 * The original SSLeay-using code for curl was written by Linas Vepstas and
 * Sampo Kellomaki 1998.
 */

#include "curl_setup.h"

#ifdef USE_NNSSL

#include "urldata.h"

/* The last #include files should be: */
#include "curl_memory.h"
#include "nn/result.h"
#include "nn/ssl.h"

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

size_t Curl_nnssl_version(char* buffer, size_t size)
{
    if (!buffer)
        return 0;

    return snprintf(buffer, size, "nn::ssl");
}

int Curl_nnssl_seed(struct SessionHandle* data)
{
    return 0;
}

int Curl_nnssl_random(struct SessionHandle* data, unsigned char* entropy, size_t length)
{
    if (entropy == NULL || data == NULL)
        return 0;

    for (size_t i = 0; i < length; i++)
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

bool Curl_nnssl_data_pending(const struct connectdata* conn, int connindex)
{
    int connection = 0;
    int result = nnsslConnectionPending(&conn->ssl[connindex].nnssl_connection, &connection);
    return nnResultIsSuccess(result & 0xffffffff) && connection > 0;
}

#endif /* USE_NNSSL */
