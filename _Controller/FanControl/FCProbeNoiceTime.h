// Датчик звука - показывает сколько времени назад был громкий звук
//-----------------------------------------------------------------

#if !defined (_FC_ProbeNoiceTime_h_)
#define _FC_ProbeNoiceTime_h_

#include "FCProbeBase.h"
#include "FCParameters.h"

class CFCProbeNoiceTime
    : public CFCProbeBase
{
    int m_nDataPin;
    // Время последнего превышения
    int m_nLastTime;
protected:
    // Загрузка показаний датчика.
    virtual int LoadValue(void)
    {
        // Текущее состояние
        int nCurrLevel = analogRead(m_nDataPin);

        // Превышение есть - фиксируем
        if (nCurrLevel > g_FCParameters.GetParameterValue(c_FCPCRestNoiceLevel))
            m_nLastTime = millis();

        return  (millis() - m_nLastTime)/1000;
    }

public:
    CFCProbeNoiceTime(const char *szProbeCode, int nDataPin)
        : CFCProbeBase(szProbeCode)
        , m_nDataPin(nDataPin)
        , m_nLastTime(0)
    {
    }
};

#endif // _FC_ProbeNoiceTime_h_
