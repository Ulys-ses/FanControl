#include "FCParamServer.h"

#include <ArduinoJson.h>

CFCParamServer g_FCParamServer;

void CFCParamServer::Connect(const char *szSSID, const char *szPassword)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(szSSID, szPassword);
  
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
}

String CFCParamServer::GetParameters(const char *szHost, uint16_t nPort, const char *szURLParamGet)
{
    int err = m_HttpClient.begin(m_WiFiClient, String(szHost)+szURLParamGet);
//    Serial.print("m_HttpClient.begin ");
//    Serial.println(err);
    if (!err)
        return "";

    int nCode = m_HttpClient.GET();
//    Serial.print("m_HttpClient.get ");
//    Serial.println(nCode);
    if (nCode != HTTP_CODE_OK && nCode != HTTP_CODE_MOVED_PERMANENTLY)
        return "";

    String strResult = m_HttpClient.getString();
    m_HttpClient.end();

    return strResult;
}

void CFCParamServer::SendProbe(const char *szHost, uint16_t nPort, const char *szURLProbePost, const char *szProbeCode, int nValue)
{
    Serial.println("Send to server");
    // Отправляем на сервер
    StaticJsonDocument<c_nProbeValueSize> jsonProbeValue;
    jsonProbeValue["Code"] = szProbeCode;
    jsonProbeValue["Value"] = nValue;
    char strProbeValue[c_nProbeValueSize];
    serializeJson(jsonProbeValue, strProbeValue, c_nProbeValueSize);
    Serial.print("strProbeValue: ");
    Serial.println(strProbeValue);
    
    int err = m_HttpClient.begin(m_WiFiClient, String(szHost)+szURLProbePost);
    Serial.print("m_HttpClient.begin: ");
    Serial.println(err);
    if (!err)
        return;

    m_HttpClient.addHeader("Content-Type", "application/json");

    int nCode = m_HttpClient.POST(strProbeValue);
    Serial.print("m_HttpClient.post ");
    Serial.println(nCode);

    m_HttpClient.end();
}
