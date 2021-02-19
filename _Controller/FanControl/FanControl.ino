#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "FCParameters.h"
#include "FCParamServer.h"
#include "FCConst.h"

#include "FCProbeHudimity.h"
#include "FCProbeMotor.h"

CFCProbeHudimity g_FCProbeHudimity("BH", c_nDHTType, c_nDHTPin);
CFCProbeMotor g_motorBathroom("BL", c_FCMCBathroom, c_FCPCBathLowLevel);
CFCProbeMotor g_motorRestroom("RL", c_FCMCRestroom, c_FCPCRestLowLevel);

void setup() 
{
    Serial.begin(115200);
    Serial.println("Start!");
    
    g_FCParamServer.Connect(c_szSSID, c_szPassword);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() 
{
    if (g_FCParameters.NeedUpdate())
    {
        String strParams = g_FCParamServer.GetParameters(c_szHost, c_nPort, c_szURLParamGet);
//        Serial.println(strParams);

        g_FCParameters.Load(strParams);
//        g_FCParameters.Print("Loaded params:==============================");
    }
//    else
//        Serial.print(".");

    int nHudimity = g_FCProbeHudimity.GetValue();
//    Serial.print("Hudimity:");
//    Serial.println(nHudimity);
    delay(5000);
}
