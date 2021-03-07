#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "FCParameters.h"
#include "FCParamServer.h"
#include "FCConst.h"

#include "FCProbeHudimity.h"
#include "FCProbeNoiceTime.h"
#include "FCProbeLightTime.h"
#include "FCProbeMotor.h"

CFCProbeHudimity   g_ProbeHudimity("BH", c_nDHTType, c_nDHTPin);
CFCProbeNoiceTime  g_ProbeNoice("RN", c_nNoicePin);
CFCProbeLightTime  g_ProbeLight("RL", c_nLightPin);

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

    // Инициализируем плату моторов
    CFCProbeMotor::sInitMotor();
    // Запускаем моторы в минимальном режиме
    g_motorBathroom.SetLevel(c_FCMLBaseLevel);
    g_motorRestroom.SetLevel(c_FCMLBaseLevel);
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


    int nHudimity = g_ProbeHudimity.GetValue();
//    Serial.print("Hudimity:");
//    Serial.println(nHudimity);
    // Вентилятор в ванной
    if (g_FCParameters.GetParameterValue(c_FCPCBathMode) == c_FCMLAutoMode)
    {
        if (nHudimity > g_FCParameters.GetParameterValue(c_FCPCBathHudimOn))
            g_motorBathroom.SetLevel(c_FCMLIntensiveLevel);
        else if (nHudimity < g_FCParameters.GetParameterValue(c_FCPCBathHudimOff))
            g_motorBathroom.SetLevel(c_FCMLBaseLevel);
    }
    else
        g_motorBathroom.SetLevel(g_FCParameters.GetParameterValue(c_FCPCBathMode));

    // Вентилятор в туалете
    int nNoiceTimeout = g_ProbeNoice.GetValue();      // Обязательно получаем, чтобы фиксировать звук!
    int nLightTimeout = g_ProbeLight.GetValue();
    if (g_FCParameters.GetParameterValue(c_FCPCRestMode) == c_FCMLAutoMode)
    {
        if (nLightTimeout > g_FCParameters.GetParameterValue(c_FCPCRestTimeShine) 
                    && nNoiceTimeout < nLightTimeout)
            g_motorRestroom.SetLevel(c_FCMLIntensiveLevel, g_FCParameters.GetParameterValue(c_FCPCRestTimeWorkHard));
        else
            g_motorRestroom.CheckTimer(c_FCMLBaseLevel);
    }
    else
        g_motorBathroom.SetLevel(g_FCParameters.GetParameterValue(c_FCPCRestMode));

    delay(50);
}
