

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <HttpClient.h>

#include "FCParameters.h"

const char* ssid     = "Alex-K-N";
const char* password = "e57an31xot";

const char* host = "10.0.0.6";
const uint16_t port = 80;

CFCParameters g_FCParameters;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start!");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int nCounter = 0;
void loop() 
{
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  HttpClient http(client);
  
      String url = "/FanControl/Parameters/Current/";
      if (client.connected()) {
      client.print(String("GET ")+url+" HTTP/1.1\r\nHost: "+host+"\r\nConnection: close\r\n\r\n");
        client.flush();
        //client.println("FanControl/Parameters/Current/");
  }
//  FanControl/Parameters/Current/
  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }
      bool bIsData = false;
      String Data;
      while(client.available()){
            String line = client.readStringUntil('\r');
            if (bIsData)
                Data = Data + line;
            Serial.print(line.length());
            Serial.print(line);
            Serial.println("-----");
            if (line == "\n")bIsData = true;
        }
        Serial.println();
        client.stop();  
            Serial.print("Data:");
            Serial.println(Data);

Serial.println("Default params:");
        for (int i = 0; i < c_FCPCParametersQuant; ++i)
{
  Serial.print(g_FCParameters.GetParameterName(EFCParameterCodes(i)));
  Serial.print(": ");
  Serial.println(g_FCParameters.GetParameterValue(EFCParameterCodes(i)));
}
            g_FCParameters.Load(Data);
Serial.println("Loaded params:");
        for (int i = 0; i < c_FCPCParametersQuant; ++i)
{
  Serial.print(g_FCParameters.GetParameterName(EFCParameterCodes(i)));
  Serial.print(": ");
  Serial.println(g_FCParameters.GetParameterValue(EFCParameterCodes(i)));
}
        delay(300000); // execute once every 5 minutes, don't flood remote service
}
