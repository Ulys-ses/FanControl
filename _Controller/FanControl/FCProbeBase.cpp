#include "FCProbeBase.h"

#include "FCConst.h"
#include "FCParameters.h"

// Получение показаний датчика
int CFCProbeBase::GetValue(void)
{
    m_nValue = LoadValue();

    // Отправляем на сервер
    SendToServer();

    return m_nValue;
}

void CFCProbeBase::SendToServer(void)
{
    if (m_lLastSend + g_FCParameters.GetParameterValue(c_FCPCTimeSaveProbValues)*1000 <= millis())
    {
        // Отправляем на сервер
        g_FCParamServer.SendProbe(c_szHost, c_nPort, c_szURLProbePost, m_szProbeCode, m_nValue);
        // Сохраняем время отправки
        m_lLastSend = millis();
    }
}
