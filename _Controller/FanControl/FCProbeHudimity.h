// Датчик влажности
//-----------------

#if !defined (_FC_ProbeHudimity_h_)
#define _FC_ProbeHudimity_h_

#include "FCProbeBase.h"

#include <DHT.h> 

class CFCProbeHudimity
    : public CFCProbeBase
{
    DHT m_dht;
protected:
    // Загрузка показаний датчика.
    virtual int LoadValue(void)
    {
        return m_dht.readHumidity();
    }

public:
    CFCProbeHudimity(const char *szProbeCode, int nDHTType, int nDHTPin)
        : CFCProbeBase(szProbeCode)
        , m_dht(nDHTPin, nDHTType)
    {
        m_dht.begin();
    }
};

#endif // _FC_ProbeHudimity_h_
