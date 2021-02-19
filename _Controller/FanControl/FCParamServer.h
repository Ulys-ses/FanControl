// Класс для взаимодействия с сервером параметров
//-----------------------------------------------

#if !defined( _FC_ParamServer_h_ )
#define _FC_ParamServer_h_

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class CFCParamServer
{
    static const int c_nProbeValueSize = 1000;

    WiFiClient m_WiFiClient;
    HTTPClient m_HttpClient;

public:
    CFCParamServer()
    {}

    // Подключение к сети
    void Connect(const char *szSSID, const char *szPassword);
    // Получнеие параметров
    String GetParameters(const char *szHost, uint16_t nPort, const char *szURLParamGet);
    // Отправка показаний
    void SendProbe(const char *szHost, uint16_t nPort, const char *szURLProbePost, const char *szProbeCode, int nValue);
};

extern CFCParamServer g_FCParamServer;

#endif // _FC_ParamServer_h_
