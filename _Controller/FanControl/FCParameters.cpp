// ћодуль хранени€ параметров управлени€ вентил€торами
//----------------------------------------------------

#include <ArduinoJson.h>

#include "FCParameters.h"

const String g_strParamNames[] = 
{
    "TimeGetParams",      // - промежуток времени между запросами параметров
    "TimeSaveProbValues", // - промежуток времени между сохранени€ми показаний датчиков
    "RestTimeWorkHard",   // - врем€ проветривани€ туалета
    "RestTimeShine",      // - врем€ горени€ лампы дл€ включени€ проветривани€
    "RestShineLevel",     // - уровень освещенности в туалете
    "RestLowFreq",        // - частота посто€нного режима в туалете
    "RestHighFreq",       // - частота интенсивного режима в туалете
    "BathLowFreq",        // - частота посто€нного режима в ванной
    "BathHighFreq",       // - частота интенсивного режима в ванной
    "BathHudimOn",        // - пороговое значение влажности дл€ включени€ проветривани€ в ванной
    "BathHudimOff"       // - пороговое значение влажности дл€ вџключени€ проветривани€ в ванной
};

CFCParameters::CFCParameters(void)
    : m_Parameters({5,60,300,300,10,20,100,20,100,60,40})       
{}

// „тение параметорв из строки
int CFCParameters::Load(const String &strJSON)
{
    // –азбираем строку
    StaticJsonDocument<1000> jsonParams;
    DeserializationError nError = deserializeJson(jsonParams, strJSON);
    if (nError)
        return c_FCEIncorrectParameterJSON;

    // «аносим в переменные
    for (int i = 0; i < jsonParams.size(); ++i)
    {
        String strParamName = jsonParams[i]["Name"];
        for (int j = 0; j < c_FCPCParametersQuant; ++j)
            if (g_strParamNames[j] == strParamName)
            {
                m_Parameters[j] = jsonParams[j]["Value"];
                break;
            }
    }

    return c_FCEOK;
}

const String CFCParameters::GetParameterName(EFCParameterCodes nParamCode)
{
    return g_strParamNames[nParamCode];
}
