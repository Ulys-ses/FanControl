// Датчик с аналоговым выходом
//----------------------------

#if !defined (_FC_ProbeAnalog_h_)
#define _FC_ProbeAnalog_h_

#include "FCProbeBase.h"

class CFCProbeAnalog
    : public CFCProbeBase
{
    int m_nDataPin;
protected:
    // Загрузка показаний датчика.
    virtual int LoadValue(void)
    {
        return analogRead(m_nDataPin);
    }

public:
    CFCProbeAnalog(const char *szProbeCode, int nDataPin)
        : CFCProbeBase(szProbeCode)
        , m_nDataPin(nDataPin)
    {
    }
};

#endif // _FC_ProbeAnalog_h_
