#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <HttpClient.h>

#include "FCParameters.h"
#include "FCConst.h"

CFCParameters g_FCParameters;

WiFiClient g_WiFiClient;
HttpClient g_HttpClient(g_WiFiClient);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start!");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(c_szSSID, c_szPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
    int err = g_HttpClient.get(c_ipHost, "", c_nPort, c_szURLParamGet);
    Serial.print("g_HttpClient.get");
    Serial.println(err);
    err = g_HttpClient.skipResponseHeaders();
    Serial.print("g_HttpClient.skipResponseHeaders");
    Serial.println(err);
    int bodyLen = g_HttpClient.contentLength();
    Serial.print("g_HttpClient.contentLength");
    Serial.println(err);
      String Data;
      while(g_HttpClient.available())
      {
            String line = g_HttpClient.readStringUntil('\r');
                Data = Data + line;
      }
    g_HttpClient.stop();  
    Serial.println(Data);

            g_FCParameters.Load(Data);
Serial.println("Loaded params:==============================");
        for (int i = 0; i < c_FCPCParametersQuant; ++i)
{
  Serial.print(g_FCParameters.GetParameterName(EFCParameterCodes(i)));
  Serial.print(": ");
  Serial.println(g_FCParameters.GetParameterValue(EFCParameterCodes(i)));
}
        delay(3000); // execute once every 5 minutes, don't flood remote service
}
