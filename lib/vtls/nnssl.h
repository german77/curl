#ifndef HEADER_CURL_NNSSL
#define HEADER_CURL_NNSSL

#include "curl_setup.h"

#ifdef USE_NNSSL
/*
 * This header should only be needed to get included by vtls.c and nnssl.c
 */

#include "urldata.h"

CURLcode Curl_nnssl_connect(struct connectdata* conn, int sockindex);
CURLcode Curl_nnssl_connect_nonblocking(struct connectdata* conn, int sockindex, bool* done);

/* close a SSL connection */
void Curl_nnssl_close(struct connectdata* conn, int sockindex);

int Curl_nnssl_init(void);
void Curl_nnssl_cleanup(void);
bool Curl_nnssl_false_start(void);

size_t Curl_nnssl_version(char* buffer, size_t size);
int Curl_nnssl_check_cxn(struct connectdata* conn);
bool Curl_nnssl_data_pending(const struct connectdata* conn, int sockindex);

/* return 0 if a find random is filled in */
int Curl_nnssl_random(struct SessionHandle* data, unsigned char* entropy, size_t length);

bool Curl_nnssl_cert_status_request(void);

int Curl_nnssl_seed(struct SessionHandle* data);

/* Set the API backend definition to NNSSL */
#define CURL_SSL_BACKEND CURLSSLBACKEND_NNSSL

/* this backend supports the CAPATH option */
#define have_curlssl_ca_path 1

/* this backend supports CURLOPT_CERTINFO */
#define have_curlssl_certinfo 1

/* this backend suppots CURLOPT_SSL_CTX_* */
#define have_curlssl_ssl_ctx 1

/* API setup for NNSSL */
#define curlssl_init Curl_nnssl_init
#define curlssl_cleanup Curl_nnssl_cleanup
#define curlssl_connect Curl_nnssl_connect
#define curlssl_connect_nonblocking Curl_nnssl_connect_nonblocking
#define curlssl_close Curl_nnssl_close
#define curlssl_version Curl_nnssl_version
#define curlssl_check_cxn Curl_nnssl_check_cxn
#define curlssl_data_pending(x, y) Curl_nnssl_data_pending(x, y)
#define curlssl_random(x, y, z) Curl_nnssl_random(x, y, z)
#define curlssl_cert_status_request() Curl_nnssl_cert_status_request()
#define curlssl_false_start() Curl_nnssl_false_start()
#define curlssl_session_free(x) Curl_nop_stmt
#define curlssl_close_all(x) Curl_nop_stmt
#define curlssl_shutdown(x,y) ((void)x, (void)y, 1)
#define curlssl_set_engine(x,y) ((void)x, (void)y, CURLE_NOT_BUILT_IN)
#define curlssl_set_engine_default(x) ((void)x, CURLE_NOT_BUILT_IN)
#define curlssl_engines_list(x) ((void)x, (struct curl_slist *)NULL)

#define DEFAULT_CIPHER_SELECTION "ALL:!EXPORT:!EXPORT40:!EXPORT56:!aNULL:!LOW:!RC4:@STRENGTH"

#endif /* USE_NNSSL */
#endif /* HEADER_CURL_NNSSL */
