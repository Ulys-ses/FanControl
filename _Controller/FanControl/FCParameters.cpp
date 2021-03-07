// Модуль хранения параметров управления вентиляторами
//----------------------------------------------------

#include "FCParameters.h"

#include <ArduinoJson.h>

CFCParameters  g_FCParameters;

const String g_strParamNames[] = 
{
    "TimeGetParams",      // - промежуток времени между запросами параметров
    "TimeSaveProbValues", // - промежуток времени между сохранениями показаний датчиков
    "RestTimeWorkHard",   // - время проветривания туалета
    "RestTimeShine",      // - время горения лампы для включения проветривания
    "RestNoiceLevel",     // - уровень громкости спуска воды
    "RestLowFreq",        // - частота постоянного режима в туалете
    "RestHighFreq",       // - частота интенсивного режима в туалете
    "BathLowFreq",        // - частота постоянного режима в ванной
    "BathHighFreq",       // - частота интенсивного режима в ванной
    "BathHudimOn",        // - пороговое значение влажности для включения проветривания в ванной
    "BathHudimOff",       // - пороговое значение влажности для вЫключения проветривания в ванной
    "RestMode",           // - режим вентияллятора в туалете (-1) - автомат, см. EFCMotorLevels 
    "BathMode"            // - режим вентияллятора в ванной (-1) - автомат, см. EFCMotorLevels  
};

CFCParameters::CFCParameters(void)
    : m_Parameters {5,60,300,300,10,20,100,20,100,60,40,c_FCMLAutoMode,c_FCMLAutoMode}
    , m_lLastUpdate (0)       
{}

// Чтение параметорв из строки
int CFCParameters::Load(const String &strJSON)
{
    // Разбираем строку
    StaticJsonDocument<1000> jsonParams;
    DeserializationError nError = deserializeJson(jsonParams, strJSON);
    if (nError)
        return c_FCEIncorrectParameterJSON;

    // Заносим в переменные
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

    // Устанавливаем время обновления
    m_lLastUpdate = millis();

    return c_FCEOK;
}

const String CFCParameters::GetParameterName(EFCParameterCodes nParamCode)
{
    return g_strParamNames[nParamCode];
}
