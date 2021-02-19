// Датчик света - показывает сколько времени горел свет
//-----------------------------------------------------

#if !defined (_FC_ProbeLightTime_h_)
#define _FC_ProbeLightTime_h_

#include "FCProbeBase.h"

class CFCProbeLightTime
    : public CFCProbeBase
{
    int m_nDataPin;
    // Состояние при последнем измерении
    int m_nLastState;
    // Время последнего измерения
    int m_nLastChange;
protected:
    // Загрузка показаний датчика.
    virtual int LoadValue(void)
    {
        // Текущее состояние
        int nNewState = digitalRead(m_nDataPin);
        int nCurrTime = millis();

        // Изменение состояния?
        if (nNewState != m_nLastState)
        {
            // Сохраним время
            int nTime = nCurrTime - m_nLastChange;

            // Фиксируем состяние
            m_nLastChange = nCurrTime;
            m_nLastState = nNewState;

            // Только при выключении
            if (0 == nNewState)
                return nTime/1000;   // В секундах
        }

        return 0;
    }

public:
    CFCProbeLightTime(const char *szProbeCode, int nDataPin)
        : CFCProbeBase(szProbeCode)
        , m_nDataPin(nDataPin)
        , m_nLastState(1)
        , m_nLastChange(0)
    {
    }
};

#endif // _FC_ProbeLightTime_h_
