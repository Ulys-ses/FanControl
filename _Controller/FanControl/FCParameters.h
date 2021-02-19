// Модуль хранения параметров управления вентиляторами
//----------------------------------------------------

#if !defined (_FC_parametes_h_)
#define _FC_parametes_h_

#include <ArduinoJson.h>

#include "FCConst.h"

// Коды параметров
enum EFCParameterCodes
{
    c_FCPCTimeGetParams,      // - промежуток времени между запросами параметров
    c_FCPCTimeSaveProbValues, // - промежуток времени между сохранениями показаний датчиков
    c_FCPCRestTimeWorkHard,   // - время проветривания туалета
    c_FCPCRestTimeShine,      // - время горения лампы для включения проветривания
    c_FCPCRestShineLevel,     // - уровень освещенности в туалете
    c_FCPCRestLowLevel,       // - уровень постоянного режима в туалете
    c_FCPCRestHighLevel,      // - уровень интенсивного режима в туалете
    c_FCPCBathLowLevel,       // - уровень постоянного режима в ванной
    c_FCPCBathHighLevel,      // - уровень интенсивного режима в ванной
    c_FCPCBathHudimOn,        // - пороговое значение влажности для включения проветривания в ванной
    c_FCPCBathHudimOff,       // - пороговое значение влажности для вЫключения проветривания в ванной

    c_FCPCParametersQuant     // - Максимальный номер для определения количества параметров
};

class CFCParameters
{
    // Параметры
    int m_Parameters[c_FCPCParametersQuant];

    // Время последнего обновления параметров
    long m_lLastUpdate;

public:
    CFCParameters(void);

    // Чтение параметорв из строки
    int Load(const String &strJSON);

    // Получение значений параметров
    int GetParameterValue(int nParamCode)
    {
        return m_Parameters[nParamCode];
    }

    // Получение имен параметров
    const String GetParameterName(EFCParameterCodes nParamCode);

    // Проверка необходимости обновления параметров
    bool NeedUpdate(void)
    {
        return  (m_lLastUpdate + m_Parameters[c_FCPCTimeGetParams] * 1000 <= millis());
    }

    // Печать параметров
    void Print(const char *szTitle)
    {
        Serial.println(szTitle);
        for (int i = 0; i < c_FCPCParametersQuant; ++i)
        {
            Serial.print(GetParameterName(EFCParameterCodes(i)));
            Serial.print(": ");
            Serial.println(GetParameterValue(i));
        }
    }
};

extern CFCParameters  g_FCParameters;

#endif // _FC_parametes_h_
