#include "curl_nintendo_proxy.h"
/*
#include "mprintf.h"

#include "nn/nifm.h"
#include "nn/result.h"

static inline u32 GetDescription(u32 result)
{
    return result >> 9 & 0x1fff;
}

static inline u32 nnnifmGetCurrentProxySettingImpl(struct ProxySettings* settings)
{
    nn::nifm::ProxySetting nifm_setting;
    memset(&nifm_setting, 0, sizeof(nifm_setting));

    u32 result = nn::nifm::GetCurrentProxySetting(&nifm_setting);

    if (nnResultIsFailure(result))
    {
        curl_mfprintf(_stderr,
                      "\nError: nn::nifm::GetCurrentProxySetting() failed. Err Desc: %d\n\n",
                      GetDescription(result));
        return result;
    }

    return result;
}

u32 nnnifmGetCurrentProxySetting(struct ProxySettings* settings)
{
    return nnnifmGetCurrentProxySettingImpl(settings);
}*/
