#ifndef HEADER_CURL_NINTENDO_PROXY
#define HEADER_CURL_NINTENDO_PROXY

struct ProxySettings
{
    char filler_0[0x4];
    char enabled;
    short port;
    char server[0x64];
    char filler_6d[0x4];
    char auto_auth_enabled;
    char user[0x20];
    char password[0x20];
    char filler_b2[0x2];
};

#ifdef __cplusplus
extern "C"
{
#endif

    int32_t nnnifmGetCurrentProxySetting(struct ProxySettings* settings);

#ifdef __cplusplus
}
#endif

#endif /* HEADER_CURL_NINTENDO_PROXY */
