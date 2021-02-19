// Базовый класс для датчиков и эффекторов
//----------------------------------------

#if !defined (_FC_ProbeBase_h_)
#define _FC_ProbeBase_h_

#include "FCParamServer.h"

#include "FCConst.h"

class CFCProbeBase
{
    // Время последней отправки на сервер
    long m_lLastSend;
    // Код датчика
    const char *m_szProbeCode;

protected:
    // Показания датчика
    int m_nValue;

    CFCProbeBase(const char *szProbeCode)
        : m_nValue(-1)
        , m_lLastSend(0)
        , m_szProbeCode(szProbeCode)
    {}

    // Загрузка показаний датчика. Для эффектора ничего не делает - переопределять не надо
    virtual int LoadValue(void)
    {
        return m_nValue;
    }

public:
    // Получение показаний датчика
    int GetValue(void);
    // Отправка показаний на сервер
    void SendToServer(void);
    
};

#endif // _FC_ProbeBase_h_
