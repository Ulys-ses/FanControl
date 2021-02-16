// Датчик влажности
//-----------------

#if !defined (_FC_ProbeHudimity_h_)
#define _FC_ProbeHudimity_h_

#include "FCProbeBase.h"

class CFCProbeHudimity
    : public CFCProbeBase
{
protected:
    // Загрузка показаний датчика.
    virtual int LoadValue(void)
    {
        return m_nValue;
    }

public:
    CFCProbeHudimity(const char *szProbeCode, HttpClient &Client)
        : CFCProbeBase(szProbeCode, Client)
    {}
};

#endif // _FC_ProbeHudimity_h_