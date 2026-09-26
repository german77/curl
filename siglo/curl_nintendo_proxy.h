#ifndef HEADER_CURL_NINTENDO_PROXY
#define HEADER_CURL_NINTENDO_PROXY

#include "nn/types.h"

struct ProxySettings
{
    u8 filler_0[0x4];
    u8 enabled;
    u16 port;
    char server[0x64];
    u8 filler_6d[0x4];
    u8 auto_auth_enabled;
    char user[0x20];
    char password[0x20];
    u8 filler_b2[0x2];
};

#ifdef __cplusplus
extern "C"
{
#endif

    u32 nnnifmGetCurrentProxySetting(struct ProxySettings* settings);

#ifdef __cplusplus
}
#endif

#endif /* HEADER_CURL_NINTENDO_PROXY */
