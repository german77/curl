#pragma once

#include "nn/types.h"

struct ProxySetting
{
    u8 filler[0x4];
    u8 enabled;
    u16 port;
    char server[0x64];
    u8 filler2[0x4];
    u8 auto_auth_enabled;
    char user[0x20];
    char password[0x20];
};

extern int nnnifmGetCurrentProxySetting(struct ProxySetting* setting);
