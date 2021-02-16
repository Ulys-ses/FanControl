// Базовый класс для датчиков и эффекторов
//----------------------------------------

#if !defined (_FC_ProbeBase_h_)
#define _FC_ProbeBase_h_

#include <HttpClient.h>

#include "FCConst.h"

const int c_nProbeValueSize = 1000;

class CFCProbeBase
{
    // Время последней отправки на сервер
    long m_lLastSend;
    // Таймаут отправок на сервер
    int m_nSendTimeout;
    // Код датчика
    const char *m_szProbeCode;
    // Клиент для отправки показаний на сервер
    HttpClient &m_HttpClient;

protected:
    // Показания датчика
    int m_nValue;

    CFCProbeBase(const char *szProbeCode, HttpClient &Client)
        : m_nValue(-1)
        , m_lLastSend(0)
        , m_nSendTimeout(60)
        , m_szProbeCode(szProbeCode)
        , m_HttpClient(Client)
    {}

    // Загрузка показаний датчика. Для эффектора ничего не делает - переопределять не надо
    virtual int LoadValue(void)
    {
        return m_nValue;
    }

public:
    // Получение показаний датчика
    int GetValue(void)
    {
        m_nValue = LoadValue();
        if (m_lLastSend + m_nSendTimeout*1000 > millis())
        {
            // TODO Отправляем на сервер
            StaticJsonDocument<c_nProbeValueSize> jsonProbeValue;
            jsonProbeValue["Code"] = m_szProbeCode;
            jsonProbeValue["Value"] = m_nValue;
            char strProbeValue[c_nProbeValueSize];
            serializeJson(jsonProbeValue, strProbeValue, c_nProbeValueSize);
            m_HttpClient.post(c_ipHost, NULL, c_nPort, c_szURLProbePost);
            m_HttpClient.print(strProbeValue);
            m_HttpClient.finishRequest();
            // Сохраняем время отправки
            m_lLastSend = millis();
        }

        retrun m_nValue;
    }
    
    // Установка таймаута отправки показаний на сервер
    void SetTimeout(int nSendTimeout)
    {
        m_nSendTimeout = nSendTimeout;
    }
};

#endif // _FC_ProbeBase_h_